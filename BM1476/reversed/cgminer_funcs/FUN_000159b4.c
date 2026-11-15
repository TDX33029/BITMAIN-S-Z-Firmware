//==================== FUN_000159b4 @ 000159b4 ====================

void FUN_000159b4(int param_1)

{
  int iVar1;
  bool bVar2;
  char local_1008 [4];
  char acStack_1004 [4];
  char acStack_1000 [4];
  char acStack_ffc [4];
  char local_ff8 [4];
  char acStack_ff4 [4];
  char acStack_ff0 [4];
  char acStack_fec [4068];
  
  if (((*(char *)(param_1 + 0x6e1) == '\0') && (*(int *)(param_1 + 0x6c0) == 0)) &&
     (*(char *)(param_1 + 0x6e0) == '\0')) {
    iVar1 = *(int *)(param_1 + 0x6d4);
    if (iVar1 != 0) {
      *(int *)(iVar1 + 0x9c) = *(int *)(iVar1 + 0x9c) + 1;
      *(int *)(*(int *)(param_1 + 0x6d4) + 0x40) = *(int *)(*(int *)(param_1 + 0x6d4) + 0x40) + -1;
      *(int *)(*(int *)(param_1 + 0x6d4) + 0x44) = *(int *)(*(int *)(param_1 + 0x6d4) + 0x44) + -1;
    }
    bVar2 = 0xfffffffe < DAT_0007b7c8;
    DAT_0007b7c8 = DAT_0007b7c8 + 1;
    DAT_0007b7cc = DAT_0007b7cc + (uint)bVar2;
  }
  else if ((DAT_00078e09 != '\0') &&
          (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
    local_1008[0] = 'D';
    local_1008[1] = 'i';
    local_1008[2] = 's';
    local_1008[3] = 'c';
    acStack_1004[0] = 'a';
    acStack_1004[1] = 'r';
    acStack_1004[2] = 'd';
    acStack_1004[3] = 'e';
    acStack_1000[0] = 'd';
    acStack_1000[1] = ' ';
    acStack_1000[2] = 'c';
    acStack_1000[3] = 'l';
    acStack_ffc[0] = 'o';
    acStack_ffc[1] = 'n';
    acStack_ffc[2] = 'e';
    acStack_ffc[3] = 'd';
    local_ff8[0] = ' ';
    local_ff8[1] = 'o';
    local_ff8[2] = 'r';
    local_ff8[3] = ' ';
    acStack_ff4[0] = 'r';
    acStack_ff4[1] = 'o';
    acStack_ff4[2] = 'l';
    acStack_ff4[3] = 'l';
    acStack_ff0[0] = 'e';
    acStack_ff0[1] = 'd';
    acStack_ff0[2] = ' ';
    acStack_ff0[3] = 'w';
    acStack_fec[0] = 'o';
    acStack_fec[1] = 'r';
    acStack_fec[2] = 'k';
    acStack_fec[3] = '\0';
    FUN_00038730(7,local_1008,0);
  }
  FUN_00015780(param_1);
  return;
}

