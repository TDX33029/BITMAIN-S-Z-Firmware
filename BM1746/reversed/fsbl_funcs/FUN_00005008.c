//==================== FUN_00005008 @ 00005008 ====================

int FUN_00005008(undefined4 *param_1,undefined4 param_2)

{
  ushort uVar1;
  int iVar2;
  uint uVar3;
  undefined4 local_14 [2];
  char *local_c;
  
  if (param_1 == (undefined4 *)0x0) {
    return 9;
  }
  local_14[0] = param_2;
  iVar2 = FUN_000030bc(local_14,&local_c,0);
  if (iVar2 == 0) {
    *param_1 = local_c;
    iVar2 = FUN_00003f2c(param_1,local_14[0]);
    if (iVar2 == 0) {
      if (-1 < *(char *)((int)param_1 + 0x2b)) {
        if ((*(byte *)((int)param_1 + 6) & 0x10) == 0) goto LAB_000050e0;
        uVar1 = *(ushort *)(param_1[7] + 0x1a);
        uVar3 = (uint)uVar1;
        if (*local_c == '\x03') {
          uVar3 = CONCAT22(*(undefined2 *)(param_1[7] + 0x14),uVar1);
        }
        param_1[2] = uVar3;
      }
      *(undefined2 *)(param_1 + 1) = *(undefined2 *)(local_c + 6);
      iVar2 = FUN_00003d50(param_1);
      if (iVar2 == 4) {
LAB_000050e0:
        *param_1 = 0;
        return 5;
      }
      if (iVar2 == 0) {
        return 0;
      }
    }
    else if (iVar2 == 4) goto LAB_000050e0;
  }
  *param_1 = 0;
  return iVar2;
}

