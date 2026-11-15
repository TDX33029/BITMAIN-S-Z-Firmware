//==================== FUN_000193b4 @ 000193b4 ====================

int FUN_000193b4(int param_1)

{
  int iVar1;
  undefined4 *puVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  bool bVar6;
  timespec tStack_1038;
  int iStack_1030;
  int local_102c;
  char local_1028 [4];
  char acStack_1024 [4];
  char acStack_1020 [4];
  char acStack_101c [4];
  char local_1018 [4];
  char acStack_1014 [4];
  char acStack_1010 [4];
  char acStack_100c [4];
  char local_1008 [4];
  uint local_1004;
  char acStack_1000 [4];
  undefined1 local_ffc;
  
  iVar1 = pthread_mutex_lock(DAT_0007704c);
  if (iVar1 != 0) {
    FUN_0000b414("hash_pop",0x1e23);
  }
  if ((DAT_00077050 == 0) || (*(int *)(*(int *)(DAT_00077050 + 0x728) + 0xc) == 0)) {
    if ((DAT_00077055 != '\0') && (DAT_00075dd8 < DAT_00075ddc)) {
      DAT_00075dd8 = DAT_00075dd8 + 1;
      DAT_00077055 = '\0';
    }
    DAT_00077056 = 1;
    iVar1 = 0;
    if (param_1 == 0) goto LAB_000196a0;
    do {
      FUN_0002a84c(&iStack_1030);
      tStack_1038.tv_nsec = local_102c * 1000;
      tStack_1038.tv_sec = iStack_1030 + 10;
      pthread_cond_signal((pthread_cond_t *)&DAT_0007b1a0);
      iVar1 = pthread_cond_timedwait
                        ((pthread_cond_t *)(DAT_0007b620 + 0x28),DAT_0007704c,&tStack_1038);
      if (((iVar1 != 0) && (DAT_0007b4c8 == '\0')) &&
         ((DAT_0007b4c8 = '\x01', DAT_0007ad48 != '\0' ||
          ((DAT_00078e08 != '\0' || (3 < DAT_000766c4)))))) {
        local_1028[0] = 'W';
        local_1028[1] = 'a';
        local_1028[2] = 'i';
        local_1028[3] = 't';
        acStack_1024[0] = 'i';
        acStack_1024[1] = 'n';
        acStack_1024[2] = 'g';
        acStack_1024[3] = ' ';
        acStack_1020[0] = 'f';
        acStack_1020[1] = 'o';
        acStack_1020[2] = 'r';
        acStack_1020[3] = ' ';
        acStack_101c[0] = 'w';
        acStack_101c[1] = 'o';
        acStack_101c[2] = 'r';
        acStack_101c[3] = 'k';
        local_1018[0] = ' ';
        local_1018[1] = 't';
        local_1018[2] = 'o';
        local_1018[3] = ' ';
        acStack_1014[0] = 'b';
        acStack_1014[1] = 'e';
        acStack_1014[2] = ' ';
        acStack_1014[3] = 'a';
        acStack_1010[0] = 'v';
        acStack_1010[1] = 'a';
        acStack_1010[2] = 'i';
        acStack_1010[3] = 'l';
        acStack_100c[0] = 'a';
        acStack_100c[1] = 'b';
        acStack_100c[2] = 'l';
        acStack_100c[3] = 'e';
        local_1008[0] = ' ';
        local_1008[1] = 'f';
        local_1008[2] = 'r';
        local_1008[3] = 'o';
        local_1004._0_1_ = 'm';
        local_1004._1_1_ = ' ';
        local_1004._2_1_ = 'p';
        local_1004._3_1_ = 'o';
        acStack_1000[0] = 'o';
        acStack_1000[1] = 'l';
        acStack_1000[2] = 's';
        acStack_1000[3] = '.';
        local_ffc = 0;
        FUN_00038730(4,local_1028,0);
      }
    } while ((DAT_00077050 == 0) || (*(int *)(*(int *)(DAT_00077050 + 0x728) + 0xc) == 0));
  }
  if (DAT_0007b4c8 != '\0') {
    if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
      local_1028[0] = 'W';
      local_1028[1] = 'o';
      local_1028[2] = 'r';
      local_1028[3] = 'k';
      acStack_1024[0] = ' ';
      acStack_1024[1] = 'a';
      acStack_1024[2] = 'v';
      acStack_1024[3] = 'a';
      acStack_1020[0] = 'i';
      acStack_1020[1] = 'l';
      acStack_1020[2] = 'a';
      acStack_1020[3] = 'b';
      acStack_101c[0] = 'l';
      acStack_101c[1] = 'e';
      acStack_101c[2] = ' ';
      acStack_101c[3] = 'f';
      local_1018[0] = 'r';
      local_1018[1] = 'o';
      local_1018[2] = 'm';
      local_1018[3] = ' ';
      acStack_1014[0] = 'p';
      acStack_1014[1] = 'o';
      acStack_1014[2] = 'o';
      acStack_1014[3] = 'l';
      acStack_1010[0] = 's';
      acStack_1010[1] = ',';
      acStack_1010[2] = ' ';
      acStack_1010[3] = 'r';
      acStack_100c[0] = 'e';
      acStack_100c[1] = 's';
      acStack_100c[2] = 'u';
      acStack_100c[3] = 'm';
      local_1008[0] = 'i';
      local_1008[1] = 'n';
      local_1008[2] = 'g';
      local_1008[3] = '.';
      local_1004 = local_1004 & 0xffffff00;
      FUN_00038730(4,local_1028,0);
    }
    DAT_0007b4c8 = '\0';
  }
  iVar5 = DAT_00077050;
  if (DAT_00077050 == 0) {
    puVar2 = puRam00000728;
    iVar1 = DAT_00077050;
  }
  else {
    puVar2 = *(undefined4 **)(DAT_00077050 + 0x728);
    if (((DAT_00077058 < (int)puVar2[3]) && (*(char *)(DAT_00077050 + 0x6e1) == '\0')) &&
       (iVar1 = *(int *)(DAT_00077050 + 0x730), *(int *)(DAT_00077050 + 0x6e4) != 0)) {
      while (iVar1 != 0) {
        if (*(char *)(iVar1 + 0x6e1) != '\0') {
          iVar4 = *(int *)(iVar1 + 0x72c);
          goto joined_r0x0001977c;
        }
        if (*(int *)(iVar1 + 0x6e4) == 0) break;
        iVar1 = *(int *)(iVar1 + 0x730);
      }
    }
    else {
      iVar1 = DAT_00077050;
    }
  }
  iVar4 = *(int *)(iVar1 + 0x72c);
joined_r0x0001977c:
  if ((iVar4 == 0) && (*(int *)(iVar1 + 0x730) == 0)) {
    free((void *)*puVar2);
    free(*(void **)(DAT_00077050 + 0x728));
    DAT_00077050 = 0;
  }
  else {
    if (puVar2[4] - puVar2[5] == iVar1) {
      puVar2[4] = iVar4 + puVar2[5];
      iVar4 = *(int *)(iVar1 + 0x72c);
    }
    if (iVar4 == 0) {
      iVar4 = *(int *)(iVar1 + 0x730);
      iVar5 = iVar4;
      DAT_00077050 = iVar4;
    }
    else {
      *(undefined4 *)(iVar4 + *(int *)(*(int *)(iVar5 + 0x728) + 0x14) + 8) =
           *(undefined4 *)(iVar1 + 0x730);
      iVar4 = *(int *)(iVar1 + 0x730);
    }
    if (iVar4 != 0) {
      *(undefined4 *)(iVar4 + *(int *)(*(int *)(iVar5 + 0x728) + 0x14) + 4) =
           *(undefined4 *)(iVar1 + 0x72c);
    }
    iVar4 = ((*(int **)(iVar5 + 0x728))[1] - 1U & *(uint *)(iVar1 + 0x744)) * 0xc;
    iVar3 = **(int **)(iVar5 + 0x728) + iVar4;
    *(int *)(iVar3 + 4) = *(int *)(iVar3 + 4) + -1;
    puVar2 = (undefined4 *)(iVar5 + 0x728);
    bVar6 = *(int *)(*(int *)*puVar2 + iVar4) == iVar1 + 0x728;
    if (bVar6) {
      iVar5 = *(int *)(iVar1 + 0x738);
      *(int *)(*(int *)*puVar2 + iVar4) = iVar5;
    }
    if (bVar6) {
      iVar5 = DAT_00077050;
    }
    if (*(int *)(iVar1 + 0x734) != 0) {
      *(undefined4 *)(*(int *)(iVar1 + 0x734) + 0x10) = *(undefined4 *)(iVar1 + 0x738);
    }
    if (*(int *)(iVar1 + 0x738) != 0) {
      *(undefined4 *)(*(int *)(iVar1 + 0x738) + 0xc) = *(undefined4 *)(iVar1 + 0x734);
    }
    *(int *)(*(int *)(iVar5 + 0x728) + 0xc) = *(int *)(*(int *)(iVar5 + 0x728) + 0xc) + -1;
  }
  if ((*(char *)(iVar1 + 0x6e1) == '\0') && (*(int *)(iVar1 + 0x6e4) != 0)) {
    DAT_00077058 = DAT_00077058 + -1;
  }
  pthread_cond_signal((pthread_cond_t *)&DAT_0007b1a0);
  pthread_cond_signal((pthread_cond_t *)(DAT_0007b620 + 0x28));
  DAT_0007b948 = time((time_t *)0x0);
LAB_000196a0:
  iVar5 = pthread_mutex_unlock(DAT_0007704c);
  if (iVar5 != 0) {
    FUN_0000b5c4("hash_pop",0x1e66);
  }
  (*DAT_00075ed8)();
  return iVar1;
}

