//==================== UndefinedInstruction @ 00000004 ====================

/* WARNING: Control flow encountered bad instruction data */

void UndefinedInstruction(void)

{
  int in_r3;
  int unaff_r7;
  
  FUN_00002200(in_r3 + unaff_r7);
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

