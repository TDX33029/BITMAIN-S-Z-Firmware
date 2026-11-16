//==================== FUN_000303a4 @ 000303a4 ====================

void FUN_000303a4(int *param_1,int param_2,undefined4 param_3,undefined4 param_4)

{
  undefined1 *puVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  undefined4 uVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  bool bVar10;
  
  uVar3 = param_1[1];
  bVar10 = (uVar3 & 0x3f) < 0x38;
  uVar4 = uVar3 + *param_1;
  if (bVar10) {
    iVar2 = 0x40;
  }
  else {
    iVar2 = 0x80;
  }
  if (bVar10) {
    iVar6 = 0x3c;
    iVar8 = 0x3d;
    iVar7 = 0x3e;
    iVar9 = 0x3f;
    uVar5 = 1;
  }
  else {
    iVar6 = 0x7c;
    iVar8 = 0x7d;
    iVar7 = 0x7e;
    iVar9 = 0x7f;
    uVar5 = 2;
  }
  memset((void *)((int)param_1 + uVar3 + 8),0,iVar2 - uVar3);
  *(undefined1 *)((int)param_1 + param_1[1] + 8) = 0x80;
  uVar3 = (uVar4 & 0x1fffffff) >> 0xd;
  *(char *)((int)param_1 + iVar9 + 8) = (char)uVar4 * '\b';
  *(char *)((int)param_1 + iVar7 + 8) = (char)((uVar4 & 0x1fffffff) >> 5);
  *(char *)((int)param_1 + iVar8 + 8) = (char)uVar3;
  *(char *)((int)param_1 + iVar6 + 8) = (char)((uVar4 & 0x1fffffff) >> 0x15);
  FUN_000300b0(param_1,param_1 + 2,uVar5,uVar3,param_4);
  iVar2 = 0;
  do {
    iVar9 = param_2 + iVar2 * 4;
    *(char *)(iVar9 + 3) = (char)param_1[0x22];
    *(char *)(iVar9 + 2) = (char)((uint)param_1[0x22] >> 8);
    *(char *)(iVar9 + 1) = (char)*(undefined2 *)((int)param_1 + 0x8a);
    puVar1 = (undefined1 *)((int)param_1 + 0x8b);
    param_1 = param_1 + 1;
    *(undefined1 *)(param_2 + iVar2 * 4) = *puVar1;
    iVar2 = iVar2 + 1;
  } while (iVar2 != 8);
  return;
}

