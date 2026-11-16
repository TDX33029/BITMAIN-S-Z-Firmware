//==================== FUN_0400ac9c @ 0400ac9c ====================

void FUN_0400ac9c(undefined4 *param_1)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  undefined4 *puVar5;
  undefined1 auStack_110 [256];
  
  puVar5 = param_1 + 4;
  FUN_04005994(auStack_110,DAT_0400ad44);
  uVar2 = FUN_04005b98(auStack_110);
  uVar4 = uVar2 + 3;
  *param_1 = 0x1000000;
  uVar3 = (uVar4 & 0xfffffffc) + 0x14;
  param_1[1] = uVar3 * 0x1000000 | (uVar3 >> 8 & 0xff) << 0x10 | (uVar3 >> 0x10 & 0xff) << 8 |
               uVar3 >> 0x18;
  param_1[2] = 0;
  param_1[3] = uVar2 << 0x18 | (uVar2 >> 8 & 0xff) << 0x10 | (uVar2 >> 0x10 & 0xff) << 8 |
               uVar2 >> 0x18;
  if ((uVar2 & 3) != 0) {
    if ((int)uVar2 < 0) {
      uVar3 = uVar4;
    }
    else {
      uVar3 = uVar2;
    }
    puVar5[(int)uVar3 >> 2] = 0;
  }
  iVar1 = (int)uVar4 >> 2;
  FUN_04005ea4(puVar5,auStack_110,uVar2);
  puVar5[iVar1] = 0;
  puVar5[iVar1 + 1] = 0;
  puVar5[iVar1 + 2] = 0;
  puVar5[iVar1 + 3] = 0;
  puVar5[iVar1 + 4] = 0;
  return;
}

