//==================== FUN_00033488 @ 00033488 ====================

void FUN_00033488(int param_1,undefined4 param_2,undefined4 param_3,int param_4)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 local_14;
  
  local_14 = 0;
  FUN_0003305c(param_1,0x68,0,0,param_4);
  if (param_4 == 0) {
    FUN_000307c8(param_1,"ASCS,");
    uVar2 = FUN_00032ed0(0,"Count",&local_14,0);
    FUN_00032534(param_1,uVar2,0,0);
  }
  else {
    iVar1 = FUN_000307c8(param_1,",\"ASCS\":[");
    uVar2 = FUN_00032ed0(0,"Count",&local_14,0);
    FUN_00032534(param_1,uVar2,1,0);
    if (iVar1 != 0) {
      *(undefined1 *)(param_1 + 0xd) = 1;
    }
  }
  return;
}

