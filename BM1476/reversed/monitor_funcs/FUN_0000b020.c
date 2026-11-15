//==================== FUN_0000b020 @ 0000b020 ====================

void FUN_0000b020(void)

{
  __pid_t _Var1;
  int __fd;
  int iVar2;
  
  _Var1 = fork();
  if (_Var1 < 0) {
LAB_0000b074:
                    /* WARNING: Subroutine does not return */
    exit(1);
  }
  if (_Var1 == 0) {
    setsid();
    _Var1 = fork();
    if (_Var1 < 1) {
      if (_Var1 == 0) {
        __fd = 0;
        do {
          iVar2 = __fd + 1;
          close(__fd);
          __fd = iVar2;
        } while (iVar2 != 0x100);
        chdir("/root/test");
        umask(0);
        return;
      }
      goto LAB_0000b074;
    }
  }
                    /* WARNING: Subroutine does not return */
  exit(0);
}

