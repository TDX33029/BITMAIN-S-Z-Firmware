//==================== FUN_00006bcc @ 00006bcc ====================

ushort FUN_00006bcc(int param_1,int param_2,undefined4 param_3,undefined2 param_4)

{
  ushort uVar1;
  uint uVar2;
  uint uVar3;
  int extraout_r2;
  int iVar4;
  
  if (param_1 == 0) {
    FUN_000014e4(&DAT_0001725c,0x451);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 0x24) != 0x11111111) {
    FUN_000014e4(&DAT_0001725c,0x452);
    DAT_00028e84 = 1;
    return 0;
  }
  iVar4 = *(int *)(param_1 + 4);
  if ((*(uint *)(iVar4 + 0x24) & 1) == 0) {
    *(undefined2 *)(iVar4 + 6) = param_4;
    *(undefined1 *)(iVar4 + 0x2e) = 0xe;
    *(undefined4 *)(*(int *)(param_1 + 4) + 8) = param_3;
    iVar4 = *(int *)(param_1 + 4);
    *(undefined2 *)(iVar4 + 0x30) = 0xffff;
    *(undefined2 *)(iVar4 + 0x32) = 0xf3ff;
    uVar2 = FUN_00006a24();
    uVar3 = param_2 - 0x1300U & 0xfffffdff;
    if (((uVar3 == 0) || ((*(uint *)(extraout_r2 + 0x24) & 2) == 0)) || ((uVar2 & 0x20) == 0)) {
      *(uint *)(extraout_r2 + 0xc) = (uVar2 & 0x3fff) << 0x10 | (uint)DAT_00028e88;
      iVar4 = *(int *)(param_1 + 4);
      do {
        uVar1 = *(ushort *)(iVar4 + 0x30);
        if ((uVar3 == 0) && ((*(ushort *)(iVar4 + 0x30) & 0x20) != 0)) {
          *(undefined2 *)(iVar4 + 0x30) = 0x20;
          break;
        }
        if ((uVar1 & 0x8000) != 0) {
          uVar1 = *(ushort *)(iVar4 + 0x32);
          *(undefined2 *)(iVar4 + 0x32) = 0xf3ff;
          if ((uVar1 & 0xfffe) == 0) {
            uVar1 = 2;
          }
          return uVar1;
        }
      } while ((uVar1 & 1) == 0);
      *(undefined2 *)(iVar4 + 0x30) = 1;
      return 0;
    }
  }
  return 1;
}

