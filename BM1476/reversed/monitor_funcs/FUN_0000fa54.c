//==================== FUN_0000fa54 @ 0000fa54 ====================

undefined4 FUN_0000fa54(char *param_1,undefined4 param_2)

{
  FILE *pFVar1;
  size_t __n;
  void *__ptr;
  undefined4 uVar2;
  int iVar3;
  undefined4 uVar4;
  char *__s;
  char acStack_90 [12];
  char acStack_84 [12];
  char acStack_78 [16];
  char acStack_68 [20];
  char acStack_54 [60];
  
  pFVar1 = fopen(param_1,"rb+");
  if (pFVar1 == (FILE *)0x0) {
    puts("open error");
                    /* WARNING: Subroutine does not return */
    exit(1);
  }
  fseek(pFVar1,0,2);
  __n = ftell(pFVar1);
  if (__n == 0) {
    uVar2 = 0;
  }
  else {
    fseek(pFVar1,0,0);
    __ptr = malloc(__n + 100);
    fread(__ptr,1,__n,pFVar1);
    fclose(pFVar1);
    uVar2 = FUN_0000d9d8(__ptr);
    iVar3 = FUN_0000da50(uVar2,"api-switch");
    strcpy(acStack_78,*(char **)(iVar3 + 0x10));
    iVar3 = FUN_0000da50(uVar2,"api-uid");
    strcpy(acStack_54,*(char **)(iVar3 + 0x10));
    iVar3 = FUN_0000da50(uVar2,"ip_server");
    strcpy(acStack_68,*(char **)(iVar3 + 0x10));
    iVar3 = FUN_0000da50(uVar2,"port_server");
    strcpy(acStack_90,*(char **)(iVar3 + 0x10));
    iVar3 = FUN_0000da50(uVar2,"port_local");
    strcpy(acStack_84,*(char **)(iVar3 + 0x10));
    FUN_0000d864(uVar2);
    pFVar1 = fopen(param_1,"w+");
    if (pFVar1 == (FILE *)0x0) {
      puts("open error");
    }
    uVar2 = FUN_0000de4c();
    uVar4 = FUN_0000de04(acStack_78);
    FUN_0000dac8(uVar2,"api-switch",uVar4);
    uVar4 = FUN_0000de04(acStack_54);
    FUN_0000dac8(uVar2,"api-uid",uVar4);
    uVar4 = FUN_0000de04(param_2);
    FUN_0000dac8(uVar2,&DAT_00011e08,uVar4);
    uVar4 = FUN_0000de04(acStack_68);
    FUN_0000dac8(uVar2,"ip_server",uVar4);
    uVar4 = FUN_0000de04(acStack_90);
    FUN_0000dac8(uVar2,"port_server",uVar4);
    uVar4 = FUN_0000de04(acStack_84);
    FUN_0000dac8(uVar2,"port_local",uVar4);
    __s = (char *)FUN_0000d9e4(uVar2);
    fputs(__s,pFVar1);
    fclose(pFVar1);
    FUN_0000d864(uVar2);
    free(__s);
    free(__ptr);
    uVar2 = 1;
  }
  return uVar2;
}

