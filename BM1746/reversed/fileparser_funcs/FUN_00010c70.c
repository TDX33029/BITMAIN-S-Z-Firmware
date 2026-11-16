//==================== FUN_00010c70 @ 00010c70 ====================

undefined4 FUN_00010c70(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 uVar1;
  undefined1 auStack_a4 [112];
  undefined1 auStack_34 [32];
  int local_14;
  int local_10;
  int local_c;
  
  local_c = 0;
  local_10 = 0;
  local_c = FUN_000107a4(param_4,0x400);
  local_10 = FUN_00010768(local_c,0,0,0);
  if (local_10 == 0) {
    uVar1 = FUN_00010714();
    uVar1 = FUN_000107c8(uVar1,0);
    FUN_000107bc("OpenSSL error: %s\n",uVar1);
    FUN_000107f0("Read Root PubK Failed!");
    uVar1 = 0xc;
  }
  else {
    FUN_00010820(auStack_a4);
    FUN_00010808(auStack_a4,param_1,param_2);
    FUN_00010774(auStack_34,auStack_a4);
    local_14 = thunk_EXT_FUN_00646572(0x2a0,auStack_34,0x20,param_3,0x100,local_10);
    if (local_10 != 0) {
      FUN_000107fc(local_10);
    }
    if (local_c != 0) {
      FUN_00010814(local_c);
    }
    if (local_14 == 1) {
      uVar1 = 0;
    }
    else {
      uVar1 = FUN_00010714();
      uVar1 = FUN_000107c8(uVar1,0);
      FUN_000107bc("OpenSSL error: %s\n",uVar1);
      FUN_000107f0("Check miner.pem Failed!");
      uVar1 = 0xd;
    }
  }
  return uVar1;
}

