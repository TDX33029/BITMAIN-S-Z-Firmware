//==================== FUN_00041b8c @ 00041b8c ====================

/* WARNING: Removing unreachable block (ram,0x00041d60) */
/* WARNING: Removing unreachable block (ram,0x00041d7c) */
/* WARNING: Removing unreachable block (ram,0x00041de0) */
/* WARNING: Removing unreachable block (ram,0x00041df0) */
/* WARNING: Removing unreachable block (ram,0x00041e04) */
/* WARNING: Removing unreachable block (ram,0x00041e14) */
/* WARNING: Removing unreachable block (ram,0x00041e28) */
/* WARNING: Removing unreachable block (ram,0x00041e20) */
/* WARNING: Removing unreachable block (ram,0x00041d98) */
/* WARNING: Removing unreachable block (ram,0x00041da4) */
/* WARNING: Removing unreachable block (ram,0x00041db4) */
/* WARNING: Removing unreachable block (ram,0x00041dc4) */

undefined1 FUN_00041b8c(void)

{
  undefined1 uVar1;
  byte bVar2;
  undefined1 *puVar3;
  byte *pbVar4;
  byte local_1028 [8];
  undefined1 uStack_1020;
  undefined1 local_101f [3];
  undefined2 local_101c;
  char acStack_1018 [4100];
  
  local_101f[0] = 0xaa;
  local_1028[4] = 0;
  local_1028[5] = 0;
  puVar3 = &uStack_1020;
  local_101f[1] = 4;
  local_101f[2] = 0x25;
  local_101c = 0x2900;
  uStack_1020 = 0x55;
  pthread_mutex_lock((pthread_mutex_t *)&DAT_000790a0);
  uVar1 = 0x55;
  while (FUN_0003c7b0(uVar1), puVar3 != (undefined1 *)((int)&local_101c + 1)) {
    puVar3 = puVar3 + 1;
    uVar1 = *puVar3;
  }
  usleep(300000);
  pbVar4 = local_1028 + -1;
  do {
    bVar2 = FUN_0003c7c8();
    pbVar4 = pbVar4 + 1;
    *pbVar4 = bVar2;
  } while (pbVar4 != local_1028 + 5);
  pthread_mutex_unlock((pthread_mutex_t *)&DAT_000790a0);
  usleep(200000);
  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
    snprintf(acStack_1018,0x1000,
             "--- %s: read_back_data[0] = 0x%x, read_back_data[1] = 0x%x, read_back_data[2] = 0x%x, read_back_data[3] = 0x%x, read_back_data[4] = 0x%x, read_back_data[5] = 0x%x\n"
             ,"get_PIC16F1704_freq_new",0xff,0,0,0,(uint)local_1028[4],(uint)local_1028[5]);
    FUN_00038730(5,acStack_1018,0);
  }
  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
    snprintf(acStack_1018,0x1000,"\n--- %s failed!\n\n","get_PIC16F1704_freq_new");
    FUN_00038730(3,acStack_1018,0);
  }
  return 0;
}

