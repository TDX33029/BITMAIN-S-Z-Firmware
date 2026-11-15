//==================== FUN_0400b900 @ 0400b900 ====================

void FUN_0400b900(int param_1,int param_2,int param_3)

{
  undefined2 uVar1;
  int iVar2;
  int local_24;
  int iStack_20;
  
  local_24 = param_2;
  iStack_20 = param_3;
  if (*(char *)(param_2 + 0x14) == '\0') {
    FUN_04005ea4(&local_24,param_2 + 0xc,4,0,param_1);
    if (*DAT_0400b9d4 == local_24) {
      *DAT_0400b9d8 = 2;
    }
  }
  else if (*(char *)(param_2 + 0x14) == '\b') {
    iVar2 = FUN_0400a4b8(param_1,param_1 + 6,0x800);
    *(undefined2 *)(param_2 + 10) = 0;
    *(undefined2 *)(param_2 + 6) = 0;
    FUN_04005ea4(param_2 + 0x10,param_2 + 0xc,4);
    FUN_04005ea4(param_2 + 0xc,DAT_0400b9dc,4);
    uVar1 = FUN_04007c5c(param_2,0x14);
    *(undefined2 *)(param_2 + 10) = uVar1;
    *(undefined1 *)(param_2 + 0x14) = 0;
    *(undefined2 *)(param_2 + 0x16) = 0;
    uVar1 = FUN_04007c5c(param_2 + 0x14,param_3 + -0x14);
    *(undefined2 *)(param_2 + 0x16) = uVar1;
    FUN_0400929c(param_1,iVar2 + param_3);
  }
  return;
}

