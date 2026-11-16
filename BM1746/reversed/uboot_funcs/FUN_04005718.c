//==================== FUN_04005718 @ 04005718 ====================

bool FUN_04005718(undefined2 *param_1,char *param_2)

{
  char cVar1;
  int iVar2;
  int iVar3;
  char *local_c;
  
  *(undefined4 *)(param_1 + 0x106) = 0;
  *(undefined4 *)(param_1 + 0x100) = 0;
  *(undefined4 *)(param_1 + 0x108) = 0;
  *(undefined4 *)(param_1 + 0x104) = 0;
  *(undefined4 *)(param_1 + 0x102) = 0;
  cVar1 = *param_2;
  *(undefined4 *)(param_1 + 0x100) = 2;
  if (cVar1 == '^') {
    *(undefined4 *)(param_1 + 0x106) = 1;
  }
  *param_1 = 6;
  local_c = param_2;
  if (*param_2 == '\0') {
    if (*(char *)(param_1 + 1) != '\x01') {
      iVar3 = 4;
      param_1[1] = 7;
      goto LAB_040057e8;
    }
LAB_0400580c:
    FUN_04005170(param_1,4);
  }
  else {
    do {
      FUN_040051b8(param_1,&local_c);
    } while (*local_c != '\0');
    if (*(char *)(param_1 + 1) == '\x01') goto LAB_0400580c;
  }
  iVar3 = *(int *)(param_1 + 0x100);
  if (iVar3 < 0x100) {
    iVar2 = iVar3 + 1;
    *(int *)(param_1 + 0x100) = iVar2;
    *(undefined1 *)((int)param_1 + iVar3) = 7;
    if (iVar2 != 0x100) {
      iVar3 = iVar3 + 2;
      *(int *)(param_1 + 0x100) = iVar3;
      *(undefined1 *)((int)param_1 + iVar2) = 0;
      if (iVar3 != 0x100) {
LAB_040057e8:
        *(int *)(param_1 + 0x100) = iVar3 + 1;
        *(undefined1 *)((int)param_1 + iVar3) = 0;
        return *(int *)(param_1 + 0x108) == 0;
      }
    }
  }
  *(undefined4 *)(param_1 + 0x108) = DAT_04005838;
  return false;
}

