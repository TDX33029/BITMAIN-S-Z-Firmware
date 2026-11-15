//==================== FUN_00008428 @ 00008428 ====================

undefined4 FUN_00008428(int param_1)

{
  int iVar1;
  char *pcVar2;
  char cVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined2 uVar6;
  undefined1 auStack_58 [68];
  
  FUN_000027e0(auStack_58,0,0x40);
  if (param_1 == 0) {
    FUN_000014e4(&DAT_00017264,0x1e4);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 0x24) != 0x11111111) {
    FUN_000014e4(&DAT_00017264,0x1e5);
    DAT_00028e84 = 1;
    return 0;
  }
  if (*(char *)(param_1 + 0x34) == '\x01') {
    *(undefined2 *)(*(int *)(param_1 + 4) + 4) = 0x40;
    FUN_000071f8(param_1,1,auStack_58);
    if (*(char *)(param_1 + 0x20) == '\0') {
      func_0xffffd370(auStack_58,0x40);
    }
    DAT_00028e88 = 0x11;
    iVar1 = FUN_00006bcc(param_1,0x600,0x80fffff1,1);
    if (iVar1 != 0) {
      return 1;
    }
    iVar1 = *(int *)(param_1 + 4);
    do {
      if ((*(ushort *)(iVar1 + 0x30) & 0x8000) != 0) goto LAB_000085f8;
    } while ((*(ushort *)(iVar1 + 0x30) & 2) == 0);
    uVar5 = 50000000;
  }
  else {
    if (*(char *)(param_1 + 0x34) != '\x02') {
      if (*(int *)(param_1 + 0x6c) == 7) {
        uVar5 = 200000000;
        uVar4 = 0x3b90200;
      }
      else {
        uVar5 = 52000000;
        uVar4 = 0x3b90100;
      }
      *(undefined4 *)(param_1 + 0x38) = uVar5;
      iVar1 = FUN_00006bcc(param_1,0x600,uVar4,0);
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
      uVar5 = *(undefined4 *)(param_1 + 0x38);
      *(undefined2 *)(iVar1 + 0x30) = 2;
      iVar1 = FUN_000082b0(param_1,uVar5);
      if (iVar1 != 0) {
        return 1;
      }
      if (*(int *)(param_1 + 0x6c) == 7) {
        DAT_00028e84 = iVar1;
        if (*(int *)(param_1 + 0x24) == 0x11111111) {
          iVar1 = *(int *)(param_1 + 4);
          cVar3 = '(';
          if (*(char *)(param_1 + 0x37) == '\x03') {
            uVar6 = 0x80;
          }
          else {
            uVar6 = 0x40;
          }
          *(undefined2 *)(iVar1 + 4) = uVar6;
          DAT_00028e88 = 0x10;
          *(ushort *)(iVar1 + 0x3e) = *(ushort *)(iVar1 + 0x3e) | 0x40;
          func_0xffffe2ac(1);
          do {
            pcVar2 = (char *)0x1500;
            if (*(char *)(param_1 + 0x34) == '\x01') {
              pcVar2 = s_0___000012f9 + 7;
            }
            iVar1 = FUN_00006bcc(param_1,pcVar2,0,1);
            cVar3 = cVar3 + -1;
            if (iVar1 != 0) {
              return 1;
            }
          } while (((*(ushort *)(*(int *)(param_1 + 4) + 0x3e) & 0x40) != 0) && (cVar3 != '\0'));
          if ((*(ushort *)(*(int *)(param_1 + 4) + 0x3e) & 0x80) == 0) {
            return 1;
          }
          func_0xffffe2ac(1);
        }
        else {
          FUN_000014e4(&DAT_00017264,0x475);
          DAT_00028e84 = 1;
        }
      }
      goto LAB_00008630;
    }
    iVar1 = FUN_00006bcc(param_1,0x600,0x3b90100,0);
    if (iVar1 != 0) {
      return 1;
    }
    iVar1 = *(int *)(param_1 + 4);
    do {
      if ((*(ushort *)(iVar1 + 0x30) & 0x8000) != 0) {
LAB_000085f8:
        *(undefined2 *)(iVar1 + 0x32) = 0xf3ff;
        return 1;
      }
    } while ((*(ushort *)(iVar1 + 0x30) & 2) == 0);
    uVar5 = 52000000;
  }
  *(undefined2 *)(iVar1 + 0x30) = 2;
  *(undefined4 *)(param_1 + 0x38) = uVar5;
  iVar1 = FUN_000082b0(param_1,uVar5);
  if (iVar1 != 0) {
    return 1;
  }
LAB_00008630:
  func_0xffffe2ac(1000);
  *(byte *)(*(int *)(param_1 + 4) + 0x28) = *(byte *)(*(int *)(param_1 + 4) + 0x28) | 4;
  return 0;
}

