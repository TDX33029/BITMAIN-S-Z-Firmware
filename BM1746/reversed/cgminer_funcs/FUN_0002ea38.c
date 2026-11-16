//==================== FUN_0002ea38 @ 0002ea38 ====================

undefined4 FUN_0002ea38(int *param_1)

{
  size_t sVar1;
  int iVar2;
  void *__ptr;
  int *piVar3;
  int *piVar4;
  char *param4;
  int iVar5;
  undefined4 uVar6;
  undefined1 auStack_3114 [252];
  char acStack_3018 [4096];
  char acStack_2018 [8196];
  
  iVar2 = DAT_0007b0f8;
  DAT_0007b0f8 = DAT_0007b0f8 + 1;
  sprintf(acStack_2018,
          "{\"id\": %d, \"method\": \"mining.authorize\", \"params\": [\"%s\", \"%s\"]}",iVar2,
          (char *)param_1[0x33],(char *)param_1[0x34]);
  sVar1 = strlen(acStack_2018);
  iVar2 = FUN_0002b5c8(param_1,acStack_2018,sVar1);
  if (iVar2 != 0) {
    while (__ptr = (void *)FUN_0002b040(param_1), __ptr != (void *)0x0) {
      iVar2 = FUN_0002cf40(param_1,__ptr);
      if (iVar2 == 0) {
        iVar2 = FUN_00059230(__ptr,0,auStack_3114);
        free(__ptr);
        piVar3 = (int *)FUN_00059e20(iVar2,"result");
        piVar4 = (int *)FUN_00059e20(iVar2,"error");
        if ((piVar3 == (int *)0x0) || (*piVar3 == 6)) {
          if (piVar4 != (int *)0x0) goto LAB_0002eb3c;
          param4 = malloc(0x11);
          if (param4 != (char *)0x0) {
            builtin_strncpy(param4,"(unknown reason)",0x11);
          }
        }
        else {
          if ((piVar4 == (int *)0x0) || (*piVar4 == 7)) {
            if ((DAT_00078e09 != '\0') &&
               (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
              snprintf(acStack_3018,0x1000,"Stratum authorisation success for pool %d",*param_1);
              FUN_00038730(7,acStack_3018,0);
            }
            *(undefined1 *)((int)param_1 + 0x83) = 1;
            iVar5 = DAT_0007b0f8;
            DAT_00078ca9 = 1;
            if (DAT_0007bb24 != 0) {
              DAT_0007b0f8 = DAT_0007b0f8 + 1;
              sprintf(acStack_2018,
                      "{\"id\": %d, \"method\": \"mining.suggest_difficulty\", \"params\": [%d]}",
                      iVar5,DAT_0007bb24);
              sVar1 = strlen(acStack_2018);
              FUN_0002b5c8(param_1,acStack_2018,sVar1);
            }
            iVar5 = DAT_0007b0f8;
            if (DAT_00075eb4 == 0) {
              uVar6 = 1;
            }
            else {
              DAT_0007b0f8 = DAT_0007b0f8 + 1;
              sprintf(acStack_2018,
                      "{\"id\": %d, \"method\": \"mining.multi_version\", \"params\": [%d]}",iVar5,
                      DAT_00075eb4);
              uVar6 = 1;
              sVar1 = strlen(acStack_2018);
              FUN_0002b5c8(param_1,acStack_2018,sVar1);
            }
            goto LAB_0002ebb4;
          }
LAB_0002eb3c:
          param4 = (char *)FUN_00057854(piVar4,3);
        }
        if ((DAT_00078e09 != '\0') &&
           (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
          snprintf(acStack_3018,0x1000,"pool %d JSON stratum auth failed: %s",*param_1,param4);
          FUN_00038730(7,acStack_3018,0);
        }
        uVar6 = 0;
        free(param4);
        FUN_0002aee0(param_1);
LAB_0002ebb4:
        if (iVar2 == 0) {
          return uVar6;
        }
        if (*(int *)(iVar2 + 4) == -1) {
          return uVar6;
        }
        iVar5 = *(int *)(iVar2 + 4) + -1;
        *(int *)(iVar2 + 4) = iVar5;
        if (iVar5 == 0) {
          FUN_0005a3e4(iVar2);
          return uVar6;
        }
        return uVar6;
      }
      free(__ptr);
    }
  }
  return 0;
}

