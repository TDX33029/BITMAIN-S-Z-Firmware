//==================== FUN_00028fc8 @ 00028fc8 ====================

undefined4 FUN_00028fc8(int param_1,int param_2,int param_3)

{
  long lVar1;
  in_addr_t iVar2;
  int iVar3;
  size_t __n;
  uint uVar4;
  char *__s;
  addrinfo *paStack_1250;
  addrinfo aStack_124c;
  undefined1 uStack_122c;
  undefined1 local_122b;
  undefined1 local_122a;
  undefined1 local_1229;
  undefined1 uStack_1228;
  undefined1 local_1227;
  undefined1 local_1226;
  undefined1 local_1225;
  char acStack_1224 [4];
  char acStack_1220 [4];
  undefined1 auStack_121c [500];
  addrinfo aaStack_1028 [128];
  
  uStack_122c = 4;
  local_122b = 1;
  lVar1 = strtol(*(char **)(param_1 + 0x260),(char **)0x0,10);
  local_1229 = (undefined1)lVar1;
  local_122a = (undefined1)((uint)lVar1 >> 8);
  acStack_1224[0] = 'C';
  acStack_1224[1] = 'G';
  acStack_1224[2] = 'M';
  acStack_1224[3] = 'I';
  acStack_1220[0] = 'N';
  acStack_1220[1] = 'E';
  acStack_1220[2] = 'R';
  acStack_1220[3] = '\0';
  iVar2 = inet_addr(*(char **)(param_1 + 0x270));
  uVar4 = iVar2 << 0x18 | (iVar2 >> 8 & 0xff) << 0x10 | (iVar2 >> 0x10 & 0xff) << 8 | iVar2 >> 0x18;
  if (uVar4 == 0xffffffff) {
    aaStack_1028[0].ai_family = 2;
    aaStack_1028[0].ai_flags = 0;
    aaStack_1028[0].ai_socktype = 0;
    paStack_1250 = &aStack_124c;
    aaStack_1028[0].ai_protocol = 0;
    aaStack_1028[0].ai_addrlen = 0;
    aaStack_1028[0].ai_addr = (sockaddr *)0x0;
    aaStack_1028[0].ai_canonname = (char *)0x0;
    aaStack_1028[0].ai_next = (addrinfo *)0x0;
    iVar3 = getaddrinfo(*(char **)(param_1 + 0x270),(char *)0x0,aaStack_1028,&paStack_1250);
    if (iVar3 == 0) {
      uVar4 = *(uint *)(paStack_1250->ai_addr->sa_data + 2);
      freeaddrinfo(paStack_1250);
      uVar4 = uVar4 << 0x18 | (uVar4 >> 8 & 0xff) << 0x10 | (uVar4 >> 0x10 & 0xff) << 8 |
              uVar4 >> 0x18;
      if (uVar4 == 0xffffffff) goto LAB_0002920c;
      goto LAB_00029044;
    }
    if (param_3 == 0) {
LAB_0002920c:
      if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 4)) {
        return 0;
      }
      snprintf((char *)aaStack_1028,0x1000,"Invalid IP address specified for socks4 proxy: %s",
               *(char **)(param_1 + 0x270));
      goto LAB_000290d0;
    }
    __s = *(char **)(param_1 + 0x270);
    uStack_1228 = 0;
    local_1225 = 1;
    local_1227 = 0;
    local_1226 = 0;
    __n = strlen(__s);
    if (0xfe < (int)__n) {
      __n = 0xff;
    }
    memcpy(auStack_121c,__s,__n);
    auStack_121c[__n] = 0;
    send(param_2,&uStack_122c,__n + 0x11,0);
  }
  else {
LAB_00029044:
    uStack_1228 = (undefined1)(uVar4 >> 0x18);
    local_1225 = (undefined1)uVar4;
    local_1227 = (undefined1)(uVar4 >> 0x10);
    local_1226 = (undefined1)(uVar4 >> 8);
    send(param_2,&uStack_122c,0x10,0);
  }
  iVar3 = FUN_00028bc4(param_2);
  if ((iVar3 == 0) && (iVar3 = FUN_00028bc4(param_2), iVar3 == 0x5a)) {
    iVar3 = 6;
    do {
      FUN_00028bc4(param_2);
      iVar3 = iVar3 + -1;
    } while (iVar3 != 0);
    return 1;
  }
  if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 4)) {
    return 0;
  }
  snprintf((char *)aaStack_1028,0x1000,"Bad response from %s:%s SOCKS4 server",
           *(char **)(param_1 + 0x274),*(char **)(param_1 + 0x278));
LAB_000290d0:
  FUN_00038730(4,aaStack_1028,0);
  return 0;
}

