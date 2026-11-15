//==================== FUN_000053a8 @ 000053a8 ====================

uint FUN_000053a8(undefined4 *param_1)

{
  uint uVar1;
  int iVar2;
  byte bVar3;
  int local_14 [2];
  
  uVar1 = FUN_00002bbc(param_1,local_14);
  if (uVar1 != 0) {
    return uVar1;
  }
  if (*(byte *)((int)param_1 + 0x11) != 0) {
    return (uint)*(byte *)((int)param_1 + 0x11);
  }
  if ((*(byte *)(param_1 + 4) & 2) == 0) {
    return 7;
  }
  if ((uint)param_1[3] <= (uint)param_1[5]) {
    return 0;
  }
  if (param_1[5] == 0) {
    uVar1 = FUN_00003a48(param_1,param_1[2],0);
    param_1[2] = 0;
LAB_0000540c:
    bVar3 = *(byte *)(param_1 + 4) | 0x40;
    *(byte *)(param_1 + 4) = bVar3;
    param_1[3] = param_1[5];
    if (uVar1 != 0) goto LAB_00005484;
LAB_00005428:
    if ((bVar3 & 0x80) == 0) {
      return 0;
    }
    iVar2 = FUN_0000676c(*(undefined1 *)(local_14[0] + 1),param_1 + 0x10,param_1[7],1);
    if (iVar2 == 0) {
      *(byte *)(param_1 + 4) = *(byte *)(param_1 + 4) & 0x7f;
      return 0;
    }
    uVar1 = 1;
  }
  else {
    uVar1 = FUN_000036fc(*param_1,param_1[6]);
    if (uVar1 == 0xffffffff) {
      uVar1 = 1;
    }
    else {
      if (uVar1 != 1) {
        if (uVar1 < *(uint *)(local_14[0] + 0x14)) {
          uVar1 = FUN_00003a48(param_1,uVar1,param_1[6]);
          goto LAB_0000540c;
        }
        bVar3 = *(byte *)(param_1 + 4) | 0x40;
        *(byte *)(param_1 + 4) = bVar3;
        param_1[3] = param_1[5];
        goto LAB_00005428;
      }
      uVar1 = 2;
    }
    *(byte *)(param_1 + 4) = *(byte *)(param_1 + 4) | 0x40;
    param_1[3] = param_1[5];
  }
LAB_00005484:
  *(char *)((int)param_1 + 0x11) = (char)uVar1;
  return uVar1;
}

