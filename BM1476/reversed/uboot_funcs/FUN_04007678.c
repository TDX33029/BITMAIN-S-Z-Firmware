//==================== FUN_04007678 @ 04007678 ====================

bool FUN_04007678(char *param_1,undefined4 *param_2)

{
  bool bVar1;
  undefined4 uVar2;
  char *local_14 [2];
  
  uVar2 = FUN_04007718(param_1,local_14,0x10);
  *param_2 = uVar2;
  bVar1 = false;
  if (*param_1 != '\0') {
    bVar1 = *local_14[0] == '\0';
  }
  return bVar1;
}

