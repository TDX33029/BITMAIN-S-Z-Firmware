//==================== FUN_000594bc @ 000594bc ====================

undefined4 FUN_000594bc(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  undefined4 uVar2;
  undefined1 auStack_468 [64];
  undefined1 auStack_428 [1032];
  int local_20;
  undefined4 local_1c;
  
  memset(auStack_428,0,0x410);
  local_20 = param_1;
  local_1c = param_2;
  FUN_0005b014(param_4,"<callback>");
  if (param_1 == 0) {
    FUN_00058034(param_4,0,"wrong arguments");
    uVar2 = 0;
  }
  else {
    iVar1 = FUN_00058154(auStack_468,&LAB_00057fd8,auStack_428);
    if (iVar1 == 0) {
      uVar2 = FUN_00059154(auStack_468,param_3,param_4);
      FUN_000581a0(auStack_468);
    }
    else {
      uVar2 = 0;
    }
  }
  return uVar2;
}

