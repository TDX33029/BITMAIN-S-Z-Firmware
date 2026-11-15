//==================== FUN_0400a538 @ 0400a538 ====================

void FUN_0400a538(undefined4 *param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  undefined4 local_10;
  undefined4 local_c;
  
  *param_1 = DAT_0400a5b4;
  iVar1 = *DAT_0400a5b8;
  *DAT_0400a5b8 = iVar1 + 1;
  *(ushort *)(param_1 + 1) =
       (ushort)DAT_0400a5bc & (ushort)(iVar1 << 8) | (ushort)((uint)iVar1 >> 8) & 0xff;
  *(undefined2 *)((int)param_1 + 6) = 0x40;
  *(undefined1 *)(param_1 + 2) = 0xff;
  *(undefined2 *)((int)param_1 + 10) = 0;
  local_10 = param_3;
  local_c = param_2;
  FUN_04005ea4(param_1 + 3,&local_10,4);
  FUN_04005ea4(param_1 + 4,&local_c);
  return;
}

