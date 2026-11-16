//==================== FUN_00007d40 @ 00007d40 ====================

undefined4 FUN_00007d40(int param_1,undefined4 param_2)

{
  int iVar1;
  
  if (param_1 == 0) {
    FUN_000014e4(&DAT_00017264,0x7b);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 0x24) != 0x11111111) {
    FUN_000014e4(&DAT_00017264,0x7c);
    DAT_00028e84 = 1;
    return 0;
  }
  if (((*(uint *)(*(int *)(param_1 + 4) + 0x24) & 0x303) == 0) &&
     (iVar1 = FUN_00006bcc(param_1,0x1000,param_2), iVar1 == 0)) {
    *(ushort *)(*(int *)(param_1 + 4) + 4) = (ushort)param_2 & 0xfff;
    return 0;
  }
  return 1;
}

