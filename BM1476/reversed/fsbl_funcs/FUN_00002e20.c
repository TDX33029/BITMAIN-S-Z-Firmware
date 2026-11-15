//==================== FUN_00002e20 @ 00002e20 ====================

uint FUN_00002e20(char *param_1)

{
  char cVar1;
  int iVar2;
  uint uVar3;
  undefined4 uVar4;
  char *pcVar6;
  char *pcVar5;
  undefined4 uVar7;
  
  if (param_1[3] == '\0') {
    cVar1 = *param_1;
  }
  else {
    uVar3 = FUN_00002ce0();
    if (uVar3 != 0) {
      return uVar3;
    }
    cVar1 = *param_1;
  }
  if ((cVar1 == '\x03') && (param_1[4] == '\x01')) {
    pcVar6 = param_1 + 0x40;
    do {
      pcVar5 = pcVar6 + 1;
      *pcVar6 = '\0';
      pcVar6 = pcVar5;
    } while (param_1 + 0x240 != pcVar5);
    uVar4 = *(undefined4 *)(param_1 + 0x10);
    uVar7 = *(undefined4 *)(param_1 + 0xc);
    param_1[0x228] = (char)uVar4;
    param_1[0x23e] = 'U';
    param_1[0x22c] = (char)uVar7;
    param_1[0x229] = (char)((uint)uVar4 >> 8);
    param_1[0x22a] = (char)((uint)uVar4 >> 0x10);
    param_1[0x22b] = (char)((uint)uVar4 >> 0x18);
    param_1[0x22d] = (char)((uint)uVar7 >> 8);
    param_1[0x22e] = (char)((uint)uVar7 >> 0x10);
    param_1[0x23f] = -0x56;
    param_1[0x22f] = (char)((uint)uVar7 >> 0x18);
    param_1[0x224] = 'r';
    param_1[0x225] = 'r';
    *(int *)(param_1 + 0x2c) = *(int *)(param_1 + 0x1c) + 1;
    param_1[0x40] = 'R';
    param_1[0x41] = 'R';
    param_1[0x42] = 'a';
    param_1[0x227] = 'a';
    param_1[0x43] = 'A';
    param_1[0x226] = 'A';
    FUN_0000676c(param_1[1],param_1 + 0x40,*(int *)(param_1 + 0x1c) + 1,1);
    param_1[4] = '\0';
  }
  iVar2 = FUN_000066f8(param_1[1],0);
  return (uint)(iVar2 != 0);
}

