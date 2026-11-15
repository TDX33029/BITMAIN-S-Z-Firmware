//==================== FUN_0005a3e4 @ 0005a3e4 ====================

void FUN_0005a3e4(int *param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  
  if (param_1 == (int *)0x0) {
    return;
  }
  iVar4 = *param_1;
  if (iVar4 == 0) {
    FUN_00057a74(param_1 + 2);
    FUN_00059594(param_1);
    return;
  }
  if (iVar4 == 1) {
    uVar2 = param_1[3];
    iVar4 = param_1[4];
    if (uVar2 != 0) {
      uVar5 = 0;
      do {
        iVar1 = *(int *)(iVar4 + uVar5 * 4);
        uVar5 = uVar5 + 1;
        if (((iVar1 != 0) && (iVar3 = *(int *)(iVar1 + 4) + -1, *(int *)(iVar1 + 4) != -1)) &&
           (*(int *)(iVar1 + 4) = iVar3, iVar3 == 0)) {
          FUN_0005a3e4();
          iVar4 = param_1[4];
          uVar2 = param_1[3];
        }
      } while (uVar5 < uVar2);
    }
    FUN_00059594(iVar4);
    FUN_00059594(param_1);
    return;
  }
  if (iVar4 == 2) {
    FUN_00059594(param_1[2]);
    FUN_00059594(param_1);
    return;
  }
  if ((iVar4 == 3) || (iVar4 == 4)) {
    FUN_00059594(param_1,param_2,param_3,param_4);
    return;
  }
  return;
}

