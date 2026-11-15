//==================== FUN_000247d4 @ 000247d4 ====================

void * FUN_000247d4(int param_1,int param_2)

{
  time_t tVar1;
  void *__dest;
  int iVar2;
  time_t tVar3;
  char cVar4;
  int iVar5;
  double dVar6;
  double dVar7;
  char local_1020 [4];
  char acStack_101c [4];
  char acStack_1018 [4];
  char acStack_1014 [4];
  char local_1010 [4];
  char acStack_100c [4];
  char acStack_1008 [4];
  char acStack_1004 [4];
  char local_1000 [4];
  char acStack_ffc [4064];
  
  iVar5 = *(int *)(param_1 + 0x24);
  FUN_0000fc58();
  if ((DAT_00078e09 != '\0') &&
     (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
    local_1020[0] = 'P';
    local_1020[1] = 'o';
    local_1020[2] = 'p';
    local_1020[3] = 'p';
    acStack_101c[0] = 'i';
    acStack_101c[1] = 'n';
    acStack_101c[2] = 'g';
    acStack_101c[3] = ' ';
    acStack_1018[0] = 'w';
    acStack_1018[1] = 'o';
    acStack_1018[2] = 'r';
    acStack_1018[3] = 'k';
    acStack_1014[0] = ' ';
    acStack_1014[1] = 'f';
    acStack_1014[2] = 'r';
    acStack_1014[3] = 'o';
    local_1010[0] = 'm';
    local_1010[1] = ' ';
    local_1010[2] = 'g';
    local_1010[3] = 'e';
    acStack_100c[0] = 't';
    acStack_100c[1] = ' ';
    acStack_100c[2] = 'q';
    acStack_100c[3] = 'u';
    acStack_1008[0] = 'e';
    acStack_1008[1] = 'u';
    acStack_1008[2] = 'e';
    acStack_1008[3] = ' ';
    acStack_1004[0] = 't';
    acStack_1004[1] = 'o';
    acStack_1004[2] = ' ';
    acStack_1004[3] = 'g';
    local_1000[0] = 'e';
    local_1000[1] = 't';
    local_1000[2] = ' ';
    local_1000[3] = 'w';
    acStack_ffc[0] = 'o';
    acStack_ffc[1] = 'r';
    acStack_ffc[2] = 'k';
    acStack_ffc[3] = '\0';
    FUN_00038730(7,local_1020,0);
  }
  tVar1 = time((time_t *)0x0);
  do {
    while( true ) {
      __dest = (void *)FUN_000193b4(1);
      iVar2 = FUN_00022b44(__dest,0);
      if (iVar2 == 0) break;
      FUN_000159b4(__dest);
      FUN_00019840();
    }
  } while (__dest == (void *)0x0);
  tVar3 = time((time_t *)0x0);
  iVar2 = tVar3 - tVar1;
  cVar4 = DAT_00078e09;
  if (0 < iVar2) {
    if (DAT_00078e09 != '\0') {
      if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 7)) {
        cVar4 = '\x01';
      }
      else {
        snprintf(local_1020,0x1000,"Get work blocked for %d seconds",iVar2);
        FUN_00038730(7,local_1020,0);
        cVar4 = DAT_00078e09;
      }
    }
    *(int *)(iVar5 + 0xe8) = *(int *)(iVar5 + 0xe8) + iVar2;
  }
  if ((cVar4 != '\0') && (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4))))
  {
    snprintf(local_1020,0x1000,"Got work from get queue to get work for thread %d",param_2);
    FUN_00038730(7,local_1020,0);
  }
  *(int *)((int)__dest + 0x6d0) = param_2;
  if (DAT_0007730d != '\0') {
    iVar2 = *(int *)(iVar5 + 0x17c) + *(int *)(iVar5 + 0x180);
    *(int *)(iVar5 + 0x17c) = iVar2;
    if (iVar2 < 1) {
      *(undefined4 *)(iVar5 + 0x180) = 1;
    }
    else if (0xf < iVar2) {
      iVar2 = *(int *)(iVar5 + 0x178) + 1;
      *(undefined4 *)(iVar5 + 0x180) = 0xffffffff;
      if (0xf < iVar2) {
        iVar2 = 0;
      }
      *(int *)(iVar5 + 0x178) = iVar2;
      memcpy(__dest,&DAT_0007731c + iVar2 * 0xa0,0xa0);
      goto LAB_0002491c;
    }
    memcpy(__dest,&DAT_00077d1c + iVar2 * 0xa0,0xa0);
  }
LAB_0002491c:
  FUN_0000fc24(param_1);
  *(undefined1 *)((int)__dest + 0x6e0) = 1;
  dVar6 = *(double *)(*(int *)(iVar5 + 4) + 0x68);
  if (*(double *)((int)__dest + 0x748) < dVar6) {
    dVar6 = *(double *)((int)__dest + 0x748);
  }
  *(double *)((int)__dest + 0x6b0) = dVar6;
  dVar7 = *(double *)(*(int *)(iVar5 + 4) + 0x78);
  if (dVar6 < dVar7) {
    dVar6 = dVar7;
  }
  *(double *)((int)__dest + 0x6b0) = dVar6;
  return __dest;
}

