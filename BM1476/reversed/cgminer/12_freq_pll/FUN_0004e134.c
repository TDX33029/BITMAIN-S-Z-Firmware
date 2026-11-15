//==================== FUN_0004e134 @ 0004e134 ====================

void FUN_0004e134(uint *param_1,int param_2)

{
  bool bVar1;
  int iVar2;
  uint *puVar3;
  uint uVar4;
  uint uVar5;
  
  puVar3 = param_1 + param_2;
  if (param_2 != 0) {
    iVar2 = 0;
    bVar1 = false;
    do {
      uVar5 = *param_1;
      uVar4 = *puVar3;
      if ((bVar1) || (uVar4 < uVar5)) {
        *param_1 = uVar4;
        bVar1 = true;
        *puVar3 = uVar5;
      }
      else if (uVar5 < uVar4) {
        return;
      }
      iVar2 = iVar2 + 1;
      param_1 = param_1 + 1;
      puVar3 = puVar3 + 1;
    } while (iVar2 != param_2);
  }
  return;
}

