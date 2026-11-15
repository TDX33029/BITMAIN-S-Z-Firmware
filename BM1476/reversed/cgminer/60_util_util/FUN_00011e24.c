//==================== FUN_00011e24 @ 00011e24 ====================

undefined4 FUN_00011e24(int *param_1)

{
  int iVar1;
  time_t tVar2;
  int iVar3;
  int iVar4;
  char acStack_1020 [16];
  char local_1010 [4];
  char acStack_100c [4];
  char acStack_1008 [4];
  char acStack_1004 [4];
  char local_1000 [4];
  char acStack_ffc [4];
  char acStack_ff8 [4];
  undefined2 uStack_ff4;
  undefined1 local_ff2;
  
  iVar4 = param_1[9];
  iVar3 = *(int *)(iVar4 + 4);
  snprintf(acStack_1020,0x10,"%d/Miner",*param_1);
  FUN_0002f0b4(acStack_1020);
  FUN_0000fc58(param_1);
  iVar1 = (**(code **)(iVar3 + 0x34))(param_1);
  if (iVar1 == 0) {
    FUN_0002ed88(iVar4);
  }
  else {
    if ((DAT_00078e09 != '\0') &&
       (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
      local_1010[0] = 'W';
      local_1010[1] = 'a';
      local_1010[2] = 'i';
      local_1010[3] = 't';
      acStack_100c[0] = 'i';
      acStack_100c[1] = 'n';
      acStack_100c[2] = 'g';
      acStack_100c[3] = ' ';
      acStack_1008[0] = 'o';
      acStack_1008[1] = 'n';
      acStack_1008[2] = ' ';
      acStack_1008[3] = 's';
      acStack_1004[0] = 'e';
      acStack_1004[1] = 'm';
      acStack_1004[2] = ' ';
      acStack_1004[3] = 'i';
      local_1000[0] = 'n';
      local_1000[1] = ' ';
      local_1000[2] = 'm';
      local_1000[3] = 'i';
      acStack_ffc[0] = 'n';
      acStack_ffc[1] = 'e';
      acStack_ffc[2] = 'r';
      acStack_ffc[3] = ' ';
      acStack_ff8[0] = 't';
      acStack_ff8[1] = 'h';
      acStack_ff8[2] = 'r';
      acStack_ff8[3] = 'e';
      uStack_ff4 = 0x6461;
      local_ff2 = 0;
      FUN_00038730(7,local_1010,0);
    }
    FUN_0002f280(param_1 + 4,"cgminer.c","miner_thread",0x23bb);
    tVar2 = time((time_t *)0x0);
    *(time_t *)(iVar4 + 0xe8) = tVar2;
    (**(code **)(iVar3 + 0x3c))(param_1);
    (**(code **)(iVar3 + 0x58))(param_1);
  }
  return 0;
}

