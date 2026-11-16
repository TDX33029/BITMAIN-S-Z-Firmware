//==================== PrefetchAbort @ 0000000c ====================

/* WARNING: Control flow encountered bad instruction data */

void PrefetchAbort(void)

{
  int in_r3;
  int unaff_r7;
  
  FUN_00002200(in_r3 + unaff_r7);
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

