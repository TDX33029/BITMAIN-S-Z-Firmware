//==================== FUN_00009fa0 @ 00009fa0 ====================

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_00009fa0(int param_1)

{
  int iVar1;
  undefined1 auStack_1020 [4104];
  
  iVar1 = FUN_0000b088();
  if (iVar1 == 0) {
    puts("process exist!");
  }
  else {
    if (1 < param_1) {
      sleep(0x28);
    }
    FUN_0000afe8();
    FUN_0000e444(&DAT_0001b3a8,&DAT_0001d410);
    FUN_00010d34(DAT_0001d420);
    iVar1 = pthread_create(&DAT_0001d404,(pthread_attr_t *)0x0,FUN_0000a7f4,(void *)0x0);
    if (iVar1 == 0) {
      sleep(1);
      if (DAT_0001b394 == 1) {
        puts("Web  Switch is OFF!");
                    /* WARNING: Subroutine does not return */
        exit(1);
      }
      while (iVar1 = FUN_0000aa14(), iVar1 == -1) {
        sleep(0x1e);
      }
      iVar1 = FUN_0000a938();
      if (iVar1 == -1) {
        DAT_0001d434 = &LAB_00010cd4;
        DAT_0001d43c = &LAB_00010d04;
        puts("encrypt connect failed.");
        close(DAT_000258bc);
        FUN_0000aa14();
      }
      else {
        DAT_0001d434 = &LAB_00010c84;
        DAT_0001d43c = &LAB_00010cac;
      }
      FUN_0000ae1c(0x1e);
      signal(10,FUN_0000ad18);
      signal(0xc,FUN_0000ad18);
      signal(2,FUN_0000a3ec);
      signal(0xf,FUN_0000a3ec);
      DAT_0001b398 = pthread_create(&DAT_0001f464,(pthread_attr_t *)0x0,FUN_0000a680,(void *)0x0);
      if ((DAT_0001b398 == 0) &&
         (DAT_0001b398 = pthread_create(&DAT_0001c3cc,(pthread_attr_t *)0x0,FUN_0000a514,(void *)0x0
                                       ), DAT_0001b398 == 0)) {
        while (DAT_0001b2ec != '\0') {
          if (((DAT_0001b3b0 + 1) % 0x14 == DAT_0001b3ac) ||
             ((DAT_0001d418 + 1) % 0x14 == DAT_0001d414)) {
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
        return 1;
      }
    }
    puts("Create pthread error!");
  }
  return 1;
}

