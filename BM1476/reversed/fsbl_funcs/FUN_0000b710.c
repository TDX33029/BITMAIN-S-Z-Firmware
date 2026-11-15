//==================== FUN_0000b710 @ 0000b710 ====================

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_0000b710(uint *param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  char local_21 [5];
  
  if (param_1 == (uint *)0x0) {
    return 0x8100;
  }
  iVar1 = FUN_0000c024();
  if (((((1 < *param_1) || (1 < param_1[1])) || (1 < param_1[2])) ||
      ((1 < param_1[5] || (1 < param_1[3])))) || (1 < param_1[4])) {
    return 0x8600;
  }
  iVar2 = FUN_0000bb18();
  if (iVar2 != 0) {
    return 0x8200;
  }
  _DAT_f800d004 = 0xdf0d;
  if ((_DAT_f800d008 & 1) != 0) {
    _DAT_f800d004 = 0xdf0d;
    return 0x8300;
  }
  if ((_DAT_f800d010 & 0x3000) != 0) {
    _DAT_f800d000 = 0x767b;
    _DAT_f800d004 = 0xdf0d;
    return 0x8400;
  }
  if (40000000 < iVar1 + 0xfeced300U) {
    iVar2 = 0x850b;
    goto LAB_0000b7d4;
  }
  iVar2 = FUN_0000aedc(0,iVar1,1);
  if (iVar2 != 0) {
    iVar2 = iVar2 + 0x8500;
    goto LAB_0000b7d4;
  }
  _DAT_f800d014 = _DAT_f800d014 & 0xffffffec | 0x10;
  FUN_0000bc50(iVar1);
  if ((param_1[2] != 0) && (iVar2 = FUN_0000e7c4(0xf800e028,iVar1), iVar2 != 0)) {
    iVar2 = iVar2 + 0x9100;
    goto LAB_0000b7d4;
  }
  if (param_1[5] == 0) {
LAB_0000b860:
    if ((param_1[1] == 0) || (iVar2 = FUN_0000e7c4(0xf800e02c,iVar1), iVar2 == 0)) {
      if ((param_1[3] == 0) || (iVar2 = FUN_0000e7c4(0xf800e030,iVar1), iVar2 == 0)) {
        if ((param_1[4] == 0) || (iVar2 = FUN_0000e7c4(0xf800e034,iVar1), iVar2 == 0)) {
          iVar2 = 0;
          if (*param_1 != 0) {
            iVar3 = FUN_0000e7c4(0xf800e020,iVar1);
            iVar1 = FUN_0000e7c4(0xf800e024,iVar1);
            if (iVar3 != 0 && iVar1 != 0) {
              iVar2 = iVar3 + 0x9600;
            }
          }
        }
        else {
          iVar2 = iVar2 + 0x9900;
        }
      }
      else {
        iVar2 = iVar2 + 0x9800;
      }
    }
    else {
      iVar2 = iVar2 + 0x9500;
    }
  }
  else {
    iVar2 = FUN_0000aedc(1,iVar1);
    if (iVar2 == 0) {
      iVar3 = 0x136;
      iVar2 = -0x7ff1f80;
      do {
        while (iVar4 = FUN_0000b254(iVar2), iVar4 == 1) {
          iVar2 = iVar2 + 4;
        }
        iVar4 = FUN_0000b3d4(iVar2,local_21);
        iVar2 = iVar2 + 4;
        if (iVar4 != 0) {
LAB_0000b9dc:
          iVar2 = FUN_0000aedc(0,iVar1,1);
          if (iVar2 == 0) {
            iVar2 = iVar4;
          }
          goto LAB_0000b9f4;
        }
        if (local_21[0] != '\0') {
          iVar4 = 9;
          goto LAB_0000b9dc;
        }
        iVar3 = iVar3 + -1;
      } while (iVar3 != 0);
      iVar2 = FUN_0000aedc(0,iVar1,1);
      if (iVar2 == 0) {
        iVar2 = FUN_0000e88c(param_1 + 6,iVar1);
        if (iVar2 != 0) {
          iVar2 = iVar2 + 0x9400;
          goto LAB_0000b7d4;
        }
        goto LAB_0000b860;
      }
    }
LAB_0000b9f4:
    iVar2 = iVar2 + 0x9700;
  }
LAB_0000b7d4:
  _DAT_f800d014 = _DAT_f800d014 & 0xffffffef | 3;
  _DAT_f800d000 = 0x767b;
  return iVar2;
}

