//==================== FUN_04005170 @ 04005170 ====================

void FUN_04005170(int param_1,int param_2)

{
  int iVar1;
  
  iVar1 = *(int *)(param_1 + 0x200);
  if (iVar1 < 0x100) {
    *(int *)(param_1 + 0x200) = iVar1 + 1;
    *(undefined1 *)(param_1 + iVar1) = 0;
    iVar1 = iVar1 + 1;
  }
  else {
    *(undefined4 *)(param_1 + 0x210) = DAT_040051b0;
  }
  iVar1 = iVar1 - (param_2 + -2);
  if (iVar1 < 0x100) {
    *(char *)(param_1 + param_2) = (char)iVar1;
  }
  else {
    *(undefined4 *)(param_1 + 0x210) = DAT_040051b4;
  }
  return;
}

