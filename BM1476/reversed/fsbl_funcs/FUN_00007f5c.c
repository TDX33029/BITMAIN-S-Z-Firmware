//==================== FUN_00007f5c @ 00007f5c ====================

undefined4 FUN_00007f5c(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  byte bVar3;
  
  if (param_1 == 0) {
    FUN_000014e4(&DAT_00017264,0x109);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 0x24) != 0x11111111) {
    FUN_000014e4(&DAT_00017264,0x10a);
    DAT_00028e84 = 1;
    return 0;
  }
  if ((*(char *)(param_1 + 0x36) != '\x02') || (3 < *(uint *)(param_1 + 0x14))) {
    if (*(char *)(param_1 + 0x34) == '\x01') {
      iVar1 = FUN_00006bcc(param_1,0x3700,*(undefined4 *)(param_1 + 0x50),0);
      if (iVar1 != 0) {
        return 1;
      }
      *(undefined1 *)(param_1 + 0x37) = 2;
      iVar1 = FUN_00006bcc(param_1,0x8600);
      if (iVar1 != 0) {
        return 1;
      }
    }
    else {
      if (((*(uint *)(param_1 + 0x34) & 0xff00ff) == 0x20005) && (*(int *)(param_1 + 0x14) == 8)) {
        if (*(int *)(param_1 + 0x6c) == 4) {
          uVar2 = 0x3b70600;
        }
        else {
          uVar2 = 0x3b70200;
        }
        *(undefined1 *)(param_1 + 0x37) = 3;
      }
      else {
        if (*(int *)(param_1 + 0x6c) == 4) {
          uVar2 = 0x3b70500;
        }
        else {
          uVar2 = 0x3b70100;
        }
        *(undefined1 *)(param_1 + 0x37) = 2;
      }
      iVar1 = FUN_00006bcc(param_1,0x600,uVar2,0);
      if (iVar1 != 0) {
        return 1;
      }
      iVar1 = *(int *)(param_1 + 4);
      do {
        if ((*(ushort *)(iVar1 + 0x30) & 0x8000) != 0) {
          *(undefined2 *)(iVar1 + 0x32) = 0xf3ff;
          return 1;
        }
      } while ((*(ushort *)(iVar1 + 0x30) & 2) == 0);
      *(undefined2 *)(iVar1 + 0x30) = 2;
    }
    func_0xffffe2ac(1000);
    bVar3 = *(byte *)(*(int *)(param_1 + 4) + 0x28);
    if (*(char *)(param_1 + 0x37) == '\x03') {
      bVar3 = bVar3 | 0x20;
    }
    else {
      bVar3 = bVar3 | 2;
    }
    *(byte *)(*(int *)(param_1 + 4) + 0x28) = bVar3;
    if (*(int *)(param_1 + 0x6c) == 4) {
      *(ushort *)(*(int *)(param_1 + 4) + 0x3e) =
           *(ushort *)(*(int *)(param_1 + 4) + 0x3e) & 0xfff8 | 4;
      return 0;
    }
  }
  return 0;
}

