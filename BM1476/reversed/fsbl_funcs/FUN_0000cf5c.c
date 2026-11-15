//==================== FUN_0000cf5c @ 0000cf5c ====================

undefined4 FUN_0000cf5c(int param_1,uint param_2,uint param_3,uint param_4,uint param_5)

{
  int iVar1;
  undefined4 uVar2;
  uint uVar3;
  bool bVar4;
  
  if (param_1 == 0) {
    FUN_000014e4(s_0___P___p_0001728e + 0x12,0x24a);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) != 0x11111111) {
    uVar2 = 0x24b;
LAB_0000cff8:
    FUN_000014e4(s_0___P___p_0001728e + 0x12,uVar2);
    DAT_00028e84 = 1;
    return 0;
  }
  bVar4 = 7 < param_2;
  if (param_2 != 8) {
    bVar4 = 5 < param_2;
  }
  if ((bVar4 && (param_2 != 8 && param_2 != 6)) && (0x12 < param_2 - 0xd)) {
    uVar2 = 0x24f;
    goto LAB_0000cff8;
  }
  if (1 < param_3) {
    uVar2 = 0x251;
    goto LAB_0000cff8;
  }
  if (1 < param_4) {
    uVar2 = 0x252;
    goto LAB_0000cff8;
  }
  if (1 < param_5) {
    uVar2 = 0x254;
    goto LAB_0000cff8;
  }
  iVar1 = FUN_0000ceb8(param_1);
  if (iVar1 != 3) {
    return 1;
  }
  *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = 0x4400000;
  *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = *(undefined4 *)(*(int *)(param_1 + 4) + 0x14);
  uVar3 = *(uint *)(*(int *)(param_1 + 4) + 0x14) & 0x3000;
  if (param_3 == 1) {
    bVar4 = 2 < param_2;
    if (param_2 != 3) {
      bVar4 = 0xe < param_2 - 0x10;
    }
    if (bVar4 && (param_2 != 3 && param_2 - 0x10 != 0xf)) {
      return 1;
    }
    uVar3 = uVar3 | 0x100;
    if (param_5 != 1) goto LAB_0000d09c;
  }
  else {
    if (param_5 != 1) goto LAB_0000d09c;
    bVar4 = 2 < param_2;
    if (param_2 != 3) {
      bVar4 = 0xe < param_2 - 0x10;
    }
    if (bVar4 && (param_2 != 3 && param_2 - 0x10 != 0xf)) {
      return 1;
    }
  }
  uVar3 = uVar3 | 0x400;
LAB_0000d09c:
  if (param_4 == 1) {
    uVar3 = uVar3 | 0x200;
  }
  *(uint *)(*(int *)(param_1 + 4) + 0x10) = param_2 & 0x1f | 0x8400000 | uVar3;
  return 0;
}

