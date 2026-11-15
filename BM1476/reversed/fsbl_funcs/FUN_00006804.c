//==================== FUN_00006804 @ 00006804 ====================

undefined4 FUN_00006804(undefined2 *param_1,undefined2 *param_2,int param_3)

{
  byte bVar1;
  ushort uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int iVar5;
  undefined1 uVar6;
  uint uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  
  if (param_1 == (undefined2 *)0x0) {
    FUN_000014e4(&DAT_0001725c,0xb9);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (param_2 == (undefined2 *)0x0) {
    FUN_000014e4(&DAT_0001725c,0xba);
    DAT_00028e84 = 1;
    return 0;
  }
  uVar3 = *(undefined4 *)(param_2 + 4);
  uVar8 = *(undefined4 *)(param_2 + 6);
  uVar10 = *(undefined4 *)(param_2 + 8);
  *param_1 = *param_2;
  *(undefined4 *)(param_1 + 0x12) = 0x11111111;
  uVar9 = *(undefined4 *)(param_2 + 10);
  uVar4 = *(undefined4 *)(param_2 + 0xe);
  *(undefined4 *)(param_1 + 4) = uVar3;
  uVar3 = *(undefined4 *)(param_2 + 0xc);
  *(int *)(param_1 + 2) = param_3;
  *(undefined4 *)(param_1 + 6) = uVar8;
  *(undefined4 *)(param_1 + 10) = uVar9;
  *(undefined4 *)(param_1 + 0xc) = uVar3;
  *(undefined4 *)(param_1 + 8) = uVar10;
  *(undefined4 *)(param_1 + 0xe) = uVar4;
  uVar6 = *(undefined1 *)(param_2 + 0x10);
  *(undefined4 *)(param_1 + 0x32) = 0;
  *(undefined4 *)(param_1 + 0x38) = 0;
  *(undefined1 *)(param_1 + 0x10) = uVar6;
  uVar2 = *(ushort *)(param_3 + 0xfe);
  *(undefined4 *)(param_1 + 0x36) = 6;
  *(undefined4 *)(param_1 + 0x100) = 0;
  *(undefined4 *)(param_1 + 0x102) = 0;
  if ((uVar2 & 0xff) == 2) {
    *(undefined1 *)(param_3 + 0x29) = 0x10;
  }
  else {
    *(undefined1 *)(param_3 + 0x29) = 0;
  }
  func_0xffffe2ac(1000);
  *(undefined1 *)(*(int *)(param_1 + 2) + 0x2f) = 1;
  iVar5 = *(int *)(param_1 + 2);
  bVar1 = *(byte *)(iVar5 + 0x2f);
  while ((bVar1 & 1) != 0) {
    bVar1 = *(byte *)(iVar5 + 0x2f);
  }
  *(char *)(param_1 + 0x1b) = (char)*(undefined2 *)(iVar5 + 0xfe);
  *(undefined4 *)(param_1 + 0x14) = *(undefined4 *)(iVar5 + 0x40);
  *(undefined1 *)(iVar5 + 0x29) = 0xf;
  if ((*(char *)(param_1 + 0x1b) == '\x02') &&
     ((*(uint *)(param_1 + 0x14) & 0xc0000000) == 0x40000000)) {
    func_0xffffe2ac(200);
  }
  iVar5 = FUN_000082b0(param_1,s__00061a80);
  if (iVar5 != 0) {
    return 1;
  }
  uVar7 = *(uint *)(param_1 + 0x14);
  if ((uVar7 & 0x1000000) == 0) {
    if ((uVar7 & 0x2000000) == 0) {
      if ((uVar7 & 0x4000000) == 0) {
        uVar6 = 1;
      }
      else {
        uVar6 = 0xb;
      }
    }
    else {
      uVar6 = 0xd;
    }
  }
  else {
    uVar6 = 0xf;
  }
  *(undefined1 *)(*(int *)(param_1 + 2) + 0x29) = uVar6;
  if (*(char *)(param_1 + 0x1b) == '\x02') {
    uVar6 = 0x18;
  }
  else {
    uVar6 = 0x10;
  }
  *(undefined1 *)(*(int *)(param_1 + 2) + 0x28) = uVar6;
  iVar5 = *(int *)(param_1 + 2);
  *(undefined2 *)(iVar5 + 0x34) = 0xfeff;
  *(undefined2 *)(iVar5 + 0x36) = 0xf3ff;
  *(undefined2 *)(iVar5 + 0x38) = 0;
  *(undefined2 *)(iVar5 + 0x3a) = 0;
  DAT_00028e88 = 0x13;
  *(undefined2 *)(iVar5 + 4) = 0x200;
  return 0;
}

