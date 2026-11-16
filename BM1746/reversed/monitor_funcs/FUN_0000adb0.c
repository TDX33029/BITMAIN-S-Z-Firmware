//==================== FUN_0000adb0 @ 0000adb0 ====================

undefined4 FUN_0000adb0(void)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = FUN_0000e8dc();
  if (iVar1 != 0) {
    if (iVar1 == -1) {
      uVar2 = 0xffffffff;
    }
    else {
      uVar2 = 1;
    }
    return uVar2;
  }
  puts("OFF LINE !!!!Please Wait!!! in recv_serpkg_struct() in client");
  do {
    while (DAT_0001b380 != 0) {
      iVar1 = FUN_0000e8dc();
      if (0 < iVar1) {
        return 1;
      }
    }
    sleep(0x1e);
    iVar1 = FUN_0000e8dc();
  } while (iVar1 < 1);
  return 1;
}

