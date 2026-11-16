//==================== FUN_000557d4 @ 000557d4 ====================

void FUN_000557d4(undefined4 *param_1,int param_2)

{
  uint uVar1;
  undefined4 *puVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  undefined8 uVar6;
  uint uVar7;
  
  uVar7 = (uint)*(undefined8 *)(param_1 + 8) & 0x3f;
  uVar6 = VectorShiftLeft(*(undefined8 *)(param_1 + 8),3,0x40,0);
  uVar5 = (uint)uVar6;
  *(undefined1 *)((int)param_1 + uVar7 + 0x28) = 0x80;
  while (uVar7 = uVar7 + 1, uVar7 != 0x38) {
    while (uVar7 = uVar7 & 0x3f, uVar7 == 0) {
      FUN_00055470(param_1);
      *(undefined1 *)(param_1 + 10) = 0;
      uVar7 = 1;
    }
    *(undefined1 *)((int)param_1 + uVar7 + 0x28) = 0;
  }
  iVar3 = 0x38;
  puVar2 = param_1;
  uVar7 = (uint)((ulonglong)uVar6 >> 0x20);
  do {
    iVar3 = iVar3 + 1;
    uVar1 = uVar5 >> 0x18;
    *(char *)(puVar2 + 0x18) = (char)(uVar7 >> 0x18);
    uVar5 = uVar5 << 8;
    puVar2 = (undefined4 *)((int)puVar2 + 1);
    uVar7 = uVar7 << 8 | uVar1;
  } while (iVar3 != 0x40);
  FUN_00055470(param_1);
  iVar4 = 0;
  iVar3 = param_2;
  do {
    *(char *)(param_2 + iVar4) = (char)((uint)*(undefined4 *)((int)param_1 + iVar4) >> 0x18);
    *(char *)(iVar3 + 1) = (char)((uint)*(undefined4 *)((int)param_1 + iVar4) >> 0x10);
    *(char *)(iVar3 + 2) = (char)((uint)*(undefined4 *)((int)param_1 + iVar4) >> 8);
    puVar2 = (undefined4 *)((int)param_1 + iVar4);
    iVar4 = iVar4 + 4;
    *(char *)(iVar3 + 3) = (char)*puVar2;
    iVar3 = iVar3 + 4;
  } while (param_2 + 0x20 != iVar3);
  *(undefined8 *)(param_1 + 8) = 0;
  *param_1 = 0x6a09e667;
  param_1[1] = 0xbb67ae85;
  param_1[2] = 0x3c6ef372;
  param_1[3] = 0xa54ff53a;
  param_1[4] = 0x510e527f;
  param_1[5] = 0x9b05688c;
  param_1[6] = 0x1f83d9ab;
  param_1[7] = 0x5be0cd19;
  return;
}

