//==================== FUN_0004886c @ 0004886c ====================

void FUN_0004886c(int param_1,int param_2,uint param_3,uint param_4)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  undefined4 uStack_1034;
  undefined4 uStack_1030;
  undefined2 uStack_102c;
  undefined1 local_102a;
  char acStack_1028 [4100];
  
  uStack_1030 = 0;
  uStack_102c = 0;
  uStack_1034 = 0;
  local_102a = 0;
  FUN_0002a92c(0x32);
  if ((param_4 == 0) || (param_4 == 1)) {
    uVar3 = param_4;
  }
  else if (param_4 == 0x11) {
    uVar3 = 2;
  }
  else {
    if (param_4 != 0xfe) {
      if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 4)) {
        return;
      }
      snprintf(acStack_1028,0x1000,"%s unknow reg_type %d\n","read_i2c_reg",param_4);
      FUN_00038730(4,acStack_1028,0);
      return;
    }
    uVar3 = 3;
  }
  iVar2 = 3;
  FUN_0003cdcc(&uStack_1034,0,(&DAT_00079130)[param_2],0x20,
               DAT_00076b6c & 0xff000000 | 0x1000000 | (param_3 & 0x7f) << 0x11 |
               (param_4 & 0xff) << 8);
  FUN_0003c150(param_1,&uStack_1034,0xb);
  FUN_0002a92c(200);
  iVar1 = uVar3 + (param_1 * 3 + param_2) * 4;
  *(undefined1 *)((int)&DAT_00078e10 + iVar1 + 0xda4) = 0;
  do {
    FUN_0002a92c(0x32);
    FUN_000481ec(param_1,(&DAT_00079130)[param_2],0x20,0);
    if (*(char *)((int)&DAT_00078e10 + iVar1 + 0xda4) != '\0') {
      return;
    }
    iVar2 = iVar2 + -1;
  } while (iVar2 != 0);
  return;
}

