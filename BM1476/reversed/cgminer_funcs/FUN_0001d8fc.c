//==================== FUN_0001d8fc @ 0001d8fc ====================

void FUN_0001d8fc(int param_1,int param_2)

{
  int iVar1;
  pthread_t local_14 [2];
  
  iVar1 = pthread_create(local_14,(pthread_attr_t *)0x0,FUN_00011c1c,(void *)0x0);
  if (iVar1 != 0) {
                    /* WARNING: Subroutine does not return */
    exit(1);
  }
  if (param_2 == 0) {
    FUN_00019adc();
  }
  else {
    FUN_0002a84c(&DAT_00077148);
    FUN_00019adc();
    if ((DAT_0007bb18 == '\0') && (DAT_00078ca9 != '\0')) {
      FUN_0001c668();
    }
  }
  if (0 < DAT_0007728c) {
    kill(DAT_0007728c,0xf);
    DAT_0007728c = 0;
  }
  pthread_cancel(local_14[0]);
                    /* WARNING: Subroutine does not return */
  exit(param_1);
}

