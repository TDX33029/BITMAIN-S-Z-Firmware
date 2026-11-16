//==================== FUN_00010de8 @ 00010de8 ====================

undefined4
FUN_00010de8(undefined4 param_1,int param_2,undefined4 *param_3,undefined1 param_4,char param_5,
            char param_6)

{
  int iVar1;
  char local_524 [128];
  undefined4 local_4a4;
  undefined4 local_4a0;
  undefined4 local_49c;
  undefined4 local_498;
  undefined4 local_494;
  undefined4 local_490;
  undefined4 local_48c;
  undefined4 local_488;
  undefined1 auStack_484 [112];
  undefined4 local_414;
  undefined1 auStack_410 [1020];
  int local_14;
  int local_10;
  int local_c;
  
  local_414 = 0;
  thunk_EXT_FUN_756e752e(auStack_410,0,0x3fc);
  FUN_00010820(auStack_484);
  local_c = 0;
  local_14 = 0;
  local_10 = 0;
  if (param_6 != '\0') {
    builtin_strncpy(local_524,"/tmp/tmpfw/",0xc);
    thunk_EXT_FUN_756e752e(local_524 + 0xc,0,0x74);
    switch(param_4) {
    case 0:
      iVar1 = FUN_00010780(local_524);
      builtin_strncpy(local_524 + iVar1,"BOOT.bin",9);
      break;
    case 1:
      iVar1 = FUN_00010780(local_524);
      builtin_strncpy(local_524 + iVar1,"devicetree.dtb",0xf);
      break;
    case 2:
      iVar1 = FUN_00010780(local_524);
      builtin_strncpy(local_524 + iVar1,"uImage",7);
      break;
    case 3:
      iVar1 = FUN_00010780(local_524);
      builtin_strncpy(local_524 + iVar1,"minerfs.image.gz",0x11);
      break;
    case 4:
      iVar1 = FUN_00010780(local_524);
      builtin_strncpy(local_524 + iVar1,"update.image.gz",0x10);
      break;
    case 5:
      iVar1 = FUN_00010780(local_524);
      builtin_strncpy(local_524 + iVar1,"crl.tar.gz",0xb);
      break;
    case 6:
      iVar1 = FUN_00010780(local_524);
      builtin_strncpy(local_524 + iVar1,"miner.btm.tar.gz",0x11);
      break;
    case 7:
      iVar1 = FUN_00010780(local_524);
      builtin_strncpy(local_524 + iVar1,"reserve",8);
      break;
    case 9:
      iVar1 = FUN_00010780(local_524);
      builtin_strncpy(local_524 + iVar1,"datafile",9);
    }
    if (param_5 != '\0') {
      iVar1 = FUN_00010780(local_524);
      builtin_strncpy(local_524 + iVar1,".sig",5);
    }
    FUN_000107bc("fileName:\'%s\', size:[%d]\r\n",local_524,param_2);
    local_10 = FUN_00010798(local_524,&DAT_00013f24);
    if (local_10 == 0) {
      FUN_000107bc("Create File \'%s\' Failed!\r\n",local_524);
    }
  }
  while (0x400 < (uint)(param_2 - local_c)) {
    local_14 = FUN_0001082c(&local_414,1,0x400,param_1);
    local_c = local_c + local_14;
    FUN_00010808(auStack_484,&local_414,0x400);
    if ((param_6 != '\0') && (local_10 != 0)) {
      FUN_000107d4(&local_414,1,0x400,local_10);
    }
  }
  local_14 = FUN_0001082c(&local_414,1,param_2 - local_c,param_1);
  FUN_00010808(auStack_484,&local_414,local_14);
  if ((param_6 != '\0') && (local_10 != 0)) {
    FUN_000107d4(&local_414,1,local_14,local_10);
  }
  local_4a4 = 0;
  local_4a0 = 0;
  local_49c = 0;
  local_498 = 0;
  local_494 = 0;
  local_490 = 0;
  local_48c = 0;
  local_488 = 0;
  FUN_00010774(&local_4a4,auStack_484);
  if ((param_6 != '\0') && (local_10 != 0)) {
    thunk_EXT_FUN_706d742e(local_10);
  }
  *param_3 = local_4a4;
  param_3[1] = local_4a0;
  param_3[2] = local_49c;
  param_3[3] = local_498;
  param_3[4] = local_494;
  param_3[5] = local_490;
  param_3[6] = local_48c;
  param_3[7] = local_488;
  return local_488;
}

