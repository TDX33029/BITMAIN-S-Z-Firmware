//==================== FUN_00045b50 @ 00045b50 ====================

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00045b50(void)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  char acStack_1028 [4100];
  
  iVar4 = 0;
  _DAT_0009d6ce = 0;
  _DAT_0009d6d2 = 0;
  do {
    while( true ) {
      if (5 < iVar4) {
        if (1 < DAT_0009d6cc) {
          if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
            snprintf(acStack_1028,0x1000,"fan-num %d fan-map %d\n",(uint)DAT_0009d6cc,DAT_0009d4c8);
            FUN_00038730(5,acStack_1028,0);
          }
          return;
        }
        if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
          snprintf(acStack_1028,0x1000,"No %d Fan find, check again",2);
          FUN_00038730(5,acStack_1028,0);
        }
        iVar4 = 0;
        FUN_0002a92c(1000);
      }
      uVar3 = *(uint *)(DAT_00078e10 + 4);
      if (uVar3 != 0xffffffff) break;
LAB_00045cfc:
      iVar4 = iVar4 + 1;
      FUN_0002a92c(0x32);
    }
    uVar2 = (uVar3 & 0x7ff) >> 8;
    uVar1 = (uVar3 & 0xff) * 0x78;
    *(uint *)(&DAT_0009d69d + uVar2 * 4) = uVar1;
    if ((uVar3 & 0xff) == 0) {
      if ((&DAT_0009d67f)[uVar2] == '\x01') {
        (&DAT_0009d67f)[uVar2] = (char)uVar3;
        if (1 << uVar2 == 0) {
          DAT_0009d4c8 = DAT_0009d4c8 & 1;
        }
        else {
          DAT_0009d4c8 = 0;
        }
        DAT_0009d6cc = DAT_0009d6cc - 1;
      }
    }
    else if ((&DAT_0009d67f)[uVar2] == '\0') {
      (&DAT_0009d67f)[uVar2] = 1;
      DAT_0009d6cc = DAT_0009d6cc + 1;
      DAT_0009d4c8 = DAT_0009d4c8 | 1 << uVar2;
    }
    if (_DAT_0009d6ce < uVar1) {
      _DAT_0009d6ce = uVar1;
    }
    if (((uVar1 < _DAT_0009d6d2) && (uVar1 != 0)) || (_DAT_0009d6d2 == 0)) {
      _DAT_0009d6d2 = uVar1;
      goto LAB_00045cfc;
    }
    iVar4 = iVar4 + 1;
    FUN_0002a92c(0x32);
  } while( true );
}

