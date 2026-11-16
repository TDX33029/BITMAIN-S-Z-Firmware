//==================== FUN_0400929c @ 0400929c ====================

undefined4 FUN_0400929c(undefined4 param_1,undefined4 param_2)

{
  int *piVar1;
  undefined4 uVar2;
  
  piVar1 = (int *)FUN_04008f68();
  if (piVar1 == (int *)0x0) {
    return 0xffffffed;
  }
  if ((piVar1[0x12] & 1U) == 0) {
    return 0xffffffea;
  }
                    /* WARNING: Could not recover jumptable at 0x040092d8. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  uVar2 = (**(code **)(*(int *)(*piVar1 + 0x3c) + 4))(piVar1,param_1,param_2);
  return uVar2;
}

