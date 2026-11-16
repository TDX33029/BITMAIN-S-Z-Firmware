//==================== FUN_0000c634 @ 0000c634 ====================

uint FUN_0000c634(int param_1,int param_2)

{
  __int32_t **pp_Var1;
  byte *pbVar2;
  byte *pbVar3;
  int iVar4;
  int iVar5;
  
  if (param_1 == 0) {
    return (uint)(param_2 != 0);
  }
  if (param_2 == 0) {
    return 1;
  }
  pp_Var1 = __ctype_tolower_loc();
  pbVar3 = (byte *)(param_1 + -1);
  pbVar2 = (byte *)(param_2 + -1);
  do {
    pbVar3 = pbVar3 + 1;
    pbVar2 = pbVar2 + 1;
    iVar4 = (*pp_Var1)[*pbVar3];
    iVar5 = (*pp_Var1)[*pbVar2];
    if (iVar4 != iVar5) {
      return iVar4 - iVar5;
    }
  } while (*pbVar3 != 0);
  return 0;
}

