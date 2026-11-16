//==================== FUN_040095ac @ 040095ac ====================

undefined4 FUN_040095ac(undefined4 param_1,byte *param_2)

{
  byte bVar1;
  undefined4 uVar2;
  
  uVar2 = func_0x03fbefb0();
  uVar2 = FUN_04009550(uVar2,param_2);
  bVar1 = *param_2;
  if ((bVar1 & 1) != 0) {
    return 0;
  }
  if (((((bVar1 != 0 || param_2[1] != 0) || param_2[2] != 0) || param_2[3] != 0) || param_2[4] != 0)
      || param_2[5] != 0) {
    uVar2 = 1;
  }
  if (((((bVar1 == 0 && param_2[1] == 0) && param_2[2] == 0) && param_2[3] == 0) && param_2[4] == 0)
      && param_2[5] == 0) {
    uVar2 = 0;
  }
  return uVar2;
}

