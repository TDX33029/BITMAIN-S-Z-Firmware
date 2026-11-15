//==================== FUN_00059230 @ 00059230 ====================

undefined4 FUN_00059230(int param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  undefined4 uVar2;
  int local_58 [2];
  undefined1 auStack_50 [64];
  
  FUN_0005b014(param_3,"<string>");
  if (param_1 == 0) {
    FUN_00058034(param_3,0,"wrong arguments");
    uVar2 = 0;
  }
  else {
    uVar2 = 0;
    local_58[1] = 0;
    local_58[0] = param_1;
    iVar1 = FUN_00058154(auStack_50,&LAB_00057f94,local_58);
    if (iVar1 == 0) {
      uVar2 = FUN_00059154(auStack_50,param_2,param_3);
      FUN_000581a0(auStack_50);
    }
  }
  return uVar2;
}

