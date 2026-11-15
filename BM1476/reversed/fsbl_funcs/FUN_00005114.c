//==================== FUN_00005114 @ 00005114 ====================

uint FUN_00005114(int param_1,int param_2)

{
  uint uVar1;
  int iVar2;
  undefined1 auStack_14 [8];
  
  uVar1 = FUN_00002bbc(param_1,auStack_14);
  if (uVar1 == 0) {
    if (param_2 == 0) {
      uVar1 = FUN_00003d50(param_1);
      return uVar1;
    }
    uVar1 = FUN_00003c84();
    if ((uVar1 & 0xfb) == 0) {
      iVar2 = *(int *)(param_1 + 0x18);
      *(undefined1 *)(param_2 + 9) = 0;
      if (iVar2 != 0) {
        FUN_00002c30(param_1 + 0x1c,param_2);
      }
      uVar1 = FUN_00003b44(param_1,0);
      if (uVar1 == 4) {
        uVar1 = 0;
      }
    }
  }
  return uVar1;
}

