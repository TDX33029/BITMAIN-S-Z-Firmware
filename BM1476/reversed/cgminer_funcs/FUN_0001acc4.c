//==================== FUN_0001acc4 @ 0001acc4 ====================

void FUN_0001acc4(int param_1,int param_2)

{
  int iVar1;
  size_t sVar2;
  uint uVar3;
  undefined4 uVar4;
  ulonglong *__src;
  pthread_rwlock_t *__rwlock;
  char *pcVar5;
  void *__s;
  undefined4 uVar6;
  undefined4 uVar7;
  char acStack_1028 [4100];
  
  iVar1 = pthread_mutex_lock((pthread_mutex_t *)(param_1 + 0xf4));
  if (iVar1 != 0) {
    FUN_0000b414("gen_stratum_work",0x1f0d);
  }
  __rwlock = (pthread_rwlock_t *)(param_1 + 0x10c);
  iVar1 = pthread_rwlock_wrlock(__rwlock);
  if (iVar1 != 0) {
    FUN_0000b480("gen_stratum_work",0x1f0d);
  }
  uVar3 = *(uint *)(param_1 + 0x288);
  iVar1 = *(int *)(param_1 + 0x28c);
  __src = (ulonglong *)(param_2 + 0x6f8);
  *(uint *)__src = uVar3;
  *(int *)(param_2 + 0x6fc) = iVar1;
  *(uint *)(param_1 + 0x288) = uVar3 + 1;
  *(uint *)(param_1 + 0x28c) = iVar1 + (uint)(0xfffffffe < uVar3);
  *(undefined4 *)(param_2 + 0x700) = 2;
  iVar1 = pthread_rwlock_unlock(__rwlock);
  if (iVar1 != 0) {
    FUN_0000b630("gen_stratum_work",0x1f12);
  }
  iVar1 = pthread_rwlock_rdlock(__rwlock);
  if (iVar1 != 0) {
    FUN_0000b69c("gen_stratum_work",0x1f12);
  }
  iVar1 = pthread_mutex_unlock((pthread_mutex_t *)(param_1 + 0xf4));
  if (iVar1 != 0) {
    FUN_0000b5c4("gen_stratum_work",0x1f12);
  }
  __s = (void *)(param_2 + 0xe0);
  memset(__s,0,0x5cf);
  memcpy(__s,(void *)(param_1 + 0x644),0x80);
  pcVar5 = *(char **)(param_1 + 0x27c);
  sVar2 = strlen(pcVar5);
  FUN_00029830(param_2 + 0x14c,pcVar5,sVar2 >> 1);
  memcpy((void *)(param_2 + (0x160 - *(size_t *)(param_2 + 0x700))),__src,
         *(size_t *)(param_2 + 0x700));
  FUN_00016494(param_2 + 0x16c);
  uVar4 = *(undefined4 *)(param_1 + 0x2cc);
  *(undefined4 *)(param_2 + 0x708) = *(undefined4 *)(param_1 + 0x2c8);
  *(undefined4 *)(param_2 + 0x70c) = uVar4;
  uVar4 = __strdup(*(undefined4 *)(param_1 + 0x2a0));
  *(undefined4 *)(param_2 + 0x6f0) = uVar4;
  uVar4 = __strdup(*(undefined4 *)(param_1 + 0x27c));
  *(undefined4 *)(param_2 + 0x710) = uVar4;
  uVar4 = __strdup(*(undefined4 *)(param_1 + 0x2b4));
  *(undefined4 *)(param_2 + 0x704) = uVar4;
  iVar1 = pthread_rwlock_unlock(__rwlock);
  if (iVar1 != 0) {
    FUN_0000b630("gen_stratum_work",0x1f27);
  }
  (*DAT_00075ed8)();
  if (DAT_00078e09 == '\0') goto LAB_0001ae28;
  pcVar5 = (char *)FUN_00029730(__s,0x8f);
  if (DAT_00078e09 != '\0') {
    if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 7)) {
LAB_0001afd8:
      if (6 < DAT_000766c4) {
LAB_0001af84:
        snprintf(acStack_1028,0x1000,"[THR%d] job_id %s, nonce1 %s, nonce2 %llu, ntime %s",
                 *(int *)(param_2 + 0x6d0),*(char **)(param_2 + 0x6f0),*(char **)(param_2 + 0x710),
                 *__src,*(char **)(param_2 + 0x704));
        FUN_00038730(7,acStack_1028,0);
      }
    }
    else {
      snprintf(acStack_1028,0x1000,"[THR%d] Generated stratum header %s",*(int *)(param_2 + 0x6d0),
               pcVar5);
      FUN_00038730(7,acStack_1028,0);
      if (DAT_00078e09 != '\0') {
        if ((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) goto LAB_0001afd8;
        goto LAB_0001af84;
      }
    }
  }
  free(pcVar5);
LAB_0001ae28:
  iVar1 = DAT_000770a8;
  uVar7 = *(undefined4 *)(param_1 + 0x4c);
  uVar4 = *(undefined4 *)(param_1 + 0x50);
  uVar6 = *(undefined4 *)(param_1 + 0x54);
  *(undefined4 *)(param_2 + 0xa0) = *(undefined4 *)(param_1 + 0x48);
  *(undefined4 *)(param_2 + 0xa4) = uVar7;
  *(undefined4 *)(param_2 + 0xa8) = uVar4;
  *(undefined4 *)(param_2 + 0xac) = uVar6;
  uVar7 = *(undefined4 *)(param_1 + 0x58);
  uVar6 = *(undefined4 *)(param_1 + 0x60);
  uVar4 = *(undefined4 *)(param_1 + 100);
  *(undefined4 *)(param_2 + 0xb4) = *(undefined4 *)(param_1 + 0x5c);
  *(undefined4 *)(param_2 + 0xbc) = uVar4;
  *(int *)(param_2 + 0x724) = DAT_000770a8;
  uVar4 = DAT_000770ac;
  DAT_0007b4cc = DAT_0007b4cc + 1;
  *(undefined4 *)(param_2 + 0xb8) = uVar6;
  *(undefined4 *)(param_2 + 0xb0) = uVar7;
  *(int *)(param_2 + 0x6d4) = param_1;
  *(undefined4 *)(param_2 + 0x720) = uVar4;
  *(undefined1 *)(param_2 + 0x6ec) = 1;
  *(undefined1 *)(param_2 + 0x6e8) = 0;
  *(undefined1 *)(param_2 + 0x788) = 0x53;
  *(undefined4 *)(param_2 + 0x6c4) = 0x3c;
  DAT_000770a8 = iVar1 + 1;
  FUN_00011a5c((int)*(undefined8 *)(param_2 + 0x708),param_2);
  FUN_0002a84c(param_2 + 0x6d8);
  return;
}

