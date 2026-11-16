//==================== FUN_00034830 @ 00034830 ====================

void FUN_00034830(undefined4 param_1,undefined4 param_2,char *param_3)

{
  long lVar1;
  int iVar2;
  int iVar3;
  
  if (DAT_0007b198 == 0) {
    FUN_0003305c(param_1,8,0,0);
    return;
  }
  if ((param_3 == (char *)0x0) || (*param_3 == '\0')) {
    FUN_0003305c(param_1,0x19,0,0);
    return;
  }
  lVar1 = strtol(param_3,(char **)0x0,10);
  iVar2 = pthread_mutex_lock((pthread_mutex_t *)&DAT_0007ba10);
  if (iVar2 != 0) {
    FUN_0000b774("switchpool",0xb5b);
  }
  FUN_00032190(&DAT_0007ba28,"switchpool",0xb5b);
  iVar2 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_0007ba10);
  if (iVar2 != 0) {
    FUN_0000b84c("switchpool",0xb5b);
  }
  if ((lVar1 < 0) || (DAT_0007b198 <= lVar1)) {
    iVar2 = pthread_rwlock_unlock((pthread_rwlock_t *)&DAT_0007ba28);
    if (iVar2 != 0) {
      FUN_0000b7e0("switchpool",0xb5e);
    }
    (*DAT_00075ed8)();
    FUN_0003305c(param_1,0x1a,lVar1,0);
    return;
  }
  iVar3 = *(int *)(DAT_0007b438 + lVar1 * 4);
  *(undefined4 *)(iVar3 + 0x84) = 1;
  iVar2 = pthread_rwlock_unlock((pthread_rwlock_t *)&DAT_0007ba28);
  if (iVar2 != 0) {
    FUN_0000b7e0("switchpool",0xb65);
  }
  (*DAT_00075ed8)();
  FUN_00019bf8(iVar3);
  FUN_0003305c(param_1,0x1b,lVar1,0);
  return;
}

