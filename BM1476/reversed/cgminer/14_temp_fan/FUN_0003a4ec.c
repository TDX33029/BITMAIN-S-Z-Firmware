//==================== FUN_0003a4ec @ 0003a4ec ====================

void FUN_0003a4ec(void)

{
  undefined4 uVar1;
  char *param0;
  int iVar2;
  int iVar3;
  uint in_fpscr;
  double dVar4;
  double dVar5;
  undefined8 uVar6;
  char local_50 [36];
  
  param0 = local_50 + 0x10;
  uVar1 = FUN_00032e64(0,"miner_count",&DAT_0009d6cb,0);
  uVar1 = FUN_00032e64(uVar1,"fan_num",&DAT_0009d6cc,0);
  iVar2 = 0;
  do {
    iVar3 = iVar2 + 1;
    sprintf(param0,"fan%d",iVar3);
    uVar1 = FUN_00032f18(uVar1,param0,&DAT_0009d69d + iVar2 * 4,0);
    iVar2 = iVar3;
  } while (iVar3 != 6);
  uVar1 = FUN_00032e64(uVar1,"temp_num",&DAT_0009d6cb);
  iVar2 = 0;
  do {
    iVar3 = iVar2 + 1;
    sprintf(param0,"temp%d",iVar3);
    uVar1 = FUN_00032e88(uVar1,param0,&DAT_0009d741 + iVar2 * 4,0);
    iVar2 = iVar3;
  } while (iVar3 != 4);
  iVar2 = 0;
  do {
    iVar3 = iVar2 + 1;
    sprintf(param0,"temp2_%d",iVar3);
    uVar1 = FUN_00032e88(uVar1,param0,&DAT_0009d751 + iVar2 * 4,0);
    iVar2 = iVar3;
  } while (iVar3 != 4);
  uVar1 = FUN_00032f18(uVar1,"temp_max",&DAT_0009d6d6,0);
  dVar5 = DAT_0007b5e8 + DAT_0007b670 + DAT_0007b428;
  uVar6 = FUN_0005cc10(SUB84(dVar5,0),(int)((ulonglong)dVar5 >> 0x20));
  DAT_0007b104 = (int)((ulonglong)uVar6 >> 0x20);
  DAT_0007b100 = (uint)uVar6;
  if (DAT_0007b100 + DAT_0007b8a4 == 0 &&
      DAT_0007b104 + ((int)DAT_0007b8a4 >> 0x1f) + (uint)CARRY4(DAT_0007b100,DAT_0007b8a4) == 0) {
    dVar5 = 0.0;
  }
  else {
    dVar5 = (double)VectorSignedFixedToFloat(CONCAT44(DAT_0007b8a4,DAT_0007b8a4),0x20,8);
    dVar4 = (double)FUN_0005cb64();
    uVar6 = CONCAT44(DAT_0007b104,DAT_0007b100);
    dVar5 = dVar5 / dVar4;
  }
  DAT_0007b104 = (int)((ulonglong)uVar6 >> 0x20);
  DAT_0007b100 = (uint)uVar6;
  DAT_00078e24 = (undefined4)((ulonglong)dVar5 >> 0x20);
  DAT_00078e20 = SUB84(dVar5,0);
  uVar1 = FUN_000357a4(uVar1,"Device Hardware%",&DAT_00078e20,1);
  uVar1 = FUN_00032ed0(uVar1,"no_matching_work",&DAT_0007b8a4,1);
  iVar2 = 0;
  do {
    iVar3 = iVar2 + 1;
    sprintf(param0,"chain_acn%d",iVar3);
    uVar1 = FUN_00032e64(uVar1,param0,&DAT_0009d6c5 + iVar2,0);
    iVar2 = iVar3;
  } while (iVar3 != 4);
  iVar2 = 0;
  do {
    iVar3 = iVar2 + 1;
    sprintf(param0,"chain_acs%d",iVar3);
    uVar1 = FUN_00032e1c(uVar1,param0,&DAT_0009d617 + iVar2 * 0x13,0);
    iVar2 = iVar3;
  } while (iVar3 != 4);
  iVar2 = 0;
  do {
    iVar3 = iVar2 + 1;
    sprintf(param0,"chain_hw%d",iVar3);
    uVar1 = FUN_00032f18(uVar1,param0,&DAT_0009d5a7 + iVar2 * 4,0);
    iVar2 = iVar3;
  } while (iVar3 != 4);
  iVar2 = 0;
  do {
    iVar3 = iVar2 + 1;
    local_50[0] = '\0';
    local_50[1] = '\0';
    local_50[2] = '\0';
    local_50[3] = '\0';
    local_50[0x10] = '\0';
    local_50[0x11] = '\0';
    local_50[0x12] = '\0';
    local_50[0x13] = '\0';
    local_50[4] = '\0';
    local_50[5] = '\0';
    local_50[6] = '\0';
    local_50[7] = '\0';
    local_50[8] = '\0';
    local_50[9] = '\0';
    local_50[10] = '\0';
    local_50[0xb] = '\0';
    local_50[0xc] = '\0';
    local_50[0xd] = '\0';
    local_50[0xe] = '\0';
    local_50[0xf] = '\0';
    local_50[0x14] = '\0';
    local_50[0x15] = '\0';
    local_50[0x16] = '\0';
    local_50[0x17] = '\0';
    local_50[0x18] = '\0';
    local_50[0x19] = '\0';
    local_50[0x1a] = '\0';
    local_50[0x1b] = '\0';
    local_50[0x1c] = '\0';
    local_50[0x1d] = '\0';
    local_50[0x1e] = '\0';
    local_50[0x1f] = '\0';
    sprintf(local_50,"frequency%d",iVar3);
    sprintf(param0,"%u",(uint)*(ushort *)(&DAT_0009d4b5 + (iVar2 + 0x128) * 2));
    uVar1 = FUN_00032eac(uVar1,local_50,&DAT_0009d4b5 + (iVar2 + 0x128) * 2,0);
    iVar2 = iVar3;
  } while (iVar3 != 4);
  iVar2 = 0;
  do {
    iVar3 = iVar2 + 1;
    sprintf(param0,"chain_rate%d",iVar3);
    if (DAT_00078e28 != '\0') {
      dVar5 = (double)VectorSignedToFloat(DAT_00078e38,(byte)(in_fpscr >> 0x16) & 3);
      sprintf(&DAT_0007c0a8 + iVar2 * 0x10,"%2.2f",DAT_00078e30 / dVar5);
    }
    uVar1 = FUN_00032e1c(uVar1,param0,&DAT_0007c0a8 + iVar2 * 0x10,0);
    iVar2 = iVar3;
  } while (iVar3 != 4);
  return;
}

