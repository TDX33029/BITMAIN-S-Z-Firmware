//==================== FUN_04004a9c @ 04004a9c ====================

void FUN_04004a9c(int param_1,int param_2,int param_3)

{
  int iVar1;
  
  iVar1 = *(int *)(param_1 + 0x200);
  if (iVar1 < 0x100) {
    *(int *)(param_1 + 0x200) = iVar1 + 1;
    *(undefined1 *)(param_1 + iVar1) = 0;
    iVar1 = iVar1 + 1;
  }
  else {
    *(undefined4 *)(param_1 + 0x210) = DAT_04004af0;
  }
  FUN_04005fa0(param_1 + param_3 + param_2,param_1 + param_2,iVar1 - param_2);
  *(int *)(param_1 + 0x200) = *(int *)(param_1 + 0x200) + param_3;
  return;
}

