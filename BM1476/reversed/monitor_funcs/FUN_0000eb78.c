//==================== FUN_0000eb78 @ 0000eb78 ====================

void FUN_0000eb78(void)

{
  if (DAT_0001b3b8._3_1_ == 'f') {
    if ((char)DAT_0001b3bc == '1') {
      if (DAT_0001b3bc._1_1_ == '\0') {
        puts("Server respond  F1\n");
                    /* WARNING: Subroutine does not return */
        exit(0);
      }
    }
    else if ((char)DAT_0001b3bc == '2') {
      if (DAT_0001b3bc._1_1_ == '\0') {
        puts("Server respond  F2\n");
                    /* WARNING: Subroutine does not return */
        exit(0);
      }
    }
    else if ((char)DAT_0001b3bc == '3') {
      if (DAT_0001b3bc._1_1_ == '\0') {
        puts("Server respond  F3\n");
                    /* WARNING: Subroutine does not return */
        exit(0);
      }
    }
    else if ((char)DAT_0001b3bc == '4') {
      if (DAT_0001b3bc._1_1_ == '\0') {
        puts("Server respond  F4\n");
                    /* WARNING: Subroutine does not return */
        exit(0);
      }
    }
    else if ((char)DAT_0001b3bc == '5') {
      if (DAT_0001b3bc._1_1_ == '\0') {
        puts("Server respond  F5\n");
        return;
      }
    }
    else if ((char)DAT_0001b3bc == '6') {
      if (DAT_0001b3bc._1_1_ == '\0') {
        puts("Server respond  F6\n");
        puts("F6 exit!!!!!!");
                    /* WARNING: Subroutine does not return */
        exit(0);
      }
    }
    else if (((char)DAT_0001b3bc == '7') && (DAT_0001b3bc._1_1_ == '\0')) {
      puts("Server respond  F7\n");
                    /* WARNING: Subroutine does not return */
      exit(0);
    }
  }
  printf("respond rev : %s",(char *)((int)&DAT_0001b3b8 + 3));
                    /* WARNING: Subroutine does not return */
  exit(0);
}

