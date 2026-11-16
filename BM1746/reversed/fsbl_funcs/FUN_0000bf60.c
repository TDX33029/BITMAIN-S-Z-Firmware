//==================== FUN_0000bf60 @ 0000bf60 ====================

bool FUN_0000bf60(undefined1 *param_1)

{
  bool bVar1;
  int iVar2;
  undefined4 local_20;
  undefined4 uStack_1c;
  undefined4 uStack_18;
  undefined4 uStack_14;
  undefined4 uStack_10;
  undefined2 uStack_c;
  undefined1 local_a;
  
  local_20 = DAT_00017274;
  uStack_1c = DAT_00017278;
  uStack_18 = DAT_0001727c;
  uStack_14 = DAT_00017280;
  uStack_10 = DAT_00017284;
  local_a = (undefined1)((uint)DAT_00017288 >> 0x10);
  uStack_c = (undefined2)DAT_00017288;
  if (param_1 == (undefined1 *)0x0) {
    bVar1 = true;
  }
  else {
    iVar2 = FUN_0000dfb0(&local_20,*param_1);
    bVar1 = iVar2 == 0;
  }
  return bVar1;
}

