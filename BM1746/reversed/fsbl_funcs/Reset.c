//==================== Reset @ 00000000 ====================

/* WARNING: Control flow encountered bad instruction data */

void Reset(void)

{
  int unaff_r7;
  int in_stack_00000008;
  
  FUN_00002200(in_stack_00000008 + unaff_r7);
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

