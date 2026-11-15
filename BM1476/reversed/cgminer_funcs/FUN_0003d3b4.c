//==================== FUN_0003d3b4 @ 0003d3b4 ====================

undefined1 FUN_0003d3b4(byte *param_1,byte *param_2)

{
  undefined1 uVar1;
  uint uVar2;
  uint param5;
  undefined1 *puVar3;
  undefined1 uStack_1029;
  undefined4 local_1028;
  undefined2 local_1024;
  undefined1 uStack_1020;
  undefined1 local_101f [3];
  undefined2 local_101c;
  char acStack_1018 [4096];
  
  local_1028 = 0xff;
  local_101f[2] = 8;
  local_101f[0] = 0xaa;
  local_1024 = 0;
  local_101f[1] = 4;
  puVar3 = &uStack_1020;
  local_101c = 0xc00;
  uStack_1020 = 0x55;
  pthread_mutex_lock((pthread_mutex_t *)&DAT_000790a0);
  uVar1 = 0x55;
  while (FUN_0003c7b0(uVar1), puVar3 != (undefined1 *)((int)&local_101c + 1)) {
    puVar3 = puVar3 + 1;
    uVar1 = *puVar3;
  }
  usleep(100000);
  puVar3 = &uStack_1029;
  do {
    uVar1 = FUN_0003c7c8();
    puVar3 = puVar3 + 1;
    *puVar3 = uVar1;
  } while (puVar3 != (undefined1 *)((int)&local_1024 + 1));
  pthread_mutex_unlock((pthread_mutex_t *)&DAT_000790a0);
  uVar2 = local_1028 >> 8 & 0xff;
  if ((uVar2 == 8) && ((char)local_1028 == '\x06')) {
    param5 = (uint)local_1028._3_1_;
    uVar2 = local_1028._2_1_ + param5 + 0xe;
    if (((uint)(byte)local_1024 == uVar2 >> 8) && ((uint)local_1024._1_1_ == (uVar2 & 0xff))) {
      *param_1 = local_1028._2_1_;
      *param_2 = local_1028._3_1_;
      if ((DAT_0007ad48 == '\0') && ((DAT_00078e08 == '\0' && (DAT_000766c4 < 5)))) {
        return 1;
      }
      snprintf(acStack_1018,0x1000,"%s ok! flash_addr_h = 0x%02x, flash_addr_l = 0x%02x",
               "read_PIC16F1704_flash_pointer_new",(uint)*param_1,param5);
      FUN_00038730(5,acStack_1018,0);
      return 1;
    }
    if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 3)) {
      return 0;
    }
    snprintf(acStack_1018,0x1000,
             "%s failed!!! read_back_data[0] = 0x%x, read_back_data[1] = 0x%x, read_back_data[2] = 0x%x, read_back_data[3] = 0x%x, read_back_data[4] = 0x%x, read_back_data[5] = 0x%x\n\n"
             ,"read_PIC16F1704_flash_pointer_new",6,8,(uint)local_1028._2_1_,param5,
             (uint)(byte)local_1024,(uint)local_1024._1_1_);
  }
  else {
    if ((DAT_0007ad48 == '\0') && ((DAT_00078e08 == '\0' && (DAT_000766c4 < 3)))) {
      return 0;
    }
    snprintf(acStack_1018,0x1000,
             "%s chain %d failed! read_back_data[0] = 0x%x, read_back_data[1] = 0x%x, read_back_data[2] = 0x%x, read_back_data[3] = 0x%x, read_back_data[4] = 0x%x, read_back_data[5] = 0x%x\n\n"
             ,"read_PIC16F1704_flash_pointer_new",DAT_0007909c,local_1028 & 0xff,uVar2,
             (uint)local_1028._2_1_,(uint)local_1028._3_1_,(uint)(byte)local_1024,
             (uint)local_1024._1_1_);
  }
  FUN_00038730(3,acStack_1018,0);
  return 0;
}

