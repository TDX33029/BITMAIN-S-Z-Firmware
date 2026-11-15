//==================== FUN_0003bb78 @ 0003bb78 ====================

uint FUN_0003bb78(int param_1)

{
  uint uVar1;
  
  switch(param_1) {
  case 0:
    uVar1 = (*(uint *)(DAT_00078e10 + 0x1f0) & 0x3ffffff) >> 0x10;
    break;
  case 1:
    uVar1 = *(uint *)(DAT_00078e10 + 0x1f0) & 0x3ff;
    break;
  case 2:
    uVar1 = (*(uint *)(DAT_00078e10 + 500) & 0x3ffffff) >> 0x10;
    break;
  case 3:
    uVar1 = *(uint *)(DAT_00078e10 + 500) & 0x3ff;
    break;
  case 4:
    uVar1 = (*(uint *)(DAT_00078e10 + 0x1f8) & 0x3ffffff) >> 0x10;
    break;
  case 5:
    uVar1 = *(uint *)(DAT_00078e10 + 0x1f8) & 0x3ff;
    break;
  case 6:
    uVar1 = (*(uint *)(DAT_00078e10 + 0x1fc) & 0x3ffffff) >> 0x10;
    break;
  case 7:
    uVar1 = *(uint *)(DAT_00078e10 + 0x1fc) & 0x3ff;
    break;
  case 8:
    uVar1 = (*(uint *)(DAT_00078e10 + 0x200) & 0x3ffffff) >> 0x10;
    break;
  case 9:
    uVar1 = *(uint *)(DAT_00078e10 + 0x200) & 0x3ff;
    break;
  default:
    printf("%s: The uart%d is not supported!!!\n","check_how_many_uart_data_in_fpga",param_1);
    uVar1 = 0;
  }
  return uVar1;
}

