//==================== FUN_0400a5c0 @ 0400a5c0 ====================

void FUN_0400a5c0(int param_1,undefined4 param_2,uint param_3,uint param_4,uint param_5)

{
  undefined2 uVar1;
  uint uVar2;
  
  if ((param_5 & 1) != 0) {
    *(undefined1 *)(param_1 + param_5 + 0x1c) = 0;
  }
  FUN_0400a538(param_1,param_2,*DAT_0400a640);
  uVar2 = (param_5 & 0xffff) + 0x1c;
  *(ushort *)(param_1 + 2) = (ushort)((uVar2 & 0xff) << 8) | (ushort)(uVar2 >> 8) & 0xff;
  *(undefined1 *)(param_1 + 9) = 0x11;
  uVar2 = (param_5 & 0xffff) + 8;
  uVar1 = FUN_04007c5c(param_1,0x14);
  *(undefined2 *)(param_1 + 10) = uVar1;
  *(ushort *)(param_1 + 0x14) = (ushort)((param_4 & 0xff) << 8) | (ushort)(param_4 >> 8) & 0xff;
  *(ushort *)(param_1 + 0x16) = (ushort)((param_3 & 0xff) << 8) | (ushort)(param_3 >> 8) & 0xff;
  *(ushort *)(param_1 + 0x18) = (ushort)((uVar2 & 0xff) << 8) | (ushort)(uVar2 >> 8) & 0xff;
  *(undefined2 *)(param_1 + 0x1a) = 0;
  return;
}

