//==================== FUN_0003d67c @ 0003d67c ====================

undefined1 FUN_0003d67c(int param_1)

{
  byte bVar1;
  undefined1 uVar2;
  char *__format;
  uint uVar3;
  uint uVar4;
  byte *pbVar5;
  uint uVar6;
  undefined1 *puVar7;
  undefined1 uStack_103c;
  undefined1 local_103b [3];
  undefined2 local_1038;
  undefined1 uStack_1035;
  undefined4 local_1034;
  uint local_1030;
  uint local_102c;
  uint local_1028;
  undefined4 local_1024;
  char acStack_1020 [4096];
  
  local_1030 = 0;
  local_102c = 0;
  local_1028 = 0;
  local_1024 = 0;
  local_103b[2] = 3;
  local_1034 = 0xff;
  local_103b[0] = 0xaa;
  local_103b[1] = 4;
  local_1038 = 0x700;
  puVar7 = &uStack_103c;
  uStack_103c = 0x55;
  pthread_mutex_lock((pthread_mutex_t *)&DAT_000790a0);
  uVar2 = 0x55;
  while (FUN_0003c7b0(uVar2), puVar7 != (undefined1 *)((int)&local_1038 + 1)) {
    puVar7 = puVar7 + 1;
    uVar2 = *puVar7;
  }
  usleep(300000);
  puVar7 = &uStack_1035;
  do {
    uVar2 = FUN_0003c7c8();
    puVar7 = puVar7 + 1;
    *puVar7 = uVar2;
  } while (puVar7 != (undefined1 *)((int)&local_1024 + 3));
  pthread_mutex_unlock((pthread_mutex_t *)&DAT_000790a0);
  bVar1 = local_1034._1_1_;
  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
    snprintf(acStack_1020,0x1000,
             "%s: read_back_data[0] = 0x%x, read_back_data[1] = 0x%x, read_back_data[2] = 0x%x, read_back_data[3] = 0x%x, \t\tread_back_data[4] = 0x%x, read_back_data[5] = 0x%x, read_back_data[6] = 0x%x, read_back_data[7] = 0x%x, \t\tread_back_data[8] = 0x%x, read_back_data[9] = 0x%x, read_back_data[10] = 0x%x, read_back_data[11] = 0x%x, \t\tread_back_data[12] = 0x%x, read_back_data[13] = 0x%x, read_back_data[14] = 0x%x, read_back_data[15] = 0x%x, \t\tread_back_data[16] = 0x%x, read_back_data[17] = 0x%x, read_back_data[18] = 0x%x, read_back_data[19] = 0x%x\n"
             ,"read_PIC16F1704_flash_data_new",local_1034 & 0xff,(uint)local_1034._1_1_,
             local_1034 >> 0x10 & 0xff,local_1034 >> 0x18,local_1030 & 0xff,local_1030 >> 8 & 0xff,
             local_1030 >> 0x10 & 0xff,local_1030 >> 0x18,local_102c & 0xff,local_102c >> 8 & 0xff,
             local_102c >> 0x10 & 0xff,local_102c >> 0x18,local_1028 & 0xff,local_1028 >> 8 & 0xff,
             local_1028 >> 0x10 & 0xff,local_1028 >> 0x18,local_1024 & 0xff,local_1024 >> 8 & 0xff,
             local_1024 >> 0x10 & 0xff,local_1024 >> 0x18);
    FUN_00038730(5,acStack_1020,0);
  }
  usleep(100000);
  if ((bVar1 == 3) && ((char)local_1034 == '\x14')) {
    uVar6 = local_1034 >> 0x10 & 0xff;
    uVar3 = (local_1024 >> 8 & 0xff) +
            (local_1024 & 0xff) +
            (local_1028 >> 0x18) +
            (local_1028 >> 0x10 & 0xff) +
            (local_1028 >> 8 & 0xff) +
            (local_1028 & 0xff) +
            (local_102c >> 0x18) +
            (local_102c >> 0x10 & 0xff) +
            (local_102c >> 8 & 0xff) +
            (local_102c & 0xff) +
            (local_1030 >> 0x18) +
            (local_1030 >> 0x10 & 0xff) +
            (local_1030 >> 8 & 0xff) + (local_1030 & 0xff) + uVar6 + (local_1034 >> 0x18) + 0x17;
    uVar4 = uVar3 & 0xffff;
    if (((local_1024 >> 0x10 & 0xff) == uVar4 >> 8) && (local_1024 >> 0x18 == (uVar3 & 0xff))) {
      puVar7 = (undefined1 *)(param_1 + -1);
      pbVar5 = (byte *)((int)&local_1034 + 2);
      while( true ) {
        puVar7 = puVar7 + 1;
        *puVar7 = (char)uVar6;
        if (pbVar5 == (byte *)((int)&local_1024 + 1)) break;
        pbVar5 = pbVar5 + 1;
        uVar6 = (uint)*pbVar5;
      }
      if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 5)) {
        return 1;
      }
      snprintf(acStack_1020,0x1000,"%s ok","read_PIC16F1704_flash_data_new");
      FUN_00038730(5,acStack_1020,0);
      return 1;
    }
    if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 3)) {
      return 0;
    }
    __format = "%s failed! crc = 0x%04x\n\n";
  }
  else {
    if ((DAT_0007ad48 == '\0') && ((DAT_00078e08 == '\0' && (DAT_000766c4 < 3)))) {
      return 0;
    }
    __format = "%s chain %d failed!";
    uVar4 = DAT_0007909c;
  }
  snprintf(acStack_1020,0x1000,__format,"read_PIC16F1704_flash_data_new",uVar4);
  FUN_00038730(3,acStack_1020,0);
  return 0;
}

