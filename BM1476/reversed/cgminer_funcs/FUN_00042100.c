//==================== FUN_00042100 @ 00042100 ====================

undefined1 FUN_00042100(void *param_1,undefined1 param_2,size_t param_3)

{
  char cVar1;
  undefined1 uVar2;
  uint uVar3;
  uint uVar4;
  undefined1 *puVar5;
  uint uVar6;
  undefined1 uStack_1050;
  undefined1 local_104f [7];
  undefined4 local_1048;
  byte abStack_1044 [4132];
  
  local_104f[6] = (undefined1)(param_3 + 0x3a);
  abStack_1044[0] = 0;
  abStack_1044[1] = 0;
  abStack_1044[2] = 0;
  abStack_1044[3] = 0;
  abStack_1044[4] = 0;
  abStack_1044[5] = 0;
  abStack_1044[6] = 0;
  abStack_1044[7] = 0;
  local_104f[5] = (undefined1)(param_3 + 0x3a >> 8);
  abStack_1044[8] = 0;
  abStack_1044[9] = 0;
  abStack_1044[10] = 0;
  abStack_1044[0xb] = 0;
  local_1048 = 0xff;
  abStack_1044[0xc] = 0;
  abStack_1044[0xd] = 0;
  abStack_1044[0xe] = 0;
  abStack_1044[0xf] = 0;
  abStack_1044[0x10] = 0;
  abStack_1044[0x11] = 0;
  abStack_1044[0x12] = 0;
  abStack_1044[0x13] = 0;
  abStack_1044[0x14] = 0;
  abStack_1044[0x15] = 0;
  abStack_1044[0x16] = 0;
  abStack_1044[0x17] = 0;
  local_104f[0] = 0xaa;
  puVar5 = &uStack_1050;
  abStack_1044[0x18] = 0;
  abStack_1044[0x19] = 0;
  abStack_1044[0x1a] = 0;
  abStack_1044[0x1b] = 0;
  local_104f[4] = (undefined1)param_3;
  abStack_1044[0x1c] = 0;
  abStack_1044[0x1d] = 0;
  abStack_1044[0x1e] = 0;
  abStack_1044[0x1f] = 0;
  local_104f[1] = 6;
  abStack_1044[0x20] = 0;
  abStack_1044[0x21] = 0;
  abStack_1044[0x22] = 0;
  abStack_1044[0x23] = 0;
  local_104f[2] = 0x34;
  uStack_1050 = 0x55;
  local_104f[3] = param_2;
  pthread_mutex_lock((pthread_mutex_t *)&DAT_000790a0);
  uVar2 = 0x55;
  while (FUN_0003c7b0(uVar2), puVar5 != local_104f + 6) {
    puVar5 = puVar5 + 1;
    uVar2 = *puVar5;
  }
  usleep(200000);
  uVar3 = 0;
  do {
    uVar2 = FUN_0003c7c8();
    uVar6 = uVar3 + 1 & 0xff;
    local_104f[uVar3 + 7] = uVar2;
    uVar3 = uVar6;
  } while ((int)uVar6 <= (int)(param_3 + 4));
  cVar1 = local_1048._1_1_;
  pthread_mutex_unlock((pthread_mutex_t *)&DAT_000790a0);
  if (((cVar1 == '4') && (uVar3 = local_1048 & 0xff, uVar3 == param_3 + 5)) &&
     (local_1048._2_1_ == '\x01')) {
    uVar4 = 0;
    uVar6 = 0;
    while( true ) {
      uVar3 = uVar6 + uVar3;
      uVar4 = uVar4 + 1 & 0xff;
      uVar6 = uVar3 & 0xffff;
      if ((int)(param_3 + 2) < (int)uVar4) break;
      uVar3 = (uint)(byte)local_104f[uVar4 + 7];
    }
    if (((uint)abStack_1044[param_3 - 1] == uVar6 >> 8) &&
       ((uint)abStack_1044[param_3] == (uVar3 & 0xff))) {
      if ((DAT_00078e09 != '\0') &&
         (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
        snprintf((char *)(abStack_1044 + 0x24),0x1000,"\n--- %s ok\n\n",
                 "read_data_from_PIC16F1704_flash");
        FUN_00038730(7,abStack_1044 + 0x24,0);
      }
      memcpy(param_1,(void *)((int)&local_1048 + 3),param_3);
      uVar2 = 1;
    }
    else if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (uVar2 = 0, 2 < DAT_000766c4)) {
      snprintf((char *)(abStack_1044 + 0x24),0x1000,"\n--- %s failed! crc = 0x%04x\n\n",
               "read_data_from_PIC16F1704_flash",uVar6);
      FUN_00038730(3,abStack_1044 + 0x24,0);
      uVar2 = 0;
    }
  }
  else {
    uVar2 = 0;
  }
  return uVar2;
}

