//==================== FUN_0400ad48 @ 0400ad48 ====================

void FUN_0400ad48(int param_1,int param_2,undefined4 param_3,int param_4)

{
  int iVar1;
  uint uVar2;
  undefined4 uVar3;
  undefined4 *puVar4;
  int iVar5;
  int iVar6;
  uint local_810 [4];
  undefined4 local_800;
  int local_7fc;
  undefined1 auStack_7f8 [2024];
  
  local_7fc = param_2 << 0x18;
  uVar2 = *DAT_0400ae28 + 1;
  *DAT_0400ae28 = uVar2;
  local_810[0] = uVar2 * 0x1000000 | (uVar2 >> 8 & 0xff) << 0x10 | (uVar2 >> 0x10 & 0xff) << 8 |
                 uVar2 >> 0x18;
  local_810[1] = 0;
  local_810[2] = 0x2000000;
  local_800 = 0x2000000;
  local_810[3] = DAT_0400ae2c | param_1 << 0x18;
  if (param_4 != 0) {
    FUN_04005ea4(auStack_7f8,param_3,param_4 * 4);
  }
  iVar5 = param_4 * 4 + 0x18;
  iVar6 = *DAT_0400ae30;
  iVar1 = FUN_0400a3e0();
  FUN_04005ea4(iVar6 + iVar1 + 0x1c,local_810,iVar5);
  if (param_1 == DAT_0400ae34) {
    uVar3 = 0x6f;
  }
  else {
    if (param_1 == DAT_0400ae34 + 5) {
      puVar4 = DAT_0400ae38;
    }
    else {
      puVar4 = DAT_0400ae3c;
    }
    uVar3 = *puVar4;
  }
  FUN_0400a644(DAT_0400ae48,*DAT_0400ae44,uVar3,*DAT_0400ae40,iVar5);
  return;
}

