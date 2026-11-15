//==================== FUN_0400339c @ 0400339c ====================

void FUN_0400339c(int *param_1)

{
  int *piVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  uint uVar5;
  int iVar6;
  
  iVar2 = *param_1;
  uVar3 = param_1[1];
  if (uVar3 != 0) {
    iVar4 = 0x14;
    uVar5 = 1;
    do {
      piVar1 = (int *)(iVar2 + iVar4);
      iVar6 = iVar2 + iVar4;
      uVar5 = uVar5 + 1;
      iVar4 = iVar4 + 0x14;
      if (0 < *piVar1) {
        func_0x03fc8570(*(undefined4 *)(iVar6 + 4));
        func_0x03fc8570(*(undefined4 *)(iVar6 + 8));
        iVar2 = *param_1;
        uVar3 = param_1[1];
      }
    } while (uVar5 <= uVar3);
  }
  func_0x03fc8570();
  *param_1 = 0;
  return;
}

