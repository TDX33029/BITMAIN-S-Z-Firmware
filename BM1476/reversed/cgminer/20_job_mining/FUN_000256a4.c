//==================== FUN_000256a4 @ 000256a4 ====================

void FUN_000256a4(void *param_1)

{
  int iVar1;
  char cVar2;
  uint uVar3;
  char *__format;
  int *piVar4;
  int iVar5;
  bool bVar6;
  pthread_t pStack_101c;
  char local_1018 [4];
  char acStack_1014 [4];
  char acStack_1010 [4];
  char acStack_100c [4];
  char local_1008 [4];
  char acStack_1004 [4];
  char acStack_1000 [4];
  char acStack_ffc [4];
  uint local_ff8 [1017];
  
  if (*(int *)((int)param_1 + 0x4c) == 0) {
    if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 4)) {
      return;
    }
    local_1018[0] = 'G';
    local_1018[1] = 'e';
    local_1018[2] = 't';
    local_1018[3] = ' ';
    acStack_1014[0] = 'A';
    acStack_1014[1] = ' ';
    acStack_1014[2] = 'E';
    acStack_1014[3] = 'r';
    acStack_1010[0] = 'r';
    acStack_1010[1] = 'o';
    acStack_1010[2] = 'r';
    acStack_1010[3] = ' ';
    acStack_100c[0] = 'D';
    acStack_100c[1] = 'a';
    acStack_100c[2] = 't';
    acStack_100c[3] = 'a';
    local_1008[0] = ' ';
    local_1008[1] = 'T';
    local_1008[2] = 'o';
    local_1008[3] = ' ';
    acStack_1004[0] = 'P';
    acStack_1004[1] = 'u';
    acStack_1004[2] = 's';
    acStack_1004[3] = 'h';
    acStack_1000[0] = '!';
    acStack_1000[1] = 'D';
    acStack_1000[2] = 'R';
    acStack_1000[3] = 'O';
    acStack_ffc[0] = 'P';
    acStack_ffc[1] = '!';
    acStack_ffc[2] = '\r';
    acStack_ffc[3] = '\n';
    local_ff8[0] = local_ff8[0] & 0xffffff00;
    FUN_00038730(4,local_1018,0);
    return;
  }
  piVar4 = *(int **)((int)param_1 + 0x6d4);
  FUN_0002a84c((int)param_1 + 0x780);
  if (DAT_0007730d != '\0') {
    iVar1 = FUN_0001b124(*(undefined4 *)((int)param_1 + 0x6d0));
    iVar5 = *(int *)(iVar1 + 0x24);
    iVar1 = pthread_mutex_lock((pthread_mutex_t *)&DAT_0007ad4c);
    if (iVar1 != 0) {
      FUN_0000b414("submit_work_async",0x202c);
    }
    *(int *)(iVar5 + 0x24) = *(int *)(iVar5 + 0x24) + 1;
    uVar3 = piVar4[2];
    piVar4[2] = uVar3 + 1;
    piVar4[3] = piVar4[3] + (uint)(0xfffffffe < uVar3);
    bVar6 = 0xfffffffe < DAT_0007b420;
    DAT_0007b420 = DAT_0007b420 + 1;
    DAT_0007b424 = DAT_0007b424 + (uint)bVar6;
    *(double *)(iVar5 + 200) = *(double *)(iVar5 + 200) + *(double *)((int)param_1 + 0x748);
    DAT_0007b5e8 = *(double *)((int)param_1 + 0x748) + DAT_0007b5e8;
    *(double *)(piVar4 + 0x1a) = *(double *)(piVar4 + 0x1a) + *(double *)((int)param_1 + 0x748);
    iVar1 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_0007ad4c);
    if (iVar1 != 0) {
      FUN_0000b5c4("submit_work_async",0x2033);
    }
    (*DAT_00075ed8)();
    if (DAT_00078e09 == '\0') {
      return;
    }
    if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 7)) {
      return;
    }
    snprintf(local_1018,0x1000,"Accepted %s %d benchmark share nonce %08x",
             *(char **)(*(int *)(iVar5 + 4) + 8),*(int *)(iVar5 + 8),*(uint *)((int)param_1 + 0x4c))
    ;
    FUN_00038730(7,local_1018,0);
    return;
  }
  iVar1 = FUN_00022b44(param_1,1);
  if (iVar1 == 0) {
    cVar2 = DAT_00078e09;
    goto LAB_0002586c;
  }
  if (DAT_00075e0d == '\0') {
    if ((char)piVar4[0x22] == '\0') {
      if ((DAT_00078e09 != '\0') &&
         (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
        snprintf(local_1018,0x1000,"Pool %d stale share detected, discarding",*piVar4);
        FUN_00038730(7,local_1018,0);
      }
      FUN_0001b600("discard",param_1);
      iVar1 = pthread_mutex_lock((pthread_mutex_t *)&DAT_0007ad4c);
      if (iVar1 != 0) {
        FUN_0000b414("submit_work_async",0x2045);
      }
      piVar4[0x26] = piVar4[0x26] + 1;
      bVar6 = 0xfffffffe < DAT_0007bc80;
      DAT_0007bc80 = DAT_0007bc80 + 1;
      DAT_0007bc84 = DAT_0007bc84 + (uint)bVar6;
      DAT_0007b428 = *(double *)((int)param_1 + 0x748) + DAT_0007b428;
      *(double *)(piVar4 + 0x1e) = *(double *)(piVar4 + 0x1e) + *(double *)((int)param_1 + 0x748);
      iVar1 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_0007ad4c);
      if (iVar1 != 0) {
        FUN_0000b5c4("submit_work_async",0x204a);
      }
      (*DAT_00075ed8)();
      FUN_00015780(param_1);
      return;
    }
    cVar2 = '\0';
    if (DAT_00078e09 != '\0') {
      if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 7))
      goto LAB_000259bc;
      __format = "Pool %d stale share detected, submitting as pool requested";
LAB_00025a90:
      snprintf(local_1018,0x1000,__format,*piVar4);
      FUN_00038730(7,local_1018,0);
      cVar2 = DAT_00078e09;
    }
  }
  else {
    cVar2 = '\0';
    if (DAT_00078e09 != '\0') {
      if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)) {
        __format = "Pool %d stale share detected, submitting as user requested";
        goto LAB_00025a90;
      }
LAB_000259bc:
      cVar2 = '\x01';
    }
  }
  *(undefined1 *)((int)param_1 + 0x6e9) = 1;
LAB_0002586c:
  if (*(char *)((int)param_1 + 0x6ec) == '\0') {
    if ((cVar2 != '\0') &&
       (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
      local_1018[0] = 'P';
      local_1018[1] = 'u';
      local_1018[2] = 's';
      local_1018[3] = 'h';
      acStack_1014[0] = 'i';
      acStack_1014[1] = 'n';
      acStack_1014[2] = 'g';
      acStack_1014[3] = ' ';
      acStack_1010[0] = 's';
      acStack_1010[1] = 'u';
      acStack_1010[2] = 'b';
      acStack_1010[3] = 'm';
      acStack_100c[0] = 'i';
      acStack_100c[1] = 't';
      acStack_100c[2] = ' ';
      acStack_100c[3] = 'w';
      local_1008[0] = 'o';
      local_1008[1] = 'r';
      local_1008[2] = 'k';
      local_1008[3] = ' ';
      acStack_1004[0] = 't';
      acStack_1004[1] = 'o';
      acStack_1004[2] = ' ';
      acStack_1004[3] = 'w';
      acStack_1000[0] = 'o';
      acStack_1000[1] = 'r';
      acStack_1000[2] = 'k';
      acStack_1000[3] = ' ';
      acStack_ffc[0] = 't';
      acStack_ffc[1] = 'h';
      acStack_ffc[2] = 'r';
      acStack_ffc[3] = 'e';
      local_ff8[0] = CONCAT13(local_ff8[0]._3_1_,0x6461);
      FUN_00038730(7,local_1018,0);
    }
    iVar1 = pthread_create(&pStack_101c,(pthread_attr_t *)0x0,(__start_routine *)&LAB_0000fc10,
                           param_1);
    if (iVar1 != 0) {
      local_1018[0] = 'F';
      local_1018[1] = 'a';
      local_1018[2] = 'i';
      local_1018[3] = 'l';
      acStack_1014[0] = 'e';
      acStack_1014[1] = 'd';
      acStack_1014[2] = ' ';
      acStack_1014[3] = 't';
      acStack_1010[0] = 'o';
      acStack_1010[1] = ' ';
      acStack_1010[2] = 'c';
      acStack_1010[3] = 'r';
      acStack_100c[0] = 'e';
      acStack_100c[1] = 'a';
      acStack_100c[2] = 't';
      acStack_100c[3] = 'e';
      local_1008[0] = ' ';
      local_1008[1] = 's';
      local_1008[2] = 'u';
      local_1008[3] = 'b';
      acStack_1004[0] = 'm';
      acStack_1004[1] = 'i';
      acStack_1004[2] = 't';
      acStack_1004[3] = '_';
      acStack_1000[0] = 'w';
      acStack_1000[1] = 'o';
      acStack_1000[2] = 'r';
      acStack_1000[3] = 'k';
      acStack_ffc[0] = '_';
      acStack_ffc[1] = 't';
      acStack_ffc[2] = 'h';
      acStack_ffc[3] = 'r';
      local_ff8[0]._0_1_ = 'e';
      local_ff8[0]._1_1_ = 'a';
      local_ff8[0]._2_1_ = 'd';
      local_ff8[0]._3_1_ = '\0';
      FUN_00038730(3,local_1018,1);
      FUN_00016ca8(1);
    }
  }
  else {
    if ((cVar2 != '\0') &&
       (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
      snprintf(local_1018,0x1000,"Pushing pool %d work to stratum queue",*piVar4);
      FUN_00038730(7,local_1018,0);
    }
    iVar1 = FUN_0002a240(piVar4[0xbc],param_1);
    if (iVar1 == 0) {
      if ((DAT_00078e09 != '\0') &&
         (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
        local_1018[0] = 'D';
        local_1018[1] = 'i';
        local_1018[2] = 's';
        local_1018[3] = 'c';
        acStack_1014[0] = 'a';
        acStack_1014[1] = 'r';
        acStack_1014[2] = 'd';
        acStack_1014[3] = 'i';
        acStack_1010[0] = 'n';
        acStack_1010[1] = 'g';
        acStack_1010[2] = ' ';
        acStack_1010[3] = 'w';
        acStack_100c[0] = 'o';
        acStack_100c[1] = 'r';
        acStack_100c[2] = 'k';
        acStack_100c[3] = ' ';
        local_1008[0] = 'f';
        local_1008[1] = 'r';
        local_1008[2] = 'o';
        local_1008[3] = 'm';
        acStack_1004[0] = ' ';
        acStack_1004[1] = 'r';
        acStack_1004[2] = 'e';
        acStack_1004[3] = 'm';
        acStack_1000[0] = 'o';
        acStack_1000[1] = 'v';
        acStack_1000[2] = 'e';
        acStack_1000[3] = 'd';
        acStack_ffc[0] = ' ';
        acStack_ffc[1] = 'p';
        acStack_ffc[2] = 'o';
        acStack_ffc[3] = 'o';
        local_ff8[0] = CONCAT22(local_ff8[0]._2_2_,0x6c);
        FUN_00038730(7,local_1018,0);
      }
      FUN_00015780(param_1);
    }
  }
  return;
}

