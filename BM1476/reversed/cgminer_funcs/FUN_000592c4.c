//==================== FUN_000592c4 @ 000592c4 ====================

undefined4 FUN_000592c4(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  undefined4 uVar2;
  int local_64;
  undefined4 local_60;
  undefined4 local_5c;
  undefined1 auStack_58 [68];
  
  FUN_0005b014(param_4,"<buffer>");
  if (param_1 == 0) {
    FUN_00058034(param_4,0,"wrong arguments");
    uVar2 = 0;
  }
  else {
    uVar2 = 0;
    local_5c = 0;
    local_64 = param_1;
    local_60 = param_2;
    iVar1 = FUN_00058154(auStack_58,&LAB_00057fb4,&local_64);
    if (iVar1 == 0) {
      uVar2 = FUN_00059154(auStack_58,param_3,param_4);
      FUN_000581a0(auStack_58);
    }
  }
  return uVar2;
}

