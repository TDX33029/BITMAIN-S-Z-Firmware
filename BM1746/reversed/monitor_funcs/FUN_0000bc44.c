//==================== FUN_0000bc44 @ 0000bc44 ====================

char * FUN_0000bc44(undefined4 param_1,char *param_2,char *param_3)

{
  char cVar1;
  int param1;
  char *pcVar2;
  char *pcVar3;
  int iVar4;
  size_t sVar5;
  void *pvVar6;
  uint uVar7;
  int iVar8;
  char *__s;
  char *pcVar9;
  int iVar10;
  char acStack_98 [4];
  void *local_94;
  void *local_90;
  char acStack_8c [104];
  
  local_94 = (void *)0x0;
  local_90 = (void *)0x0;
  param1 = FUN_0000ba9c(param_1,&local_94,&local_90);
  printf("sum = %d\n",param1);
  memset(acStack_8c,0,100);
  pcVar2 = strchr(param_3,0x7c);
  __s = pcVar2;
  if (pcVar2 != (char *)0x0) {
    __s = pcVar2 + 1;
    *pcVar2 = '\0';
  }
  pcVar2 = strrchr(param_3,0x7c);
  if (pcVar2 != (char *)0x0) {
    *pcVar2 = '\0';
  }
  if ((__s == (char *)0x0) || (*__s == '\0')) {
LAB_0000bdac:
    sVar5 = strlen(param_2);
    if (param_2[sVar5 - 1] == ',' || param_2[sVar5 - 1] == '|') {
      param_2[sVar5 - 1] = '\0';
    }
    if (0 < param1) {
      iVar8 = 0;
      do {
        iVar10 = iVar8 * 4;
        pvVar6 = *(void **)((int)local_94 + iVar8 * 4);
        iVar8 = iVar8 + 1;
        if (pvVar6 != (void *)0x0) {
          free(pvVar6);
        }
        pvVar6 = *(void **)((int)local_90 + iVar10);
        if (pvVar6 != (void *)0x0) {
          free(pvVar6);
        }
      } while (iVar8 != param1);
    }
    if (local_94 != (void *)0x0) {
      free(local_94);
    }
    if (local_90 != (void *)0x0) {
      free(local_90);
    }
    printf("after format :%s\n\n",param_2);
    return param_2;
  }
  do {
    pcVar2 = strchr(__s,0x2c);
    pcVar3 = strchr(__s,0x7c);
    uVar7 = 1 - (int)pcVar2;
    if ((char *)0x1 < pcVar2) {
      uVar7 = 0;
    }
    if (pcVar3 < pcVar2) {
      uVar7 = uVar7 | 1;
    }
    if (uVar7 == 0) {
      strncpy(acStack_98,pcVar2,1);
LAB_0000bd20:
      pcVar9 = pcVar2 + 1;
      *pcVar2 = '\0';
    }
    else {
      strncpy(acStack_98,pcVar3,1);
      pcVar9 = (char *)0x0;
      pcVar2 = pcVar3;
      if (pcVar3 != (char *)0x0) goto LAB_0000bd20;
    }
    pcVar3 = strchr(__s,0x3d);
    pvVar6 = local_94;
    pcVar2 = pcVar3;
    if (pcVar3 != (char *)0x0) {
      pcVar2 = pcVar3 + 1;
      *pcVar3 = '\0';
    }
    if (0 < param1) {
      iVar8 = 0;
      iVar10 = 0;
      do {
        iVar4 = strcmp(*(char **)((int)pvVar6 + iVar8),__s);
        iVar10 = iVar10 + 1;
        if (iVar4 == 0) {
          sprintf(acStack_8c,"%s=%s",*(char **)((int)local_90 + iVar8),pcVar2);
          strcat(param_2,acStack_8c);
          if ((pcVar9 == (char *)0x0) || (*pcVar9 == '\0')) goto LAB_0000bdac;
          strncat(param_2,acStack_98,1);
          cVar1 = *pcVar9;
          goto LAB_0000be5c;
        }
        iVar8 = iVar8 + 4;
      } while (iVar10 != param1);
    }
    if (pcVar9 == (char *)0x0) goto LAB_0000bdac;
    cVar1 = *pcVar9;
LAB_0000be5c:
    if (cVar1 == '\0') goto LAB_0000bdac;
    __s = pcVar9;
  } while( true );
}

