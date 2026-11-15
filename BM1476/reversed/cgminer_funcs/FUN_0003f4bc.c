//==================== FUN_0003f4bc @ 0003f4bc ====================

undefined4 FUN_0003f4bc(int param_1)

{
  byte *pbVar1;
  undefined4 uVar2;
  undefined1 *puVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  undefined1 *puVar7;
  undefined1 uStack_1025;
  undefined4 local_1024;
  undefined4 local_1020;
  undefined4 uStack_101c;
  undefined4 uStack_1018;
  undefined1 local_1014;
  undefined1 auStack_1013 [3];
  char acStack_1010 [4096];
  
  iVar6 = 0;
  uVar4 = 0x22;
  local_1020 = 0;
  uStack_101c = 0;
  uStack_1018 = 0;
  do {
    pbVar1 = (byte *)(param_1 + iVar6);
    iVar6 = iVar6 + 1;
    uVar5 = uVar4 + *pbVar1;
    uVar4 = uVar5 & 0xffff;
  } while (iVar6 != 0xc);
  puVar3 = (undefined1 *)(param_1 + -1);
  puVar7 = (undefined1 *)((int)&local_1024 + 3);
  local_1024 = 0x1210aa55;
  do {
    puVar3 = puVar3 + 1;
    puVar7 = puVar7 + 1;
    *puVar7 = *puVar3;
  } while (puVar7 != (undefined1 *)((int)&uStack_1018 + 3));
  puVar3 = &uStack_1025;
  _local_1014 = CONCAT11((char)uVar5,(char)(uVar4 >> 8));
  pthread_mutex_lock((pthread_mutex_t *)&DAT_000790a0);
  do {
    puVar3 = puVar3 + 1;
    FUN_0003c7b0(*puVar3);
  } while (puVar3 != auStack_1013);
  usleep(200000);
  uVar4 = FUN_0003c7c8();
  uVar5 = FUN_0003c7c8();
  pthread_mutex_unlock((pthread_mutex_t *)&DAT_000790a0);
  usleep(200000);
  if ((uVar4 == 0x12) && (uVar5 == 1)) {
    if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 5)) {
      uVar2 = 1;
    }
    else {
      snprintf(acStack_1010,0x1000,"%s ok","write_hash_ID_PIC16F1704_new");
      FUN_00038730(5,acStack_1010,0);
      uVar2 = 1;
    }
  }
  else if ((DAT_0007ad48 != '\0') || ((DAT_00078e08 != '\0' || (uVar2 = 0, 2 < DAT_000766c4)))) {
    snprintf(acStack_1010,0x1000,
             "%s chain %d failed! read_back_data[0] = 0x%02x, read_back_data[1] = 0x%02x\n\n",
             "write_hash_ID_PIC16F1704_new",DAT_0007909c,uVar4,uVar5);
    FUN_00038730(3,acStack_1010,0);
    uVar2 = 0;
  }
  return uVar2;
}

