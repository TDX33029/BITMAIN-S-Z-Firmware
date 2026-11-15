//==================== FUN_000570d4 @ 000570d4 ====================

undefined4 FUN_000570d4(uint param_1,int param_2,int param_3,code *param_4,undefined4 param_5)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  
  if ((param_1 & 0x1f) == 0) {
    if ((param_3 != 0) && ((param_1 & 0x20) == 0)) {
      uVar3 = (*param_4)(" ",1,param_5);
      return uVar3;
    }
  }
  else {
    iVar1 = (*param_4)("\n",1,param_5);
    if (iVar1 != 0) {
      return 0xffffffff;
    }
    if (0 < param_2) {
      iVar1 = 0;
      do {
        iVar1 = iVar1 + 1;
        iVar2 = (*param_4)("                                ",param_1 & 0x1f,param_5);
        if (iVar2 != 0) {
          return 0xffffffff;
        }
      } while (iVar1 != param_2);
    }
  }
  return 0;
}

