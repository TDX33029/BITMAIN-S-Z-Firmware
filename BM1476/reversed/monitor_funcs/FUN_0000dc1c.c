//==================== FUN_0000dc1c @ 0000dc1c ====================

undefined4 FUN_0000dc1c(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 extraout_r2;
  int *piVar3;
  int iVar4;
  
  piVar3 = *(int **)(param_1 + 8);
  if (piVar3 != (int *)0x0) {
    iVar4 = 0;
    do {
      iVar1 = FUN_0000c634(piVar3[8],param_2);
      if (iVar1 == 0) {
        uVar2 = FUN_0000dba4(param_1,iVar4,extraout_r2,param_4);
        return uVar2;
      }
      piVar3 = (int *)*piVar3;
      iVar4 = iVar4 + 1;
    } while (piVar3 != (int *)0x0);
  }
  return 0;
}

