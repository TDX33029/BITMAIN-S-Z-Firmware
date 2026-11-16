//==================== FUN_00010004 @ 00010004 ====================

int FUN_00010004(char *param_1,int param_2)

{
  FILE *__stream;
  size_t __n;
  void *__ptr;
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  int iVar5;
  
  __stream = fopen(param_1,"rb+");
  if (__stream == (FILE *)0x0) {
    puts("open error");
  }
  fseek(__stream,0,2);
  __n = ftell(__stream);
  if (__n == 0) {
    param_2 = 0;
  }
  else {
    fseek(__stream,0,0);
    __ptr = malloc(__n + 100);
    fread(__ptr,1,__n,__stream);
    fclose(__stream);
    uVar1 = FUN_0000d9d8(__ptr);
    iVar2 = FUN_0000da50(uVar1,"pools");
    FUN_0000d9fc();
    piVar4 = *(int **)(iVar2 + 8);
    if (piVar4 != (int *)0x0) {
      iVar2 = 0;
      do {
        iVar3 = FUN_0000da50(piVar4,&DAT_00011ea0);
        iVar5 = iVar2 * 0xa4;
        iVar2 = iVar2 + 1;
        strcpy((char *)(param_2 + iVar5 + 4),*(char **)(iVar3 + 0x10));
        iVar3 = FUN_0000da50(piVar4,&DAT_00011ea4);
        strcpy((char *)(param_2 + iVar5 + 0x54),*(char **)(iVar3 + 0x10));
        iVar3 = FUN_0000da50(piVar4,&DAT_00011eac);
        strcpy((char *)(param_2 + iVar5 + 0x7c),*(char **)(iVar3 + 0x10));
        piVar4 = (int *)*piVar4;
      } while (piVar4 != (int *)0x0);
    }
    iVar2 = FUN_0000da50(uVar1,"api-listen");
    *(undefined4 *)(param_2 + 0x1ec) = *(undefined4 *)(iVar2 + 0x14);
    iVar2 = FUN_0000da50(uVar1,"api-network");
    *(undefined4 *)(param_2 + 0x1f0) = *(undefined4 *)(iVar2 + 0x14);
    iVar2 = FUN_0000da50(uVar1,"bitmain-nobeeper");
    *(undefined4 *)(param_2 + 500) = *(undefined4 *)(iVar2 + 0x14);
    iVar2 = FUN_0000da50(uVar1,"bitmain-notempoverctrl");
    *(undefined4 *)(param_2 + 0x1f8) = *(undefined4 *)(iVar2 + 0x14);
    iVar2 = FUN_0000da50(uVar1,"api-allow");
    strcpy((char *)(param_2 + 0x1fc),*(char **)(iVar2 + 0x10));
    iVar2 = FUN_0000da50(uVar1,"bitmain-freq");
    strcpy((char *)(param_2 + 0x206),*(char **)(iVar2 + 0x10));
    iVar2 = FUN_0000da50(uVar1,"bitmain-voltage");
    strcpy((char *)(param_2 + 0x215),*(char **)(iVar2 + 0x10));
    free(__ptr);
    FUN_0000d864(uVar1);
  }
  return param_2;
}

