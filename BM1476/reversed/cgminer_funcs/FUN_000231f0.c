//==================== FUN_000231f0 @ 000231f0 ====================

undefined4 FUN_000231f0(void)

{
  int iVar1;
  pthread_t __th;
  int iVar2;
  int iVar3;
  int iVar4;
  
  iVar1 = FUN_0001da58();
  __th = pthread_self();
  pthread_detach(__th);
  FUN_00018834(iVar1,iVar1 + 0x82);
  FUN_00022f88();
  iVar1 = pthread_rwlock_rdlock((pthread_rwlock_t *)&DAT_0007b650);
  if (iVar1 != 0) {
    FUN_0000b69c("restart_thread",0x1496);
  }
  iVar1 = DAT_0007bb6c;
  iVar2 = pthread_rwlock_unlock((pthread_rwlock_t *)&DAT_0007b650);
  if (iVar2 != 0) {
    FUN_0000b630("restart_thread",0x1498);
  }
  (*DAT_00075ed8)();
  if (0 < iVar1) {
    iVar2 = 0;
    do {
      while( true ) {
        iVar3 = *(int *)(DAT_0007b5e0 + iVar2 * 4);
        iVar2 = iVar2 + 1;
        iVar4 = *(int *)(iVar3 + 0x24);
        if ((iVar4 == 0) || (*(int *)(iVar4 + 0x20) != 0)) break;
        *(undefined1 *)(iVar3 + 0x3e) = 1;
        FUN_0001aa18(iVar4);
        (**(code **)(*(int *)(iVar4 + 4) + 0x4c))(iVar4);
        if (iVar2 == iVar1) goto LAB_000232b0;
      }
    } while (iVar2 != iVar1);
  }
LAB_000232b0:
  iVar1 = pthread_mutex_lock((pthread_mutex_t *)&DAT_0007b62c);
  if (iVar1 != 0) {
    FUN_0000b414("restart_thread",0x14a4);
  }
  pthread_cond_broadcast((pthread_cond_t *)&DAT_0007bae0);
  iVar1 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_0007b62c);
  if (iVar1 != 0) {
    FUN_0000b5c4("restart_thread",0x14a6);
  }
  (*DAT_00075ed8)();
  return 0;
}

