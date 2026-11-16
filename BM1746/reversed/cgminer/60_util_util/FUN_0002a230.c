//==================== FUN_0002a230 @ 0002a230 ====================

void FUN_0002a230(int param_1)

{
  int iVar1;
  
  iVar1 = pthread_mutex_lock((pthread_mutex_t *)(param_1 + 0xc));
  if (iVar1 != 0) {
    FUN_0000b708("tq_freezethaw",0x4d4);
  }
  *(undefined1 *)(param_1 + 8) = 1;
  pthread_cond_signal((pthread_cond_t *)(param_1 + 0x28));
  FUN_00029388((pthread_mutex_t *)(param_1 + 0xc),"tq_freezethaw",0x4d7);
  (*DAT_00075ed8)();
  return;
}

