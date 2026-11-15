//==================== FUN_00029730 @ 00029730 ====================

char * FUN_00029730(int param_1,int param_2)

{
  byte *pbVar1;
  char cVar2;
  char *pcVar3;
  char *pcVar4;
  int iVar5;
  char acStack_1018 [4096];
  
  iVar5 = param_2 * 2 + 1;
  pcVar3 = calloc((4 - iVar5 % 4) + iVar5,1);
  if (pcVar3 == (char *)0x0) {
    snprintf(acStack_1018,0x1000,"Failed to calloc in %s %s():%d","util.c","bin2hex",0x398);
    FUN_00038730(3,acStack_1018,1);
    FUN_00016ca8(1);
  }
  pcVar4 = pcVar3;
  if (0 < param_2) {
    iVar5 = 0;
    do {
      pbVar1 = (byte *)(param_1 + iVar5);
      iVar5 = iVar5 + 1;
      cVar2 = "0123456789abcdefbin2hex"[*pbVar1 >> 4];
      pcVar4[1] = "0123456789abcdefbin2hex"[*pbVar1 & 0xf];
      *pcVar4 = cVar2;
      pcVar4 = pcVar4 + 2;
    } while (iVar5 != param_2);
    pcVar4 = pcVar3 + param_2 * 2;
  }
  *pcVar4 = '\0';
  return pcVar3;
}

