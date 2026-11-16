//==================== FUN_0000a680 @ 0000a680 ====================

void FUN_0000a680(void)

{
  int iVar1;
  undefined1 auStack_1020 [4100];
  
  while( true ) {
    if (DAT_0001b2ec == '\0') {
      return;
    }
    puts("next command!");
    if (DAT_0001b394 == 1) break;
    iVar1 = FUN_0000e62c(&DAT_0001b3a8,&DAT_0001d444);
    if (iVar1 == 0) {
      puts("queue empty1");
      sleep(1);
    }
    else {
      printf("get a command head:%s!\n",(char *)&DAT_0001d444);
      printf("get a command:%s!\n",(char *)((int)&DAT_0001d444 + 3));
      FUN_0000e698(DAT_0001b3a8,DAT_0001b3ac,DAT_0001b3b0,FUN_0000e75c);
      memcpy(auStack_1020,&DAT_0001d454,0xfff);
      iVar1 = FUN_0000f854(DAT_0001d444,DAT_0001d448,DAT_0001d44c,DAT_0001d450);
      if (iVar1 != 0) {
        puts("after deal_cmd()");
        printf("get a command nonce:%s!\n",&DAT_0001e44c);
        DAT_0001b2f4 = 0;
        FUN_00010ec0(&DAT_0001d444,DAT_0001d420,DAT_0001d420);
        printf("new node:%s,%s==========\n",(char *)((int)&DAT_0001d444 + 3),&DAT_0001e44c);
        memcpy(auStack_1020,&DAT_0001d454,0xfff);
        FUN_0000e7f4(DAT_0001d444,DAT_0001d448,DAT_0001d44c,DAT_0001d450);
      }
    }
  }
  puts("Web  Switch is OFF!");
                    /* WARNING: Subroutine does not return */
  exit(1);
}

