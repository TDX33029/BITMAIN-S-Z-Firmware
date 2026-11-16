//==================== FUN_000299ac @ 000299ac ====================

undefined4 FUN_000299ac(int *param_1,undefined4 param_2,int param_3)

{
  char *__s;
  int iVar1;
  size_t sVar2;
  void *pvVar3;
  undefined4 uVar4;
  int *piVar5;
  char acStack_1018 [4096];
  
  __s = (char *)FUN_000292f4(param_2,param_3 != 0);
  if (__s == (char *)0x0) {
    uVar4 = 0;
  }
  else {
    iVar1 = pthread_mutex_lock((pthread_mutex_t *)(param_1 + 0x3d));
    if (iVar1 != 0) {
      FUN_0000b708("parse_extranonce_equihash",0x9da);
    }
    iVar1 = pthread_rwlock_wrlock((pthread_rwlock_t *)(param_1 + 0x43));
    if (iVar1 != 0) {
      piVar5 = __errno_location();
      snprintf(acStack_1018,0x1000,"WTF WRLOCK ERROR ON LOCK! errno=%d in %s %s():%d",*piVar5,
               "util.c","parse_extranonce_equihash",0x9da);
      FUN_00038730(3,acStack_1018,1);
      FUN_00016ca8(1);
    }
    free((void *)param_1[0x9f]);
    param_1[0x9f] = (int)__s;
    sVar2 = strlen(__s);
    param_1[0x18d] = sVar2 >> 1;
    free((void *)param_1[0xa0]);
    sVar2 = param_1[0x18d];
    pvVar3 = calloc(sVar2,1);
    param_1[0xa0] = (int)pvVar3;
    if (pvVar3 == (void *)0x0) {
      snprintf(acStack_1018,0x1000,"%s: Failed to calloc pool->nonce1bin in %s %s():%d",
               "parse_extranonce_equihash","util.c","parse_extranonce_equihash",0x9e2);
      FUN_00038730(3,acStack_1018,1);
      FUN_00016ca8(1);
      pvVar3 = (void *)param_1[0xa0];
      sVar2 = param_1[0x18d];
    }
    FUN_00029830(pvVar3,param_1[0x9f],sVar2);
    param_1[0xa2] = 0;
    param_1[0xa3] = 0;
    param_1[0xa4] = 0x40 - param_1[0x18d];
    iVar1 = pthread_rwlock_unlock((pthread_rwlock_t *)(param_1 + 0x43));
    if (iVar1 != 0) {
      piVar5 = __errno_location();
      snprintf(acStack_1018,0x1000,"WTF RWLOCK ERROR ON UNLOCK! errno=%d in %s %s():%d",*piVar5,
               "util.c","parse_extranonce_equihash",0x9e8);
      FUN_00038730(3,acStack_1018,1);
      FUN_00016ca8(1);
    }
    iVar1 = pthread_mutex_unlock((pthread_mutex_t *)(param_1 + 0x3d));
    if (iVar1 != 0) {
      piVar5 = __errno_location();
      snprintf(acStack_1018,0x1000,"WTF MUTEX ERROR ON UNLOCK! errno=%d in %s %s():%d",*piVar5,
               "util.c","parse_extranonce_equihash",0x9e8);
      FUN_00038730(3,acStack_1018,1);
      FUN_00016ca8(1);
    }
    (*DAT_00075ed8)();
    if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 6)) {
      uVar4 = 1;
    }
    else {
      snprintf(acStack_1018,0x1000,"Pool %d extranonce set to %s",*param_1,__s);
      FUN_00038730(6,acStack_1018,0);
      uVar4 = 1;
    }
  }
  return uVar4;
}

