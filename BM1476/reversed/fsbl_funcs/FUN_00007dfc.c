//==================== FUN_00007dfc @ 00007dfc ====================

undefined4 FUN_00007dfc(int param_1,int param_2)

{
  int iVar1;
  undefined1 *puVar2;
  
  if (param_1 == 0) {
    FUN_000014e4(&DAT_00017264,0xb3);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 0x24) != 0x11111111) {
    FUN_000014e4(&DAT_00017264,0xb4);
    DAT_00028e84 = 1;
    return 0;
  }
  puVar2 = (undefined1 *)(param_2 + -1);
  do {
    puVar2 = puVar2 + 1;
    *puVar2 = 0;
  } while (puVar2 != (undefined1 *)(param_2 + 7));
  iVar1 = FUN_00006bcc(param_1,0x3700,*(undefined4 *)(param_1 + 0x50),0);
  if (iVar1 == 0) {
    *(undefined2 *)(*(int *)(param_1 + 4) + 4) = 8;
    FUN_000071f8(param_1,1,param_2);
    DAT_00028e88 = 0x11;
    if (*(char *)(param_1 + 0x20) == '\0') {
      func_0xffffd23c(param_2,8);
    }
    iVar1 = FUN_00006bcc(param_1,0xb300,0,1);
    if (iVar1 == 0) {
      iVar1 = *(int *)(param_1 + 4);
      do {
        if ((*(ushort *)(iVar1 + 0x30) & 0x8000) != 0) {
          *(undefined2 *)(iVar1 + 0x32) = 0xf3ff;
          return 1;
        }
      } while ((*(ushort *)(iVar1 + 0x30) & 2) == 0);
      *(undefined2 *)(iVar1 + 0x30) = 2;
      return 0;
    }
  }
  return 1;
}

