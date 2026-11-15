//==================== FUN_000109a8 @ 000109a8 ====================

undefined4 FUN_000109a8(undefined4 param_1)

{
  undefined4 uVar1;
  undefined4 local_8014;
  undefined1 auStack_8010 [32764];
  int local_14;
  int local_10;
  int local_c;
  
  local_8014 = 0;
  thunk_EXT_FUN_756e752e(auStack_8010,0,0x7ffc);
  local_10 = FUN_00010798(param_1,&DAT_00013f00);
  if (local_10 == 0) {
    FUN_000107f0("Load Nand Image File Failed!");
    uVar1 = 1;
  }
  else {
    for (local_c = 0; local_c < 0xf; local_c = local_c + 1) {
      FUN_0001082c(&local_8014,*(undefined4 *)(&DAT_0002507c + local_c * 0x84),1,local_10);
      local_14 = FUN_00010798(local_c * 0x84 + 0x25080,&DAT_00013f24);
      if (local_14 == 0) {
        FUN_000107bc("Try To Write To File \'%s\' Failed!\n",local_c * 0x84 + 0x25080);
        if (local_10 != 0) {
          thunk_EXT_FUN_706d742e(local_10);
        }
        return 2;
      }
      FUN_000107d4(&local_8014,*(undefined4 *)(&DAT_0002507c + local_c * 0x84),1,local_14);
      if (local_14 != 0) {
        thunk_EXT_FUN_706d742e(local_14);
      }
      local_14 = 0;
    }
    if (local_10 != 0) {
      thunk_EXT_FUN_706d742e(local_10);
    }
    uVar1 = 0;
  }
  return uVar1;
}

