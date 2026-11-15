//==================== FUN_0400301c @ 0400301c ====================

undefined4 FUN_0400301c(undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  undefined4 uVar2;
  undefined1 auStack_14 [8];
  
  iVar1 = func_0x03ffafa8(param_1,DAT_04003060);
  if (iVar1 < 0) {
    return 0;
  }
  uVar2 = func_0x03ffae84(param_1,iVar1,param_2,auStack_14);
  return uVar2;
}

