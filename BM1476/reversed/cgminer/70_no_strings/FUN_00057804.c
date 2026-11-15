//==================== FUN_00057804 @ 00057804 ====================

undefined4 FUN_00057804(undefined4 param_1,char *param_2,undefined4 param_3)

{
  FILE *__stream;
  undefined4 uVar1;
  
  __stream = fopen(param_2,"w");
  if (__stream == (FILE *)0x0) {
    uVar1 = 0xffffffff;
  }
  else {
    uVar1 = FUN_000577f0(param_1,__stream,param_3);
    fclose(__stream);
  }
  return uVar1;
}

