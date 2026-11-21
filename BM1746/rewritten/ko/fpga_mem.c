/**
 * fpga_mem.c — fpga_mem_driver.ko 语义化转写 (模块级总览)
 *
 * 原始模块 : rootfs_1/lib/modules/fpga_mem_driver.ko (源文件 fpga_mem.c)
 *            ARM EABI5 可重定位, vermagic 4.6.0-xilinx-g03c746f7, Dual BSD/GPL
 * 设备     : 字符设备 "fpga_mem" — alloc_chrdev_region(&fpga_mem_num, 0, 1, "fpga_mem")
 *            动态主设备号, baseminor=0, 仅 1 个次设备号; class "fpga_mem";
 *            用户节点 /dev/fpga_mem 由 device_create 自动创建
 * 模块参数 : int fpga_mem_offset_addr (module_param, param_ops_int; 权限位未核证)
 *            默认 0x0F000000 (rootfs_1 与 rootfs_sd 两版构建 .data 段实测一致);
 *            按 DDR 大小加载时可传 0x3F000000/0x1F000000/0x0F000000 —
 *            将 DDR 顶部窗口交 PL DMA 使用 (analysis/out/ASIC_INIT_REPORT.md)
 * mmap 行为: remap_pfn_range(vma, vma->vm_start, fpga_mem_offset_addr >> PAGE_SHIFT,
 *            vma->vm_end - vma->vm_start, prot) → 用户态直映 DDR 工作下发窗口;
 *            页属性 = 原 prot & ~0x3C (清 ARM L_PTE_MT_MASK → UNCACHED 强序非缓存);
 *            vma 标志 |= VM_IO | VM_DONTEXPAND | VM_DONTDUMP (0x04044000, Linux 4.6 编号)
 * 窗口大小 : request_mem_region/ioremap 申请 0x1000000 字节 (16MB,
 *            = fpga_axi.h DDR_WINDOW_SIZE)
 * 覆盖     : 5 个函数 (见文件末尾覆盖率表)。本模块与 ko_bitmain_axi 的反编译地址空间
 *            重叠 (如 0x00010024/0x0001002c 两边都有), 以各横幅"来源"行区分归属。
 */

#include "../include/common_types.h"
#include "../include/fpga_axi.h"

/* ==================== 本文件常量 ==================== */
#define FPGA_MEM_REGION_SIZE     0x1000000u  /* 16MB DDR 工作下发窗口 (= fpga_axi.h DDR_WINDOW_SIZE) */
#define FPGA_MEM_DEFAULT_OFFSET  0x0F000000  /* 模块参数默认值 (.data[0] 实测) */

/* vma 标志位 (Linux 4.6 include/linux/mm.h; v4.5/v4.6 源码核实编号一致) */
#define VM_IO         0x00004000u        /* 内存映射 I/O */
#define VM_DONTEXPAND 0x00040000u        /* 禁止 mremap 扩展 */
#define VM_DONTDUMP   0x04000000u        /* 不进入 core dump */

/* ARM 非 LPAE pgprot 位 (v4.6 arch/arm/include/asm/pgtable-2level.h) */
#define ARM_L_PTE_MT_MASK 0x0000003Cu    /* 内存类型域 [5:2]; 清零 = L_PTE_MT_UNCACHED (强序非缓存) */

#define GFP_KERNEL   0x24000C0u          /* Linux 4.6 gfp.h: __GFP_IO|__GFP_FS|__GFP_DIRECT_RECLAIM|__GFP_KSWAPD_RECLAIM */
#define PAGE_SHIFT   12                  /* ARM32 4KB 页 */

/* ==================== 类型还原说明 ====================
 * dev_t / struct cdev / struct file / struct inode / struct vm_area_struct /
 * struct file_operations / struct class / struct resource / struct module
 * 均按 Linux 4.6 内核真实类型签名还原; 本文件为语义转写产物, 不独立编译。
 * 机器码锁定的关键字段偏移 (32 位内核):
 *   vm_area_struct: vm_start@0x00, vm_end@0x04, vm_page_prot@0x24, vm_flags@0x28
 *   struct cdev:    owner@0x24
 *
 * 涉及的内核 API (签名按 v4.6 还原, 供对照):
 *   int alloc_chrdev_region(dev_t *dev, unsigned baseminor, unsigned count, const char *name);
 *   void cdev_init(struct cdev *, const struct file_operations *);
 *   int cdev_add(struct cdev *, dev_t, unsigned);
 *   struct resource *request_mem_region(resource_size_t start, resource_size_t n, const char *name);
 *   void __iomem *ioremap(resource_size_t, unsigned long);
 *   struct class *class_create(struct module *owner, const char *name);  (宏: __class_create + 静态 lock_class_key)
 *   void *kmalloc(size_t, gfp_t);
 */

/* ==================== 模块全局 ==================== */
/* module_param(fpga_mem_offset_addr, int, <perm>) — 二进制含 __param_fpga_mem_offset_addr */
static int fpga_mem_offset_addr = FPGA_MEM_DEFAULT_OFFSET;
static dev_t fpga_mem_num;                 /* 动态分配的设备号 */
static struct cdev *p_fpga_mem;            /* kmalloc 的字符设备对象 */
static struct resource *base_vir_mem_addr; /* request_mem_region 返回的资源 */
static uint32_t *base_vir_addr;            /* ioremap 后 DDR 窗口虚拟地址 (语义 void __iomem *) */
static struct class *fpga_mem_class;       /* class_create 返回 */

/* 前向声明 (file_operations 初始化器引用) */
int fpga_mem_open(struct inode *inode, struct file *filp);
int fpga_mem_mmap(struct file *filp, struct vm_area_struct *vma);
int fpga_mem_release(struct inode *inode, struct file *filp);

/* fops 对象 (二进制 .rodata 符号 fpga_mem_fops):
 * 重定位实证仅 4 个槽位被赋值 — owner@0x00 <- __this_module,
 * mmap@0x28, open@0x2c, release@0x34; 其余成员全为 NULL。 */
static const struct file_operations fpga_mem_fops = {
	.owner   = THIS_MODULE,
	.mmap    = fpga_mem_mmap,
	.open    = fpga_mem_open,
	.release = fpga_mem_release,
};

// === fpga_mem_open === 原 FUN_00010024 @ 0x00010024
// 来源: reversed/ko_fpga_mem/fpga_mem_open.c | 分类: ko_mem
// 语义: file_operations.open — 无任何初始化动作, 直接成功返回 0。
//       (inode/filp 为内核回调签名的形参, 二进制函数体未使用)
int fpga_mem_open(struct inode *inode, struct file *filp)
{
	return 0;
}

// === fpga_mem_mmap === 原 FUN_0001002c @ 0x0001002c
// 来源: reversed/ko_fpga_mem/fpga_mem_mmap.c | 分类: ko_mem
// 语义: 把用户 vma 映射到 DDR 工作下发窗口 (物理基址 = 模块参数
//       fpga_mem_offset_addr, pfn = 该地址 >> PAGE_SHIFT):
//       vma 标志加 VM_IO|VM_DONTEXPAND|VM_DONTDUMP, 页属性清内存类型域
//       (UNCACHED 强序非缓存); 映射长度由用户态 mmap 的 vma 决定。
//       成功 0, 失败 -9。
int fpga_mem_mmap(struct file *filp, struct vm_area_struct *vma)
{
	int ret;

	/* vm_flags |= 0x04044000 (vm_area_struct+0x28) */
	vma->vm_flags |= VM_IO | VM_DONTEXPAND | VM_DONTDUMP;
	/* vm_page_prot = 原 prot & ~0x3C (vm_area_struct+0x24): 清 L_PTE_MT_MASK → 强序非缓存 */
	vma->vm_page_prot = vma->vm_page_prot & ~ARM_L_PTE_MT_MASK;

	/* pfn = fpga_mem_offset_addr >> 12 (原码算术右移 0xc; 地址为正, 与逻辑移位等价) */
	ret = remap_pfn_range(vma, vma->vm_start, fpga_mem_offset_addr >> PAGE_SHIFT,
			      vma->vm_end - vma->vm_start, vma->vm_page_prot);
	if (ret != 0) {
		printk("fpga_mem_mmap error!\n");
		return -9; /* 原码 0xfffffff7 */
	}
	return 0;
}

// === fpga_mem_release === 原 FUN_00010094 @ 0x00010094
// 来源: reversed/ko_fpga_mem/fpga_mem_release.c | 分类: ko_mem
// 语义: file_operations.release — 无清理动作, 直接成功返回 0。
int fpga_mem_release(struct inode *inode, struct file *filp)
{
	return 0;
}

// === init_module === 原 FUN_0001009c @ 0x0001009c
// 来源: reversed/ko_fpga_mem/init_module.c | 分类: ko_mem
// 语义: 模块加载入口 — 注册字符设备并建立 /dev/fpga_mem:
//       1) alloc_chrdev_region 申请设备号;
//       2) kmalloc cdev 并 cdev_init/cdev_add (owner=THIS_MODULE@0x24);
//       3) request_mem_region(fpga_mem_offset_addr, 0x1000000, "fpga_vir_mem") + ioremap;
//       4) class_create + device_create 建 /dev/fpga_mem。
//       返回: 0 成功; 设备号失败返回其负错误码; 错误码依次为 -10(kmalloc)
//       / -11(cdev_add) / -12(request_mem_region) / -13(ioremap) / -14(class),
//       为原码自定义的递增编码, 不做回滚清理 (原码如此)。
int init_module(void)
{
	int ret;

	printk("In fpga mem driver!\n");
	ret = alloc_chrdev_region(&fpga_mem_num, 0, 1, "fpga_mem");
	if (ret < 0) {
		printk("alloc fpga_mem fail!\n");
		return ret;
	}

	/* 原始形态: kmem_cache_alloc(_DAT_00011060 (即 kmalloc_caches[kmalloc-64]), GFP_KERNEL)
	 * — kmalloc(sizeof(struct cdev)) 的内联展开 */
	p_fpga_mem = kmalloc(sizeof(struct cdev), GFP_KERNEL);
	if (p_fpga_mem == NULL) {
		printk("kmalloc cdev fail!\n");
		return -10;
	}

	cdev_init(p_fpga_mem, &fpga_mem_fops);
	p_fpga_mem->owner = THIS_MODULE; /* cdev+0x24 <- &__this_module */
	ret = cdev_add(p_fpga_mem, fpga_mem_num, 1);
	if (ret != 0) {
		printk("add fpga_mem fail!\n");
		return -11;
	}

	base_vir_mem_addr = request_mem_region(fpga_mem_offset_addr, FPGA_MEM_REGION_SIZE,
					       "fpga_vir_mem");
	if (base_vir_mem_addr == NULL) {
		printk("request_mem_region failed!\n");
		return -12;
	}
	printk("request_mem_region OK!\n");

	base_vir_addr = ioremap(fpga_mem_offset_addr, FPGA_MEM_REGION_SIZE);
	if (base_vir_addr == NULL) {
		printk("ioremap fail!\n"); /* 原反编译带无用实参 0 (寄存器残留), 省略 */
		printk("!!!*base_vir_addr = 0x%x\n", base_vir_addr); /* 打印的是指针值而非内容 */
		return -13;
	}
	printk("fpga mem virtual address is 0x%x\n", base_vir_addr);

	/* 原码 __class_create(&__this_module, "fpga_mem", <静态 lock_class_key>) —
	 * 第三实参为 lock_class_key, 反编译中与 fpga_mem_class 符号重叠显示 */
	fpga_mem_class = class_create(THIS_MODULE, "fpga_mem");
	if ((unsigned long)fpga_mem_class < 0xfffff001u) { /* 非 IS_ERR: 错误指针值域 0xfffff001..0xffffffff */
		device_create(fpga_mem_class, NULL, fpga_mem_num, NULL, "fpga_mem");
		return 0;
	}
	printk("Err:failed in creating fpga mem class.\n");
	return -14;
}

// === cleanup_module === 原 FUN_00010254 @ 0x00010254
// 来源: reversed/ko_fpga_mem/cleanup_module.c | 分类: ko_mem
// 语义: 模块卸载入口 — 逆序释放 init_module 申请的全部资源
//       (设备号/cdev/ioremap/内存区/class+设备节点), 无条件全部执行。
void cleanup_module(void)
{
	unregister_chrdev_region(fpga_mem_num, 1);
	cdev_del(p_fpga_mem);
	kfree(p_fpga_mem);
	iounmap(base_vir_addr);
	release_mem_region(fpga_mem_offset_addr, FPGA_MEM_REGION_SIZE); /* 原码 __release_region(&iomem_resource, fpga_mem_offset_addr, 0x1000000) */
	device_destroy(fpga_mem_class, fpga_mem_num);
	class_destroy(fpga_mem_class);
	printk("Bye Bye fpga mem driver!\n");
}

/* ==================== 覆盖率表 (5/5) ====================
 * 原 FUN_00010024 @ 0x00010024 -> fpga_mem_open     (reversed/ko_fpga_mem/fpga_mem_open.c)
 * 原 FUN_0001002c @ 0x0001002c -> fpga_mem_mmap     (reversed/ko_fpga_mem/fpga_mem_mmap.c)
 * 原 FUN_00010094 @ 0x00010094 -> fpga_mem_release  (reversed/ko_fpga_mem/fpga_mem_release.c)
 * 原 FUN_0001009c @ 0x0001009c -> init_module       (reversed/ko_fpga_mem/init_module.c)
 * 原 FUN_00010254 @ 0x00010254 -> cleanup_module    (reversed/ko_fpga_mem/cleanup_module.c)
 */
