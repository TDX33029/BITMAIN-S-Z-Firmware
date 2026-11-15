//==================== FUN_00002c30 @ 00002c30 ====================

void FUN_00002c30(int *param_1,undefined4 *param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  char cVar5;
  int iVar6;
  char *pcVar7;
  
  iVar6 = *param_1;
  iVar3 = 0;
  iVar4 = 0;
  pcVar7 = (char *)(iVar6 + -1);
  do {
    pcVar7 = pcVar7 + 1;
    cVar5 = *pcVar7;
    iVar4 = iVar4 + 1;
    if (cVar5 != ' ') {
      if (cVar5 == '\x05') {
        cVar5 = -0x1b;
      }
      iVar2 = iVar3 + 1;
      iVar1 = iVar3;
      if (iVar4 == 9) {
        *(undefined1 *)((int)param_2 + iVar3 + 9) = 0x2e;
        iVar2 = iVar3 + 2;
        iVar1 = iVar3 + 1;
      }
      iVar3 = iVar2;
      *(char *)((int)param_2 + iVar1 + 9) = cVar5;
    }
  } while (iVar4 != 0xb);
  *(undefined1 *)((int)param_2 + iVar3 + 9) = 0;
  *(undefined1 *)(param_2 + 2) = *(undefined1 *)(iVar6 + 0xb);
  *param_2 = *(undefined4 *)(iVar6 + 0x1c);
  *(undefined2 *)((int)param_2 + 6) = *(undefined2 *)(iVar6 + 0x16);
  *(undefined2 *)(param_2 + 1) = *(undefined2 *)(iVar6 + 0x18);
  return;
}

