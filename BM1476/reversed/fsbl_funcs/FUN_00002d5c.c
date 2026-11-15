//==================== FUN_00002d5c @ 00002d5c ====================

bool FUN_00002d5c(int param_1,int param_2)

{
  int iVar1;
  uint uVar2;
  undefined1 *puVar4;
  undefined1 *puVar3;
  uint uVar5;
  int iVar6;
  
  if ((*(char *)(param_1 + 3) != '\0') && (iVar6 = FUN_00002ce0(), iVar6 != 0)) {
    return true;
  }
  if (param_2 - 2U < *(int *)(param_1 + 0x14) - 2U) {
    iVar6 = (param_2 - 2U) * (uint)*(ushort *)(param_1 + 10) + *(int *)(param_1 + 0x28);
  }
  else {
    iVar6 = 0;
  }
  *(int *)(param_1 + 0x2c) = iVar6;
  puVar4 = (undefined1 *)(param_1 + 0x40);
  do {
    puVar3 = puVar4 + 1;
    *puVar4 = 0;
    puVar4 = puVar3;
  } while (puVar3 != (undefined1 *)(param_1 + 0x240));
  if (*(short *)(param_1 + 10) == 0) {
    uVar2 = 0;
    uVar5 = 0;
  }
  else {
    uVar5 = 0;
    do {
      iVar1 = FUN_0000676c(*(undefined1 *)(param_1 + 1),(undefined1 *)(param_1 + 0x40),iVar6 + uVar5
                           ,1);
      uVar2 = (uint)*(ushort *)(param_1 + 10);
      if (iVar1 != 0) break;
      uVar5 = uVar5 + 1;
    } while (uVar5 < uVar2);
  }
  return uVar5 != uVar2;
}

