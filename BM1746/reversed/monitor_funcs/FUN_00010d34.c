//==================== FUN_00010d34 @ 00010d34 ====================

void FUN_00010d34(void)

{
  void *pvVar1;
  
  pvVar1 = malloc(8);
  if (pvVar1 == (void *)0x0) {
                    /* WARNING: Subroutine does not return */
    exit(0);
  }
  *(undefined4 *)((int)pvVar1 + 4) = 0;
  DAT_0001d440 = 0;
  DAT_0001d420 = pvVar1;
  return;
}

