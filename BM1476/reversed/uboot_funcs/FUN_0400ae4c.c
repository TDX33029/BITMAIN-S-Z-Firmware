//==================== FUN_0400ae4c @ 0400ae4c ====================

void FUN_0400ae4c(undefined4 param_1)

{
  uint uVar1;
  uint *puVar2;
  uint uVar3;
  uint *puVar4;
  uint uVar5;
  undefined1 auStack_1018 [4096];
  
  uVar1 = FUN_04005b98();
  puVar2 = (uint *)FUN_0400ac9c(auStack_1018);
  puVar4 = puVar2 + 1;
  *puVar2 = uVar1 << 0x18 | (uVar1 >> 8 & 0xff) << 0x10 | (uVar1 >> 0x10 & 0xff) << 8 |
            uVar1 >> 0x18;
  uVar5 = uVar1 + 3;
  if ((uVar1 & 3) != 0) {
    if ((int)uVar1 < 0) {
      uVar3 = uVar5;
    }
    else {
      uVar3 = uVar1;
    }
    puVar4[(int)uVar3 >> 2] = 0;
  }
  FUN_04005ea4(puVar4,param_1,uVar1);
  if ((int)uVar5 < 0) {
    uVar5 = uVar1 + 6;
  }
  FUN_0400ad48(DAT_0400aee0,1,auStack_1018,
               (int)((int)puVar4 + ((uVar5 & 0xfffffffc) - (int)auStack_1018)) >> 2);
  return;
}

