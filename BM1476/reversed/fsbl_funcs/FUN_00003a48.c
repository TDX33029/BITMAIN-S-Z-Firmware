//==================== FUN_00003a48 @ 00003a48 ====================

int FUN_00003a48(int *param_1,uint param_2,uint param_3)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  bool bVar4;
  bool bVar5;
  
  if (param_2 < 2) {
    return 2;
  }
  iVar3 = *param_1;
  uVar2 = *(uint *)(iVar3 + 0x14);
  if (param_2 < uVar2) {
    if (param_3 == 0) {
      iVar1 = iVar3;
      uVar2 = param_2;
      goto LAB_00003ae0;
    }
    bVar5 = param_3 <= uVar2;
    bVar4 = uVar2 == param_3;
    if (bVar5 && !bVar4) {
      bVar5 = param_3 != 0;
      bVar4 = param_3 == 1;
    }
    if (bVar5 && !bVar4) {
      iVar1 = FUN_0000351c(iVar3,param_3,0xffffffff);
      if (iVar1 != 0) {
        return iVar1;
      }
      do {
        iVar1 = *param_1;
        uVar2 = param_2;
LAB_00003ae0:
        param_2 = FUN_000036fc(iVar1,uVar2);
        if (param_2 == 0) {
          return 0;
        }
        if (param_2 == 1) {
          return 2;
        }
        if (param_2 == 0xffffffff) {
          return 1;
        }
        if (uVar2 < 2) {
          return 2;
        }
        if (*(uint *)(iVar3 + 0x14) <= uVar2) {
          return 2;
        }
        iVar1 = FUN_0000351c(iVar3,uVar2,0);
        if (iVar1 != 0) {
          return iVar1;
        }
        if (*(uint *)(iVar3 + 0x10) < *(uint *)(iVar3 + 0x14) - 2) {
          *(uint *)(iVar3 + 0x10) = *(uint *)(iVar3 + 0x10) + 1;
          *(byte *)(iVar3 + 4) = *(byte *)(iVar3 + 4) | 1;
        }
      } while (param_2 < *(uint *)(iVar3 + 0x14));
      return 0;
    }
  }
  return 2;
}

