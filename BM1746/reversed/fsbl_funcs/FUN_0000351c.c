//==================== FUN_0000351c @ 0000351c ====================

int FUN_0000351c(char *param_1,uint param_2,uint param_3)

{
  char cVar1;
  undefined2 uVar2;
  int iVar3;
  uint uVar4;
  byte *pbVar5;
  byte bVar6;
  uint uVar7;
  
  cVar1 = *param_1;
  bVar6 = (byte)(param_3 >> 8);
  if (cVar1 == '\x02') {
    if ((*(int *)(param_1 + 0x20) + (param_2 >> 8) == *(int *)(param_1 + 0x2c)) ||
       (iVar3 = FUN_00002f3c(), iVar3 == 0)) {
      iVar3 = (param_2 & 0xff) * 2;
      param_1[iVar3 + 0x40] = (char)param_3;
      param_1[iVar3 + 0x41] = bVar6;
      param_1[3] = '\x01';
      return 0;
    }
    return iVar3;
  }
  if (cVar1 == '\x03') {
    if ((*(int *)(param_1 + 0x20) + (param_2 >> 7) == *(int *)(param_1 + 0x2c)) ||
       (iVar3 = FUN_00002f3c(), iVar3 == 0)) {
      param_3 = param_3 & 0xfffffff;
      iVar3 = (param_2 & 0x7f) * 4;
      uVar2 = *(undefined2 *)(param_1 + iVar3 + 0x42);
      param_1[iVar3 + 0x40] = (char)param_3;
      param_1[iVar3 + 0x41] = (char)(param_3 >> 8);
      param_1[iVar3 + 0x42] = (char)(param_3 >> 0x10);
      param_1[iVar3 + 0x43] = (byte)((ushort)uVar2 >> 8) & 0xf0 | (byte)(param_3 >> 0x18);
      param_1[3] = '\x01';
      return 0;
    }
    return iVar3;
  }
  if (cVar1 != '\x01') {
    return 2;
  }
  uVar7 = param_2 + (param_2 >> 1);
  if ((*(int *)(param_1 + 0x20) + (uVar7 >> 9) != *(int *)(param_1 + 0x2c)) &&
     (iVar3 = FUN_00002f3c(), iVar3 != 0)) {
    return iVar3;
  }
  uVar4 = uVar7 + 1;
  uVar7 = uVar7 & 0x1ff;
  if ((param_2 & 1) == 0) {
    param_1[uVar7 + 0x40] = (char)param_3;
    param_1[3] = '\x01';
    if (((uVar4 >> 9) + *(int *)(param_1 + 0x20) != *(int *)(param_1 + 0x2c)) &&
       (iVar3 = FUN_00002f3c(param_1), iVar3 != 0)) {
      return iVar3;
    }
    pbVar5 = (byte *)(param_1 + (uVar4 & 0x1ff) + 0x40);
    bVar6 = *pbVar5 & 0xf0 | bVar6 & 0xf;
  }
  else {
    param_1[uVar7 + 0x40] = param_1[uVar7 + 0x40] & 0xfU | (byte)(param_3 << 4);
    param_1[3] = '\x01';
    if (((uVar4 >> 9) + *(int *)(param_1 + 0x20) != *(int *)(param_1 + 0x2c)) &&
       (iVar3 = FUN_00002f3c(param_1), iVar3 != 0)) {
      return iVar3;
    }
    bVar6 = (byte)((param_3 & 0xfff) >> 4);
    pbVar5 = (byte *)(param_1 + (uVar4 & 0x1ff) + 0x40);
  }
  *pbVar5 = bVar6;
  param_1[3] = '\x01';
  return 0;
}

