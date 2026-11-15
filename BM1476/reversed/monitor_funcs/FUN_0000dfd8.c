//==================== FUN_0000dfd8 @ 0000dfd8 ====================

int FUN_0000dfd8(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  uint uVar6;
  int *piVar7;
  int *piVar8;
  
  if ((param_1 == 0) || (iVar1 = FUN_0000c540(), iVar1 == 0)) {
    return 0;
  }
  uVar5 = *(undefined4 *)(param_1 + 0x1c);
  iVar2 = *(int *)(param_1 + 0x10);
  uVar4 = *(undefined4 *)(param_1 + 0x14);
  *(undefined4 *)(iVar1 + 0x18) = *(undefined4 *)(param_1 + 0x18);
  *(undefined4 *)(iVar1 + 0x1c) = uVar5;
  uVar6 = *(uint *)(param_1 + 0xc);
  *(undefined4 *)(iVar1 + 0x14) = uVar4;
  *(uint *)(iVar1 + 0xc) = uVar6 & 0xfffffeff;
  if (iVar2 == 0) {
LAB_0000e030:
    if (*(int *)(param_1 + 0x20) != 0) {
      iVar2 = FUN_0000c574();
      *(int *)(iVar1 + 0x20) = iVar2;
      if (iVar2 == 0) goto LAB_0000e0a0;
    }
    if ((param_2 != 0) && (piVar7 = *(int **)(param_1 + 8), piVar7 != (int *)0x0)) {
      piVar8 = (int *)0x0;
      do {
        piVar3 = (int *)FUN_0000dfd8(piVar7,1);
        if (piVar3 == (int *)0x0) goto LAB_0000e0a0;
        if (piVar8 != (int *)0x0) {
          *piVar8 = (int)piVar3;
        }
        piVar7 = (int *)*piVar7;
        if (piVar8 != (int *)0x0) {
          piVar3[1] = (int)piVar8;
        }
        else {
          *(int **)(iVar1 + 8) = piVar3;
        }
        piVar8 = piVar3;
      } while (piVar7 != (int *)0x0);
    }
  }
  else {
    iVar2 = FUN_0000c574();
    *(int *)(iVar1 + 0x10) = iVar2;
    if (iVar2 != 0) goto LAB_0000e030;
LAB_0000e0a0:
    FUN_0000d864(iVar1);
    iVar1 = 0;
  }
  return iVar1;
}

