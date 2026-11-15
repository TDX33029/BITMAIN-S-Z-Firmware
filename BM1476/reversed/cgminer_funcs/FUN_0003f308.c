//==================== FUN_0003f308 @ 0003f308 ====================

undefined4 FUN_0003f308(uint param_1)

{
  undefined1 uVar1;
  uint param5;
  uint param6;
  undefined4 uVar2;
  undefined1 *puVar3;
  undefined1 uStack_1020;
  undefined1 local_101f [7];
  char acStack_1018 [4100];
  
  local_101f[5] = (undefined1)(param_1 + 0x15);
  local_101f[2] = 0x10;
  puVar3 = &uStack_1020;
  local_101f[4] = (undefined1)(param_1 + 0x15 >> 8);
  local_101f[3] = (undefined1)param_1;
  local_101f[0] = 0xaa;
  uStack_1020 = 0x55;
  local_101f[1] = 5;
  pthread_mutex_lock((pthread_mutex_t *)&DAT_000790a0);
  uVar1 = 0x55;
  while (FUN_0003c7b0(uVar1), puVar3 != local_101f + 5) {
    puVar3 = puVar3 + 1;
    uVar1 = *puVar3;
  }
  usleep(200000);
  param5 = FUN_0003c7c8();
  param6 = FUN_0003c7c8();
  pthread_mutex_unlock((pthread_mutex_t *)&DAT_000790a0);
  usleep(200000);
  if ((param5 == 0x10) && (param6 == 1)) {
    if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 5)) {
      uVar2 = 1;
    }
    else {
      snprintf(acStack_1018,0x1000,"%s ok, voltage = 0x%02x","set_PIC16F1704_voltage_new",param_1);
      FUN_00038730(5,acStack_1018,0);
      uVar2 = 1;
    }
  }
  else if ((DAT_0007ad48 != '\0') || ((DAT_00078e08 != '\0' || (uVar2 = 0, 2 < DAT_000766c4)))) {
    snprintf(acStack_1018,0x1000,
             "%s chain %d failed! read_back_data[0] = 0x%02x, read_back_data[1] = 0x%02x\n\n",
             "set_PIC16F1704_voltage_new",DAT_0007909c,param5,param6);
    FUN_00038730(3,acStack_1018,0);
    uVar2 = 0;
  }
  return uVar2;
}

