//==================== FUN_00031cdc @ 00031cdc ====================

int * FUN_00031cdc(int *param_1,undefined4 param_2,int param_3,char *param_4,char param_5)

{
  int *piVar1;
  int *piVar2;
  int iVar3;
  undefined2 *puVar4;
  char *pcVar5;
  size_t sVar6;
  undefined4 *puVar7;
  undefined4 uVar8;
  char acStack_1020 [4100];
  
  piVar2 = malloc(0x18);
  iVar3 = __strdup(param_2);
  *piVar2 = param_3;
  piVar1 = param_1;
  if (param_1 == (int *)0x0) {
    piVar2[4] = (int)piVar2;
    piVar2[5] = (int)piVar2;
    piVar1 = piVar2;
  }
  piVar2[1] = iVar3;
  if (param_1 != (int *)0x0) {
    piVar2[4] = piVar1[4];
    piVar1[4] = (int)piVar2;
    piVar2[5] = (int)piVar1;
    *(int **)(piVar2[4] + 0x14) = piVar2;
  }
  *(char *)(piVar2 + 3) = param_5;
  if (param_4 == (char *)0x0) {
    *(undefined1 *)(piVar2 + 3) = 0;
    param_4 = "(null)";
    *piVar2 = 2;
  }
  else if (param_5 != '\0') {
    switch(param_3) {
    case 0:
    case 1:
    case 2:
      sVar6 = strlen(param_4);
      pcVar5 = malloc(sVar6 + 1);
      piVar2[2] = (int)pcVar5;
      strcpy(pcVar5,param_4);
      return piVar1;
    case 3:
      pcVar5 = malloc(4);
      piVar2[2] = (int)pcVar5;
      *pcVar5 = *param_4;
      return piVar1;
    case 4:
    case 5:
      puVar4 = malloc(4);
      piVar2[2] = (int)puVar4;
      *puVar4 = *(undefined2 *)param_4;
      return piVar1;
    case 6:
    case 7:
    case 8:
    case 9:
    case 0x10:
      puVar7 = malloc(4);
      piVar2[2] = (int)puVar7;
      *puVar7 = *(undefined4 *)param_4;
      return piVar1;
    case 10:
    case 0xb:
      puVar7 = malloc(8);
      piVar2[2] = (int)puVar7;
      uVar8 = *(undefined4 *)(param_4 + 4);
      *puVar7 = *(undefined4 *)param_4;
      puVar7[1] = uVar8;
      return piVar1;
    case 0xc:
    case 0xd:
    case 0x11:
    case 0x12:
    case 0x14:
    case 0x15:
    case 0x17:
    case 0x18:
    case 0x19:
      puVar7 = malloc(8);
      piVar2[2] = (int)puVar7;
      uVar8 = *(undefined4 *)(param_4 + 4);
      *puVar7 = *(undefined4 *)param_4;
      puVar7[1] = uVar8;
      return piVar1;
    case 0xe:
      pcVar5 = malloc(1);
      piVar2[2] = (int)pcVar5;
      *pcVar5 = *param_4;
      return piVar1;
    case 0xf:
      puVar7 = malloc(8);
      piVar2[2] = (int)puVar7;
      uVar8 = *(undefined4 *)(param_4 + 4);
      *puVar7 = *(undefined4 *)param_4;
      puVar7[1] = uVar8;
      return piVar1;
    case 0x13:
    case 0x16:
    case 0x1a:
      puVar7 = malloc(4);
      piVar2[2] = (int)puVar7;
      *puVar7 = *(undefined4 *)param_4;
      return piVar1;
    }
    if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
      snprintf(acStack_1020,0x1000,"API: unknown1 data type %d ignored",param_3);
      FUN_00038730(3,acStack_1020,0);
    }
    *piVar2 = 1;
    *(undefined1 *)(piVar2 + 3) = 0;
    piVar2[2] = (int)"Unknown";
    return piVar1;
  }
  piVar2[2] = (int)param_4;
  return piVar1;
}

