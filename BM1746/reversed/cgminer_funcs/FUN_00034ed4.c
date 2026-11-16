//==================== FUN_00034ed4 @ 00034ed4 ====================

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00034ed4(int param_1,undefined4 param_2,undefined4 param_3,int param_4)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  double local_c0;
  undefined4 local_b8;
  undefined4 local_b4;
  undefined4 local_b0;
  undefined4 local_ac;
  undefined4 local_a8;
  undefined4 local_a4;
  undefined4 local_a0;
  undefined4 local_9c;
  char local_98 [32];
  undefined4 local_78;
  undefined4 local_74;
  undefined4 local_70;
  undefined4 local_6c;
  undefined4 local_68;
  undefined4 local_64;
  undefined4 local_60;
  undefined4 local_5c;
  undefined4 local_58;
  undefined4 local_54;
  undefined4 local_50;
  undefined4 local_4c;
  undefined4 local_48;
  undefined4 local_44;
  undefined4 local_40;
  undefined4 local_3c;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  
  local_b8 = 0;
  local_b4 = 0;
  local_b0 = 0;
  local_ac = 0;
  local_a8 = 0;
  local_a4 = 0;
  local_a0 = 0;
  local_9c = 0;
  local_98[0] = '\0';
  local_98[1] = '\0';
  local_98[2] = '\0';
  local_98[3] = '\0';
  local_98[4] = '\0';
  local_98[5] = '\0';
  local_98[6] = '\0';
  local_98[7] = '\0';
  local_98[8] = '\0';
  local_98[9] = '\0';
  local_98[10] = '\0';
  local_98[0xb] = '\0';
  local_98[0xc] = '\0';
  local_98[0xd] = '\0';
  local_98[0xe] = '\0';
  local_98[0xf] = '\0';
  local_98[0x10] = '\0';
  local_98[0x11] = '\0';
  local_98[0x12] = '\0';
  local_98[0x13] = '\0';
  local_98[0x14] = '\0';
  local_98[0x15] = '\0';
  local_98[0x16] = '\0';
  local_98[0x17] = '\0';
  local_98[0x18] = '\0';
  local_98[0x19] = '\0';
  local_98[0x1a] = '\0';
  local_98[0x1b] = '\0';
  local_98[0x1c] = '\0';
  local_98[0x1d] = '\0';
  local_98[0x1e] = '\0';
  local_98[0x1f] = '\0';
  local_78 = 0;
  local_74 = 0;
  local_70 = 0;
  local_6c = 0;
  local_68 = 0;
  local_64 = 0;
  local_60 = 0;
  local_5c = 0;
  local_58 = 0;
  local_54 = 0;
  local_50 = 0;
  local_4c = 0;
  local_48 = 0;
  local_44 = 0;
  local_40 = 0;
  local_3c = 0;
  local_38 = 0;
  local_34 = 0;
  local_30 = 0;
  local_2c = 0;
  local_28 = 0;
  local_24 = 0;
  local_20 = 0;
  local_1c = 0;
  iVar1 = FUN_0001da58();
  FUN_0003305c(param_1,7,0,0,param_4);
  if (param_4 == 0) {
    iVar3 = 0;
  }
  else {
    iVar3 = FUN_000307c8(param_1,",\"POOLS\":[");
  }
  local_b8 = CONCAT22(local_b8._2_2_,0x30);
  local_c0 = (_DAT_0007acc0 / 1000.0) / DAT_000755f8;
  uVar2 = FUN_00032e1c(0,&DAT_0006721c,&local_b8,0);
  uVar2 = FUN_00034eb0(uVar2,"GHS5s",&DAT_00077158,0);
  uVar2 = FUN_00034eb0(uVar2,"GHSavg",&local_c0,0);
  sprintf(local_98,"%d",DAT_0007b8a8);
  uVar2 = FUN_00032e1c(uVar2,&DAT_00067230,local_98,0);
  sprintf((char *)&local_78,"%d",DAT_0007bb20);
  uVar2 = FUN_00032e1c(uVar2,&DAT_00067234,&local_78,0);
  if (iVar1 == 0) {
    local_58 = CONCAT13(local_58._3_1_,0x6f6e);
    local_38 = CONCAT13(local_38._3_1_,0x6f6e);
    uVar2 = FUN_00032e1c(uVar2,"pool",&local_58,0);
    uVar2 = FUN_00032e1c(uVar2,&DAT_0006723c,&local_38,0);
  }
  else {
    uVar2 = FUN_00032e1c(uVar2,"pool",*(undefined4 *)(iVar1 + 0xc4),0);
    uVar2 = FUN_00032e1c(uVar2,&DAT_0006723c,*(undefined4 *)(iVar1 + 0xcc),0);
  }
  FUN_00032534(param_1,uVar2,param_4,param_4);
  if (iVar3 != 0) {
    *(undefined1 *)(param_1 + 0xd) = 1;
  }
  return;
}

