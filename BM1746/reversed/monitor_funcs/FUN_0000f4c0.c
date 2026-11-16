//==================== FUN_0000f4c0 @ 0000f4c0 ====================

undefined4 FUN_0000f4c0(void)

{
  ushort uVar1;
  char *pcVar2;
  long lVar3;
  int iVar4;
  hostent *phVar5;
  int *piVar6;
  int iVar7;
  undefined4 uVar8;
  int iVar9;
  bool bVar10;
  undefined1 auStack_d4 [12];
  sockaddr local_c8;
  char acStack_b8 [40];
  sigset_t sStack_90;
  
  pcVar2 = (char *)FUN_0000f990(DAT_0001b2f0,"port_local",auStack_d4);
  lVar3 = strtol(pcVar2,(char **)0x0,10);
  iVar4 = FUN_0000e284(&DAT_00011ba4,acStack_b8);
  if (iVar4 == 0) {
    puts("get IP error");
  }
  sigemptyset(&sStack_90);
  sigaddset(&sStack_90,0xd);
  sigprocmask(0,&sStack_90,(sigset_t *)0x0);
  system("/etc/init.d/cgminer restart");
  sigprocmask(1,&sStack_90,(sigset_t *)0x0);
  sleep(5);
  phVar5 = gethostbyname(acStack_b8);
  if (phVar5 == (hostent *)0x0) {
    puts("Failed to resolve host ");
    uVar8 = 0;
  }
  else {
    iVar4 = socket(2,1,0);
    if (iVar4 == -1) {
      piVar6 = __errno_location();
      pcVar2 = strerror(*piVar6);
      printf("Socket initialisation failed: %s\n",pcVar2);
      uVar8 = 0;
    }
    else {
      local_c8.sa_data[6] = '\0';
      local_c8.sa_data[7] = '\0';
      local_c8.sa_data[8] = '\0';
      local_c8.sa_data[9] = '\0';
      local_c8.sa_data[10] = '\0';
      local_c8.sa_data[0xb] = '\0';
      local_c8.sa_data[0xc] = '\0';
      local_c8.sa_data[0xd] = '\0';
      uVar1 = (ushort)((uint)lVar3 >> 8) & 0xff | (ushort)(lVar3 << 8);
      iVar9 = 6;
      local_c8.sa_data._2_4_ = *(undefined4 *)*phVar5->h_addr_list;
      local_c8.sa_data[0] = (char)uVar1;
      local_c8.sa_data[1] = (char)(uVar1 >> 8);
      local_c8.sa_family = 2;
      while (iVar7 = connect(iVar4,&local_c8,0x10), iVar7 < 0) {
        bVar10 = iVar9 == 0;
        iVar9 = iVar9 + -1;
        if (bVar10) {
          close(iVar4);
          goto LAB_0000f600;
        }
        piVar6 = __errno_location();
        pcVar2 = strerror(*piVar6);
        printf("Cgminer!Socket connect failed: %s\n",pcVar2);
        puts("restart not well!!\nwait next 5s......");
        sleep(5);
      }
      close(iVar4);
      if (iVar9 < 1) {
LAB_0000f600:
        puts("restart timeout!");
        uVar8 = 0xff;
      }
      else {
        puts("restart OK!");
        uVar8 = 1;
      }
    }
  }
  return uVar8;
}

