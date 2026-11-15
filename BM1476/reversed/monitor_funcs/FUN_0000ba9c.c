//==================== FUN_0000ba9c @ 0000ba9c ====================

size_t FUN_0000ba9c(char *param_1,int *param_2,int *param_3)

{
  FILE *__stream;
  size_t sVar1;
  void *__ptr;
  undefined4 uVar2;
  void *pvVar3;
  int iVar4;
  size_t sVar5;
  size_t sVar6;
  char *pcVar7;
  size_t unaff_r11;
  size_t local_30;
  
  __stream = fopen(param_1,"rb+");
  if (__stream == (FILE *)0x0) {
    puts("open error0");
    local_30 = 0;
  }
  else {
    fseek(__stream,0,2);
    sVar1 = ftell(__stream);
    if (sVar1 == 0) {
      local_30 = 0;
    }
    else {
      fseek(__stream,0,0);
      __ptr = malloc(sVar1 + 100);
      fread(__ptr,1,sVar1,__stream);
      fclose(__stream);
      uVar2 = FUN_0000d9d8(__ptr);
      local_30 = FUN_0000d9fc();
      sVar1 = local_30 << 2;
      pvVar3 = malloc(sVar1);
      *param_2 = (int)pvVar3;
      pvVar3 = malloc(sVar1);
      if (0 < (int)local_30) {
        unaff_r11 = 0;
        sVar1 = unaff_r11;
      }
      *param_3 = (int)pvVar3;
      if (0 < (int)local_30) {
        do {
          iVar4 = FUN_0000da28(uVar2,sVar1);
          sVar5 = strlen(*(char **)(iVar4 + 0x20));
          iVar4 = FUN_0000da28(uVar2,sVar1);
          sVar6 = strlen(*(char **)(iVar4 + 0x10));
          iVar4 = *param_2;
          pvVar3 = malloc(sVar5 + 1);
          *(void **)(iVar4 + sVar1 * 4) = pvVar3;
          iVar4 = *param_3;
          pvVar3 = malloc(sVar6 + 1);
          *(void **)(iVar4 + sVar1 * 4) = pvVar3;
          pcVar7 = *(char **)(*param_2 + sVar1 * 4);
          iVar4 = FUN_0000da28(uVar2,sVar1);
          strcpy(pcVar7,*(char **)(iVar4 + 0x20));
          pcVar7 = *(char **)(*param_3 + sVar1 * 4);
          iVar4 = FUN_0000da28(uVar2,sVar1);
          strcpy(pcVar7,*(char **)(iVar4 + 0x10));
          *(char *)(*(int *)(*param_2 + sVar1 * 4) + sVar5) = (char)unaff_r11;
          iVar4 = sVar1 * 4;
          sVar1 = sVar1 + 1;
          *(char *)(*(int *)(*param_3 + iVar4) + sVar6) = (char)unaff_r11;
        } while (sVar1 != local_30);
      }
      FUN_0000d864(uVar2);
      free(__ptr);
    }
  }
  return local_30;
}

