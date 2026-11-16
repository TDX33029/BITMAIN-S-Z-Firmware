//==================== DataAbort @ 00000010 ====================

/* WARNING: Control flow encountered bad instruction data */

void DataAbort(void)

{
  FUN_00002200();
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

