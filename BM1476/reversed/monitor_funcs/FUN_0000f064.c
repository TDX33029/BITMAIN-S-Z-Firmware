//==================== FUN_0000f064 @ 0000f064 ====================

undefined4 FUN_0000f064(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  size_t param1;
  
  param1 = strlen(&DAT_00025ab4);
  printf("02revbody_size=%d\n",param1);
  FUN_0000ecd0(param_1,param1);
  DAT_00026f90 = (*DAT_0001d43c)(DAT_00026f98,DAT_000258bc,&DAT_00025ab4,param1,param_4);
  if (DAT_00026f90 == -1) {
    perror("send");
                    /* WARNING: Subroutine does not return */
    exit(1);
  }
  printf("send what I rev:\n%s\n",&DAT_00025ab4);
  return 1;
}

