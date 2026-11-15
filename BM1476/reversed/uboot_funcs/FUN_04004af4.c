//==================== FUN_04004af4 @ 04004af4 ====================

void FUN_04004af4(int param_1,int param_2,undefined1 param_3)

{
  char cVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  if (*(char *)(param_1 + param_2) != '\x03') goto LAB_04004b10;
  iVar5 = param_2 + 2;
  if (*(byte *)(param_1 + iVar5) < 2) goto LAB_04004b10;
  *(byte *)(param_1 + iVar5) = *(byte *)(param_1 + iVar5) - 1;
  iVar4 = *(int *)(param_1 + 0x200);
  if (iVar4 < 0x100) {
    iVar3 = iVar4 + 1;
    *(int *)(param_1 + 0x200) = iVar3;
    *(undefined1 *)(param_1 + iVar4) = 3;
    if (iVar3 == 0x100) {
      iVar4 = 0x100;
    }
    cVar1 = *(char *)(param_1 + param_2 + 1);
    cVar2 = *(char *)(param_1 + iVar5);
    if (iVar3 == 0x100) goto LAB_04004bc0;
    iVar5 = iVar4 + 2;
    *(int *)(param_1 + 0x200) = iVar5;
    if (iVar5 != 0x100) {
      iVar4 = iVar4 + 3;
    }
    *(char *)(param_1 + iVar3) = cVar1 + cVar2;
    if (iVar5 == 0x100) {
      iVar4 = 0x100;
      goto LAB_04004bc0;
    }
    *(int *)(param_1 + 0x200) = iVar4;
    *(undefined1 *)(param_1 + iVar5) = 1;
  }
  else {
LAB_04004bc0:
    *(undefined4 *)(param_1 + 0x210) = DAT_04004bd0;
  }
  param_2 = iVar4 + -3;
LAB_04004b10:
  FUN_04004a9c(param_1,param_2,2);
  *(undefined1 *)(param_1 + param_2) = param_3;
  iVar5 = *(int *)(param_1 + 0x200) - param_2;
  if (iVar5 < 0x100) {
    *(char *)(param_1 + param_2 + 1) = (char)iVar5;
  }
  else {
    *(undefined4 *)(param_1 + 0x210) = DAT_04004bcc;
  }
  return;
}

