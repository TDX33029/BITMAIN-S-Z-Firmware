//==================== FUN_00005194 @ 00005194 ====================

int FUN_00005194(undefined4 param_1,int param_2)

{
  int iVar1;
  undefined4 local_44 [2];
  undefined1 auStack_3c [24];
  int local_24;
  undefined1 auStack_20 [15];
  char local_11;
  
  local_44[0] = param_1;
  FUN_000027e0(auStack_3c,0,0x2c);
  iVar1 = FUN_000030bc(local_44,auStack_3c,0);
  if ((iVar1 == 0) && (iVar1 = FUN_00003f2c(auStack_3c,local_44[0]), iVar1 == 0)) {
    if (local_11 < '\0') {
      iVar1 = 6;
    }
    else if ((param_2 != 0) && (*(undefined1 *)(param_2 + 9) = 0, local_24 != 0)) {
      FUN_00002c30(auStack_20,param_2);
    }
  }
  return iVar1;
}

