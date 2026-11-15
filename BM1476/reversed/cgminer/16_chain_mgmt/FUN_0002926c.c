//==================== FUN_0002926c @ 0002926c ====================

void FUN_0002926c(timespec *param_1)

{
  int iVar1;
  
  do {
    iVar1 = clock_nanosleep(1,1,param_1,(timespec *)0x0);
  } while (iVar1 == 4);
  return;
}

