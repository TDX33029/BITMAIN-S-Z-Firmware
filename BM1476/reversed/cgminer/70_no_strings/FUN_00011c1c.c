//==================== FUN_00011c1c @ 00011c1c ====================

void FUN_00011c1c(void)

{
  pthread_t __th;
  
  __th = pthread_self();
  pthread_detach(__th);
  pthread_setcanceltype(1,(int *)0x0);
  sleep(5);
                    /* WARNING: Subroutine does not return */
  exit(1);
}

