//==================== FUN_0003bd44 @ 0003bd44 ====================

int FUN_0003bd44(undefined4 param_1,undefined1 *param_2,uint param_3)

{
  uint uVar1;
  undefined1 uVar3;
  undefined1 uVar2;
  int iVar4;
  int iVar5;
  undefined1 *puVar6;
  int iVar7;
  undefined4 uVar8;
  
  switch(param_1) {
  case 0:
    iVar5 = 0x210;
    iVar7 = 0x85;
    break;
  case 1:
    iVar5 = 0x218;
    iVar7 = 0x87;
    break;
  case 2:
    iVar5 = 0x220;
    iVar7 = 0x89;
    break;
  case 3:
    iVar5 = 0x228;
    iVar7 = 0x8b;
    break;
  case 4:
    iVar5 = 0x230;
    iVar7 = 0x8d;
    break;
  case 5:
    iVar5 = 0x238;
    iVar7 = 0x8f;
    break;
  case 6:
    iVar5 = 0x240;
    iVar7 = 0x91;
    break;
  case 7:
    iVar5 = 0x248;
    iVar7 = 0x93;
    break;
  case 8:
    iVar5 = 0x250;
    iVar7 = 0x95;
    break;
  case 9:
    iVar5 = 600;
    iVar7 = 0x97;
    break;
  default:
    printf("%s: The uart%d is not supported!!!\n","read_uart_data_in_fpga",param_3);
    return 0;
  }
  uVar1 = param_3 >> 2;
  *(uint *)(DAT_00078e10 + iVar5) = param_3 & 0x3ff | 0x80000000;
  if (uVar1 == 0) {
    iVar5 = 0;
  }
  else {
    iVar5 = uVar1 * 4;
    puVar6 = param_2;
    do {
      uVar8 = *(undefined4 *)(DAT_00078e10 + iVar7 * 4);
      puVar6[3] = (char)uVar8;
      *puVar6 = (char)((uint)uVar8 >> 0x18);
      puVar6[1] = (char)((uint)uVar8 >> 0x10);
      puVar6[2] = (char)((uint)uVar8 >> 8);
      puVar6 = puVar6 + 4;
    } while (puVar6 != param_2 + iVar5);
  }
  param_3 = param_3 & 3;
  iVar4 = iVar5;
  if (param_3 != 0) {
    uVar8 = *(undefined4 *)(DAT_00078e10 + iVar7 * 4);
    uVar2 = (undefined1)((uint)uVar8 >> 0x10);
    uVar3 = (undefined1)((uint)uVar8 >> 0x18);
    if (param_3 == 2) {
      iVar4 = iVar5 + 2;
      param_2[uVar1 * 4] = uVar3;
      param_2[iVar5 + 1] = uVar2;
    }
    else if (param_3 == 3) {
      param_2[uVar1 * 4] = uVar3;
      iVar4 = iVar5 + 3;
      param_2[iVar5 + 1] = uVar2;
      param_2[iVar5 + 2] = (char)((uint)uVar8 >> 8);
    }
    else {
      iVar4 = iVar5 + 1;
      param_2[uVar1 * 4] = uVar3;
    }
  }
  return iVar4;
}

