//==================== FUN_04008f30 @ 04008f30 ====================

/* WARNING: Control flow encountered bad instruction data */

void FUN_04008f30(void)

{
  int *piVar1;
  
  piVar1 = (int *)FUN_04008c70();
  if (*piVar1 != 0) {
    func_0x03fd10d0();
  }
  if (*piVar1 == 0) {
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  return;
}

