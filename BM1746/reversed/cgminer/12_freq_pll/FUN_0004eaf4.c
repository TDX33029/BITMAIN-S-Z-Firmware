//==================== FUN_0004eaf4 @ 0004eaf4 ====================

undefined4 FUN_0004eaf4(undefined4 param_1,undefined4 param_2)

{
  char *pcVar1;
  char *pcVar2;
  char *__ptr;
  void *__ptr_00;
  uint uVar3;
  void *__ptr_01;
  undefined4 uVar4;
  int iVar5;
  int iVar6;
  size_t sVar7;
  uint *puVar8;
  char *pcVar9;
  uint *puVar10;
  char *pcVar11;
  uint uVar12;
  uint uVar13;
  int iVar14;
  int iVar15;
  size_t local_8b4;
  size_t local_888;
  undefined4 local_87c;
  undefined4 uStack_878;
  undefined4 uStack_874;
  undefined4 uStack_870;
  undefined4 local_86c;
  undefined4 uStack_868;
  undefined4 uStack_864;
  undefined2 local_860;
  undefined4 local_85c;
  undefined4 local_858;
  undefined4 local_854;
  undefined4 local_850;
  undefined4 local_84c;
  undefined4 local_848;
  undefined4 local_844;
  undefined2 local_840;
  uint uStack_82c;
  uint local_828 [513];
  
  __ptr = malloc(0x4400);
  iVar15 = 0;
  puVar10 = &uStack_82c;
  __ptr_00 = malloc(0x4400);
  memset(local_828,0,0x800);
  FUN_0004df94(param_2,0x540,local_828,0x800,0x15,1);
  puVar8 = (uint *)(__ptr + 0x1e);
  do {
    puVar10 = puVar10 + 1;
    uVar12 = *puVar10;
    uVar3 = uVar12 << 0x18 | (uVar12 >> 8 & 0xff) << 0x10 | (uVar12 >> 0x10 & 0xff) << 8;
    uVar12 = uVar3 | uVar12 >> 0x18;
    FUN_0004df30(param_1,(int)uVar12 / 2,&local_85c,0x32);
    iVar5 = (int)uVar3 >> 0x1f;
    FUN_0004df94((int)&local_85c + ((uVar12 - iVar5 & 1) + iVar5) * 0x19,0x19,&local_87c,0x1e,0x14,0
                );
    *(undefined4 *)((int)puVar8 + -0x1e) = local_87c;
    *(undefined4 *)((int)puVar8 + -0x1a) = uStack_878;
    *(undefined4 *)((int)puVar8 + -0x16) = uStack_874;
    *(undefined4 *)((int)puVar8 + -0x12) = uStack_870;
    *(undefined4 *)((int)puVar8 + -0xe) = local_86c;
    uVar3 = local_828[iVar15];
    iVar15 = iVar15 + 1;
    *(undefined4 *)((int)puVar8 + -10) = uStack_868;
    *(undefined2 *)((int)puVar8 + -2) = local_860;
    *(undefined4 *)((int)puVar8 + -6) = uStack_864;
    *puVar8 = uVar3;
    __ptr_01 = (void *)FUN_00029730((char *)((int)puVar8 + -0x1e),0x22);
    free(__ptr_01);
    puVar8 = (uint *)((int)puVar8 + 0x22);
  } while (iVar15 != 0x200);
  iVar15 = 0x1e;
  local_888 = 4;
  local_8b4 = 0x1b;
  uVar3 = 0x200;
  while( true ) {
    if (uVar3 == 0) {
      local_888 = local_888 << 1;
    }
    else {
      iVar5 = iVar15 + local_888;
      uVar13 = 0;
      uVar12 = 0;
      pcVar11 = __ptr + iVar5;
      pcVar9 = __ptr;
      do {
        iVar6 = 0;
        do {
          pcVar1 = pcVar9 + iVar6;
          pcVar2 = pcVar11 + iVar6;
          iVar6 = iVar6 + 1;
          if (*pcVar1 != *pcVar2) {
            puts("Invalid solution: invalid collision length between StepRows");
            goto LAB_0004ee20;
          }
        } while (iVar6 != 3);
        iVar6 = FUN_0004ea38(pcVar11,pcVar9,iVar15,local_888);
        if (iVar6 != 0) {
          puts("Invalid solution: Index tree incorrectly ordered");
          goto LAB_0004ee20;
        }
        iVar6 = FUN_0004ea58(pcVar9,pcVar11,iVar15,local_888);
        if (iVar6 == 0) {
          puts("Invalid solution: duplicate indices");
          free(__ptr);
          free(__ptr_00);
          return 1;
        }
        local_854 = 0;
        local_850 = 0;
        local_84c = 0;
        local_848 = 0;
        local_844 = 0;
        local_840 = 0;
        iVar6 = 0;
        local_85c = 0;
        local_858 = 0;
        do {
          *(char *)((int)&local_85c + iVar6) = pcVar11[iVar6] ^ pcVar9[iVar6];
          iVar6 = iVar6 + 1;
        } while (iVar6 < iVar15);
        uVar13 = uVar13 + 1;
        iVar6 = (local_888 * 2 + local_8b4) * (uVar12 >> 1);
        uVar12 = uVar13 * 2;
        pcVar11 = pcVar11 + iVar5 * 2;
        iVar14 = local_8b4 + iVar6;
        memcpy((void *)((int)__ptr_00 + iVar6),(void *)((int)&local_85c + 3),local_8b4);
        memcpy((void *)((int)__ptr_00 + iVar14),pcVar9 + iVar15,local_888);
        memcpy((void *)((int)__ptr_00 + iVar14 + local_888),pcVar9 + iVar15 + iVar5,local_888);
        pcVar9 = pcVar9 + iVar5 * 2;
      } while (uVar12 < uVar3);
      uVar3 = uVar13;
      local_888 = local_888 * 2;
    }
    iVar15 = iVar15 + -3;
    memcpy(__ptr,__ptr_00,0x4400);
    memset(__ptr_00,0,0x4400);
    if (iVar15 == 3) break;
    local_8b4 = local_8b4 - 3;
  }
  if (local_8b4 == 0) {
LAB_0004eea8:
    free(__ptr);
    free(__ptr_00);
    uVar4 = 0;
  }
  else {
    if (*__ptr == '\0') {
      sVar7 = 0;
      pcVar9 = __ptr;
      do {
        sVar7 = sVar7 + 1;
        if (sVar7 == local_8b4) goto LAB_0004eea8;
        pcVar9 = pcVar9 + 1;
      } while (*pcVar9 == '\0');
    }
LAB_0004ee20:
    free(__ptr);
    free(__ptr_00);
    uVar4 = 2;
  }
  return uVar4;
}

