//==================== FUN_00010210 @ 00010210 ====================

void FUN_00010210(int param_1,char *param_2)

{
  FILE *__stream;
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  char *__s;
  int iVar5;
  int iVar6;
  
  __stream = fopen(param_2,"w+");
  if (__stream == (FILE *)0x0) {
    puts("open error");
  }
  uVar1 = FUN_0000de4c();
  iVar5 = 0;
  uVar2 = FUN_0000de34();
  FUN_0000dac8(uVar1,"pools",uVar2);
  do {
    uVar3 = FUN_0000de4c();
    iVar6 = iVar5 * 0xa4;
    iVar5 = iVar5 + 1;
    FUN_0000da90(uVar2,uVar3);
    uVar4 = FUN_0000de04(param_1 + iVar6 + 4);
    FUN_0000dac8(uVar3,&DAT_00011ea0,uVar4);
    uVar4 = FUN_0000de04(param_1 + iVar6 + 0x54);
    FUN_0000dac8(uVar3,&DAT_00011ea4,uVar4);
    uVar4 = FUN_0000de04(param_1 + iVar6 + 0x7c);
    FUN_0000dac8(uVar3,&DAT_00011eac,uVar4);
  } while (iVar5 != 3);
  if (*(int *)(param_1 + 0x1ec) == 1) {
    uVar2 = FUN_0000dd80();
    FUN_0000dac8(uVar1,"api-listen",uVar2);
  }
  else if (*(int *)(param_1 + 0x1ec) == 0) {
    uVar2 = FUN_0000dd98();
    FUN_0000dac8(uVar1,"api-listen",uVar2);
    iVar5 = *(int *)(param_1 + 0x1f0);
    goto joined_r0x000102e8;
  }
  iVar5 = *(int *)(param_1 + 0x1f0);
joined_r0x000102e8:
  if (iVar5 == 1) {
    uVar2 = FUN_0000dd80();
    FUN_0000dac8(uVar1,"api-network",uVar2);
    iVar5 = *(int *)(param_1 + 500);
  }
  else {
    if (iVar5 == 0) {
      uVar2 = FUN_0000dd98();
      FUN_0000dac8(uVar1,"api-network",uVar2);
    }
    iVar5 = *(int *)(param_1 + 500);
  }
  if (iVar5 == 1) {
    uVar2 = FUN_0000dd80();
    FUN_0000dac8(uVar1,"bitmain-nobeeper",uVar2);
    iVar5 = *(int *)(param_1 + 0x1f8);
  }
  else {
    iVar5 = *(int *)(param_1 + 0x1f8);
  }
  if (iVar5 == 1) {
    uVar2 = FUN_0000dd80();
    FUN_0000dac8(uVar1,"bitmain-notempoverctrl",uVar2);
  }
  uVar2 = FUN_0000de04(param_1 + 0x1fc);
  FUN_0000dac8(uVar1,"api-allow",uVar2);
  uVar2 = FUN_0000de04(param_1 + 0x206);
  FUN_0000dac8(uVar1,"bitmain-freq",uVar2);
  uVar2 = FUN_0000de04(param_1 + 0x215);
  FUN_0000dac8(uVar1,"bitmain-voltage",uVar2);
  __s = (char *)FUN_0000d9e4(uVar1);
  fputs(__s,__stream);
  fclose(__stream);
  FUN_0000d864(uVar1);
  free(__s);
  return;
}

