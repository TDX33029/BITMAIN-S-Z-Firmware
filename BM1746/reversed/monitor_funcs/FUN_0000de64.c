//==================== FUN_0000de64 @ 0000de64 ====================

int FUN_0000de64(int param_1,int param_2)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  int *piVar4;
  undefined4 *puVar5;
  bool bVar6;
  uint in_fpscr;
  undefined8 uVar7;
  
  iVar1 = FUN_0000de34();
  iVar3 = 0;
  if (iVar1 != 0) {
    iVar3 = param_2;
  }
  if ((iVar1 != 0 && param_2 != 0) && -1 < iVar3) {
    puVar5 = (undefined4 *)(param_1 + -4);
    iVar3 = 0;
    piVar4 = (int *)0x0;
    do {
      puVar5 = puVar5 + 1;
      uVar7 = VectorSignedToFloat(*puVar5,(byte)(in_fpscr >> 0x16) & 3);
      piVar2 = (int *)FUN_0000ddd4((int)uVar7);
      bVar6 = iVar3 == 0;
      iVar3 = iVar3 + 1;
      if (bVar6) {
        *(int **)(iVar1 + 8) = piVar2;
      }
      else {
        *piVar4 = (int)piVar2;
        piVar2[1] = (int)piVar4;
      }
      piVar4 = piVar2;
    } while (iVar3 < param_2);
  }
  return iVar1;
}

