//==================== FUN_00007100 @ 00007100 ====================

void FUN_00007100(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  int iVar9;
  int iVar10;
  bool bVar11;
  
  iVar2 = *(int *)(param_1 + 4);
  uVar8 = *(uint *)(param_1 + 0x200);
  iVar9 = *(int *)(param_1 + 0x204);
  uVar7 = *(ushort *)(iVar2 + 4) & 0xfff;
  uVar5 = param_2 * uVar7;
  if (uVar5 < 0x10000) {
    iVar1 = 0;
  }
  else {
    uVar6 = uVar5 >> 0x10;
    if ((uVar5 & 0xffff) != 0) {
      uVar6 = uVar6 + 1;
    }
    if (uVar6 == 1) {
      iVar1 = 0;
    }
    else {
      iVar3 = 0;
      iVar4 = iVar9;
      uVar5 = uVar8;
      iVar10 = param_1;
      do {
        iVar1 = iVar3 + 1;
        iVar3 = param_1 + iVar3 * 0xc;
        *(uint *)(iVar3 + 0x84) = uVar5;
        bVar11 = 0xfffeffff < uVar5;
        uVar5 = uVar5 + 0x10000;
        *(int *)(iVar3 + 0x88) = iVar4;
        iVar4 = iVar4 + (uint)bVar11;
        *(undefined2 *)(iVar10 + 0x80) = 0x21;
        *(undefined2 *)(iVar10 + 0x82) = 0;
        iVar3 = iVar1;
        iVar10 = iVar10 + 0xc;
      } while (iVar1 != uVar6 - 1);
      bVar11 = CARRY4(uVar8,iVar1 * 0x10000);
      uVar8 = uVar8 + iVar1 * 0x10000;
      iVar9 = iVar9 + (uint)bVar11;
    }
  }
  iVar1 = param_1 + iVar1 * 0xc;
  *(uint *)(iVar1 + 0x84) = uVar8;
  *(int *)(iVar1 + 0x88) = iVar9;
  *(short *)(iVar1 + 0x82) = (short)param_2 * (short)uVar7;
  *(undefined2 *)(iVar1 + 0x80) = 0x23;
  *(int *)(iVar2 + 0x58) = param_1 + 0x80;
  if (*(char *)(param_1 + 0x20) == '\0') {
    func_0xffffd370(param_1 + 0x80,0x180);
  }
  *(uint *)(param_1 + 0x200) = 0;
  *(undefined4 *)(param_1 + 0x204) = 0;
  return;
}

