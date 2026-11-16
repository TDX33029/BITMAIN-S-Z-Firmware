//==================== FUN_04007548 @ 04007548 ====================

uint FUN_04007548(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  uint uVar1;
  undefined1 auStack_830 [2072];
  undefined4 uStack_c;
  undefined4 uStack_8;
  undefined4 uStack_4;
  
  uStack_c = param_2;
  uStack_8 = param_3;
  uStack_4 = param_4;
  uVar1 = FUN_040069f0(auStack_830,0x817,param_1,&uStack_c);
  if (0x815 < uVar1) {
    uVar1 = 0x816;
  }
  func_0x03fc7fbc(auStack_830);
  return uVar1;
}

