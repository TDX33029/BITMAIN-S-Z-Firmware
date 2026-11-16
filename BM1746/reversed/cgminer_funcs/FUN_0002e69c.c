//==================== FUN_0002e69c @ 0002e69c ====================

undefined4 FUN_0002e69c(int *param_1)

{
  size_t sVar1;
  int iVar2;
  void *__ptr;
  int *piVar3;
  int *piVar4;
  char *pcVar5;
  int iVar6;
  undefined4 uVar7;
  undefined1 auStack_3114 [252];
  char local_3018 [4];
  char acStack_3014 [4];
  char acStack_3010 [4];
  char acStack_300c [4];
  char local_3008 [4];
  char acStack_3004 [4];
  char acStack_3000 [4];
  char acStack_2ffc [4];
  char local_2ff8 [4];
  char acStack_2ff4 [4];
  char acStack_2ff0 [4];
  char acStack_2fec [4];
  char local_2fe8 [4048];
  char acStack_2018 [8196];
  
  iVar2 = DAT_0007b0f8;
  DAT_0007b0f8 = DAT_0007b0f8 + 1;
  sprintf(acStack_2018,"{\"id\": %d, \"method\": \"mining.extranonce.subscribe\", \"params\": []}",
          iVar2);
  sVar1 = strlen(acStack_2018);
  iVar2 = FUN_0002b5c8(param_1,acStack_2018,sVar1);
  if (iVar2 == 0) {
LAB_0002e7a8:
    uVar7 = 0;
  }
  else {
    while (iVar2 = FUN_00028afc(param_1,2), iVar2 != 0) {
      __ptr = (void *)FUN_0002b040(param_1);
      if (__ptr == (void *)0x0) goto LAB_0002e7a8;
      iVar2 = FUN_0002cf40(param_1,__ptr);
      if (iVar2 == 0) {
        iVar2 = FUN_00059230(__ptr,0,auStack_3114);
        free(__ptr);
        piVar3 = (int *)FUN_00059e20(iVar2,"result");
        piVar4 = (int *)FUN_00059e20(iVar2,"error");
        if ((piVar3 == (int *)0x0) || (*piVar3 == 6)) {
          if (piVar4 != (int *)0x0) goto LAB_0002e81c;
          pcVar5 = malloc(0x11);
          if (pcVar5 != (char *)0x0) {
            builtin_strncpy(pcVar5,"(unknown reason)",0x11);
          }
LAB_0002e868:
          if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (5 < DAT_000766c4)) {
            snprintf(local_3018,0x1000,"Pool %d JSON extranonce subscribe failed: %s",*param_1,
                     pcVar5);
            FUN_00038730(6,local_3018,0);
          }
          uVar7 = 0;
          free(pcVar5);
        }
        else {
          if ((piVar4 == (int *)0x0) || (*piVar4 == 7)) {
            if ((DAT_0007ad48 != '\0') || ((DAT_00078e08 != '\0' || (5 < DAT_000766c4)))) {
              iVar6 = *param_1;
              pcVar5 = "Stratum extranonce subscribe for pool %d";
LAB_0002e8fc:
              uVar7 = 1;
              snprintf(local_3018,0x1000,pcVar5,iVar6);
              FUN_00038730(6,local_3018,0);
              goto LAB_0002e918;
            }
          }
          else {
LAB_0002e81c:
            pcVar5 = (char *)FUN_00029294(piVar4,1);
            if (((pcVar5 == (char *)0x0) &&
                (pcVar5 = (char *)FUN_0005a178(piVar4), pcVar5 == (char *)0x0)) ||
               ((iVar6 = strcmp(pcVar5,
                                "Method \'subscribe\' not found for service \'mining.extranonce\'"),
                iVar6 != 0 && (iVar6 = strcmp(pcVar5,"Unrecognized request provided"), iVar6 != 0)))
               ) {
              pcVar5 = (char *)FUN_00057854(piVar4,3);
              goto LAB_0002e868;
            }
            if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (5 < DAT_000766c4)) {
              iVar6 = *param_1;
              pcVar5 = "Cannot subscribe to mining.extranonce for pool %d";
              goto LAB_0002e8fc;
            }
          }
          uVar7 = 1;
        }
LAB_0002e918:
        if (iVar2 == 0) {
          return uVar7;
        }
        if (*(int *)(iVar2 + 4) == -1) {
          return uVar7;
        }
        iVar6 = *(int *)(iVar2 + 4) + -1;
        *(int *)(iVar2 + 4) = iVar6;
        if (iVar6 == 0) {
          FUN_0005a3e4(iVar2);
          return uVar7;
        }
        return uVar7;
      }
      free(__ptr);
    }
    if ((DAT_00078e09 != '\0') &&
       (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
      local_3018[0] = 'T';
      local_3018[1] = 'i';
      local_3018[2] = 'm';
      local_3018[3] = 'e';
      acStack_3014[0] = 'd';
      acStack_3014[1] = ' ';
      acStack_3014[2] = 'o';
      acStack_3014[3] = 'u';
      acStack_3010[0] = 't';
      acStack_3010[1] = ' ';
      acStack_3010[2] = 'w';
      acStack_3010[3] = 'a';
      acStack_300c[0] = 'i';
      acStack_300c[1] = 't';
      acStack_300c[2] = 'i';
      acStack_300c[3] = 'n';
      local_3008[0] = 'g';
      local_3008[1] = ' ';
      local_3008[2] = 'f';
      local_3008[3] = 'o';
      acStack_3004[0] = 'r';
      acStack_3004[1] = ' ';
      acStack_3004[2] = 'r';
      acStack_3004[3] = 'e';
      acStack_3000[0] = 's';
      acStack_3000[1] = 'p';
      acStack_3000[2] = 'o';
      acStack_3000[3] = 'n';
      acStack_2ffc[0] = 's';
      acStack_2ffc[1] = 'e';
      acStack_2ffc[2] = ' ';
      acStack_2ffc[3] = 'e';
      local_2ff8[0] = 'x';
      local_2ff8[1] = 't';
      local_2ff8[2] = 'r';
      local_2ff8[3] = 'a';
      acStack_2ff4[0] = 'n';
      acStack_2ff4[1] = 'o';
      acStack_2ff4[2] = 'n';
      acStack_2ff4[3] = 'c';
      acStack_2ff0[0] = 'e';
      acStack_2ff0[1] = '.';
      acStack_2ff0[2] = 's';
      acStack_2ff0[3] = 'u';
      acStack_2fec[0] = 'b';
      acStack_2fec[1] = 's';
      acStack_2fec[2] = 'c';
      acStack_2fec[3] = 'r';
      local_2fe8[0] = 'i';
      local_2fe8[1] = 'b';
      local_2fe8[2] = 'e';
      local_2fe8[3] = '\0';
      FUN_00038730(7,local_3018,0);
    }
    uVar7 = 1;
  }
  return uVar7;
}

