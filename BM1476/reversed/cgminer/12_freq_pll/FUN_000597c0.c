//==================== FUN_000597c0 @ 000597c0 ====================

undefined1 FUN_000597c0(int *param_1)

{
  undefined1 uVar1;
  int iVar2;
  
  if (param_1[1] == 0) {
    uVar1 = 0;
  }
  else {
    iVar2 = param_1[1] + -1;
    param_1[1] = iVar2;
    uVar1 = *(undefined1 *)(*param_1 + iVar2);
    *(undefined1 *)(*param_1 + iVar2) = 0;
  }
  return uVar1;
}

