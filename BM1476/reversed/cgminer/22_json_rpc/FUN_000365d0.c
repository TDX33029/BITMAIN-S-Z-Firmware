//==================== FUN_000365d0 @ 000365d0 ====================

void FUN_000365d0(int param_1,undefined4 param_2,undefined4 param_3,int param_4,undefined1 param_5)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  if (DAT_0007b720 == 0) {
    FUN_0003305c(param_1,10,0,0);
    return;
  }
  FUN_0003305c(param_1,0x3c,0,0,param_4);
  if (param_4 == 0) {
    iVar2 = 0;
  }
  else {
    iVar2 = FUN_000307c8(param_1,",\"NOTIFY\":[");
  }
  if (0 < DAT_0007b720) {
    iVar3 = 0;
    do {
      uVar1 = FUN_0001b83c(iVar3);
      iVar4 = iVar3 + 1;
      FUN_000363fc(param_1,iVar3,uVar1,param_4,param_5);
      iVar3 = iVar4;
    } while (iVar4 < DAT_0007b720);
  }
  if (iVar2 != 0) {
    *(undefined1 *)(param_1 + 0xd) = 1;
  }
  return;
}

