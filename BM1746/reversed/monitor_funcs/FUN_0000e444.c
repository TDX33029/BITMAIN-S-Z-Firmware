//==================== FUN_0000e444 @ 0000e444 ====================

undefined4 FUN_0000e444(undefined4 *param_1,undefined4 *param_2)

{
  void *pvVar1;
  
  pvVar1 = malloc(36999);
  *param_1 = pvVar1;
  if (pvVar1 != (void *)0x0) {
    param_1[2] = 0;
    param_1[1] = 0;
    pvVar1 = malloc(36999);
    *param_2 = pvVar1;
    if (pvVar1 != (void *)0x0) {
      param_2[2] = 0;
      param_2[1] = 0;
      return 1;
    }
  }
                    /* WARNING: Subroutine does not return */
  exit(0);
}

