//==================== FUN_0000bb98 @ 0000bb98 ====================

void FUN_0000bb98(undefined4 *param_1)

{
  undefined4 uVar1;
  undefined1 uVar2;
  int iVar3;
  undefined1 uVar4;
  undefined1 uVar5;
  
  if (param_1 == (undefined4 *)0x0) {
    return;
  }
  uVar1 = FUN_0000c6dc(&DAT_00021be0,0);
  *param_1 = uVar1;
  uVar1 = FUN_0000c888(&DAT_00021be0,4);
  param_1[1] = uVar1;
  uVar1 = FUN_0000c888(&DAT_00021be0,0);
  iVar3 = param_1[3];
  param_1[2] = uVar1;
  if (iVar3 - 2U < 4) {
    uVar2 = (&DAT_00015cb6)[iVar3];
    uVar5 = (&DAT_00015cba)[iVar3];
    uVar4 = (&DAT_00015cbe)[iVar3];
  }
  else {
    uVar2 = 0xd;
    uVar4 = 8;
    uVar5 = 0xc;
  }
  uVar1 = FUN_0000c6dc(&DAT_00021be0,uVar2);
  param_1[4] = uVar1;
  uVar1 = FUN_0000c888(&DAT_00021be0,uVar5);
  param_1[5] = uVar1;
  uVar1 = FUN_0000c888(&DAT_00021be0,uVar4);
  param_1[6] = uVar1;
  return;
}

