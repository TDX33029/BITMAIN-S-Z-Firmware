//==================== FUN_0000afe8 @ 0000afe8 ====================

void FUN_0000afe8(void)

{
  sigset_t sStack_88;
  
  sigemptyset(&sStack_88);
  sigfillset(&sStack_88);
  sigdelset(&sStack_88,2);
  sigdelset(&sStack_88,0xd);
  return;
}

