//==================== FUN_00038b58 @ 00038b58 ====================

void FUN_00038b58(undefined4 *param_1,char *param_2,char *param_3,int param_4)

{
  void *pvVar1;
  undefined4 *puVar2;
  undefined4 uVar3;
  int iVar4;
  uint uVar5;
  int iVar6;
  undefined4 *puVar7;
  size_t __nmemb;
  uint uVar8;
  char acStack_1018 [4096];
  
  if (*(char *)(param_1 + 1) == '\0') {
    iVar6 = param_1[10];
  }
  else {
    snprintf(acStack_1018,0x1000,"List %s store can\'t %s() - from %s %s() line %d in %s %s():%d",
             (char *)*param_1,"k_alloc_items",param_2,param_3,param_4,"klist.c","k_alloc_items",0x13
            );
    FUN_00038730(3,acStack_1018,1);
    FUN_00016ca8(1);
    iVar6 = param_1[10];
  }
  if (iVar6 < 1) {
    __nmemb = param_1[9];
  }
  else {
    iVar4 = param_1[6];
    if (iVar6 <= iVar4) {
      return;
    }
    __nmemb = param_1[9];
    if (iVar6 < (int)(iVar4 + __nmemb)) {
      __nmemb = iVar6 - iVar4;
    }
  }
  iVar6 = param_1[0xc];
  param_1[0xc] = iVar6 + 1;
  pvVar1 = realloc((void *)param_1[0xd],(iVar6 + 1) * 4);
  param_1[0xd] = pvVar1;
  if (pvVar1 == (void *)0x0) {
    snprintf(acStack_1018,0x1000,"List %s item_memory failed to realloc count=%d in %s %s():%d",
             (char *)*param_1,param_1[0xc],"klist.c","k_alloc_items",0x21);
    FUN_00038730(3,acStack_1018,1);
    FUN_00016ca8(1);
  }
  puVar2 = calloc(__nmemb,0x10);
  if (puVar2 == (undefined4 *)0x0) {
    snprintf(acStack_1018,0x1000,
             "List %s failed to calloc %d new items - total was %d, limit was %d in %s %s():%d",
             (char *)*param_1,__nmemb,param_1[6],param_1[10],"klist.c","k_alloc_items",0x26);
    FUN_00038730(3,acStack_1018,1);
    FUN_00016ca8(1);
  }
  uVar8 = __nmemb - 1;
  *(undefined4 **)(param_1[0xd] + (param_1[0xc] + -1) * 4) = puVar2;
  param_1[7] = __nmemb;
  param_1[8] = __nmemb;
  param_1[6] = param_1[6] + __nmemb;
  *puVar2 = *param_1;
  puVar2[1] = 0;
  puVar2[2] = puVar2 + 4;
  puVar7 = puVar2 + 4;
  if (1 < (int)uVar8) {
    puVar7 = puVar2;
  }
  uVar5 = (uint)(1 < (int)uVar8);
  if (1 < (int)uVar8) {
    do {
      uVar3 = *param_1;
      uVar5 = uVar5 + 1;
      puVar7[5] = puVar7;
      puVar7[6] = puVar7 + 8;
      puVar7[4] = uVar3;
      puVar7 = puVar7 + 4;
    } while (uVar5 != uVar8);
  }
  puVar7 = puVar2 + uVar8 * 4;
  puVar2[uVar8 * 4] = *param_1;
  puVar7[1] = puVar2 + (__nmemb - 2) * 4;
  puVar7[2] = 0;
  param_1[3] = puVar2;
  if (*(char *)(param_1 + 0xb) != '\0') {
    param_1[4] = puVar7;
  }
  do {
    iVar6 = param_1[0xe];
    param_1[0xe] = iVar6 + 1;
    pvVar1 = realloc((void *)param_1[0xf],(iVar6 + 1) * 4);
    param_1[0xf] = pvVar1;
    if (pvVar1 == (void *)0x0) {
      snprintf(acStack_1018,0x1000,"List %s data_memory failed to realloc count=%d in %s %s():%d",
               (char *)*param_1,param_1[0xe],"klist.c","k_alloc_items",0x45);
      FUN_00038730(3,acStack_1018,1);
      FUN_00016ca8(1);
    }
    pvVar1 = calloc(1,param_1[5]);
    puVar2[3] = pvVar1;
    if (pvVar1 == (void *)0x0) {
      snprintf(acStack_1018,0x1000,"List %s failed to calloc item data in %s %s():%d",
               (char *)*param_1,"klist.c","k_alloc_items",0x49);
      FUN_00038730(3,acStack_1018,1);
      FUN_00016ca8(1);
      pvVar1 = (void *)puVar2[3];
    }
    *(void **)(param_1[0xf] + (param_1[0xe] + -1) * 4) = pvVar1;
    puVar2 = (undefined4 *)puVar2[2];
  } while (puVar2 != (undefined4 *)0x0);
  return;
}

