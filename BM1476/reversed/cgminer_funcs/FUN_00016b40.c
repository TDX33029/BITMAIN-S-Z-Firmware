//==================== FUN_00016b40 @ 00016b40 ====================

void FUN_00016b40(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  
  *(int *)(param_1 + 0x168) = *(int *)(param_1 + 0x168) + -1;
  iVar2 = *(int *)(param_2 + 0x72c);
  if ((iVar2 == 0) && (*(int *)(param_2 + 0x730) == 0)) {
    free((void *)**(undefined4 **)(*(int *)(param_1 + 0x160) + 0x728));
    free(*(void **)(*(int *)(param_1 + 0x160) + 0x728));
    *(undefined4 *)(param_1 + 0x160) = 0;
    return;
  }
  iVar1 = *(int *)(*(int *)(param_1 + 0x160) + 0x728);
  if (*(int *)(iVar1 + 0x10) - *(int *)(iVar1 + 0x14) == param_2) {
    *(int *)(iVar1 + 0x10) = iVar2 + *(int *)(iVar1 + 0x14);
    iVar2 = *(int *)(param_2 + 0x72c);
  }
  if (iVar2 != 0) {
    iVar1 = iVar2 + *(int *)(*(int *)(*(int *)(param_1 + 0x160) + 0x728) + 0x14);
    *(undefined4 *)(iVar1 + 8) = *(undefined4 *)(param_2 + 0x730);
  }
  else {
    iVar1 = *(int *)(param_2 + 0x730);
    *(int *)(param_1 + 0x160) = iVar1;
  }
  if (iVar2 != 0) {
    iVar1 = *(int *)(param_1 + 0x160);
  }
  if (*(int *)(param_2 + 0x730) != 0) {
    *(undefined4 *)(*(int *)(param_2 + 0x730) + *(int *)(*(int *)(iVar1 + 0x728) + 0x14) + 4) =
         *(undefined4 *)(param_2 + 0x72c);
    iVar1 = *(int *)(param_1 + 0x160);
  }
  iVar2 = ((*(int **)(iVar1 + 0x728))[1] - 1U & *(uint *)(param_2 + 0x744)) * 0xc;
  iVar1 = **(int **)(iVar1 + 0x728) + iVar2;
  *(int *)(iVar1 + 4) = *(int *)(iVar1 + 4) + -1;
  iVar1 = **(int **)(*(int *)(param_1 + 0x160) + 0x728);
  if (*(int *)(iVar1 + iVar2) == param_2 + 0x728) {
    *(undefined4 *)(iVar1 + iVar2) = *(undefined4 *)(param_2 + 0x738);
  }
  if (*(int *)(param_2 + 0x734) != 0) {
    *(undefined4 *)(*(int *)(param_2 + 0x734) + 0x10) = *(undefined4 *)(param_2 + 0x738);
  }
  if (*(int *)(param_2 + 0x738) != 0) {
    *(undefined4 *)(*(int *)(param_2 + 0x738) + 0xc) = *(undefined4 *)(param_2 + 0x734);
  }
  iVar2 = *(int *)(*(int *)(param_1 + 0x160) + 0x728);
  *(int *)(iVar2 + 0xc) = *(int *)(iVar2 + 0xc) + -1;
  return;
}

