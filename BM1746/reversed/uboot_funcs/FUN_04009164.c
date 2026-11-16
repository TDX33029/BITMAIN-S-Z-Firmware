//==================== FUN_04009164 @ 04009164 ====================

int FUN_04009164(void)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  int *piVar5;
  
  iVar2 = func_0x03fbefb0(DAT_0400924c);
  iVar3 = func_0x03fbefb0(DAT_04009250);
  if ((iVar3 == 0) || (iVar3 = FUN_04005a60(iVar3,DAT_04009254), iVar2 == 0 || iVar3 != 0)) {
    piVar4 = (int *)FUN_04008f68();
    if (piVar4 == (int *)0x0) {
      FUN_04007548(DAT_04009258);
      return -0x13;
    }
  }
  else {
    piVar4 = (int *)FUN_04009008(iVar2);
    if (piVar4 == (int *)0x0) {
      return -0x16;
    }
  }
  piVar1 = DAT_0400925c;
  iVar2 = -0x13;
  piVar5 = piVar4;
  do {
    if (piVar5 != (int *)0x0) {
      if ((piVar5[0x12] & 1U) == 0) {
        iVar2 = *piVar1;
      }
      else {
        iVar2 = (*(code *)**(undefined4 **)(*piVar5 + 0x3c))(piVar5);
        if (-1 < iVar2) {
          *(undefined4 *)piVar5[10] = 2;
          return 0;
        }
      }
    }
    FUN_040097e4(0);
    piVar5 = (int *)FUN_04008f68();
  } while (piVar4 != piVar5);
  return iVar2;
}

