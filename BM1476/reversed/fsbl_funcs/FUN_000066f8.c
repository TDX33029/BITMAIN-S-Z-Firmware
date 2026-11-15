//==================== FUN_000066f8 @ 000066f8 ====================

undefined4 FUN_000066f8(int param_1,int param_2,undefined4 *param_3)

{
  uint uVar1;
  
  uVar1 = FUN_000064b8();
  if ((uVar1 & 1) != 0) {
    return 3;
  }
  if (param_2 == 1) {
    *param_3 = *(undefined4 *)(&DAT_000215c4 + param_1 * 0x220);
    return 0;
  }
  if (param_2 == 0) {
    return 0;
  }
  if (param_2 == 3) {
    *param_3 = 0x80;
    return 0;
  }
  return 4;
}

