//==================== FUN_00056b3c @ 00056b3c ====================

void FUN_00056b3c(uint *param_1,int param_2,int param_3)

{
  int iVar1;
  uint uVar2;
  uint *puVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
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
      acStack_1024[2] = 'e';
      acStack_1024[3] = 'n';
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
    uVar8 = param_2 - 1;
    iVar1 = FUN_0005c710(0x34);
    uVar5 = param_1[uVar8];
    uVar6 = 0;
    do {
      uVar4 = 0;
      uVar6 = uVar6 + 0x9e3779b9;
      uVar7 = (uVar6 & 0xf) >> 2;
      puVar3 = param_1;
      do {
        uVar9 = uVar4 & 3;
        uVar2 = puVar3[1];
        uVar4 = uVar4 + 1;
        uVar5 = ((uVar5 ^ *(uint *)(param_3 + (uVar9 ^ uVar7) * 4)) + (uVar2 ^ uVar6) ^
                (uVar5 << 4 ^ uVar2 >> 3) + (uVar5 >> 5 ^ uVar2 << 2)) + *puVar3;
        *puVar3 = uVar5;
        puVar3 = puVar3 + 1;
      } while (uVar4 != uVar8);
      uVar4 = *param_1;
      uVar5 = ((uVar5 ^ *(uint *)(param_3 + (uVar7 ^ uVar8 & 3) * 4)) + (uVar6 ^ uVar4) ^
              (uVar4 >> 3 ^ uVar5 * 0x10) + (uVar4 << 2 ^ uVar5 >> 5)) + param_1[uVar8];
      param_1[uVar8] = uVar5;
    } while (uVar6 != iVar1 * -0x61c88647 + 0xb54cda56U);
  }
  return;
}

