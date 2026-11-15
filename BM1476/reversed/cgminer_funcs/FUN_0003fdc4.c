//==================== FUN_0003fdc4 @ 0003fdc4 ====================

/* WARNING: Removing unreachable block (ram,0x0003ff94) */
/* WARNING: Removing unreachable block (ram,0x0003ffa0) */
/* WARNING: Removing unreachable block (ram,0x0003ffb0) */
/* WARNING: Removing unreachable block (ram,0x0003ffc0) */
/* WARNING: Removing unreachable block (ram,0x0003ffd0) */
/* WARNING: Removing unreachable block (ram,0x0003ffe0) */

undefined1 FUN_0003fdc4(void)

{
  undefined1 uVar1;
  byte bVar2;
  undefined1 *puVar3;
  byte *pbVar4;
  byte local_1020 [8];
  undefined1 uStack_1018;
  undefined1 local_1017 [3];
  undefined2 local_1014;
  char acStack_1010 [4096];
  
  local_1017[0] = 0xaa;
  local_1020[4] = 0;
  local_1020[5] = 0;
  local_1017[1] = 4;
  local_1017[2] = 0x16;
  puVar3 = &uStack_1018;
  local_1014 = 0x1a00;
  uStack_1018 = 0x55;
  pthread_mutex_lock((pthread_mutex_t *)&DAT_000790a0);
  uVar1 = 0x55;
  while (FUN_0003c7b0(uVar1), puVar3 != (undefined1 *)((int)&local_1014 + 1)) {
    puVar3 = puVar3 + 1;
    uVar1 = *puVar3;
  }
  usleep(100000);
  pbVar4 = local_1020 + -1;
  do {
    bVar2 = FUN_0003c7c8();
    pbVar4 = pbVar4 + 1;
    *pbVar4 = bVar2;
  } while (pbVar4 != local_1020 + 5);
  pthread_mutex_unlock((pthread_mutex_t *)&DAT_000790a0);
  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
    snprintf(acStack_1010,0x1000,"%s chain %d failed!","heart_beat_PIC16F1704_new",DAT_0007909c);
    FUN_00038730(3,acStack_1010,0);
    if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
      snprintf(acStack_1010,0x1000,
               "%s: read_back_data[0] = 0x%x, read_back_data[1] = 0x%x, read_back_data[2] = 0x%x, read_back_data[3] = 0x%x, read_back_data[4] = 0x%x, read_back_data[5] = 0x%x\n"
               ,"heart_beat_PIC16F1704_new",0xff,0,0,0,(uint)local_1020[4],(uint)local_1020[5]);
      FUN_00038730(3,acStack_1010,0);
    }
  }
  return 0;
}

