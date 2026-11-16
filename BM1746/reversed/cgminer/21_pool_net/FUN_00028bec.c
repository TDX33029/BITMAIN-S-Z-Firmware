//==================== FUN_00028bec @ 00028bec ====================

undefined4 FUN_00028bec(int param_1,int param_2,int param_3)

{
  char cVar1;
  byte bVar2;
  size_t __n;
  ssize_t sVar3;
  int iVar4;
  char *pcVar5;
  undefined4 uVar6;
  byte *pbVar7;
  char *param4;
  undefined4 uVar8;
  byte bStack_1421;
  byte local_1420 [1024];
  char acStack_1020 [4100];
  
  pcVar5 = *(char **)(param_1 + 0x270);
  param4 = *(char **)(param_1 + 0x260);
  if (param_3 == 0) {
    snprintf((char *)local_1420,0x400,"CONNECT %s:%s HTTP/1.1\r\nHost: %s:%s\r\n\r\n",pcVar5,param4,
             pcVar5,param4);
  }
  else {
    snprintf((char *)local_1420,0x400,"CONNECT %s:%s HTTP/1.0\r\n\r\n",pcVar5,param4);
  }
  if ((DAT_00078e09 != '\0') &&
     (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
    snprintf(acStack_1020,0x1000,"Sending proxy %s:%s - %s",*(char **)(param_1 + 0x274),
             *(char **)(param_1 + 0x278),(char *)local_1420);
    FUN_00038730(7,acStack_1020,0);
  }
  __n = strlen((char *)local_1420);
  send(param_2,local_1420,__n,0);
  sVar3 = recv(param_2,local_1420,0xc,0);
  cVar1 = DAT_00078e09;
  if (sVar3 < 1) {
    if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 4)) {
      return 0;
    }
    uVar6 = *(undefined4 *)(param_1 + 0x274);
    uVar8 = *(undefined4 *)(param_1 + 0x278);
    pcVar5 = "Couldn\'t read from proxy %s:%s after sending CONNECT";
  }
  else {
    local_1420[sVar3] = 0;
    if ((cVar1 != '\0') &&
       (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
      snprintf(acStack_1020,0x1000,"Received from proxy %s:%s - %s",*(char **)(param_1 + 0x274),
               *(char **)(param_1 + 0x278),(char *)local_1420);
      FUN_00038730(7,acStack_1020,0);
    }
    iVar4 = strcmp((char *)local_1420,"HTTP/1.1 200");
    if ((iVar4 != 0) && (iVar4 = strcmp((char *)local_1420,"HTTP/1.0 200"), iVar4 != 0)) {
      if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 4)) {
        return 0;
      }
      snprintf(acStack_1020,0x1000,"HTTP Error from proxy %s:%s - %s",*(char **)(param_1 + 0x274),
               *(char **)(param_1 + 0x278),(char *)local_1420);
      goto LAB_00028e04;
    }
    pbVar7 = &bStack_1421;
    do {
      bVar2 = FUN_00028bc4(param_2);
      pbVar7 = pbVar7 + 1;
      *pbVar7 = bVar2;
      if (bVar2 == 0xff) goto LAB_00028db8;
    } while (pbVar7 != local_1420 + 3);
    do {
      iVar4 = strncmp((char *)local_1420,"\r\n\r\n",4);
      if (iVar4 == 0) {
        if ((DAT_00078e09 == '\0') ||
           (((DAT_0007ad48 == '\0' && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 7)))) {
          return 1;
        }
        snprintf(acStack_1020,0x1000,"Success negotiating with %s:%s HTTP proxy",
                 *(char **)(param_1 + 0x274),*(char **)(param_1 + 0x278));
        FUN_00038730(7,acStack_1020,0);
        return 1;
      }
      local_1420[0] = local_1420[1];
      local_1420[1] = local_1420[2];
      local_1420[2] = local_1420[3];
      local_1420[3] = FUN_00028bc4(param_2);
    } while (local_1420[3] != 0xff);
LAB_00028db8:
    if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 4)) {
      return 0;
    }
    uVar6 = *(undefined4 *)(param_1 + 0x274);
    uVar8 = *(undefined4 *)(param_1 + 0x278);
    pcVar5 = "Couldn\'t read HTTP byte from proxy %s:%s";
  }
  snprintf(acStack_1020,0x1000,pcVar5,uVar6,uVar8);
LAB_00028e04:
  FUN_00038730(4,acStack_1020,0);
  return 0;
}

