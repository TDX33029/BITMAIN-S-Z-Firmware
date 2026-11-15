//==================== FUN_0000aa14 @ 0000aa14 ====================

int FUN_0000aa14(void)

{
  char *pcVar1;
  long lVar2;
  hostent *phVar3;
  int iVar4;
  sockaddr local_5c;
  char acStack_4c [60];
  
  if (DAT_0001b394 == 1) {
    puts("Web  Switch is OFF!");
                    /* WARNING: Subroutine does not return */
    exit(1);
  }
  memset(acStack_4c,0,0x3c);
  thunk_FUN_0000f990(DAT_0001b2f0,"ip_server",acStack_4c);
  pcVar1 = strchr(acStack_4c,0x3a);
  *pcVar1 = '\0';
  lVar2 = strtol(pcVar1 + 1,(char **)0x0,10);
  phVar3 = gethostbyname(acStack_4c);
  if (phVar3 == (hostent *)0x0) {
    perror("gethostbyname");
                    /* WARNING: Subroutine does not return */
    exit(1);
  }
  DAT_000258bc = socket(2,1,0);
  if (DAT_000258bc == -1) {
    perror("socket");
                    /* WARNING: Subroutine does not return */
    exit(1);
  }
  local_5c.sa_data._0_2_ = (ushort)((uint)lVar2 >> 8) & 0xff | (ushort)(lVar2 << 8);
  local_5c.sa_family = 2;
  local_5c.sa_data._2_4_ = *(undefined4 *)*phVar3->h_addr_list;
  local_5c.sa_data[6] = '\0';
  local_5c.sa_data[7] = '\0';
  local_5c.sa_data[8] = '\0';
  local_5c.sa_data[9] = '\0';
  local_5c.sa_data[10] = '\0';
  local_5c.sa_data[0xb] = '\0';
  local_5c.sa_data[0xc] = '\0';
  local_5c.sa_data[0xd] = '\0';
  iVar4 = connect(DAT_000258bc,&local_5c,0x10);
  if (iVar4 == -1) {
    puts("connect failed");
    DAT_0001b380 = 0;
  }
  else {
    DAT_0001b380 = 1;
    puts("server connected");
  }
  return iVar4;
}

