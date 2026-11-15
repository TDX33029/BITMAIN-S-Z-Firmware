//==================== FUN_00010b9c @ 00010b9c ====================

undefined4 FUN_00010b9c(void)

{
  undefined4 uVar1;
  undefined4 local_110;
  undefined1 auStack_10c [252];
  int local_10;
  int local_c;
  
  local_10 = FUN_00010798("/tmp/256BFF",&DAT_00013f24);
  if (local_10 == 0) {
    FUN_000107f0("GenFile Failed!");
    uVar1 = 1;
  }
  else {
    local_110 = 0;
    thunk_EXT_FUN_756e752e(auStack_10c,0,0xfc);
    for (local_c = 0; local_c < 0x100; local_c = local_c + 1) {
      auStack_10c[local_c + -4] = 0xff;
    }
    FUN_000107d4(&local_110,0x100,1,local_10);
    thunk_EXT_FUN_706d742e(local_10);
    uVar1 = 0;
  }
  return uVar1;
}

