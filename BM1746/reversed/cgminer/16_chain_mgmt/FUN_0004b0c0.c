//==================== FUN_0004b0c0 @ 0004b0c0 ====================

/* WARNING: Restarted to delay deadcode elimination for space: stack */

void FUN_0004b0c0(uint *param_1)

{
  pthread_t __th;
  int iVar1;
  char *pcVar2;
  char *pcVar3;
  int iVar4;
  int iVar5;
  uint param3;
  int iVar6;
  char cStack_149f5;
  char acStack_149f4 [4];
  char acStack_149f0 [95];
  char acStack_14991 [1489];
  undefined1 auStack_143c0 [4072];
  undefined1 auStack_133d8 [4368];
  char acStack_122c8 [74404];
  
  memset(acStack_14991 + 1,0,0x5d0);
  param3 = *param_1;
  __th = pthread_self();
  pthread_detach(__th);
  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
    snprintf(acStack_122c8,0x1000,"Start A New Asic Response.Chain Id:[%d]",param3);
    FUN_00038730(5,acStack_122c8,0);
  }
  if ((DAT_00078e09 != '\0') &&
     (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
    snprintf(acStack_122c8,0x1000,"%s %d","get_asic_response",param3);
    FUN_00038730(7,acStack_122c8,0);
  }
  memset(acStack_122c8,0,0x122a0);
  iVar6 = 0;
  memset(acStack_149f0,0,0x5d);
  iVar5 = 0;
  do {
    do {
      do {
        if ((&DAT_00079118)[param3] == '\0') {
          return;
        }
        FUN_0002a954(500,0);
        iVar1 = FUN_0003c5e8(param3 & 0xff,acStack_14991 + 1,0x5d0);
        if (0 < iVar1) {
          pcVar3 = acStack_14991;
          pcVar2 = pcVar3 + iVar1;
          do {
            pcVar3 = pcVar3 + 1;
            acStack_122c8[iVar6] = *pcVar3;
            iVar6 = iVar6 + 1;
            if (0x1229f < iVar6) {
              iVar6 = 0;
            }
          } while (pcVar3 != pcVar2);
        }
      } while (iVar6 == iVar5);
      if (iVar5 < iVar6) {
        iVar1 = -iVar5;
      }
      else {
        iVar1 = 0x122a0 - iVar5;
      }
      iVar1 = iVar6 + iVar1;
    } while (iVar1 < 3);
    do {
      pcVar2 = &cStack_149f5;
      acStack_149f4[2] = 0;
      acStack_149f4[0] = '\0';
      acStack_149f4[1] = '\0';
      iVar4 = iVar5;
      do {
        pcVar3 = acStack_122c8 + iVar4;
        iVar4 = iVar4 + 1;
        pcVar2 = pcVar2 + 1;
        *pcVar2 = *pcVar3;
        if (0x1229f < iVar4) {
          iVar4 = 0;
        }
      } while (pcVar2 != acStack_149f4 + 2);
      if ((acStack_149f4[0] == -0x56) && (acStack_149f4[1] == 'U')) {
        if ((acStack_149f4[2] & 0xf0U) == 0xe0) {
          if (iVar1 < 0x5d) break;
          pcVar2 = acStack_149f4 + 3;
          do {
            pcVar3 = acStack_122c8 + iVar5;
            iVar5 = iVar5 + 1;
            pcVar2 = pcVar2 + 1;
            *pcVar2 = *pcVar3;
            if (0x1229f < iVar5) {
              iVar5 = 0;
            }
          } while (pcVar2 != acStack_149f0 + 0x5c);
          FUN_0004a754(auStack_143c0,auStack_133d8,acStack_149f0,0x5d,param3);
          iVar1 = iVar1 + -0x5d;
        }
        else if (acStack_149f4[2] == 0xcc) {
          if (iVar1 < 9) break;
          pcVar2 = acStack_149f4 + 3;
          do {
            pcVar3 = acStack_122c8 + iVar5;
            iVar5 = iVar5 + 1;
            pcVar2 = pcVar2 + 1;
            *pcVar2 = *pcVar3;
            if (0x1229f < iVar5) {
              iVar5 = 0;
            }
          } while (pcVar2 != acStack_149f0 + 8);
          iVar1 = iVar1 + -9;
          FUN_0004acec(acStack_149f0,9,param3 & 0xff);
        }
        else {
          if (iVar1 < 9) break;
          pcVar2 = acStack_149f4 + 3;
          do {
            pcVar3 = acStack_122c8 + iVar5;
            iVar5 = iVar5 + 1;
            pcVar2 = pcVar2 + 1;
            *pcVar2 = *pcVar3;
            if (0x1229f < iVar5) {
              iVar5 = 0;
            }
          } while (pcVar2 != acStack_149f0 + 8);
          iVar1 = iVar1 + -9;
          FUN_0004ae48(acStack_149f0,9,param3);
        }
      }
      else {
        iVar5 = iVar5 + 1;
        iVar1 = iVar1 + -1;
        if (0x1229f < iVar5) {
          iVar5 = 0;
        }
      }
    } while (2 < iVar1);
  } while( true );
}

