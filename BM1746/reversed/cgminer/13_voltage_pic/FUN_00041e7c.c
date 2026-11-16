//==================== FUN_00041e7c @ 00041e7c ====================

undefined1 FUN_00041e7c(void *param_1,undefined1 param_2,uint param_3)

{
  byte *pbVar1;
  undefined1 uVar2;
  uint uVar3;
  undefined2 uVar4;
  uint uVar5;
  undefined1 *puVar6;
  undefined1 uStack_1035;
  undefined4 local_1034;
  undefined1 local_1030 [4];
  char local_102c [4];
  undefined4 local_1028;
  undefined4 uStack_1024;
  undefined4 uStack_1020;
  undefined1 local_101c;
  char acStack_1018 [4100];
  
  uVar5 = (param_3 & 0xffff) * 2 + 0x39;
  uVar3 = 0;
  uStack_1024 = 0;
  uVar4 = (undefined2)uVar5;
  uStack_1020 = 0;
  local_1034 = 0xff;
  local_101c = 0;
  local_1030[0] = 0;
  if (0 < (int)param_3) {
    do {
      pbVar1 = (byte *)((int)param_1 + uVar3);
      uVar3 = uVar3 + 1 & 0xff;
      uVar5 = (uVar5 & 0xffff) + (uint)*pbVar1;
      uVar4 = (undefined2)uVar5;
    } while ((int)uVar3 < (int)param_3);
  }
  local_1028 = (uint)CONCAT11((char)param_3,param_2);
  local_102c[2] = (char)param_3 + '\x06';
  local_102c[0] = 'U';
  local_102c[1] = 0xaa;
  local_102c[3] = 0x33;
  memcpy((void *)((int)&local_1028 + 2),param_1,param_3);
  local_102c[param_3 + 7] = (char)uVar4;
  local_102c[param_3 + 6] = (char)((ushort)uVar4 >> 8);
  pthread_mutex_lock((pthread_mutex_t *)&DAT_000790a0);
  if (0 < (int)(param_3 + 8)) {
    uVar3 = 0;
    do {
      FUN_0003c7b0(local_102c[uVar3]);
      uVar3 = uVar3 + 1 & 0xff;
    } while ((int)uVar3 < (int)(param_3 + 8));
  }
  usleep(200000);
  puVar6 = &uStack_1035;
  do {
    uVar2 = FUN_0003c7c8();
    puVar6 = puVar6 + 1;
    *puVar6 = uVar2;
  } while (puVar6 != local_1030);
  pthread_mutex_unlock((pthread_mutex_t *)&DAT_000790a0);
  if ((((local_1034 & 0xff) == 5) && (local_1034._1_1_ == 0x33)) && (local_1034._2_1_ == 1)) {
    uVar2 = 1;
    if ((DAT_00078e09 != '\0') &&
       (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
      snprintf(acStack_1018,0x1000,"\n--- %s ok\n\n","write_data_to_PIC16F1704_flash");
      FUN_00038730(7,acStack_1018,0);
      uVar2 = 1;
    }
  }
  else if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (uVar2 = 0, 2 < DAT_000766c4)) {
    snprintf(acStack_1018,0x1000,
             "\n--- %s failed! read_back_data[0] = 0x%02x, read_back_data[1] = 0x%02x, read_back_data[2] = 0x%02x\n\n"
             ,"write_data_to_PIC16F1704_flash",local_1034 & 0xff,(uint)local_1034._1_1_,
             (uint)local_1034._2_1_);
    FUN_00038730(3,acStack_1018,0);
    uVar2 = 0;
  }
  return uVar2;
}

