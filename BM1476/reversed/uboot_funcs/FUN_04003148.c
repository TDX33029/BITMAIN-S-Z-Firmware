//==================== FUN_04003148 @ 04003148 ====================

bool FUN_04003148(uint param_1,undefined4 param_2,undefined4 param_3,int param_4)

{
  int iVar1;
  undefined4 uVar2;
  undefined1 auStack_98 [128];
  
  param_1 = param_1 & 0x1c0;
  if (param_1 == 0x80) {
    iVar1 = FUN_04006044(param_2,param_3);
    return iVar1 != 0;
  }
  if (param_1 == 0x100) {
    iVar1 = *(int *)(param_4 + 0x208);
    uVar2 = FUN_04005b98(param_2);
    iVar1 = FUN_0400583c(param_4,param_2,uVar2,auStack_98 + (0x80 - (iVar1 * 8 + 0x18)));
    return iVar1 != 0;
  }
  if (param_1 == 0x40) {
    iVar1 = FUN_04005a60(param_2,param_3);
    return iVar1 == 0;
  }
  FUN_04007548(DAT_0400320c,param_1);
  return false;
}

