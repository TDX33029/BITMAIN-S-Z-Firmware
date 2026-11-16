//==================== FUN_000036fc @ 000036fc ====================

uint FUN_000036fc(char *param_1,uint param_2)

{
  char cVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  
  if (param_2 < 2) {
    return 1;
  }
  if (*(uint *)(param_1 + 0x14) <= param_2) {
    return 1;
  }
  cVar1 = *param_1;
  if (cVar1 == '\x02') {
    if ((*(int *)(param_1 + 0x20) + (param_2 >> 8) == *(int *)(param_1 + 0x2c)) ||
       (iVar2 = FUN_00002f3c(), iVar2 == 0)) {
      iVar2 = (param_2 & 0xff) * 2;
      return (uint)CONCAT11(param_1[iVar2 + 0x41],param_1[iVar2 + 0x40]);
    }
  }
  else if (cVar1 == '\x03') {
    if ((*(int *)(param_1 + 0x20) + (param_2 >> 7) == *(int *)(param_1 + 0x2c)) ||
       (iVar2 = FUN_00002f3c(), iVar2 == 0)) {
      iVar2 = (param_2 & 0x7f) * 4;
      return CONCAT31(*(undefined3 *)(param_1 + iVar2 + 0x41),param_1[iVar2 + 0x40]) & 0xfffffff;
    }
  }
  else {
    if (cVar1 != '\x01') {
      return 1;
    }
    iVar2 = *(int *)(param_1 + 0x20);
    uVar4 = param_2 + (param_2 >> 1);
    iVar3 = *(int *)(param_1 + 0x2c);
    if (iVar2 + (uVar4 >> 9) != iVar3) {
      iVar2 = FUN_00002f3c();
      if (iVar2 != 0) {
        return 0xffffffff;
      }
      iVar2 = *(int *)(param_1 + 0x20);
      iVar3 = *(int *)(param_1 + 0x2c);
    }
    cVar1 = param_1[(uVar4 & 0x1ff) + 0x40];
    if ((iVar2 + (uVar4 + 1 >> 9) == iVar3) || (iVar2 = FUN_00002f3c(param_1), iVar2 == 0)) {
      if ((param_2 & 1) == 0) {
        return CONCAT11(param_1[(uVar4 + 1 & 0x1ff) + 0x40],cVar1) & 0xfff;
      }
      return (uint)(ushort)(CONCAT11(param_1[(uVar4 + 1 & 0x1ff) + 0x40],cVar1) >> 4);
    }
  }
  return 0xffffffff;
}

