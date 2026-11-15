//==================== FUN_0000df24 @ 0000df24 ====================

int FUN_0000df24(undefined8 *param_1,int param_2)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  int *piVar4;
  bool bVar5;
  undefined8 uVar6;
  
  iVar1 = FUN_0000de34();
  iVar3 = 0;
  if (iVar1 != 0) {
    iVar3 = param_2;
  }
  if ((iVar1 != 0 && param_2 != 0) && -1 < iVar3) {
    iVar3 = 0;
    piVar4 = (int *)0x0;
    do {
      uVar6 = *param_1;
      param_1 = param_1 + 1;
      piVar2 = (int *)FUN_0000ddd4((int)uVar6);
      bVar5 = iVar3 == 0;
      iVar3 = iVar3 + 1;
      if (bVar5) {
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

