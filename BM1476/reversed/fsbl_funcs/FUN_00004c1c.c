//==================== FUN_00004c1c @ 00004c1c ====================

void FUN_00004c1c(undefined4 *param_1)

{
  int iVar1;
  undefined4 uVar2;
  char *pcVar3;
  undefined4 uVar4;
  int local_14 [2];
  
  iVar1 = FUN_00002bbc(param_1,local_14);
  if ((iVar1 == 0) && ((*(byte *)(param_1 + 4) & 0x40) != 0)) {
    if ((*(byte *)(param_1 + 4) & 0x80) != 0) {
      iVar1 = FUN_0000676c(*(undefined1 *)(local_14[0] + 1),param_1 + 0x10,param_1[7],1);
      if (iVar1 != 0) {
        return;
      }
      *(byte *)(param_1 + 4) = *(byte *)(param_1 + 4) & 0x7f;
    }
    uVar2 = FUN_00006760(0);
    if ((param_1[8] != *(int *)(local_14[0] + 0x2c)) &&
       (iVar1 = FUN_00002f3c(local_14[0]), iVar1 != 0)) {
      return;
    }
    iVar1 = param_1[9];
    *(byte *)(iVar1 + 0xb) = *(byte *)(iVar1 + 0xb) | 0x20;
    uVar4 = param_1[2];
    pcVar3 = (char *)*param_1;
    *(char *)(iVar1 + 0x1a) = (char)uVar4;
    *(char *)(iVar1 + 0x1b) = (char)((uint)uVar4 >> 8);
    if (*pcVar3 == '\x03') {
      *(char *)(iVar1 + 0x14) = (char)((uint)uVar4 >> 0x10);
      *(char *)(iVar1 + 0x15) = (char)((uint)uVar4 >> 0x18);
    }
    uVar4 = param_1[3];
    *(char *)(iVar1 + 0x17) = (char)((uint)uVar2 >> 8);
    *(char *)(iVar1 + 0x16) = (char)uVar2;
    *(char *)(iVar1 + 0x1c) = (char)uVar4;
    *(char *)(iVar1 + 0x18) = (char)((uint)uVar2 >> 0x10);
    *(char *)(iVar1 + 0x1d) = (char)((uint)uVar4 >> 8);
    *(char *)(iVar1 + 0x1e) = (char)((uint)uVar4 >> 0x10);
    *(char *)(iVar1 + 0x1f) = (char)((uint)uVar4 >> 0x18);
    *(char *)(iVar1 + 0x19) = (char)((uint)uVar2 >> 0x18);
    *(undefined1 *)(iVar1 + 0x12) = 0;
    *(undefined1 *)(iVar1 + 0x13) = 0;
    *(undefined1 *)(local_14[0] + 3) = 1;
    FUN_00002e20(local_14[0]);
    *(byte *)(param_1 + 4) = *(byte *)(param_1 + 4) & 0xbf;
  }
  return;
}

