//==================== FUN_00016dec @ 00016dec ====================

undefined * FUN_00016dec(undefined4 param_1,int param_2)

{
  uint *puVar1;
  char *__s;
  char *param3;
  uint uVar2;
  int *piVar3;
  int *piVar4;
  undefined *puVar5;
  undefined4 uVar6;
  char *param4;
  uint uVar7;
  undefined4 *puVar8;
  char acStack_1028 [4100];
  
  if ((param_2 != 0) && (DAT_00076ff4 == 0)) {
    DAT_00076ff4 = 1;
  }
  if (DAT_00075604 == 0x10) {
LAB_00017080:
    piVar4 = (int *)FUN_00059e20(param_1,"include");
    if (piVar4 == (int *)0x0) {
      puVar5 = (undefined *)0x0;
    }
    else if (*piVar4 == 2) {
      uVar6 = FUN_0005a178(piVar4);
      puVar5 = (undefined *)FUN_00016d98(uVar6,0);
    }
    else {
      puVar5 = (undefined *)0x0;
    }
    return puVar5;
  }
  if ((DAT_00075604 & 8) == 0) {
    puVar8 = &DAT_0007561c;
    do {
      if (puVar8[-7] != 0) {
        __s = (char *)__strdup();
        param3 = strtok(__s,"|");
        while (param3 != (char *)0x0) {
          if ((param3[1] == '-') &&
             (piVar4 = (int *)FUN_00059e20(param_1,param3 + 2), piVar4 != (int *)0x0)) {
            if ((puVar8[-6] & 6) == 0) {
LAB_00016eb8:
              if (((puVar8[-6] & 1) != 0) && (*piVar4 == 5)) {
                param4 = (char *)(*(code *)puVar8[-5])(puVar8[-2]);
                goto LAB_00017060;
              }
              param4 = "Invalid value";
            }
            else {
              if (*piVar4 != 2) {
                if (*piVar4 == 1) {
                  uVar7 = 0;
                  while ((uVar2 = FUN_00059fec(piVar4), uVar7 < uVar2 &&
                         (piVar3 = (int *)FUN_0005a008(piVar4,uVar7), piVar3 != (int *)0x0))) {
                    if (*piVar3 == 2) {
                      uVar6 = FUN_0005a178();
                      param4 = (char *)(*(code *)puVar8[-4])(uVar6,puVar8[-2]);
                      if (puVar8[-6] == 4) {
                        FUN_0005b1d4(uVar6,puVar8[-2]);
                      }
LAB_00016fc4:
                      if (param4 != (char *)0x0) goto joined_r0x00016fd4;
                    }
                    else if (*piVar3 == 0) {
                      param4 = (char *)FUN_00016dec();
                      goto LAB_00016fc4;
                    }
                    uVar7 = uVar7 + 1;
                  }
                  goto LAB_00016e64;
                }
                goto LAB_00016eb8;
              }
              uVar6 = FUN_0005a178();
              param4 = (char *)(*(code *)puVar8[-4])(uVar6,puVar8[-2]);
              if (puVar8[-6] == 4) {
                FUN_0005b1d4(uVar6,puVar8[-2]);
              }
LAB_00017060:
              if (param4 == (char *)0x0) goto LAB_00016e64;
            }
joined_r0x00016fd4:
            if (param_2 == 0) {
              snprintf(&DAT_00076f2c,200,"Parsing JSON option %s: %s",param3,param4);
              return &DAT_00076f2c;
            }
            if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
              snprintf(acStack_1028,0x1000,"Invalid config option %s: %s",param3,param4);
              FUN_00038730(3,acStack_1028,0);
            }
            DAT_00076ff4 = -1;
          }
LAB_00016e64:
          param3 = strtok((char *)0x0,"|");
        }
        free(__s);
      }
      puVar1 = puVar8 + 1;
      if (*puVar1 == 0x10) goto LAB_00017080;
      puVar8 = puVar8 + 7;
    } while ((*puVar1 & 8) == 0);
  }
                    /* WARNING: Subroutine does not return */
  __assert_fail("!(opt->type & OPT_SUBTABLE)","cgminer.c",0x7a0,"parse_config");
}

