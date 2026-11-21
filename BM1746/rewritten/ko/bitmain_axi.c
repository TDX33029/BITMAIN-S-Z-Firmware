/**
 * bitmain_axi.c — bitmain_axi.ko 语义化转写 (模块级总览)
 *
 * 原始模块 : rootfs_1/lib/modules/bitmain_axi.ko (源文件 axi_fpga.c)
 *            ARM EABI5 可重定位, vermagic 4.6.0-xilinx-g03c746f7, Dual BSD/GPL
 * 设备     : 字符设备 "axi_fpga_dev" — alloc_chrdev_region(&axi_fpga_dev_num, 0, 1, "axi_fpga_dev")
 *            动态主设备号, baseminor=0, 仅 1 个次设备号; class "axi_fpga_dev";
 *            用户节点 /dev/axi_fpga_dev 由 device_create 自动创建
 * mmap 行为: remap_pfn_range(vma, vma->vm_start, pfn=0x40000, vma->vm_end - vma->vm_start, prot)
 *            → 物理窗口 0x40000000 (AXI-FPGA PL 寄存器基址, 见 include/fpga_axi.h 窗口语义);
 *            页属性 = 原 prot & ~0x3C (清 ARM L_PTE_MT_MASK → 内存类型 UNCACHED, 强序非缓存);
 *            vma 标志 |= VM_IO | VM_DONTEXPAND | VM_DONTDUMP (0x04044000, Linux 4.6 编号)
 * 窗口大小 : request_mem_region/ioremap 申请 0x1400 字节;
 *            用户态 (cgminer) 实际使用前 0x1200 字节寄存器区 (fpga_axi.h AXI_WINDOW_SIZE)
 * 覆盖     : 5 个函数 (见文件末尾覆盖率表)。本模块与 ko_fpga_mem 的反编译地址空间重叠
 *            (如 0x00010024/0x0001002c 两边都有), 以各横幅"来源"行区分归属。
 */

#include "../include/common_types.h"
#include "../include/fpga_axi.h"

/* ==================== 本文件常量 ==================== */
#define AXI_FPGA_PHYS_BASE   0x40000000u /* AXI-FPGA PL 寄存器窗口物理基址 */
#define AXI_FPGA_REGION_SIZE 0x1400      /* request_mem_region/ioremap 申请长度 */

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
static dev_t axi_fpga_dev_num;             /* 动态分配的设备号 */
static struct cdev *p_axi_fpga_dev;        /* kmalloc 的字符设备对象 */
static struct resource *base_vir_mem_addr; /* request_mem_region 返回的资源 */
static uint32_t *base_vir_addr;            /* ioremap 后寄存器窗口虚拟地址 (语义 void __iomem *) */
static struct class *axi_fpga_class;       /* class_create 返回 */

/* 前向声明 (file_operations 初始化器引用) */
int axi_fpga_dev_open(struct inode *inode, struct file *filp);
int axi_fpga_dev_mmap(struct file *filp, struct vm_area_struct *vma);
int axi_fpga_dev_release(struct inode *inode, struct file *filp);

/* fops 对象 (二进制 .rodata 符号 axi_fpga_dev_fops):
 * 重定位实证仅 4 个槽位被赋值 — owner@0x00 <- __this_module,
 * mmap@0x28, open@0x2c, release@0x34; 其余成员全为 NULL。 */
static const struct file_operations axi_fpga_dev_fops = {
	.owner   = THIS_MODULE,
	.mmap    = axi_fpga_dev_mmap,
	.open    = axi_fpga_dev_open,
	.release = axi_fpga_dev_release,
};

// === axi_fpga_dev_open === 原 FUN_00010024 @ 0x00010024
// 来源: reversed/ko_bitmain_axi/axi_fpga_dev_open.c | 分类: ko_axi
// 语义: file_operations.open — 无任何初始化动作, 直接成功返回 0。
//       (inode/filp 为内核回调签名的形参, 二进制函数体未使用)
int axi_fpga_dev_open(struct inode *inode, struct file *filp)
{
	return 0;
}

// === axi_fpga_dev_mmap === 原 FUN_0001002c @ 0x0001002c
// 来源: reversed/ko_bitmain_axi/axi_fpga_dev_mmap.c | 分类: ko_axi
// 语义: 把用户 vma 映射到 AXI-FPGA 寄存器窗口 (物理 0x40000000, pfn=0x40000):
//       vma 标志加 VM_IO|VM_DONTEXPAND|VM_DONTDUMP, 页属性清内存类型域
//       (UNCACHED 强序非缓存, 保证寄存器读写不被 cache 缓存);
//       映射长度由用户态 mmap 的 vma 决定。成功 0, 失败 -11。
int axi_fpga_dev_mmap(struct file *filp, struct vm_area_struct *vma)
{
	int ret;

	/* vm_flags |= 0x04044000 (vm_area_struct+0x28) */
	vma->vm_flags |= VM_IO | VM_DONTEXPAND | VM_DONTDUMP;
	/* vm_page_prot = 原 prot & ~0x3C (vm_area_struct+0x24): 清 L_PTE_MT_MASK → 强序非缓存 */
	vma->vm_page_prot = vma->vm_page_prot & ~ARM_L_PTE_MT_MASK;

	/* pfn = 0x40000, 即物理 0x40000000 >> PAGE_SHIFT; 长度 = vma->vm_end - vma->vm_start */
	ret = remap_pfn_range(vma, vma->vm_start, AXI_FPGA_PHYS_BASE >> PAGE_SHIFT,
			      vma->vm_end - vma->vm_start, vma->vm_page_prot);
	if (ret != 0) {
		printk("axi_fpga_dev_mmap error!\n");
		return -11; /* 原码 0xfffffff5 */
	}
	return 0;
}

// === axi_fpga_dev_release === 原 FUN_00010088 @ 0x00010088
// 来源: reversed/ko_bitmain_axi/axi_fpga_dev_release.c | 分类: ko_axi
// 语义: file_operations.release — 无清理动作, 直接成功返回 0。
int axi_fpga_dev_release(struct inode *inode, struct file *filp)
{
	return 0;
}

// === init_module === 原 FUN_00010090 @ 0x00010090
// 来源: reversed/ko_bitmain_axi/init_module.c | 分类: ko_axi
// 语义: 模块加载入口 — 注册字符设备并建立 /dev/axi_fpga_dev:
//       1) alloc_chrdev_region 申请设备号;
//       2) kmalloc cdev 并 cdev_init/cdev_add (owner=THIS_MODULE@0x24);
//       3) request_mem_region(0x40000000, 0x1400, "axi_fpga_vir_mem") + ioremap;
//       4) 读首个寄存器字并打印 (前面插 DSB 全系统同步屏障);
//       5) class_create + device_create 建 /dev/axi_fpga_dev。
//       返回: 0 成功; 设备号失败返回其负错误码; kmalloc/请求区失败 1;
//             ioremap 失败 -1; class 失败 -1。失败路径不做回滚清理 (原码如此)。
int init_module(void)
{
	int ret;

	printk("In axi fpga driver!\n");
	ret = alloc_chrdev_region(&axi_fpga_dev_num, 0, 1, "axi_fpga_dev");
	if (ret < 0) {
		printk("alloc axi_fpga_dev fail!\n");
		return ret;
	}

	/* 原始形态: kmem_cache_alloc(_DAT_0001105c (即 kmalloc_caches[kmalloc-64]), GFP_KERNEL)
	 * — kmalloc(sizeof(struct cdev)) 的内联展开 */
	p_axi_fpga_dev = kmalloc(sizeof(struct cdev), GFP_KERNEL);
	if (p_axi_fpga_dev == NULL) {
		printk("kmalloc cdev fail!\n");
		return 1;
	}

	cdev_init(p_axi_fpga_dev, &axi_fpga_dev_fops);
	p_axi_fpga_dev->owner = THIS_MODULE; /* cdev+0x24 <- &__this_module */
	ret = cdev_add(p_axi_fpga_dev, axi_fpga_dev_num, 1);
	if (ret != 0) {
		printk("add axi_fpga_dev fail!\n");
		return ret;
	}

	base_vir_mem_addr = request_mem_region(AXI_FPGA_PHYS_BASE, AXI_FPGA_REGION_SIZE,
					       "axi_fpga_vir_mem");
	if (base_vir_mem_addr == NULL) {
		printk("request_mem_region failed!\n");
		return 1;
	}
	printk("request_mem_region OK!\n");

	base_vir_addr = ioremap(AXI_FPGA_PHYS_BASE, AXI_FPGA_REGION_SIZE);
	if (base_vir_addr == NULL) {
		printk("ioremap fail!\n"); /* 原反编译带无用实参 0 (寄存器残留), 省略 */
		printk("!!!*base_vir_addr = 0x%x\n", base_vir_addr); /* 打印的是指针值而非内容 */
		return -1;
	}
	printk("AXI fpga dev virtual address is 0x%x\n", base_vir_addr);

	DataSynchronizationBarrier(0xf); /* ARM DSB 指令 (option 0xF=SY 全系统), 读 MMIO 前的同步屏障 */
	printk("*base_vir_addr = 0x%x\n", *base_vir_addr); /* 直接读寄存器窗口首字 */

	/* 原码 __class_create(&__this_module, "axi_fpga_dev", <静态 lock_class_key>) —
	 * 第三实参为 lock_class_key, 反编译中与 axi_fpga_class 符号重叠显示 */
	axi_fpga_class = class_create(THIS_MODULE, "axi_fpga_dev");
	if ((unsigned long)axi_fpga_class < 0xfffff001u) { /* 非 IS_ERR: 错误指针值域 0xfffff001..0xffffffff */
		device_create(axi_fpga_class, NULL, axi_fpga_dev_num, NULL, "axi_fpga_dev");
		return 0;
	}
	printk("Err:failed in creating axi fpga class.\n");
	return -1;
}

// === cleanup_module === 原 FUN_0001023c @ 0x0001023c
// 来源: reversed/ko_bitmain_axi/cleanup_module.c | 分类: ko_axi
// 语义: 模块卸载入口 — 逆序释放 init_module 申请的全部资源
//       (设备号/cdev/ioremap/内存区/class+设备节点), 无条件全部执行。
void cleanup_module(void)
{
	unregister_chrdev_region(axi_fpga_dev_num, 1);
	cdev_del(p_axi_fpga_dev);
	kfree(p_axi_fpga_dev);
	iounmap(base_vir_addr);
	release_mem_region(AXI_FPGA_PHYS_BASE, AXI_FPGA_REGION_SIZE); /* 原码 __release_region(&iomem_resource, 0x40000000, 0x1400) */
	device_destroy(axi_fpga_class, axi_fpga_dev_num);
	class_destroy(axi_fpga_class);
	printk("Bye Bye axi fpga driver!\n");
}

/* ==================== 覆盖率表 (5/5) ====================
 * 原 FUN_00010024 @ 0x00010024 -> axi_fpga_dev_open     (reversed/ko_bitmain_axi/axi_fpga_dev_open.c)
 * 原 FUN_0001002c @ 0x0001002c -> axi_fpga_dev_mmap     (reversed/ko_bitmain_axi/axi_fpga_dev_mmap.c)
 * 原 FUN_00010088 @ 0x00010088 -> axi_fpga_dev_release  (reversed/ko_bitmain_axi/axi_fpga_dev_release.c)
 * 原 FUN_00010090 @ 0x00010090 -> init_module           (reversed/ko_bitmain_axi/init_module.c)
 * 原 FUN_0001023c @ 0x0001023c -> cleanup_module        (reversed/ko_bitmain_axi/cleanup_module.c)
 */
