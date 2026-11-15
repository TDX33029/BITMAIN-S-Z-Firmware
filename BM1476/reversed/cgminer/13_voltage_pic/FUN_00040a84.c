//==================== FUN_00040a84 @ 00040a84 ====================

undefined1 FUN_00040a84(int param_1)

{
  undefined1 uVar1;
  byte *pbVar2;
  uint uVar3;
  undefined1 *puVar4;
  uint uVar5;
  undefined1 uStack_1034;
  undefined1 local_1033 [3];
  undefined2 local_1030;
  undefined1 uStack_102d;
  undefined4 local_102c;
  uint local_1028;
  undefined4 local_1024;
  char acStack_1020 [4096];
  
  local_1028 = 0;
  local_1024 = 0;
  local_102c = 0xff;
  local_1033[1] = 4;
  local_1033[0] = 0xaa;
  puVar4 = &uStack_1034;
  local_1033[2] = 0x23;
  local_1030 = 0x2700;
  uStack_1034 = 0x55;
  pthread_mutex_lock((pthread_mutex_t *)&DAT_000790a0);
  uVar1 = 0x55;
  while (FUN_0003c7b0(uVar1), puVar4 != (undefined1 *)((int)&local_1030 + 1)) {
    puVar4 = puVar4 + 1;
    uVar1 = *puVar4;
  }
  usleep(200000);
  puVar4 = &uStack_102d;
  do {
    uVar1 = FUN_0003c7c8();
    puVar4 = puVar4 + 1;
    *puVar4 = uVar1;
  } while (puVar4 != (undefined1 *)((int)&local_1024 + 3));
  pthread_mutex_unlock((pthread_mutex_t *)&DAT_000790a0);
  if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 5)) {
    if (local_102c._1_1_ == 0x23) {
LAB_00040c88:
      if ((char)local_102c == '\f') {
        uVar3 = local_102c >> 0x10 & 0xff;
        uVar5 = (local_1024 >> 8 & 0xff) +
                (local_1024 & 0xff) +
                (local_1028 >> 0x18) +
                (local_1028 >> 0x10 & 0xff) +
                (local_1028 >> 8 & 0xff) + (local_1028 & 0xff) + uVar3 + (local_102c >> 0x18) + 0x2f
        ;
        if (((local_1024 >> 0x10 & 0xff) == uVar5 >> 8) && (local_1024 >> 0x18 == (uVar5 & 0xff))) {
          puVar4 = (undefined1 *)(param_1 + -1);
          pbVar2 = (byte *)((int)&local_102c + 2);
          while( true ) {
            puVar4 = puVar4 + 1;
            *puVar4 = (char)uVar3;
            if (pbVar2 == (byte *)((int)&local_1024 + 1)) break;
            pbVar2 = pbVar2 + 1;
            uVar3 = (uint)*pbVar2;
          }
          if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 5)) {
            return 1;
          }
          snprintf(acStack_1020,0x1000,"%s ok","read_temperature_offset_PIC16F1704_new");
          FUN_00038730(5,acStack_1020,0);
          return 1;
        }
        if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 3)) {
          return 0;
        }
        snprintf(acStack_1020,0x1000,"%s failed! crc = 0x%04x",
                 "read_temperature_offset_PIC16F1704_new",uVar5);
        goto LAB_00040c4c;
      }
    }
  }
  else {
    uVar5 = (uint)local_102c._1_1_;
    snprintf(acStack_1020,0x1000,
             "%s: read_back_data[0] = 0x%x, read_back_data[1] = 0x%x, read_back_data[2] = 0x%x, read_back_data[3] = 0x%x, \t\tread_back_data[4] = 0x%x, read_back_data[5] = 0x%x, read_back_data[6] = 0x%x, read_back_data[7] = 0x%x, \t\tread_back_data[8] = 0x%x, read_back_data[9] = 0x%x, read_back_data[10] = 0x%x, read_back_data[11] = 0x%x\n"
             ,"read_temperature_offset_PIC16F1704_new",local_102c & 0xff,uVar5,
             local_102c >> 0x10 & 0xff,local_102c >> 0x18,local_1028 & 0xff,local_1028 >> 8 & 0xff,
             local_1028 >> 0x10 & 0xff,local_1028 >> 0x18,local_1024 & 0xff,local_1024 >> 8 & 0xff,
             local_1024 >> 0x10 & 0xff,local_1024 >> 0x18);
    FUN_00038730(5,acStack_1020,0);
    if (uVar5 == 0x23) goto LAB_00040c88;
  }
  if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 3)) {
    return 0;
  }
  snprintf(acStack_1020,0x1000,"%s failed!","read_temperature_offset_PIC16F1704_new");
LAB_00040c4c:
  FUN_00038730(3,acStack_1020,0);
  return 0;
}

