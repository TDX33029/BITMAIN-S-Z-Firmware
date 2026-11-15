//==================== FUN_0000ad18 @ 0000ad18 ====================

void FUN_0000ad18(int param_1)

{
  int iVar1;
  undefined1 auStackY_1100 [128];
  socklen_t sStack_7c;
  char acStack_78 [108];
  
  printf("receive signal :%d\n",param_1);
  if (param_1 == 10) {
    FUN_0000ace4(6000000,6000000);
    DAT_0001b380 = 0;
    close(DAT_000258bc);
    FUN_0000e4cc(&DAT_0001b3a8);
    FUN_0000e4cc(&DAT_0001d410);
    return;
  }
  if (param_1 != 0xc) {
    return;
  }
  FUN_0000e4cc(&DAT_0001b3a8);
  FUN_0000e4cc(&DAT_0001d410);
  FUN_0000ace4(0x1e,0x1e);
  sStack_7c = 0x68;
  if (DAT_0001b394 == 1) {
    puts("Web  Switch is OFF!");
                    /* WARNING: Subroutine does not return */
    exit(1);
  }
  getsockopt(DAT_000258bc,6,0xb,acStack_78,&sStack_7c);
  if (acStack_78[0] == '\x01') {
    puts("Connect to normal!!!");
  }
  else {
    puts("Off Line!!! in tcp_state()");
    DAT_0001b380 = 0;
    memcpy(auStackY_1100 + 0x80,&DAT_0001c3e4,0xfff);
    FUN_0000e7f4(DAT_0001c3d4,DAT_0001c3d8,DAT_0001c3dc,DAT_0001c3e0);
    memset(&DAT_00026d24,0,0x46);
    iVar1 = FUN_0000aa14();
    if (iVar1 == -1) {
      puts("tcp_state, sock_connect_server failed!");
    }
    else {
      iVar1 = FUN_0000a938();
      if (iVar1 == -1) {
        DAT_0001d434 = &LAB_00010cd4;
        DAT_0001d43c = &LAB_00010d04;
        close(DAT_000258bc);
        FUN_0000aa14();
        puts("encrypt connect failed.");
      }
      else {
        DAT_0001d434 = &LAB_00010c84;
        DAT_0001d43c = &LAB_00010cac;
      }
    }
  }
  return;
}

