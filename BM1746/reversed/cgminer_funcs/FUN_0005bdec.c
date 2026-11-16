//==================== FUN_0005bdec @ 0005bdec ====================

undefined4 FUN_0005bdec(undefined4 param_1,undefined4 *param_2,int *param_3,code *param_4)

{
  int *piVar1;
  char *pcVar2;
  int iVar3;
  size_t sVar4;
  size_t sVar5;
  void *__ptr;
  int iVar6;
  int iVar7;
  int iVar8;
  int *piVar9;
  char *pcVar10;
  int iVar11;
  int iVar12;
  int local_30;
  size_t local_2c [2];
  
  pcVar2 = getenv("POSIXLY_CORRECT");
  pcVar10 = (char *)param_2[1];
  if (pcVar2 == (char *)0x0) {
    if (pcVar10 != (char *)0x0) {
      if (*pcVar10 == '-') {
        piVar9 = param_2 + 1;
        iVar8 = 1;
      }
      else {
        iVar8 = 1;
        piVar1 = param_2 + 2;
        do {
          piVar9 = piVar1;
          pcVar10 = (char *)*piVar9;
          iVar8 = iVar8 + 1;
          if (pcVar10 == (char *)0x0) {
            return 0;
          }
          piVar1 = piVar9 + 1;
        } while (*pcVar10 != '-');
      }
      goto LAB_0005be40;
    }
  }
  else {
    piVar9 = param_2 + 1;
    if ((pcVar10 != (char *)0x0) && (*pcVar10 == '-')) {
      iVar8 = 1;
LAB_0005be40:
      if (pcVar10[1] == '-') {
        if (pcVar10[2] == '\0') {
          FUN_0005bdb8(param_1,param_2,iVar8);
          return 0;
        }
        if (*param_3 != 0) {
                    /* WARNING: Subroutine does not return */
          __assert_fail("*offset == 0","opt/parse.c",0x3b,"parse_one");
        }
        pcVar2 = (char *)FUN_0005ba08(&local_30,local_2c);
        sVar4 = local_2c[0];
        while (pcVar2 != (char *)0x0) {
          iVar6 = *piVar9;
          local_2c[0] = sVar4;
          iVar7 = strncmp((char *)(iVar6 + 2),pcVar2,sVar4);
          if (iVar7 == 0) {
            sVar5 = sVar4 + 2;
            if (*(char *)(iVar6 + sVar5) == '=') {
              iVar7 = iVar6 + sVar4 + 3;
              pcVar10 = pcVar2 + -2;
              iVar12 = DAT_0009d77c + local_30 * 0x1c;
              iVar11 = *(int *)(iVar12 + 4);
              if (iVar11 == 1) {
                local_2c[0] = sVar5;
                if (iVar7 != 0) {
                  (*param_4)("%s: %.*s: %s",*param_2,sVar5,pcVar10,"doesn\'t allow an argument");
                  return 0xffffffff;
                }
                goto LAB_0005bf60;
              }
              local_2c[0] = sVar5;
              if (iVar7 != 0) goto LAB_0005c08c;
            }
            else {
              if (*(char *)(iVar6 + sVar5) != '\0') goto LAB_0005bfe4;
              pcVar10 = pcVar2 + -2;
              iVar12 = local_30 * 0x1c + DAT_0009d77c;
              iVar11 = *(int *)(iVar12 + 4);
              local_2c[0] = sVar5;
              if (iVar11 == 1) goto LAB_0005bf60;
            }
            pcVar10 = pcVar2 + -2;
            iVar3 = *param_3;
            local_2c[0] = sVar5;
            goto LAB_0005c074;
          }
LAB_0005bfe4:
          pcVar2 = (char *)FUN_0005ba60(pcVar2,&local_30,local_2c);
          sVar4 = local_2c[0];
        }
      }
      else {
        for (pcVar10 = (char *)FUN_0005bab8(&local_30); pcVar10 != (char *)0x0;
            pcVar10 = (char *)FUN_0005bb14(pcVar10,&local_30)) {
          iVar6 = *piVar9;
          iVar3 = *param_3 + 1;
          if (*(char *)(iVar6 + iVar3) == *pcVar10) {
            pcVar10 = pcVar10 + -1;
            *param_3 = iVar3;
            local_2c[0] = 2;
            iVar12 = DAT_0009d77c + local_30 * 0x1c;
            iVar11 = *(int *)(iVar12 + 4);
            if (iVar11 == 1) {
LAB_0005bf60:
              iVar7 = 0;
              __ptr = (void *)(**(code **)(iVar12 + 8))(*(undefined4 *)(iVar12 + 0x14));
            }
            else {
LAB_0005c074:
              if ((iVar3 == 0) || (iVar7 = iVar6 + iVar3 + 1, *(char *)(iVar6 + iVar3 + 1) == '\0'))
              {
                iVar7 = param_2[iVar8 + 1];
                if (iVar7 == 0) {
                  (*param_4)("%s: %.*s: %s",*param_2,local_2c[0],pcVar10,"requires an argument");
                  return 0xffffffff;
                }
              }
              else {
                *param_3 = 0;
              }
LAB_0005c08c:
              if (iVar11 == 4) {
                FUN_0005b1d4(iVar7,*(undefined4 *)(iVar12 + 0x14));
              }
              iVar6 = DAT_0009d77c + local_30 * 0x1c;
              __ptr = (void *)(**(code **)(iVar6 + 0xc))(iVar7,*(undefined4 *)(iVar6 + 0x14));
            }
            if (__ptr != (void *)0x0) {
              (*param_4)("%s: %.*s: %s",*param_2,local_2c[0],pcVar10,__ptr);
              free(__ptr);
              return 0xffffffff;
            }
            if (*param_3 != 0) {
              if (*(char *)(*piVar9 + *param_3 + 1) != '\0') {
                return 1;
              }
              *param_3 = 0;
            }
            FUN_0005bdb8(param_1,param_2,iVar8);
            if ((iVar7 != 0) && (*piVar9 == iVar7)) {
              FUN_0005bdb8(param_1,param_2,iVar8);
            }
            return 1;
          }
        }
      }
      pcVar2 = (char *)*piVar9;
      sVar4 = strlen(pcVar2);
      (*param_4)("%s: %.*s: %s",*param_2,sVar4,pcVar2,"unrecognized option");
      return 0xffffffff;
    }
  }
  return 0;
}

