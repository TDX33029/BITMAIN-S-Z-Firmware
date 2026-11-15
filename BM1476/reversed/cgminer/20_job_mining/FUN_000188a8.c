//==================== FUN_000188a8 @ 000188a8 ====================

undefined4 FUN_000188a8(void)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = pthread_mutex_lock(DAT_0007704c);
  if (iVar1 != 0) {
    FUN_0000b414("total_staged",0xb5b);
  }
  if (DAT_00077050 == 0) {
    uVar2 = 0;
  }
  else {
    uVar2 = *(undefined4 *)(*(int *)(DAT_00077050 + 0x728) + 0xc);
  }
  iVar1 = pthread_mutex_unlock(DAT_0007704c);
  if (iVar1 != 0) {
    FUN_0000b5c4("total_staged",0xb5d);
  }
  (*DAT_00075ed8)();
  return uVar2;
}

