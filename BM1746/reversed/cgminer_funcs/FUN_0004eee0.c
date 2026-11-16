//==================== FUN_0004eee0 @ 0004eee0 ====================

void FUN_0004eee0(undefined4 param_1,int param_2,undefined4 param_3)

{
  int iVar1;
  undefined1 local_50 [2];
  undefined1 local_4e;
  undefined1 local_4d;
  char local_20 [8];
  int local_18;
  undefined4 local_14;
  
  memset(local_50,0,0x40);
  local_4e = 1;
  local_4d = 1;
  iVar1 = FUN_0005c710(0x200,param_2);
  iVar1 = param_2 * iVar1;
  builtin_strncpy(local_20,"ZcashPoW",8);
  if (iVar1 < 0) {
    iVar1 = iVar1 + 7;
  }
  local_50[0] = (undefined1)(iVar1 >> 3);
  local_18 = param_2;
  local_14 = param_3;
  FUN_00054dcc(param_1,local_50);
  return;
}

