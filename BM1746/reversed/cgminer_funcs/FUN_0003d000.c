//==================== FUN_0003d000 @ 0003d000 ====================

undefined4 FUN_0003d000(int param_1,int param_2)

{
  undefined1 uVar1;
  uint param5;
  uint param6;
  undefined4 uVar2;
  undefined1 *puVar3;
  int iVar4;
  undefined1 uStack_1018;
  undefined1 local_1017 [7];
  char acStack_1010 [4096];
  
  local_1017[3] = (undefined1)param_1;
  local_1017[0] = 0xaa;
  local_1017[2] = 1;
  iVar4 = param_2 + param_1 + 7;
  local_1017[6] = (undefined1)iVar4;
  puVar3 = &uStack_1018;
  local_1017[4] = (undefined1)param_2;
  local_1017[5] = (undefined1)((uint)iVar4 >> 8);
  uStack_1018 = 0x55;
  local_1017[1] = 6;
  pthread_mutex_lock((pthread_mutex_t *)&DAT_000790a0);
  uVar1 = 0x55;
  while (FUN_0003c7b0(uVar1), puVar3 != local_1017 + 6) {
    puVar3 = puVar3 + 1;
    uVar1 = *puVar3;
  }
  usleep(100000);
  param5 = FUN_0003c7c8();
  param6 = FUN_0003c7c8();
  pthread_mutex_unlock((pthread_mutex_t *)&DAT_000790a0);
  if ((param5 == 1) && (param6 == 1)) {
    if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 5)) {
      uVar2 = 1;
    }
    else {
      snprintf(acStack_1010,0x1000,"%s ok","set_PIC16F1704_flash_pointer_new");
      FUN_00038730(5,acStack_1010,0);
      uVar2 = 1;
    }
  }
  else if ((DAT_0007ad48 != '\0') || ((DAT_00078e08 != '\0' || (uVar2 = 0, 2 < DAT_000766c4)))) {
    snprintf(acStack_1010,0x1000,
             "%s chain %d failed! read_back_data[0] = 0x%02x, read_back_data[1] = 0x%02x\n\n",
             "set_PIC16F1704_flash_pointer_new",DAT_0007909c,param5,param6);
    FUN_00038730(3,acStack_1010,0);
    uVar2 = 0;
  }
  return uVar2;
}

