//==================== FUN_0005852c @ 0005852c ====================

uint FUN_0005852c(int param_1,undefined4 param_2)

{
  char cVar1;
  byte bVar2;
  uint uVar3;
  char *pcVar4;
  int *piVar5;
  byte *__dest;
  undefined4 uVar6;
  uint uVar7;
  int iVar8;
  int iVar9;
  byte *pbVar10;
  bool bVar11;
  bool bVar12;
  longlong lVar13;
  undefined1 auStack_3c [4];
  size_t local_38;
  char *local_34;
  undefined4 local_30;
  undefined4 uStack_2c;
  
  iVar9 = param_1 + 0x28;
  FUN_00059680(iVar9);
  if (*(int *)(param_1 + 0x34) == 0x100) {
    FUN_00059594(*(undefined4 *)(param_1 + 0x38));
    *(undefined4 *)(param_1 + 0x38) = 0;
  }
  do {
    do {
      uVar3 = FUN_0005832c(param_1,param_2);
    } while (uVar3 == 0x20 || uVar3 == 9);
  } while (uVar3 == 10 || uVar3 == 0xd);
  if (uVar3 == 0xffffffff) {
    *(undefined4 *)(param_1 + 0x34) = 0;
    return 0;
  }
  if (uVar3 != 0xfffffffe) {
    FUN_00059774(iVar9,uVar3 & 0xff);
    if (((uVar3 == 0x7b || uVar3 == 0x7d) || (uVar3 == 0x5b || uVar3 == 0x5d)) ||
       (uVar3 == 0x3a || uVar3 == 0x2c)) goto LAB_00058650;
    if (uVar3 == 0x22) {
      *(undefined4 *)(param_1 + 0x38) = 0;
      *(undefined4 *)(param_1 + 0x34) = 0xffffffff;
      do {
        uVar3 = FUN_000584fc(param_1,param_2);
        while( true ) {
          if (uVar3 == 0x22) {
            __dest = (byte *)FUN_0005956c(*(int *)(param_1 + 0x2c) + 1);
            *(byte **)(param_1 + 0x38) = __dest;
            uVar6 = 0;
            if (__dest == (byte *)0x0) goto LAB_000589a8;
            iVar9 = FUN_00059694(iVar9);
            pbVar10 = (byte *)(iVar9 + 1);
            goto LAB_00058a08;
          }
          if (uVar3 == 0xfffffffe) goto LAB_000589c8;
          if (uVar3 == 0xffffffff) {
            FUN_00058034(param_2,param_1,"premature end of input");
            uVar6 = *(undefined4 *)(param_1 + 0x38);
            goto LAB_000589a8;
          }
          if (uVar3 < 0x20) {
            FUN_000584b0(param_1,uVar3);
            if (uVar3 == 10) {
              pcVar4 = "unexpected newline";
            }
            else {
              pcVar4 = "control character 0x%x";
            }
            FUN_00058034(param_2,param_1,pcVar4,uVar3);
            uVar6 = *(undefined4 *)(param_1 + 0x38);
            goto LAB_000589a8;
          }
          if (uVar3 != 0x5c) break;
          uVar3 = FUN_000584fc(param_1,param_2);
          if (uVar3 == 0x75) {
            uVar3 = FUN_000584fc(param_1,param_2);
            iVar8 = 4;
            do {
              uVar7 = uVar3 - 0x41;
              bVar12 = 4 < uVar7;
              bVar11 = uVar7 == 5;
              if (5 < uVar7) {
                bVar12 = 8 < uVar3 - 0x30;
                bVar11 = uVar3 - 0x30 == 9;
              }
              if ((bVar12 && !bVar11) && (5 < uVar3 - 0x61)) goto LAB_00058c44;
              uVar3 = FUN_000584fc(param_1,param_2);
              iVar8 = iVar8 + -1;
            } while (iVar8 != 0);
          }
          else {
            if ((((uVar3 != 0x22 && uVar3 != 0x5c) && (uVar3 != 0x2f && uVar3 != 0x62)) &&
                ((uVar3 & 0xfffffff7) != 0x66)) && (uVar3 != 0x72 && uVar3 != 0x74)) {
LAB_00058c44:
              FUN_00058034(param_2,param_1,"invalid escape");
              uVar6 = *(undefined4 *)(param_1 + 0x38);
              goto LAB_000589a8;
            }
            uVar3 = FUN_000584fc(param_1,param_2);
          }
        }
      } while( true );
    }
    bVar11 = 0x2c < uVar3;
    if (uVar3 != 0x2d) {
      bVar11 = 8 < uVar3 - 0x30;
    }
    if (!bVar11 || (uVar3 == 0x2d || uVar3 - 0x30 == 9)) {
      *(undefined4 *)(param_1 + 0x34) = 0xffffffff;
      if (uVar3 == 0x2d) {
        uVar3 = FUN_000584fc(param_1,param_2);
      }
      if (uVar3 == 0x30) {
        uVar3 = FUN_000584fc(param_1,param_2);
        if (uVar3 - 0x30 < 10) goto LAB_0005896c;
      }
      else {
        if (9 < uVar3 - 0x30) {
          FUN_000584b0(param_1,uVar3);
          return *(uint *)(param_1 + 0x34);
        }
        do {
          uVar3 = FUN_000584fc(param_1,param_2);
        } while (uVar3 - 0x30 < 10);
      }
      if (uVar3 == 0x45 || uVar3 == 0x2e) {
        if (uVar3 == 0x2e) {
          uVar3 = FUN_0005832c(param_1,param_2);
          if (9 < uVar3 - 0x30) {
            FUN_00058274(param_1,uVar3);
            return *(uint *)(param_1 + 0x34);
          }
          FUN_00059774(iVar9,uVar3 & 0xff);
          do {
            uVar3 = FUN_000584fc(param_1,param_2);
          } while (uVar3 - 0x30 < 10);
        }
        if ((uVar3 & 0xffffffdf) != 0x45) goto LAB_0005887c;
      }
      else if (uVar3 != 0x65) {
        FUN_000584b0(param_1,uVar3);
        pcVar4 = (char *)FUN_00059694(iVar9);
        piVar5 = __errno_location();
        *piVar5 = 0;
        lVar13 = strtoll(pcVar4,&local_34,10);
        if (*piVar5 == 0x22) {
          if (lVar13 < 0) {
            pcVar4 = "too big negative integer";
          }
          else {
            pcVar4 = "too big integer";
          }
          FUN_00058034(param_2,param_1,pcVar4);
          return *(uint *)(param_1 + 0x34);
        }
        if (local_34 != pcVar4 + *(int *)(param_1 + 0x2c)) {
                    /* WARNING: Subroutine does not return */
          __assert_fail("end == saved_text + lex->saved_text.length","load.c",0x206,
                        "lex_scan_number");
        }
        *(longlong *)(param_1 + 0x38) = lVar13;
        *(undefined4 *)(param_1 + 0x34) = 0x101;
        return 0x101;
      }
      uVar3 = FUN_000584fc(param_1,param_2);
      if (uVar3 == 0x2b || uVar3 == 0x2d) {
        uVar3 = FUN_000584fc(param_1,param_2);
      }
      if (9 < uVar3 - 0x30) {
LAB_0005896c:
        FUN_000584b0(param_1,uVar3);
        return *(uint *)(param_1 + 0x34);
      }
      do {
        uVar3 = FUN_000584fc(param_1,param_2);
      } while (uVar3 - 0x30 < 10);
LAB_0005887c:
      FUN_000584b0(param_1,uVar3);
      iVar9 = FUN_000597e8(iVar9,&local_30);
      if (iVar9 == 0) {
        *(undefined4 *)(param_1 + 0x34) = 0x102;
        *(undefined4 *)(param_1 + 0x38) = local_30;
        *(undefined4 *)(param_1 + 0x3c) = uStack_2c;
        return 0x102;
      }
      FUN_00058034(param_2,param_1,"real number overflow");
      return *(uint *)(param_1 + 0x34);
    }
    if ((uVar3 & 0xffffffdf) - 0x41 < 0x1a) {
      do {
        uVar3 = FUN_000584fc(param_1,param_2);
      } while ((uVar3 & 0xffffffdf) - 0x41 < 0x1a);
      FUN_000584b0(param_1,uVar3);
      pcVar4 = (char *)FUN_00059694(iVar9);
      iVar9 = strcmp(pcVar4,"true");
      if (iVar9 == 0) {
        *(undefined4 *)(param_1 + 0x34) = 0x103;
        return 0x103;
      }
      iVar9 = strcmp(pcVar4,"false");
      if (iVar9 == 0) {
        *(undefined4 *)(param_1 + 0x34) = 0x104;
        return 0x104;
      }
      iVar9 = strcmp(pcVar4,"null");
      if (iVar9 == 0) {
        *(undefined4 *)(param_1 + 0x34) = 0x105;
        return 0x105;
      }
    }
    else {
      cVar1 = *(char *)(param_1 + *(int *)(param_1 + 0x10) + 8);
      while (cVar1 != '\0') {
        FUN_00059774(iVar9);
        iVar8 = *(int *)(param_1 + 0x10) + 1;
        *(int *)(param_1 + 0x10) = iVar8;
        *(int *)(param_1 + 0x24) = *(int *)(param_1 + 0x24) + 1;
        cVar1 = *(char *)(param_1 + iVar8 + 8);
      }
    }
  }
  uVar3 = 0xffffffff;
LAB_00058650:
  *(uint *)(param_1 + 0x34) = uVar3;
  return uVar3;
LAB_00058a08:
  while( true ) {
    bVar2 = *pbVar10;
    if (bVar2 == 0x22) {
      *__dest = 0;
      *(undefined4 *)(param_1 + 0x34) = 0x100;
      return 0x100;
    }
    if (bVar2 == 0x5c) break;
    *__dest = bVar2;
    pbVar10 = pbVar10 + 1;
    __dest = __dest + 1;
  }
  bVar2 = pbVar10[1];
  if (bVar2 == 0x75) {
    iVar9 = FUN_000581cc(pbVar10 + 1);
    if (iVar9 - 0xd800U < 0x400) {
      if ((pbVar10[6] != 0x5c) || (pbVar10[7] != 0x75)) {
LAB_000589b4:
        FUN_00058034(param_2,param_1,"invalid Unicode \'\\u%04X\'",iVar9);
LAB_000589c8:
        uVar6 = *(undefined4 *)(param_1 + 0x38);
LAB_000589a8:
        FUN_00059594(uVar6);
        return *(uint *)(param_1 + 0x34);
      }
      iVar8 = FUN_000581cc(pbVar10 + 7);
      pbVar10 = pbVar10 + 0xc;
      if (0x3ff < iVar8 - 0xdc00U) {
        FUN_00058034(param_2,param_1,"invalid Unicode \'\\u%04X\\u%04X\'",iVar9,iVar8);
        goto LAB_000589c8;
      }
      iVar9 = (iVar8 - 0xdc00U) + (iVar9 - 0xd800U) * 0x400 + 0x10000;
    }
    else {
      if (iVar9 - 0xdc00U < 0x400) goto LAB_000589b4;
      if (iVar9 == 0) {
        FUN_00058034(param_2,param_1,"\\u0000 is not allowed");
        goto LAB_000589c8;
      }
      pbVar10 = pbVar10 + 6;
    }
    iVar9 = FUN_000599dc(iVar9,auStack_3c,&local_38);
    if (iVar9 != 0) {
                    /* WARNING: Subroutine does not return */
      __assert_fail("0","load.c",0x1ae,"lex_scan_string");
    }
    memcpy(__dest,auStack_3c,local_38);
    __dest = __dest + local_38;
    goto LAB_00058a08;
  }
  if (bVar2 == 0x66) {
    *__dest = 0xc;
  }
  else if (bVar2 < 0x67) {
    if (bVar2 != 0x5c) {
      if (0x5c < bVar2) {
        if (bVar2 != 0x62) goto LAB_00058a58;
        *__dest = 8;
        goto LAB_00058a8c;
      }
      if ((bVar2 != 0x22) && (bVar2 != 0x2f)) {
LAB_00058a58:
                    /* WARNING: Subroutine does not return */
        __assert_fail("0","load.c",0x1bc,"lex_scan_string");
      }
    }
    *__dest = bVar2;
  }
  else if (bVar2 == 0x72) {
    *__dest = 0xd;
  }
  else if (bVar2 == 0x74) {
    *__dest = 9;
  }
  else {
    if (bVar2 != 0x6e) goto LAB_00058a58;
    *__dest = 10;
  }
LAB_00058a8c:
  __dest = __dest + 1;
  pbVar10 = pbVar10 + 2;
  goto LAB_00058a08;
}

