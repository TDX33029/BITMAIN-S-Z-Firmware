//==================== FUN_0000ecd0 @ 0000ecd0 ====================

undefined4 FUN_0000ecd0(char *param_1,uint param_2)

{
  size_t sVar1;
  undefined1 local_1a;
  undefined1 local_19;
  
  local_1a = (undefined1)(param_2 >> 8);
  printf("body_len1=%d\n",param_2 >> 8 & 0xff);
  local_19 = (undefined1)param_2;
  printf("body_len2=%d\n",param_2 & 0xff);
  DAT_00026f90 = (*DAT_0001d43c)(DAT_00026f98,DAT_000258bc,&DAT_00011b8c,2);
  if (DAT_00026f90 == -1) {
    perror("send head");
                    /* WARNING: Subroutine does not return */
    exit(1);
  }
  sVar1 = strlen(param_1);
  DAT_00026f90 = (*DAT_0001d43c)(DAT_00026f98,DAT_000258bc,param_1,sVar1);
  if (DAT_00026f90 != -1) {
    DAT_00026f90 = (*DAT_0001d43c)(DAT_00026f98,DAT_000258bc,&local_1a,1);
    if (DAT_00026f90 != -1) {
      DAT_00026f90 = (*DAT_0001d43c)(DAT_00026f98,DAT_000258bc,&local_19,1);
      if (DAT_00026f90 != -1) {
        return 1;
      }
    }
  }
  perror("send");
                    /* WARNING: Subroutine does not return */
  exit(1);
}

