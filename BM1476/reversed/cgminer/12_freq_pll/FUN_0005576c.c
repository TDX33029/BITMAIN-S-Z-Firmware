//==================== FUN_0005576c @ 0005576c ====================

void FUN_0005576c(int param_1,undefined1 *param_2,int param_3)

{
  undefined1 *puVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  
  uVar4 = *(uint *)(param_1 + 0x20);
  iVar5 = *(int *)(param_1 + 0x24);
  uVar3 = uVar4 & 0x3f;
  if (param_3 == 0) {
    return;
  }
  puVar1 = param_2;
  while( true ) {
    iVar2 = param_1 + uVar3;
    uVar3 = uVar3 + 1;
    *(undefined1 *)(iVar2 + 0x28) = *puVar1;
    *(uint *)(param_1 + 0x20) = uVar4 + 1;
    *(uint *)(param_1 + 0x24) = iVar5 + (uint)(0xfffffffe < uVar4);
    if (uVar3 == 0x40) {
      FUN_00055470(param_1);
      uVar3 = 0;
    }
    if (puVar1 + 1 == param_2 + param_3) break;
    uVar4 = *(uint *)(param_1 + 0x20);
    iVar5 = *(int *)(param_1 + 0x24);
    puVar1 = puVar1 + 1;
  }
  return;
}

