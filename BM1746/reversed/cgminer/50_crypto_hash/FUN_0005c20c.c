//==================== FUN_0005c20c @ 0005c20c ====================

char * FUN_0005c20c(char *param_1,char *param_2)

{
  size_t sVar1;
  size_t sVar2;
  size_t sVar3;
  char *pcVar4;
  int iVar5;
  char *pcVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  undefined4 *puVar10;
  undefined4 *puVar11;
  char *pcVar12;
  char *pcVar13;
  uint uVar14;
  uint uVar15;
  char *local_88;
  int local_80;
  char acStack_7c [80];
  undefined4 local_2c;
  
  puVar10 = DAT_0009d77c;
  uVar15 = DAT_0009d774;
  if (param_2 == (char *)0x0) {
    if (DAT_0009d774 == 0) {
      sVar2 = strlen(param_1);
      local_88 = "";
      sVar2 = DAT_0009d770 + 0x14 + sVar2;
      goto LAB_0005c2e8;
    }
    uVar14 = 0;
    puVar11 = DAT_0009d77c;
    do {
      uVar14 = uVar14 + 1;
      if (((code *)puVar11[2] == FUN_0005b3e0) &&
         (local_88 = (char *)puVar11[5], local_88 != (char *)0x0)) {
        sVar2 = strlen(local_88);
        goto LAB_0005c5c8;
      }
      puVar11 = puVar11 + 7;
    } while (uVar14 != DAT_0009d774);
    sVar2 = 0;
    local_88 = "";
LAB_0005c5c8:
    sVar1 = strlen(param_1);
    sVar2 = DAT_0009d770 + 0x14 + sVar1 + sVar2;
  }
  else {
    sVar1 = strlen(param_1);
    sVar2 = strlen(param_2);
    sVar2 = DAT_0009d770 + 0x14 + sVar1 + sVar2;
    local_88 = param_2;
    if (DAT_0009d774 == 0) goto LAB_0005c2e8;
    puVar10 = DAT_0009d77c;
    uVar15 = DAT_0009d774;
  }
  uVar14 = 0;
  do {
    while (puVar10[1] == 8) {
      uVar14 = uVar14 + 1;
      sVar1 = strlen((char *)puVar10[6]);
      puVar10 = puVar10 + 7;
      sVar2 = sVar2 + 3 + sVar1;
      if (uVar15 <= uVar14) goto LAB_0005c2e8;
    }
    pcVar4 = (char *)puVar10[6];
    if (pcVar4 != (char *)&DAT_0009d784) {
      sVar1 = strlen((char *)*puVar10);
      sVar3 = strlen(pcVar4);
      iVar5 = sVar2 + 6 + sVar1 + sVar3;
      iVar9 = iVar5 + 0x15;
      if (puVar10[4] != 0) {
        iVar9 = iVar5 + 0x76;
      }
      sVar2 = iVar9 + 1;
    }
    uVar14 = uVar14 + 1;
    puVar10 = puVar10 + 7;
  } while (uVar14 < uVar15);
LAB_0005c2e8:
  pcVar4 = malloc(sVar2);
  if (pcVar4 != (char *)0x0) {
    iVar5 = sprintf(pcVar4,"Usage: %s",param_1);
    pcVar12 = pcVar4 + iVar5;
    builtin_strncpy(pcVar4 + iVar5," [-",4);
    pcVar6 = (char *)FUN_0005bab8(&local_80);
    pcVar13 = pcVar12;
    if (pcVar6 != (char *)0x0) {
      iVar5 = 0;
      do {
        if ((undefined4 *)DAT_0009d77c[local_80 * 7 + 6] != &DAT_0009d784) {
          pcVar12[iVar5 + 3] = *pcVar6;
          iVar5 = iVar5 + 1;
        }
        pcVar6 = (char *)FUN_0005bb14(pcVar6,&local_80);
      } while (pcVar6 != (char *)0x0);
      if (iVar5 != 0) {
        pcVar13 = pcVar12 + iVar5 + 4;
        (pcVar12 + iVar5 + 3)[0] = ']';
        (pcVar12 + iVar5 + 3)[1] = '\0';
      }
    }
    iVar5 = sprintf(pcVar13," %s",local_88);
    (pcVar13 + iVar5)[0] = '\n';
    (pcVar13 + iVar5)[1] = '\0';
    pcVar13 = pcVar13 + iVar5 + 1;
    if (DAT_0009d774 != 0) {
      iVar5 = 0;
      uVar15 = 0;
      do {
        puVar10 = (undefined4 *)((int)DAT_0009d77c + iVar5);
        if ((undefined4 *)puVar10[6] != &DAT_0009d784) {
          if (puVar10[1] == 8) {
            iVar9 = sprintf(pcVar13,"%s:\n");
            pcVar13 = pcVar13 + iVar9;
          }
          else {
            iVar9 = sprintf(pcVar13,"%s",(char *)*puVar10);
            if (*(int *)((int)DAT_0009d77c + iVar5 + 4) == 2) {
              pcVar12 = *(char **)((int)DAT_0009d77c + iVar5);
              pcVar6 = strchr(pcVar12,0x20);
              if ((pcVar6 == (char *)0x0) && (pcVar6 = strchr(pcVar12,0x3d), pcVar6 == (char *)0x0))
              {
                builtin_strncpy(pcVar13 + iVar9," <arg>",7);
                iVar9 = iVar9 + 6;
              }
            }
            iVar7 = sprintf(pcVar13 + iVar9,"%.*s");
            iVar8 = sprintf(pcVar13 + iVar7 + iVar9,"%s",
                            *(char **)((int)DAT_0009d77c + iVar5 + 0x18));
            iVar8 = iVar7 + iVar9 + iVar8;
            if (*(int *)((int)DAT_0009d77c + iVar5 + 0x10) != 0) {
              local_2c = 0x2e2e2e;
              (**(code **)((int)DAT_0009d77c + iVar5 + 0x10))
                        (acStack_7c,*(undefined4 *)((int)DAT_0009d77c + iVar5 + 0x14));
              iVar9 = sprintf(pcVar13 + iVar8," (default: %s)",acStack_7c);
              iVar8 = iVar8 + iVar9;
            }
            (pcVar13 + iVar8)[0] = '\n';
            (pcVar13 + iVar8)[1] = '\0';
            pcVar13 = pcVar13 + iVar8 + 1;
          }
        }
        uVar15 = uVar15 + 1;
        iVar5 = iVar5 + 0x1c;
      } while (uVar15 < DAT_0009d774);
    }
    *pcVar13 = '\0';
  }
  return pcVar4;
}

