//==================== FUN_0003f6cc @ 0003f6cc ====================

undefined1 FUN_0003f6cc(int param_1)

{
  undefined1 uVar1;
  char *__format;
  uint uVar2;
  byte *pbVar3;
  uint uVar4;
  undefined1 *puVar5;
  uint uVar6;
  undefined1 uStack_1038;
  undefined1 local_1037 [3];
  undefined2 local_1034;
  undefined1 uStack_1031;
  undefined4 local_1030;
  uint local_102c;
  uint local_1028;
  undefined4 local_1024;
  char acStack_1020 [4096];
  
  local_102c = 0;
  local_1028 = 0;
  local_1024 = 0;
  local_1030 = 0xff;
  local_1037[0] = 0xaa;
  puVar5 = &uStack_1038;
  local_1037[1] = 4;
  local_1037[2] = 0x13;
  local_1034 = 0x1700;
  uStack_1038 = 0x55;
  pthread_mutex_lock((pthread_mutex_t *)&DAT_000790a0);
  uVar1 = 0x55;
  while (FUN_0003c7b0(uVar1), puVar5 != (undefined1 *)((int)&local_1034 + 1)) {
    puVar5 = puVar5 + 1;
    uVar1 = *puVar5;
  }
  usleep(200000);
  puVar5 = &uStack_1031;
  do {
    uVar1 = FUN_0003c7c8();
    puVar5 = puVar5 + 1;
    *puVar5 = uVar1;
  } while (puVar5 != (undefined1 *)((int)&local_1024 + 3));
  pthread_mutex_unlock((pthread_mutex_t *)&DAT_000790a0);
  if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 5)) {
    if (local_1030._1_1_ == 0x13) {
LAB_0003f8f4:
      if ((char)local_1030 == '\x10') {
        uVar4 = local_1030 >> 0x10 & 0xff;
        uVar2 = (local_1024 >> 8 & 0xff) +
                (local_1024 & 0xff) +
                (local_1028 >> 0x18) +
                (local_1028 >> 0x10 & 0xff) +
                (local_1028 >> 8 & 0xff) +
                (local_1028 & 0xff) +
                (local_102c >> 0x18) +
                (local_102c >> 0x10 & 0xff) +
                (local_102c >> 8 & 0xff) + (local_102c & 0xff) + uVar4 + (local_1030 >> 0x18) + 0x23
        ;
        uVar6 = uVar2 & 0xffff;
        if (((local_1024 >> 0x10 & 0xff) == uVar6 >> 8) && (local_1024 >> 0x18 == (uVar2 & 0xff))) {
          puVar5 = (undefined1 *)(param_1 + -1);
          pbVar3 = (byte *)((int)&local_1030 + 2);
          while( true ) {
            puVar5 = puVar5 + 1;
            *puVar5 = (char)uVar4;
            if (pbVar3 == (byte *)((int)&local_1024 + 1)) break;
            pbVar3 = pbVar3 + 1;
            uVar4 = (uint)*pbVar3;
          }
          if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 5)) {
            return 1;
          }
          snprintf(acStack_1020,0x1000,"%s ok","read_hash_id_PIC16F1704_new");
          FUN_00038730(5,acStack_1020,0);
          return 1;
        }
        if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 3)) {
          return 0;
        }
        __format = "%s failed! crc = 0x%04x";
        goto LAB_0003f8b4;
      }
    }
  }
  else {
    uVar6 = (uint)local_1030._1_1_;
    snprintf(acStack_1020,0x1000,
             "%s: read_back_data[0] = 0x%x, read_back_data[1] = 0x%x, read_back_data[2] = 0x%x, read_back_data[3] = 0x%x,\t\tread_back_data[4] = 0x%x, read_back_data[5] = 0x%x, read_back_data[6] = 0x%x, read_back_data[7] = 0x%x,\t\tread_back_data[8] = 0x%x, read_back_data[9] = 0x%x, read_back_data[10] = 0x%x, read_back_data[11] = 0x%x,\t\tread_back_data[12] = 0x%x, read_back_data[13] = 0x%x, read_back_data[14] = 0x%x, read_back_data[15] = 0x%x\n"
             ,"read_hash_id_PIC16F1704_new",local_1030 & 0xff,uVar6,local_1030 >> 0x10 & 0xff,
             local_1030 >> 0x18,local_102c & 0xff,local_102c >> 8 & 0xff,local_102c >> 0x10 & 0xff,
             local_102c >> 0x18,local_1028 & 0xff,local_1028 >> 8 & 0xff,local_1028 >> 0x10 & 0xff,
             local_1028 >> 0x18,local_1024 & 0xff,local_1024 >> 8 & 0xff,local_1024 >> 0x10 & 0xff,
             local_1024 >> 0x18);
    FUN_00038730(5,acStack_1020,0);
    if (uVar6 == 0x13) goto LAB_0003f8f4;
  }
  if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 3)) {
    return 0;
  }
  __format = "%s chain %d failed!\n\n";
  uVar6 = DAT_0007909c;
LAB_0003f8b4:
  snprintf(acStack_1020,0x1000,__format,"read_hash_id_PIC16F1704_new",uVar6);
  FUN_00038730(3,acStack_1020,0);
  return 0;
}

