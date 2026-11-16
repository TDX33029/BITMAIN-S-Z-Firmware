//==================== FUN_00003868 @ 00003868 ====================

uint FUN_00003868(int *param_1,uint param_2)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  
  iVar5 = *param_1;
  if (param_2 == 0) {
    uVar4 = *(uint *)(iVar5 + 0xc);
    if (uVar4 == 0) {
      uVar4 = 1;
      iVar1 = *(int *)(iVar5 + 0x10);
    }
    else {
      iVar1 = *(int *)(iVar5 + 0x10);
      if (*(uint *)(iVar5 + 0x14) <= uVar4) {
        uVar4 = 1;
      }
    }
    if (iVar1 == 0) {
      return 0;
    }
    uVar2 = *(uint *)(iVar5 + 0x14);
  }
  else {
    uVar3 = FUN_000036fc(iVar5);
    if (uVar3 < 2) {
      return 1;
    }
    if (uVar3 == 0xffffffff) {
      return 0xffffffff;
    }
    if (uVar3 < *(uint *)(iVar5 + 0x14)) {
      return uVar3;
    }
    if (*(int *)(iVar5 + 0x10) == 0) {
      return 0;
    }
    uVar3 = param_2 + 1;
    if (*(uint *)(iVar5 + 0x14) <= uVar3) {
      uVar3 = 2;
    }
    uVar4 = FUN_000036fc(*param_1,uVar3);
    if (uVar4 == 0xffffffff || uVar4 == 1) {
      return uVar4;
    }
    if (uVar4 == 0) {
      if (uVar3 != 0) goto LAB_00003908;
      uVar2 = *(uint *)(iVar5 + 0x14);
      uVar4 = param_2;
    }
    else {
      uVar4 = *(uint *)(iVar5 + 0xc);
      uVar2 = *(uint *)(iVar5 + 0x14);
      if (uVar4 < 2) {
        uVar4 = param_2;
      }
      else if (uVar2 <= uVar4) {
        uVar4 = param_2;
      }
    }
  }
  uVar3 = uVar4;
  while( true ) {
    uVar3 = uVar3 + 1;
    if ((uVar2 <= uVar3) && (uVar3 = 2, uVar4 < 2)) {
      return 0;
    }
    uVar2 = FUN_000036fc(*param_1,uVar3);
    if (uVar2 == 0) break;
    if (uVar2 == 0xffffffff || uVar2 == 1) {
      return uVar2;
    }
    if (uVar3 == uVar4) {
      return 0;
    }
    uVar2 = *(uint *)(iVar5 + 0x14);
  }
LAB_00003908:
  if ((1 < uVar3) && (uVar3 < *(uint *)(iVar5 + 0x14))) {
    iVar1 = FUN_0000351c(iVar5,uVar3,0xffffffff);
    if (iVar1 == 0 && param_2 != 0) {
      if (param_2 < 2) {
        return 1;
      }
      if (*(uint *)(iVar5 + 0x14) <= param_2) {
        return 1;
      }
      iVar1 = FUN_0000351c(iVar5,param_2,uVar3);
    }
    if (iVar1 == 0) {
      *(uint *)(iVar5 + 0xc) = uVar3;
      if (*(uint *)(iVar5 + 0x10) <= *(int *)(iVar5 + 0x14) - 2U) {
        *(uint *)(iVar5 + 0x10) = *(uint *)(iVar5 + 0x10) - 1;
      }
      *(byte *)(iVar5 + 4) = *(byte *)(iVar5 + 4) | 1;
      return uVar3;
    }
    if (iVar1 == 1) {
      return 0xffffffff;
    }
  }
  return 1;
}

