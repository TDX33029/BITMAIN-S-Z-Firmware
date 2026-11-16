//==================== FUN_0000edfc @ 0000edfc ====================

void FUN_0000edfc(undefined4 param_1)

{
  char *__src;
  char *param0;
  char *__s;
  int iVar1;
  char *pcVar2;
  size_t sVar3;
  size_t sVar4;
  size_t sVar5;
  size_t sVar6;
  size_t sVar7;
  char *pcVar8;
  uint uVar9;
  char local_98 [12];
  char local_8c [104];
  
  memset(local_8c + 0x28,0,0x3c);
  local_98[0] = '\0';
  local_98[1] = '\0';
  local_98[2] = '\0';
  local_98[3] = '\0';
  local_98[4] = '\0';
  local_98[5] = '\0';
  local_98[6] = '\0';
  local_98[7] = '\0';
  local_98[8] = '\0';
  local_98[9] = '\0';
  local_8c[0] = '\0';
  local_8c[1] = '\0';
  local_8c[2] = '\0';
  local_8c[3] = '\0';
  local_8c[4] = '\0';
  local_8c[5] = '\0';
  local_8c[6] = '\0';
  local_8c[7] = '\0';
  local_8c[8] = '\0';
  local_8c[9] = '\0';
  local_8c[10] = '\0';
  local_8c[0xb] = '\0';
  local_8c[0xc] = '\0';
  local_8c[0xd] = '\0';
  local_8c[0xe] = '\0';
  local_8c[0xf] = '\0';
  local_8c[0x10] = '\0';
  local_8c[0x11] = '\0';
  local_8c[0x12] = '\0';
  local_8c[0x13] = '\0';
  local_8c[0x14] = '\0';
  local_8c[0x15] = '\0';
  local_8c[0x16] = '\0';
  local_8c[0x17] = '\0';
  local_8c[0x18] = '\0';
  local_8c[0x19] = '\0';
  local_8c[0x1a] = '\0';
  local_8c[0x1b] = '\0';
  local_8c[0x1c] = '\0';
  local_8c[0x1d] = '\0';
  local_8c[0x1e] = '\0';
  local_8c[0x1f] = '\0';
  local_8c[0x20] = '\0';
  local_8c[0x21] = '\0';
  local_8c[0x22] = '\0';
  local_8c[0x23] = '\0';
  local_8c[0x24] = '\0';
  local_8c[0x25] = '\0';
  local_8c[0x26] = '\0';
  local_8c[0x27] = '\0';
  __src = calloc(0x1e,1);
  param0 = calloc(0x19,1);
  __s = calloc(0x3c,1);
  iVar1 = FUN_0000e284(&DAT_00011ba4,local_8c);
  if (iVar1 == 0) {
    puts("get ip error!");
  }
  sprintf(param0,"IP=%s,",local_8c);
  builtin_strncpy(__s,"UID=",5);
  pcVar2 = (char *)thunk_FUN_0000f990(DAT_0001b2f0,"api-uid",local_8c + 0x28);
  sVar3 = strlen(__s);
  pcVar2 = stpcpy(__s + sVar3,pcVar2);
  pcVar2[0] = ',';
  pcVar2[1] = '\0';
  sprintf(local_98,"TYPE=%d",7);
  pcVar2 = (char *)FUN_0000e1f8(&DAT_0001e44c);
  sVar3 = strlen(pcVar2);
  pcVar2 = (char *)FUN_0000e340(__src,&DAT_00011ba4);
  sVar4 = strlen(pcVar2);
  sVar5 = strlen(param0);
  sVar6 = strlen(__s);
  sVar7 = strlen(local_98);
  uVar9 = sVar4 + sVar3 + sVar5 + sVar6 + sVar7 & 0xff;
  FUN_0000ecd0(param_1,uVar9);
  pcVar2 = calloc(uVar9 + 4,1);
  pcVar8 = stpcpy(pcVar2,&DAT_0001e44c);
  pcVar8 = stpcpy(pcVar8,__src);
  pcVar8 = stpcpy(pcVar8,param0);
  pcVar8 = stpcpy(pcVar8,__s);
  strcpy(pcVar8,local_98);
  DAT_00026f90 = (*DAT_0001d43c)(DAT_00026f98,DAT_000258bc,pcVar2,uVar9);
  if (DAT_00026f90 == -1) {
    perror("send");
  }
  printf("send pakge str :%s\n",pcVar2);
  free(__src);
  free(param0);
  free(__s);
  if (pcVar2 == (char *)0x0) {
    return;
  }
  free(pcVar2);
  return;
}

