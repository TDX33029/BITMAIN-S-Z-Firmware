//==================== FUN_000072c8 @ 000072c8 ====================

undefined4 FUN_000072c8(int param_1,undefined4 param_2,int param_3,undefined4 param_4)

{
  int iVar1;
  char *pcVar2;
  
  if (((((*(char *)(param_1 + 0x36) == '\x02') &&
        ((*(uint *)(param_1 + 0x28) & 0xc0000000) == 0x40000000)) || (*(int *)(param_1 + 0xc) == 0))
      || ((*(uint *)(*(int *)(param_1 + 4) + 0x24) & 0x10000) != 0)) &&
     ((*(int *)(*(int *)(param_1 + 4) + 4) == 0x200 ||
      (iVar1 = FUN_00007d40(param_1,0x200), iVar1 == 0)))) {
    if (*(int *)(param_1 + 0x204) == 0) {
      FUN_000071f8(param_1,param_3,param_4);
      if (*(char *)(param_1 + 0x20) == '\0') {
        func_0xffffd23c(param_4,param_3 << 9);
      }
    }
    else {
      FUN_00007100(param_1,param_3);
    }
    if (param_3 == 1) {
      DAT_00028e88 = 0x13;
      pcVar2 = s_0_8_0__0_0_0___000010f5 + 0xb;
    }
    else {
      DAT_00028e88 = 0x37;
      pcVar2 = "\x01";
    }
    iVar1 = FUN_00006bcc(param_1,pcVar2,param_2,param_3);
    if (iVar1 == 0) {
      iVar1 = *(int *)(param_1 + 4);
      do {
        if ((*(ushort *)(iVar1 + 0x30) & 0x8000) != 0) {
          *(undefined2 *)(iVar1 + 0x32) = 0xf3ff;
          return 1;
        }
      } while ((*(ushort *)(iVar1 + 0x30) & 2) == 0);
      *(undefined2 *)(iVar1 + 0x30) = 2;
      if (*(char *)(param_1 + 0x20) == '\0') {
        func_0xffffd23c(param_4,param_3 << 9);
        return 0;
      }
      return 0;
    }
  }
  return 1;
}

