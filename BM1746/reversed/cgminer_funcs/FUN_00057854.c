//==================== FUN_00057854 @ 00057854 ====================

undefined4 FUN_00057854(undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  undefined4 uVar2;
  undefined1 auStack_1c [12];
  
  iVar1 = FUN_0005961c(auStack_1c);
  if (iVar1 == 0) {
    iVar1 = FUN_000577a4(param_1,&LAB_000570bc,auStack_1c,param_2);
    uVar2 = 0;
    if (iVar1 == 0) {
      FUN_00059694(auStack_1c);
      uVar2 = FUN_000595b4();
    }
    FUN_00059654(auStack_1c);
  }
  else {
    uVar2 = 0;
  }
  return uVar2;
}

