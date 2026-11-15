//==================== FUN_0000dec8 @ 0000dec8 ====================

int FUN_0000dec8(float *param_1,int param_2)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  int *piVar4;
  bool bVar5;
  float fVar6;
  
  iVar1 = FUN_0000de34();
  iVar3 = 0;
  if (iVar1 != 0) {
    iVar3 = param_2;
  }
  if ((iVar1 != 0 && param_2 != 0) && -1 < iVar3) {
    iVar3 = 0;
    piVar4 = (int *)0x0;
    do {
      fVar6 = *param_1;
      param_1 = param_1 + 1;
      piVar2 = (int *)FUN_0000ddd4(SUB84((double)fVar6,0));
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

