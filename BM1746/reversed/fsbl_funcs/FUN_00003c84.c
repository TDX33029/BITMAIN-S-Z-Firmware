//==================== FUN_00003c84 @ 00003c84 ====================

int FUN_00003c84(int *param_1)

{
  char cVar1;
  byte bVar2;
  byte bVar3;
  int iVar4;
  int iVar5;
  
  iVar4 = 4;
  iVar5 = *param_1;
  while( true ) {
    if (param_1[6] == 0) {
      if (iVar4 == 0) {
        return 0;
      }
      goto LAB_00003d30;
    }
    if ((*(int *)(iVar5 + 0x2c) != param_1[6]) && (iVar4 = FUN_00002f3c(iVar5), iVar4 != 0))
    goto LAB_00003d30;
    cVar1 = *(char *)param_1[7];
    if (cVar1 == '\0') break;
    bVar2 = ((char *)param_1[7])[0xb];
    bVar3 = bVar2 & 0x3f;
    *(byte *)((int)param_1 + 6) = bVar3;
    if ((bVar3 != 0xf && (cVar1 != -0x1b && cVar1 != '.')) && ((bVar2 & 0x1f) != 8)) {
      return 0;
    }
    iVar4 = FUN_00003b44(param_1);
    if (iVar4 != 0) {
LAB_00003d30:
      param_1[6] = 0;
      return iVar4;
    }
  }
  iVar4 = 4;
  goto LAB_00003d30;
}

