//==================== FUN_04002fdc @ 04002fdc ====================

/* WARNING: Control flow encountered bad instruction data */

undefined4 FUN_04002fdc(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  
  iVar1 = func_0x03ffafa8(param_1,DAT_04003018);
  if (-1 < iVar1) {
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  return param_3;
}

