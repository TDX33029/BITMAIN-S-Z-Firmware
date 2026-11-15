//==================== FUN_0000a514 @ 0000a514 ====================

void FUN_0000a514(void)

{
  int iVar1;
  int *piVar2;
  undefined1 auStack_1020 [4100];
  
  do {
    if (DAT_0001b2ec == '\0') {
      return;
    }
    while (iVar1 = FUN_0000e7d8(&DAT_0001d410), iVar1 != 0) {
      DAT_0001d41c = 0;
      iVar1 = FUN_0000e62c(&DAT_0001d410,&DAT_0001b3b8);
      if (iVar1 == 0) {
        puts("respond queue  empty");
      }
      else {
        FUN_0000e698(DAT_0001d410,DAT_0001d414,DAT_0001d418,FUN_0000e75c);
        FUN_0000eb78();
        printf("response node:%s,%s----------\n",(char *)((int)&DAT_0001b3b8 + 3),&DAT_0001c3c0);
        piVar2 = (int *)FUN_00010d90(&DAT_0001b3b8,DAT_0001d420);
        if (piVar2 == (int *)0x0) {
          puts("no cmd for responding!");
                    /* WARNING: Subroutine does not return */
          exit(1);
        }
        iVar1 = *piVar2;
        if (((*(char *)(iVar1 + 3) == '0') && (*(char *)(iVar1 + 4) == '1')) &&
           (*(char *)(iVar1 + 5) == '\0')) {
          thunk_FUN_0000fa54(DAT_0001b2f0,0x1b3bb);
        }
        printf("Delete node:%s,%s----------\n",(char *)((int)&DAT_0001b3b8 + 3),&DAT_0001c3c0);
        FUN_00010e24(&DAT_0001b3b8,DAT_0001d420);
        memcpy(auStack_1020,&DAT_0001b3c8,0xfff);
        FUN_0000e7f4(DAT_0001b3b8,DAT_0001b3bc,DAT_0001b3c0,DAT_0001b3c4);
      }
      DAT_0001b2f4 = 0;
      if (DAT_0001b2ec == '\0') {
        DAT_0001b2f4 = 0;
        return;
      }
    }
  } while( true );
}

