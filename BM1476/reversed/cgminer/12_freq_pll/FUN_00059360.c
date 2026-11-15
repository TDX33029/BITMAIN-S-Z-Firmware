//==================== FUN_00059360 @ 00059360 ====================

undefined4 FUN_00059360(int param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  char *pcVar2;
  undefined4 uVar3;
  undefined1 auStack_50 [64];
  
  pcVar2 = "<stdin>";
  if (param_1 != stdin) {
    pcVar2 = "<stream>";
  }
  FUN_0005b014(param_3,pcVar2);
  if (param_1 == 0) {
    FUN_00058034(param_3,0,"wrong arguments");
    uVar3 = 0;
  }
  else {
    iVar1 = FUN_00058154(auStack_50,fgetc,param_1);
    if (iVar1 == 0) {
      uVar3 = FUN_00059154(auStack_50,param_2,param_3);
      FUN_000581a0(auStack_50);
    }
    else {
      uVar3 = 0;
    }
  }
  return uVar3;
}

