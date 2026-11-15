//==================== FUN_00016624 @ 00016624 ====================

void FUN_00016624(int param_1,int param_2)

{
  size_t __size;
  uint uVar1;
  void *pvVar2;
  uint uVar3;
  int *piVar4;
  int iVar5;
  undefined4 uVar6;
  int iVar7;
  uint uVar8;
  uint uVar9;
  int iVar10;
  undefined4 *puVar11;
  int iVar12;
  uint local_2c;
  
  *(int *)(param_1 + 0x168) = *(int *)(param_1 + 0x168) + 1;
  *(undefined4 *)(param_2 + 0x730) = 0;
  *(int *)(param_2 + 0x73c) = param_2 + 0x724;
  *(undefined4 *)(param_2 + 0x740) = 4;
  if (*(int *)(param_1 + 0x160) == 0) {
    *(int *)(param_1 + 0x160) = param_2;
    *(undefined4 *)(param_2 + 0x72c) = 0;
    iVar7 = *(int *)(param_1 + 0x160);
    pvVar2 = malloc(0x2c);
    *(void **)(iVar7 + 0x728) = pvVar2;
    pvVar2 = *(void **)(*(int *)(param_1 + 0x160) + 0x728);
    if (pvVar2 == (void *)0x0) goto LAB_00016a78;
    memset(pvVar2,0,0x2c);
    *(int *)(*(int *)(*(int *)(param_1 + 0x160) + 0x728) + 0x10) = *(int *)(param_1 + 0x160) + 0x728
    ;
    *(undefined4 *)(*(int *)(*(int *)(param_1 + 0x160) + 0x728) + 4) = 0x20;
    *(undefined4 *)(*(int *)(*(int *)(param_1 + 0x160) + 0x728) + 8) = 5;
    *(undefined4 *)(*(int *)(*(int *)(param_1 + 0x160) + 0x728) + 0x14) = 0x728;
    puVar11 = *(undefined4 **)(*(int *)(param_1 + 0x160) + 0x728);
    pvVar2 = malloc(0x180);
    *puVar11 = pvVar2;
    pvVar2 = (void *)**(undefined4 **)(*(int *)(param_1 + 0x160) + 0x728);
    if (pvVar2 == (void *)0x0) goto LAB_00016a78;
    memset(pvVar2,0,0x180);
    *(undefined4 *)(*(int *)(*(int *)(param_1 + 0x160) + 0x728) + 0x28) = 0xa0111fe1;
  }
  else {
    *(int *)(*(int *)(*(int *)(*(int *)(param_1 + 0x160) + 0x728) + 0x10) + 8) = param_2;
    iVar7 = *(int *)(*(int *)(param_1 + 0x160) + 0x728);
    *(int *)(param_2 + 0x72c) = *(int *)(iVar7 + 0x10) - *(int *)(iVar7 + 0x14);
    *(int *)(*(int *)(*(int *)(param_1 + 0x160) + 0x728) + 0x10) = param_2 + 0x728;
  }
  iVar7 = *(int *)(*(int *)(param_1 + 0x160) + 0x728);
  *(int *)(iVar7 + 0xc) = *(int *)(iVar7 + 0xc) + 1;
  uVar8 = *(byte *)(param_2 + 0x724) + 0x112410d + (uint)*(byte *)(param_2 + 0x727) * 0x1000000 +
          (uint)*(byte *)(param_2 + 0x726) * 0x10000 + (uint)*(byte *)(param_2 + 0x725) * 0x100 ^
          0x7f76d;
  uVar3 = 0x9f49bac6 - uVar8 ^ uVar8 << 8;
  uVar1 = (-0x112410d - uVar8) - uVar3 ^ uVar3 >> 0xd;
  puVar11 = (undefined4 *)(param_2 + 0x728);
  *puVar11 = *(undefined4 *)(*(int *)(param_1 + 0x160) + 0x728);
  uVar8 = (uVar8 - uVar3) - uVar1 ^ uVar1 >> 0xc;
  uVar3 = (uVar3 - uVar1) - uVar8 ^ uVar8 << 0x10;
  uVar9 = (uVar1 - uVar8) - uVar3 ^ uVar3 >> 5;
  uVar1 = (uVar8 - uVar3) - uVar9 ^ uVar9 >> 3;
  uVar8 = (uVar3 - uVar9) - uVar1 ^ uVar1 << 10;
  uVar1 = (uVar9 - uVar1) - uVar8 ^ uVar8 >> 0xf;
  *(uint *)(param_2 + 0x744) = uVar1;
  piVar4 = *(int **)(*(int *)(param_1 + 0x160) + 0x728);
  iVar7 = (uVar1 & piVar4[1] - 1U) * 0xc;
  iVar5 = *piVar4 + iVar7;
  *(int *)(iVar5 + 4) = *(int *)(iVar5 + 4) + 1;
  uVar6 = *(undefined4 *)(**(int **)(*(int *)(param_1 + 0x160) + 0x728) + iVar7);
  *(undefined4 *)(param_2 + 0x734) = 0;
  *(undefined4 *)(param_2 + 0x738) = uVar6;
  iVar5 = **(int **)(*(int *)(param_1 + 0x160) + 0x728);
  if (*(int *)(iVar5 + iVar7) != 0) {
    *(undefined4 **)(*(int *)(iVar5 + iVar7) + 0xc) = puVar11;
    iVar5 = **(int **)(*(int *)(param_1 + 0x160) + 0x728);
  }
  *(undefined4 **)(iVar5 + iVar7) = puVar11;
  iVar7 = **(int **)(*(int *)(param_1 + 0x160) + 0x728) + iVar7;
  if (((uint)((*(int *)(iVar7 + 8) + 1) * 10) <= *(uint *)(iVar7 + 4)) &&
     (*(int *)(*(int *)(param_2 + 0x728) + 0x24) != 1)) {
    __size = *(int *)(*(int *)(param_2 + 0x728) + 4) * 0x18;
    pvVar2 = malloc(__size);
    if (pvVar2 == (void *)0x0) {
LAB_00016a78:
                    /* WARNING: Subroutine does not return */
      exit(-1);
    }
    memset(pvVar2,0,__size);
    iVar7 = *(int *)(param_2 + 0x728);
    iVar5 = 0;
    *(uint *)(iVar7 + 0x18) =
         (uint)((*(int *)(iVar7 + 4) * 2 - 1U & *(uint *)(iVar7 + 0xc)) != 0) +
         (*(uint *)(iVar7 + 0xc) >> (*(int *)(iVar7 + 8) + 1U & 0xff));
    *(undefined4 *)(*(int *)(param_2 + 0x728) + 0x1c) = 0;
    piVar4 = *(int **)(param_2 + 0x728);
    uVar1 = piVar4[1];
    if (uVar1 != 0) {
      local_2c = 0;
      do {
        iVar7 = *(int *)(*piVar4 + iVar5);
        if (*(int *)(*piVar4 + iVar5) != 0) {
          while( true ) {
            iVar12 = *(int *)(iVar7 + 0x10);
            uVar8 = uVar1 * 2 - 1 & *(uint *)(iVar7 + 0x1c);
            uVar1 = *(int *)((int)pvVar2 + uVar8 * 0xc + 4) + 1;
            *(uint *)((int)pvVar2 + uVar8 * 0xc + 4) = uVar1;
            if ((uint)piVar4[6] < uVar1) {
              piVar4[7] = piVar4[7] + 1;
              uVar6 = FUN_0005c648(*(undefined4 *)((int)pvVar2 + uVar8 * 0xc + 4),
                                   *(undefined4 *)(*(int *)(param_2 + 0x728) + 0x18));
              *(undefined4 *)((int)pvVar2 + uVar8 * 0xc + 8) = uVar6;
            }
            *(undefined4 *)(iVar7 + 0xc) = 0;
            *(undefined4 *)(iVar7 + 0x10) = *(undefined4 *)((int)pvVar2 + uVar8 * 0xc);
            iVar10 = *(int *)((int)pvVar2 + uVar8 * 0xc);
            if (iVar10 != 0) {
              *(int *)(iVar10 + 0xc) = iVar7;
            }
            *(int *)((int)pvVar2 + uVar8 * 0xc) = iVar7;
            piVar4 = *(int **)(param_2 + 0x728);
            if (iVar12 == 0) break;
            uVar1 = piVar4[1];
            iVar7 = iVar12;
          }
          uVar1 = piVar4[1];
        }
        iVar5 = iVar5 + 0xc;
        local_2c = local_2c + 1;
      } while (local_2c < uVar1);
    }
    free((void *)*piVar4);
    *(int *)(*(int *)(param_2 + 0x728) + 4) = *(int *)(*(int *)(param_2 + 0x728) + 4) << 1;
    *(int *)(*(int *)(param_2 + 0x728) + 8) = *(int *)(*(int *)(param_2 + 0x728) + 8) + 1;
    **(undefined4 **)(param_2 + 0x728) = pvVar2;
    iVar7 = *(int *)(param_2 + 0x728);
    if (*(uint *)(iVar7 + 0xc) >> 1 < *(uint *)(iVar7 + 0x1c)) {
      iVar5 = *(int *)(iVar7 + 0x20) + 1;
    }
    else {
      iVar5 = 0;
    }
    *(int *)(iVar7 + 0x20) = iVar5;
    if (1 < *(uint *)(*(int *)(param_2 + 0x728) + 0x20)) {
      *(undefined4 *)(*(int *)(param_2 + 0x728) + 0x24) = 1;
    }
  }
  return;
}

