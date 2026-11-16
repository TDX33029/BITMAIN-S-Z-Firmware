//==================== FUN_00056cd8 @ 00056cd8 ====================

void FUN_00056cd8(uint *param_1,int param_2,int param_3)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint *puVar8;
  char local_1028 [4];
  char acStack_1024 [4];
  char acStack_1020 [4];
  char acStack_101c [4];
  char local_1018 [4];
  char acStack_1014 [4];
  char acStack_1010 [4];
  char acStack_100c [4];
  char local_1008 [4068];
  
  if (param_2 < 2) {
    if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
      local_1028[0] = 'X';
      local_1028[1] = 'X';
      local_1028[2] = 'T';
      local_1028[3] = 'E';
      acStack_1024[0] = 'A';
      acStack_1024[1] = ' ';
      acStack_1024[2] = 'd';
      acStack_1024[3] = 'e';
      acStack_1020[0] = 'c';
      acStack_1020[1] = 'o';
      acStack_1020[2] = 'd';
      acStack_1020[3] = 'e';
      acStack_101c[0] = ' ';
      acStack_101c[1] = 'f';
      acStack_101c[2] = 'a';
      acStack_101c[3] = 'i';
      local_1018[0] = 'l';
      local_1018[1] = 'e';
      local_1018[2] = 's';
      local_1018[3] = ',';
      acStack_1014[0] = ' ';
      acStack_1014[1] = 'N';
      acStack_1014[2] = ' ';
      acStack_1014[3] = '<';
      acStack_1010[0] = '=';
      acStack_1010[1] = ' ';
      acStack_1010[2] = '1';
      acStack_1010[3] = ' ';
      acStack_100c[0] = 'f';
      acStack_100c[1] = 'o';
      acStack_100c[2] = 'u';
      acStack_100c[3] = 'n';
      local_1008[0] = 'd';
      local_1008[1] = '.';
      local_1008[2] = '\n';
      local_1008[3] = '\0';
      FUN_00038730(3,local_1028,0);
    }
  }
  else {
    iVar1 = FUN_0005c710(0x34);
    uVar3 = *param_1;
    iVar1 = iVar1 + 6;
    uVar5 = iVar1 * -0x61c88647;
    do {
      uVar7 = (uVar5 & 0xf) >> 2;
      uVar2 = param_2 - 1U;
      puVar8 = param_1 + param_2;
      do {
        uVar6 = uVar2 & 3;
        uVar4 = puVar8[-2];
        uVar2 = uVar2 - 1;
        uVar3 = puVar8[-1] -
                ((uVar4 ^ *(uint *)(param_3 + (uVar6 ^ uVar7) * 4)) + (uVar3 ^ uVar5) ^
                (uVar3 >> 3 ^ uVar4 << 4) + (uVar3 << 2 ^ uVar4 >> 5));
        puVar8 = puVar8 + -1;
        *puVar8 = uVar3;
      } while (uVar2 != 0);
      uVar2 = param_1[param_2 - 1U];
      iVar1 = iVar1 + -1;
      uVar3 = *param_1 -
              ((uVar2 << 4 ^ uVar3 >> 3) + (uVar2 >> 5 ^ uVar3 * 4) ^
              (uVar3 ^ uVar5) + (uVar2 ^ *(uint *)(param_3 + uVar7 * 4)));
      uVar5 = uVar5 + 0x61c88647;
      *param_1 = uVar3;
    } while (iVar1 != 0);
  }
  return;
}

