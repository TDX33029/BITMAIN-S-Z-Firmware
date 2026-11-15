//==================== FUN_0003c7e0 @ 0003c7e0 ====================

/* WARNING: Removing unreachable block (ram,0x0003ca0c) */
/* WARNING: Removing unreachable block (ram,0x0003ca14) */

undefined4 FUN_0003c7e0(char param_1,undefined1 param_2,int param_3,int param_4)

{
  undefined1 uVar1;
  char *pcVar2;
  char *pcVar3;
  byte bVar4;
  byte *pbVar5;
  undefined1 *puVar6;
  byte bVar7;
  uint uVar8;
  uint uVar9;
  undefined1 local_1040 [4];
  undefined1 local_103c [12];
  byte local_1030 [4];
  undefined4 local_102c;
  undefined4 uStack_1028;
  undefined4 local_1024;
  char acStack_1020 [4096];
  
  pcVar3 = acStack_1020 + 0x1000;
  uStack_1028 = 0;
  local_1030[1] = 0xaa;
  local_1024 = 0;
  local_1030[0] = 0x55;
  if (0 < param_4) {
    pcVar3 = (char *)0x0;
  }
  local_102c = (uint)CONCAT11(param_2,param_1 << 1);
  local_1030[3] = 0x32;
  local_1030[2] = (char)(param_4 + 6U);
  pcVar2 = (char *)0x0;
  if (0 < param_4) {
    do {
      pbVar5 = (byte *)(pcVar3 + param_3);
      pcVar3 = (char *)((uint)(pcVar3 + 1) & 0xff);
      local_1030[(int)(pcVar2 + 6)] = *pbVar5;
      pcVar2 = pcVar3;
    } while ((int)pcVar3 < param_4);
  }
  if (param_4 < -3) {
    bVar7 = 0;
    bVar4 = 0;
  }
  else {
    uVar8 = 0;
    pbVar5 = local_1030 + 1;
    do {
      pbVar5 = pbVar5 + 1;
      uVar9 = uVar8 + *pbVar5;
      uVar8 = uVar9 & 0xffff;
    } while (pbVar5 != local_1030 + param_4 + 5);
    bVar4 = (byte)(uVar8 >> 8);
    bVar7 = (byte)uVar9;
  }
  local_1030[param_4 + 6] = bVar4;
  local_1030[param_4 + 7] = bVar7;
  pthread_mutex_lock((pthread_mutex_t *)&DAT_000790a0);
  uVar8 = 0;
  do {
    FUN_0003c7b0(local_1030[uVar8]);
    uVar8 = uVar8 + 1 & 0xff;
  } while (uVar8 <= (param_4 + 6U & 0xff) + 1);
  puVar6 = local_1040 + -1;
  usleep(200000);
  do {
    uVar1 = FUN_0003c7c8();
    puVar6 = puVar6 + 1;
    *puVar6 = uVar1;
  } while (puVar6 != local_103c);
  pthread_mutex_unlock((pthread_mutex_t *)&DAT_000790a0);
  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
    snprintf(acStack_1020,0x1000,
             "--- %s failed! read_back_data[0] = 0x%02x, read_back_data[1] = 0x%02x, read_back_data[2] = 0x%02x, read_back_data[3] = 0x%02x\n\n"
             ,"write_dc_dc",0xff,0,0,0);
    FUN_00038730(5,acStack_1020,0);
  }
  return 0xffffffff;
}

