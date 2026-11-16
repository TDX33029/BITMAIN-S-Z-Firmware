//==================== FUN_00013808 @ 00013808 ====================

undefined4 FUN_00013808(int param_1,undefined4 *param_2)

{
  uint uVar1;
  undefined1 auStack_b10 [256];
  undefined1 auStack_a10 [2048];
  undefined4 local_210;
  undefined1 auStack_20c [124];
  undefined4 local_190;
  undefined1 auStack_18c [124];
  undefined4 local_110;
  undefined1 auStack_10c [252];
  undefined4 local_10;
  char local_b;
  char local_a;
  undefined1 local_9;
  
  if (param_1 < 2) {
    FUN_000108f8(*param_2);
    local_10 = 1;
  }
  else if (*(char *)param_2[1] == '-') {
    local_110 = 0;
    thunk_EXT_FUN_756e752e(auStack_10c,0,0xfc);
    local_190 = 0;
    thunk_EXT_FUN_756e752e(auStack_18c,0,0x7c);
    local_210 = 0;
    thunk_EXT_FUN_756e752e(auStack_20c,0,0x7c);
    thunk_EXT_FUN_756e752e(auStack_a10,0,0x800);
    local_9 = 0;
    local_a = '\0';
    local_b = '\0';
    thunk_EXT_FUN_756e752e(auStack_b10,0,0xfd);
    if (param_1 == 5) {
      uVar1 = FUN_00010780(param_2[2]);
      if (0xff < uVar1) {
        FUN_000107f0("fileName Too Long!");
        return 3;
      }
      FUN_000107b0(&local_110,param_2[2]);
      uVar1 = FUN_00010780(param_2[3]);
      if (0x7f < uVar1) {
        FUN_000107f0("fileName Too Long!");
        return 3;
      }
      FUN_000107b0(&local_190,param_2[3]);
      uVar1 = FUN_00010780(param_2[4]);
      if (0x7f < uVar1) {
        FUN_000107f0("pemName Too Long!");
        return 4;
      }
      FUN_000107b0(&local_210,param_2[4]);
    }
    else if (param_1 == 3) {
      uVar1 = FUN_00010780(param_2[2]);
      if (0x7f < uVar1) {
        FUN_000107f0("fileName Too Long!");
        return 3;
      }
      FUN_000107b0(&local_190,param_2[2]);
    }
    else if (param_1 != 2) {
      FUN_000107f0("Param Err!");
      FUN_000108f8(*param_2);
      return 3;
    }
    local_10 = 0;
    switch(*(undefined1 *)(param_2[1] + 1)) {
    default:
      FUN_000107f0("Command Not Support!");
      FUN_000108f8(*param_2);
      local_10 = 5;
      break;
    case 0x6e:
      local_10 = FUN_000109a8(&local_190);
      break;
    case 0x71:
      local_10 = FUN_00010b9c();
      break;
    case 0x78:
      local_b = '\x01';
    case 0x70:
      if (local_b == '\0') {
        local_a = '\x01';
      }
    case 0x73:
      if (local_a == '\0') {
        local_9 = 1;
      }
    case 0x66:
      local_10 = FUN_00012c40(&local_190,&local_110,&local_210,auStack_a10,local_9,local_a,local_b);
    }
  }
  else {
    FUN_000108f8(*param_2);
    local_10 = 2;
  }
  return local_10;
}

