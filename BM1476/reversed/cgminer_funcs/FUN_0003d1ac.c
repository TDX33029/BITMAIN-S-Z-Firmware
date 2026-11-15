//==================== FUN_0003d1ac @ 0003d1ac ====================

undefined4 FUN_0003d1ac(int param_1)

{
  byte *pbVar1;
  undefined4 uVar2;
  undefined1 *puVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  undefined1 *puVar7;
  undefined1 uStack_1029;
  undefined4 local_1028;
  undefined4 local_1024;
  undefined4 uStack_1020;
  undefined4 uStack_101c;
  undefined4 uStack_1018;
  undefined1 local_1014;
  undefined1 auStack_1013 [3];
  char acStack_1010 [4096];
  
  iVar6 = 0;
  uVar4 = 0x16;
  local_1024 = 0;
  uStack_1020 = 0;
  uStack_101c = 0;
  uStack_1018 = 0;
  do {
    pbVar1 = (byte *)(param_1 + iVar6);
    iVar6 = iVar6 + 1;
    uVar5 = uVar4 + *pbVar1;
    uVar4 = uVar5 & 0xffff;
  } while (iVar6 != 0x10);
  puVar3 = (undefined1 *)(param_1 + -1);
  puVar7 = (undefined1 *)((int)&local_1028 + 3);
  local_1028 = 0x214aa55;
  do {
    puVar3 = puVar3 + 1;
    puVar7 = puVar7 + 1;
    *puVar7 = *puVar3;
  } while (puVar7 != (undefined1 *)((int)&uStack_1018 + 3));
  puVar3 = &uStack_1029;
  _local_1014 = CONCAT11((char)uVar5,(char)(uVar4 >> 8));
  pthread_mutex_lock((pthread_mutex_t *)&DAT_000790a0);
  do {
    puVar3 = puVar3 + 1;
    FUN_0003c7b0(*puVar3);
  } while (puVar3 != auStack_1013);
  usleep(100000);
  uVar4 = FUN_0003c7c8();
  uVar5 = FUN_0003c7c8();
  pthread_mutex_unlock((pthread_mutex_t *)&DAT_000790a0);
  if ((uVar4 == 2) && (uVar5 == 1)) {
    if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 5)) {
      uVar2 = 1;
    }
    else {
      snprintf(acStack_1010,0x1000,"%s ok","send_data_to_PIC16F1704_new");
      FUN_00038730(5,acStack_1010,0);
      uVar2 = 1;
    }
  }
  else if ((DAT_0007ad48 != '\0') || ((DAT_00078e08 != '\0' || (uVar2 = 0, 2 < DAT_000766c4)))) {
    snprintf(acStack_1010,0x1000,
             "%s chain %d failed! read_back_data[0] = 0x%02x, read_back_data[1] = 0x%02x\n\n",
             "send_data_to_PIC16F1704_new",DAT_0007909c,uVar4,uVar5);
    FUN_00038730(3,acStack_1010,0);
    uVar2 = 0;
  }
  return uVar2;
}

