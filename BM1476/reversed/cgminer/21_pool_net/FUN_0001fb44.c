//==================== FUN_0001fb44 @ 0001fb44 ====================

void FUN_0001fb44(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  undefined4 extraout_r2;
  undefined8 uVar2;
  
  iVar1 = FUN_00018834(param_1,param_1 + 0x81);
  if (iVar1 != 0) {
    return;
  }
  FUN_0002a84c(param_1 + 0xa8);
  uVar2 = FUN_0001da58();
  if (param_1 == (int)uVar2) {
    FUN_00019bf8(0,(int)((ulonglong)uVar2 >> 0x20),extraout_r2,param_4);
    return;
  }
  return;
}

