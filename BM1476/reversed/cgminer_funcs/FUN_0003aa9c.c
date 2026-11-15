//==================== FUN_0003aa9c @ 0003aa9c ====================

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0003aa9c(void)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  
  do {
    iVar4 = 2;
    _DAT_0009d6ce = 0;
    _DAT_0009d6d2 = 0;
    do {
      iVar5 = 6;
      do {
        while (uVar3 = *(uint *)(DAT_00078e10 + 4), uVar3 == 0xffffffff) {
LAB_0003ab1c:
          FUN_0002a92c(0x32);
          iVar5 = iVar5 + -1;
          if (iVar5 == 0) goto LAB_0003abb0;
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
            DAT_0009d6cc = DAT_0009d6cc + -1;
          }
        }
        else if ((&DAT_0009d67f)[uVar2] == '\0') {
          (&DAT_0009d67f)[uVar2] = 1;
          DAT_0009d6cc = DAT_0009d6cc + '\x01';
          DAT_0009d4c8 = DAT_0009d4c8 | 1 << uVar2;
        }
        if (_DAT_0009d6ce < uVar1) {
          _DAT_0009d6ce = uVar1;
        }
        if (((uVar1 < _DAT_0009d6d2) && (uVar1 != 0)) || (_DAT_0009d6d2 == 0)) {
          _DAT_0009d6d2 = uVar1;
          goto LAB_0003ab1c;
        }
        FUN_0002a92c(0x32);
        iVar5 = iVar5 + -1;
      } while (iVar5 != 0);
LAB_0003abb0:
      iVar4 = iVar4 + -1;
    } while (iVar4 != 0);
    sleep(1);
  } while( true );
}

