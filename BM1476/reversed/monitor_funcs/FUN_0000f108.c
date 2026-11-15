//==================== FUN_0000f108 @ 0000f108 ====================

/* WARNING: Restarted to delay deadcode elimination for space: ram */

undefined1 FUN_0000f108(char *param_1)

{
  undefined1 uVar1;
  int iVar2;
  char *pcVar3;
  char *pcVar4;
  size_t sVar5;
  long lVar6;
  char *__s;
  size_t sVar7;
  int iVar8;
  undefined **ppuVar9;
  char *unaff_r6;
  char *unaff_r8;
  char *unaff_r10;
  undefined1 auStack_5c [12];
  undefined1 auStack_50 [44];
  
  ppuVar9 = &PTR_DAT_0001b330;
  iVar8 = 0;
  do {
    ppuVar9 = ppuVar9 + 1;
    iVar2 = strcmp(param_1,*ppuVar9);
    if (iVar2 == 0) {
      pcVar3 = (&PTR_s__000118e4_0x24_0001b2fc)[iVar8];
      sVar5 = strlen(pcVar3);
      unaff_r8 = malloc(sVar5 + 1);
      memset(unaff_r8,0,sVar5 + 1);
      strcpy(unaff_r8,pcVar3);
      pcVar3 = (&PTR_DAT_0001b318)[iVar8];
      sVar5 = strlen(pcVar3);
      unaff_r6 = malloc(sVar5 + 1);
      memset(unaff_r6,0,sVar5 + 1);
      strcpy(unaff_r6,pcVar3);
      pcVar3 = (&PTR_DAT_0001b350)[iVar8];
      sVar5 = strlen(pcVar3);
      unaff_r10 = malloc(sVar5 + 1);
      memset(unaff_r10,0,sVar5 + 1);
      strcpy(unaff_r10,pcVar3);
      break;
    }
    iVar8 = iVar8 + 1;
  } while (iVar8 != 7);
  pcVar3 = calloc(0x1000,1);
  pcVar4 = (char *)FUN_0000e1f8(&DAT_0001e44c);
  sVar5 = strlen(pcVar4);
  pcVar4 = (char *)FUN_0000f990(DAT_0001b2f0,"port_local",auStack_5c);
  lVar6 = strtol(pcVar4,(char **)0x0,10);
  iVar8 = FUN_0000e284(&DAT_00011ba4,auStack_50);
  if (iVar8 == 0) {
    puts("get IP error");
  }
  pcVar4 = (char *)FUN_0000bea4(unaff_r6,auStack_50,(int)(short)lVar6);
  if (*pcVar4 == '\0') {
    printf("nothing returned form callapi\n\n\n ");
    uVar1 = 0;
  }
  else {
    printf("result of callapi:%s\n ",pcVar4);
    __s = (char *)FUN_0000bc44(unaff_r8,pcVar3,pcVar4);
    sVar7 = strlen(__s);
    printf("state_len=%d\n",sVar7 + sVar5);
    FUN_0000ecd0(unaff_r10,sVar7 + sVar5);
    DAT_00026f90 = (*DAT_0001d43c)(DAT_00026f98,DAT_000258bc,&DAT_0001e44c,sVar5);
    if (DAT_00026f90 == -1) {
      perror("send");
    }
    DAT_00026f90 = (*DAT_0001d43c)(DAT_00026f98,DAT_000258bc,pcVar3,sVar7);
    if (DAT_00026f90 == -1) {
      perror("send");
    }
    printf("sendbytes=%d\n",DAT_00026f90);
    printf("send pakge str :%s\n",pcVar3);
    if (unaff_r10 != (char *)0x0) {
      free(unaff_r10);
    }
    if (unaff_r8 != (char *)0x0) {
      free(unaff_r8);
    }
    if (unaff_r6 != (char *)0x0) {
      free(unaff_r6);
    }
    free(pcVar4);
    free(pcVar3);
    uVar1 = 1;
  }
  return uVar1;
}

