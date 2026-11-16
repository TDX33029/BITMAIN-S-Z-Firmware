//==================== FUN_00040128 @ 00040128 ====================

undefined4 FUN_00040128(byte *param_1)

{
  undefined1 uVar1;
  byte bVar2;
  undefined1 *puVar3;
  byte *pbVar4;
  uint uVar5;
  byte bStack_1029;
  undefined4 local_1028;
  byte local_1024 [4];
  undefined1 uStack_1020;
  undefined1 local_101f [3];
  undefined2 local_101c;
  char acStack_1018 [4100];
  
  local_101f[0] = 0xaa;
  local_1028 = 0xff;
  local_1024[0] = 0;
  puVar3 = &uStack_1020;
  local_101f[1] = 4;
  local_101f[2] = 0x17;
  local_101c = 0x1b00;
  uStack_1020 = 0x55;
  pthread_mutex_lock((pthread_mutex_t *)&DAT_000790a0);
  uVar1 = 0x55;
  while (FUN_0003c7b0(uVar1), puVar3 != (undefined1 *)((int)&local_101c + 1)) {
    puVar3 = puVar3 + 1;
    uVar1 = *puVar3;
  }
  usleep(400000);
  pbVar4 = &bStack_1029;
  do {
    bVar2 = FUN_0003c7c8();
    pbVar4 = pbVar4 + 1;
    *pbVar4 = bVar2;
  } while (pbVar4 != local_1024);
  pthread_mutex_unlock((pthread_mutex_t *)&DAT_000790a0);
  bVar2 = local_1028._1_1_;
  if ((DAT_00078e09 == '\0') ||
     (((DAT_0007ad48 == '\0' && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 7)))) {
    if (local_1028._1_1_ == 0x17) goto LAB_0004027c;
  }
  else {
    uVar5 = (uint)local_1028._1_1_;
    snprintf(acStack_1018,0x1000,
             "%s: read_back_data[0] = 0x%x, read_back_data[1] = 0x%x, read_back_data[2] = 0x%x, read_back_data[3] = 0x%x, read_back_data[4] = 0x%x"
             ,"get_PIC16F1704_software_version_new",local_1028 & 0xff,uVar5,
             local_1028 >> 0x10 & 0xff,local_1028 >> 0x18,(uint)local_1024[0]);
    FUN_00038730(7,acStack_1018,0);
    if (uVar5 == 0x17) {
LAB_0004027c:
      if ((char)local_1028 == '\x05') {
        uVar5 = local_1028._2_1_ + 0x1c;
        if ((local_1028 >> 0x18 == uVar5 >> 8) && ((uint)local_1024[0] == (uVar5 & 0xff))) {
          *param_1 = local_1028._2_1_;
          return 1;
        }
        if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 3)) {
          return 0;
        }
        snprintf(acStack_1018,0x1000,"%s failed! crc = 0x%04x","get_PIC16F1704_software_version_new"
                 ,uVar5);
        goto LAB_000403c8;
      }
    }
  }
  if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 3)) {
    return 0;
  }
  snprintf(acStack_1018,0x1000,"%s chain %d failed!","get_PIC16F1704_software_version_new",
           DAT_0007909c);
  FUN_00038730(3,acStack_1018,0);
  if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 3)) {
    return 0;
  }
  snprintf(acStack_1018,0x1000,
           "%s: read_back_data[0] = 0x%x, read_back_data[1] = 0x%x, read_back_data[2] = 0x%x, read_back_data[3] = 0x%x, read_back_data[4] = 0x%x"
           ,"get_PIC16F1704_software_version_new",local_1028 & 0xff,(uint)bVar2,
           local_1028 >> 0x10 & 0xff,local_1028 >> 0x18,(uint)local_1024[0]);
LAB_000403c8:
  FUN_00038730(3,acStack_1018,0);
  return 0;
}

