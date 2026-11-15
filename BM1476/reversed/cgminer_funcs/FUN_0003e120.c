//==================== FUN_0003e120 @ 0003e120 ====================

undefined4 FUN_0003e120(char param_1,int param_2,undefined1 param_3,size_t param_4,byte *param_5)

{
  ushort uVar1;
  uint uVar2;
  int iVar3;
  byte *pbVar4;
  int iVar5;
  uint param3;
  int iVar6;
  bool bVar7;
  uint local_105c;
  byte bStack_1055;
  undefined1 local_1054 [4];
  undefined4 uStack_1050;
  undefined4 local_104c;
  byte local_1048 [4];
  undefined4 local_1044;
  undefined4 uStack_1040;
  undefined4 uStack_103c;
  undefined4 uStack_1038;
  undefined4 uStack_1034;
  undefined4 uStack_1030;
  undefined4 local_102c;
  char acStack_1028 [4100];
  
  uStack_1040 = 0;
  local_1054 = (undefined1  [4])0x0;
  uStack_103c = 0;
  uStack_1038 = 0;
  uStack_1050 = 0;
  uStack_1034 = 0;
  uStack_1030 = 0;
  local_1044._0_2_ = CONCAT11(param_3,(byte)param_2 | param_1 << 1);
  uVar1 = (ushort)local_1044;
  local_1044 = (uint)(ushort)local_1044;
  local_102c = 0;
  local_1048[0] = 0x55;
  local_1048[1] = 0xaa;
  local_1048[2] = 0;
  local_1048[3] = 0x26;
  local_104c = 0;
  if (param_2 == 0) {
    iVar5 = param_4 + 4;
    memcpy((void *)((int)&local_1044 + 2),param_5,param_4);
    local_105c = param_4 + 6;
    uVar2 = local_105c & 0xff;
    local_1048[2] = (char)local_105c;
    if (0 < iVar5) {
      iVar6 = param_4 + 7;
      goto LAB_0003e1e4;
    }
    iVar6 = param_4 + 7;
    local_1048[param_4 + 6] = 0;
    local_1048[param_4 + 7] = 0;
    pthread_mutex_lock((pthread_mutex_t *)&DAT_000790a0);
    if (iVar6 < 0) {
      usleep(200000);
      goto LAB_0003e3e0;
    }
    param3 = 0;
  }
  else {
    uVar2 = 7;
    iVar6 = 8;
    iVar5 = 5;
    local_105c = 7;
    local_1044 = (uint)CONCAT12((char)param_4,uVar1);
    local_1048[0] = 0x55;
    local_1048[1] = 0xaa;
    local_1048[2] = 7;
    local_1048[3] = 0x26;
LAB_0003e1e4:
    iVar3 = 0;
    pbVar4 = local_1048 + 2;
    param3 = 0;
    while( true ) {
      iVar3 = iVar3 + 1;
      param3 = param3 + uVar2 & 0xffff;
      if (iVar5 <= iVar3) break;
      pbVar4 = pbVar4 + 1;
      uVar2 = (uint)*pbVar4;
    }
    local_1048[local_105c] = (byte)(param3 >> 8);
    local_1048[iVar6] = (byte)param3;
    pthread_mutex_lock((pthread_mutex_t *)&DAT_000790a0);
  }
  iVar5 = 0;
  do {
    if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 5)) {
      uVar2 = (uint)local_1048[iVar5];
    }
    else {
      uVar2 = (uint)local_1048[iVar5];
      snprintf(acStack_1028,0x1000,"write pic i2c[%d] %02x\n",iVar5,uVar2);
      FUN_00038730(5,acStack_1028,0);
    }
    iVar5 = iVar5 + 1;
    FUN_0003c7b0(uVar2);
  } while (iVar5 <= iVar6);
  usleep(200000);
  if (param_2 != 1) {
LAB_0003e3e0:
    pthread_mutex_unlock((pthread_mutex_t *)&DAT_000790a0);
    return 0;
  }
  if (-1 < (int)(param_4 + 4)) {
    iVar5 = 0;
    do {
      uVar2 = FUN_0003c7c8();
      bVar7 = DAT_0007ad48 != '\0';
      iVar6 = iVar5 + 1;
      local_1054[iVar5] = (char)uVar2;
      if (((bVar7) || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
        snprintf(acStack_1028,0x1000,"read pic i2c[%d] %02x\n",iVar5,uVar2);
        FUN_00038730(5,acStack_1028,0);
      }
      iVar5 = iVar6;
    } while (iVar6 <= (int)(param_4 + 4));
  }
  if (-3 < (int)param_4) {
    pbVar4 = &bStack_1055;
    do {
      pbVar4 = pbVar4 + 1;
      param3 = param3 + *pbVar4 & 0xffff;
    } while (pbVar4 != local_1054 + param_4 + 2);
  }
  if (*(ushort *)(local_1054 + param_4 + 3) == param3) {
    if ((((byte)local_1054[0] == local_105c) && (local_1054[1] == '&')) && (local_1054[2] == '\x01')
       ) {
      memcpy(param_5,local_1054 + 3,param_4);
      goto LAB_0003e3e0;
    }
    if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
      snprintf(acStack_1028,0x1000,"pic read failed %d %d %d\n",(uint)*param_5,(uint)param_5[1],
               (uint)param_5[2]);
      FUN_00038730(5,acStack_1028,0);
    }
  }
  else if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
    snprintf(acStack_1028,0x1000,"crc failed 0x%x\n",param3);
    FUN_00038730(5,acStack_1028,0);
  }
  pthread_mutex_unlock((pthread_mutex_t *)&DAT_000790a0);
  return 0xffffffff;
}

