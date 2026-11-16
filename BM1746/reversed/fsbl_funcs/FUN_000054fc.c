//==================== FUN_000054fc @ 000054fc ====================

int FUN_000054fc(undefined4 param_1)

{
  int iVar1;
  uint uVar2;
  undefined4 local_6c [2];
  char *local_64;
  char *local_60;
  byte local_5a;
  int local_44;
  char local_35;
  char *local_34 [2];
  uint local_2c;
  
  local_6c[0] = param_1;
  iVar1 = FUN_000030bc(local_6c,&local_64,2);
  if (iVar1 != 0) {
    return iVar1;
  }
  local_60 = local_64;
  iVar1 = FUN_00003f2c(&local_60,local_6c[0]);
  if (iVar1 != 0) {
    return iVar1;
  }
  if (local_35 < '\0') {
    return 6;
  }
  if ((local_5a & 1) == 0) {
    uVar2 = (uint)*(ushort *)(local_44 + 0x1a);
    if (*local_64 == '\x03') {
      uVar2 = CONCAT22(*(undefined2 *)(local_44 + 0x14),*(ushort *)(local_44 + 0x1a));
    }
    if ((local_5a & 0x10) != 0) {
      local_34[0] = local_64;
      local_2c = uVar2;
      iVar1 = FUN_00003d50(local_34);
      if (iVar1 != 0) {
        return iVar1;
      }
      iVar1 = FUN_00003c84(local_34);
      if (iVar1 == 0) goto LAB_00005560;
      if (iVar1 != 4) {
        return iVar1;
      }
    }
    iVar1 = FUN_00002f8c(&local_60);
    if (iVar1 == 0 && uVar2 != 0) {
      iVar1 = FUN_00003a48(&local_60,uVar2,0);
    }
    if (iVar1 == 0) {
      iVar1 = FUN_00002e20(local_64);
    }
  }
  else {
LAB_00005560:
    iVar1 = 7;
  }
  return iVar1;
}

