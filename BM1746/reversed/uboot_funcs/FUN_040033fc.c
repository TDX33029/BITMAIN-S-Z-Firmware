//==================== FUN_040033fc @ 040033fc ====================

uint FUN_040033fc(undefined4 param_1,int param_2,int *param_3,int *param_4)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  
  uVar4 = param_2 + 1;
  uVar1 = FUN_04005b98();
  uVar5 = param_4[1];
  if (uVar4 < uVar5) {
    iVar3 = uVar4 * 0x14;
    do {
      if (0 < *(int *)(*param_4 + iVar3)) {
        iVar2 = FUN_04005a90(param_1,*(undefined4 *)(*param_4 + iVar3 + 4),uVar1);
        if (iVar2 == 0) {
          *param_3 = *param_4 + iVar3 + 4;
          return uVar4;
        }
        uVar5 = param_4[1];
      }
      uVar4 = uVar4 + 1;
      iVar3 = iVar3 + 0x14;
    } while (uVar4 < uVar5);
  }
  *DAT_040034a4 = 3;
  *param_3 = 0;
  return 0;
}

