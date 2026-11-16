//==================== FUN_000584b0 @ 000584b0 ====================

void FUN_000584b0(int param_1,int param_2)

{
  int iVar1;
  
  if (param_2 + 2U < 2) {
    return;
  }
  FUN_00058274();
  iVar1 = FUN_000597c0(param_1 + 0x28);
  if (iVar1 != param_2) {
                    /* WARNING: Subroutine does not return */
    __assert_fail("c == d","load.c",0x10c,"lex_unget_unsave");
  }
  return;
}

