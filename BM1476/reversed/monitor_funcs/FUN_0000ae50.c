//==================== FUN_0000ae50 @ 0000ae50 ====================

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0000ae50(void)

{
  int iVar1;
  undefined1 auStack_1020 [4104];
  
  while (DAT_0001b2ec != '\0') {
    if (((DAT_0001b3b0 + 1) % 0x14 == DAT_0001b3ac) || ((DAT_0001d418 + 1) % 0x14 == DAT_0001d414))
    {
      puts("the queue is full");
      sleep(5);
    }
    else {
      iVar1 = FUN_0000adb0();
      if (iVar1 == -1) {
        puts("Bad Package REV!");
        sleep(1);
      }
      else if (DAT_0001e457 == 'f') {
        puts("this is respond ");
        memcpy(auStack_1020,&DAT_0001e460,0x1003);
        FUN_0000e598(&DAT_0001d410,_DAT_0001e454,DAT_0001e458,DAT_0001e45c);
      }
      else {
        puts("this is command");
        memcpy(auStack_1020,&DAT_0001e460,0x1003);
        FUN_0000e598(&DAT_0001b3a8,_DAT_0001e454,DAT_0001e458,DAT_0001e45c);
      }
    }
  }
  return;
}

