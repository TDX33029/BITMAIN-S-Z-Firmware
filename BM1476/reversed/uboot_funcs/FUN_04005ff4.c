//==================== FUN_04005ff4 @ 04005ff4 ====================

int FUN_04005ff4(byte *param_1,byte *param_2,int param_3)

{
  int iVar1;
  code *UNRECOVERED_JUMPTABLE;
  byte *pbVar2;
  
  if (param_3 == 0) {
    return 0;
  }
  if ((uint)*param_1 - (uint)*param_2 != 0) {
    return (uint)*param_1 - (uint)*param_2;
  }
  pbVar2 = param_2 + param_3 + -1;
  do {
    if (param_2 == pbVar2) {
      return 0;
    }
    param_1 = param_1 + 1;
    param_2 = param_2 + 1;
  } while ((uint)*param_1 - (uint)*param_2 == 0);
                    /* WARNING: Could not recover jumptable at 0x04006038. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  iVar1 = (*UNRECOVERED_JUMPTABLE)((uint)*param_1 - (uint)*param_2);
  return iVar1;
}

