//==================== FUN_0000f990 @ 0000f990 ====================

char * FUN_0000f990(char *param_1,undefined4 param_2,char *param_3)

{
  FILE *__stream;
  size_t __n;
  void *__ptr;
  undefined4 uVar1;
  int iVar2;
  
  __stream = fopen(param_1,"rb+");
  if (__stream == (FILE *)0x0) {
    puts("open error1111");
  }
  fseek(__stream,0,2);
  __n = ftell(__stream);
  if (__n == 0) {
    param_3 = (char *)0x0;
  }
  else {
    fseek(__stream,0,0);
    __ptr = malloc(__n + 100);
    fread(__ptr,1,__n,__stream);
    fclose(__stream);
    uVar1 = FUN_0000d9d8(__ptr);
    iVar2 = FUN_0000da50(uVar1,param_2);
    strcpy(param_3,*(char **)(iVar2 + 0x10));
    FUN_0000d864(uVar1);
    free(__ptr);
  }
  return param_3;
}

