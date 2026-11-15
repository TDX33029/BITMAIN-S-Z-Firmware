//==================== FUN_00002fd8 @ 00002fd8 ====================

undefined4 FUN_00002fd8(int param_1,int param_2)

{
  byte bVar1;
  undefined4 uVar2;
  byte *pbVar3;
  byte *pbVar4;
  byte *pbVar5;
  int iVar6;
  bool bVar7;
  
  *(undefined1 *)(param_1 + 3) = 0;
  *(undefined4 *)(param_1 + 0x2c) = 0xffffffff;
  if ((param_2 != -1) && (iVar6 = FUN_00002f3c(), iVar6 != 0)) {
    return 4;
  }
  if (*(short *)(param_1 + 0x23e) != -0x55ab) {
    return 3;
  }
  bVar1 = *(byte *)(param_1 + 0x40);
  bVar7 = 0xea < bVar1;
  if (bVar1 != 0xeb) {
    bVar7 = bVar1 != 0xe8;
  }
  if (!bVar7 || (bVar1 == 0xeb || bVar1 == 0xe9)) {
    if (((*(char *)(param_1 + 0x76) == 'F') && (*(char *)(param_1 + 0x77) == 'A')) &&
       (*(char *)(param_1 + 0x78) == 'T')) {
      return 0;
    }
    pbVar3 = (byte *)0x17244;
    iVar6 = *(byte *)(param_1 + 0x92) - 0x46;
    pbVar4 = (byte *)(param_1 + 0x93);
    while (iVar6 == 0) {
      pbVar5 = pbVar4 + 1;
      pbVar3 = pbVar3 + 1;
      iVar6 = (uint)*pbVar4 - (uint)*pbVar3;
      pbVar4 = pbVar5;
      if (pbVar5 == (byte *)(param_1 + 0x97)) {
        if (iVar6 == 0) {
          uVar2 = 0;
        }
        else {
          uVar2 = 2;
        }
        return uVar2;
      }
    }
  }
  return 2;
}

