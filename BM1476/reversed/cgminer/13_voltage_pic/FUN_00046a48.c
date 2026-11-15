//==================== FUN_00046a48 @ 00046a48 ====================

int FUN_00046a48(int param_1)

{
  int param3;
  int iVar1;
  char *pcVar2;
  int iVar3;
  undefined4 local_1034;
  undefined4 local_1030;
  undefined1 local_102c;
  char acStack_1028 [4100];
  
  pcVar2 = &DAT_0009d4bb;
  iVar1 = 0;
  param3 = 0;
  iVar3 = 1;
  do {
    pcVar2 = pcVar2 + 1;
    if (*pcVar2 == '\x01') {
      local_1034 = 0;
      local_1030 = 0;
      local_102c = 0;
      pthread_mutex_lock((pthread_mutex_t *)&DAT_000790d0);
      DAT_0007909c = param3;
      iVar3 = FUN_00042100(&local_1034,0,9);
      FUN_0002a92c(100);
      pthread_mutex_unlock((pthread_mutex_t *)&DAT_000790d0);
      if (iVar3 == 0) {
        if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 5)) {
          return 0;
        }
        snprintf(acStack_1028,0x1000,"chain%d pic not support flash data storage, adopt file mode\n"
                 ,param3);
        FUN_00038730(5,acStack_1028,0);
        return 0;
      }
      if (0 < iVar3) {
        FUN_00056cd8((int)&local_1034 + 1,2,&DAT_00069af8 + param3 * 0x10);
        *(undefined4 *)(param_1 + iVar1) = local_1034;
        *(undefined4 *)(param_1 + iVar1 + 4) = local_1030;
        *(undefined1 *)(param_1 + iVar1 + 8) = local_102c;
      }
    }
    param3 = param3 + 1;
    iVar1 = iVar1 + 9;
    if (param3 == 4) {
      return iVar3;
    }
  } while( true );
}

