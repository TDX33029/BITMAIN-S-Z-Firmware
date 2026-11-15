//==================== FUN_00033b88 @ 00033b88 ====================

void FUN_00033b88(undefined4 param_1,undefined4 param_2,byte *param_3)

{
  byte bVar1;
  __int32_t **pp_Var2;
  
  if ((param_3 == (byte *)0x0) || (bVar1 = *param_3, bVar1 == 0)) {
    FUN_0003305c(param_1,0x4b,0,0);
    return;
  }
  pp_Var2 = __ctype_tolower_loc();
  bVar1 = *(byte *)(*pp_Var2 + bVar1);
  *param_3 = bVar1;
  if (bVar1 == 0x66 || bVar1 == 0x74) {
    DAT_0007b625 = bVar1 == 0x74;
    FUN_0003305c(param_1,0x4d,DAT_0007b625,0);
    return;
  }
  FUN_0003305c(param_1,0x4c,0,0);
  return;
}

