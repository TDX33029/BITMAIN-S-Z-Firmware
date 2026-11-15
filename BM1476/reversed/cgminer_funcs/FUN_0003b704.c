//==================== FUN_0003b704 @ 0003b704 ====================

undefined4 FUN_0003b704(void)

{
  undefined4 uVar1;
  
  DAT_0007bd90 = open("/dev/axi_fpga_dev",2);
  if (DAT_0007bd90 < 0) {
    printf("/dev/axi_fpga_dev open failed. fd = %d\n",DAT_0007bd90);
    uVar1 = 0xffffffff;
  }
  else {
    DAT_00078e10 = mmap((void *)0x0,0x1200,3,1,DAT_0007bd90,0);
    if (DAT_00078e10 == (ushort *)0x0) {
      printf("mmap axi_fpga_addr failed. axi_fpga_addr = 0x%x\n",0);
      uVar1 = 0xffffffff;
    }
    else {
      printf("mmap axi_fpga_addr = 0x%x\n",(uint)DAT_00078e10);
      printf("axi_fpga_addr data = 0x%x\n",(uint)*DAT_00078e10);
      DAT_0007c0e8 = open("/dev/fpga_mem",2);
      if (DAT_0007c0e8 < 0) {
        printf("/dev/fpga_mem open failed. fd_fpga_mem = %d\n",DAT_0007c0e8);
        uVar1 = 0xffffffff;
      }
      else {
        DAT_00078eb8 = mmap((void *)0x0,0x1000000,3,1,DAT_0007c0e8,0);
        if (DAT_00078eb8 == (void *)0x0) {
          printf("mmap fpga_mem_addr failed. fpga_mem_addr = 0x%x\n",0);
          uVar1 = 0xffffffff;
        }
        else {
          printf("mmap fpga_mem_addr = 0x%x\n",(uint)DAT_00078eb8);
          uVar1 = 0;
        }
      }
    }
  }
  return uVar1;
}

