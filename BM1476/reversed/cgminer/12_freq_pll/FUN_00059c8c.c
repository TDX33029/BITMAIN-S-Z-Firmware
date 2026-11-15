//==================== FUN_00059c8c @ 00059c8c ====================

undefined4 FUN_00059c8c(char *param_1,size_t param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  if (param_2 == 0xffffffff) {
    param_2 = strlen(param_1);
  }
  if (0 < (int)param_2) {
    iVar3 = 0;
    do {
      iVar2 = FUN_00059ac4(param_1[iVar3]);
      if (iVar2 == 0) {
        return 0;
      }
      if (1 < iVar2) {
        if ((int)param_2 < iVar3 + iVar2) {
          return 0;
        }
        iVar1 = FUN_00059b40(param_1 + iVar3,iVar2,0);
        iVar3 = iVar3 + iVar2 + -1;
        if (iVar1 == 0) {
          return 0;
        }
      }
      iVar3 = iVar3 + 1;
    } while (iVar3 < (int)param_2);
  }
  return 1;
}

