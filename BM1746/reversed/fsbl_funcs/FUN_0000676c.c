//==================== FUN_0000676c @ 0000676c ====================

bool FUN_0000676c(int param_1,undefined4 param_2,int param_3,int param_4)

{
  uint uVar1;
  int iVar2;
  
  uVar1 = FUN_000064b8();
  if ((uVar1 & 1) != 0) {
    return (bool)3;
  }
  if (param_4 == 0) {
    return (bool)4;
  }
  if (*(int *)(&DAT_00021590 + param_1 * 0x220) == 0) {
    param_3 = param_3 << 9;
  }
  iVar2 = FUN_00007430(param_1 * 0x220 + 0x21560,param_3,param_4,param_2);
  return iVar2 != 0;
}

