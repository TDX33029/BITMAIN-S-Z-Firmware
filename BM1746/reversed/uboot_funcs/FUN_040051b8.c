//==================== FUN_040051b8 @ 040051b8 ====================

void FUN_040051b8(int param_1,undefined4 *param_2)

{
  undefined1 uVar1;
  undefined4 uVar2;
  byte *pbVar3;
  uint uVar4;
  int iVar5;
  undefined4 uVar6;
  int iVar7;
  byte *pbVar8;
  int iVar9;
  byte bVar10;
  int iVar11;
  int iVar12;
  char *pcVar13;
  int iVar14;
  int iVar15;
  int iVar16;
  char cVar17;
  undefined4 uVar18;
  bool bVar19;
  ulonglong uVar20;
  
  uVar2 = DAT_040056fc;
  iVar15 = *(int *)(param_1 + 0x200);
  iVar16 = 0;
  iVar11 = *(int *)(param_1 + 0x208);
  iVar14 = iVar15;
LAB_040051e0:
  pbVar8 = (byte *)*param_2;
  pbVar3 = pbVar8 + 1;
  *param_2 = pbVar3;
  bVar10 = *pbVar8;
  if (bVar10 < 0x2c) {
    if (0x29 < bVar10) {
      if (bVar10 == 0x2a) {
        if (pbVar8[1] == 0x3f) {
          uVar18 = 0xc;
          *param_2 = pbVar8 + 2;
          goto LAB_04005328;
        }
        FUN_04004af4(param_1,iVar14,10);
      }
      else if (pbVar8[1] == 0x3f) {
        uVar18 = 0xd;
        *param_2 = pbVar8 + 2;
LAB_04005328:
        FUN_04004af4(param_1,iVar14,uVar18);
      }
      else {
        FUN_04004af4(param_1,iVar14,0xb);
      }
      goto LAB_040051e0;
    }
    if (bVar10 == 0x24) {
      iVar12 = *(int *)(param_1 + 0x200);
      if (0xff < iVar12) goto LAB_0400536c;
      *(int *)(param_1 + 0x200) = iVar12 + 1;
      *(undefined1 *)(param_1 + iVar12) = 9;
      goto LAB_040051e0;
    }
    if (bVar10 < 0x25) {
      if (bVar10 == 0) {
        *param_2 = pbVar8;
        return;
      }
    }
    else {
      if (bVar10 == 0x28) {
        iVar14 = *(int *)(param_1 + 0x200);
        iVar12 = *(int *)(param_1 + 0x208) + 1;
        *(int *)(param_1 + 0x208) = iVar12;
        uVar1 = (undefined1)iVar12;
        if (iVar14 < 0x100) {
          iVar12 = iVar14 + 1;
          *(int *)(param_1 + 0x200) = iVar12;
          *(undefined1 *)(param_1 + iVar14) = 6;
          if (iVar12 == 0x100) goto LAB_040054f0;
          *(int *)(param_1 + 0x200) = iVar14 + 2;
          *(undefined1 *)(param_1 + iVar12) = uVar1;
        }
        else {
LAB_040054f0:
          *(undefined4 *)(param_1 + 0x210) = DAT_04005708;
        }
        FUN_040051b8(param_1,param_2);
        pcVar13 = (char *)*param_2;
        *param_2 = pcVar13 + 1;
        if (*pcVar13 != ')') {
          *(undefined4 *)(param_1 + 0x210) = DAT_04005714;
          return;
        }
        iVar12 = *(int *)(param_1 + 0x200);
        if (iVar12 < 0x100) {
          iVar5 = iVar12 + 1;
          *(int *)(param_1 + 0x200) = iVar5;
          *(undefined1 *)(param_1 + iVar12) = 7;
          if (iVar5 == 0x100) goto LAB_0400536c;
          *(int *)(param_1 + 0x200) = iVar12 + 2;
          *(undefined1 *)(param_1 + iVar5) = uVar1;
          goto LAB_040051e0;
        }
        goto LAB_0400536c;
      }
      if (bVar10 == 0x29) {
        *param_2 = pbVar8;
        if (0 < iVar16) {
          FUN_04005170(param_1,iVar16);
        }
        if (iVar11 == 0) {
          *(undefined4 *)(param_1 + 0x210) = DAT_04005704;
          return;
        }
        return;
      }
    }
LAB_0400521c:
    *param_2 = pbVar8;
    iVar14 = *(int *)(param_1 + 0x200);
    uVar18 = *(undefined4 *)(param_1 + 0x204);
    while ((*pbVar8 != 0 && (iVar12 = FUN_04005b14(uVar2), iVar12 == 0))) {
      pbVar3 = (byte *)*param_2;
      iVar12 = *(int *)(param_1 + 0x204);
      *param_2 = pbVar3 + 1;
      if (iVar12 < 0x100) {
        pbVar3 = (byte *)(uint)*pbVar3;
      }
      if (iVar12 < 0x100) {
        *(int *)(param_1 + 0x204) = iVar12 + 1;
        *(char *)(param_1 + iVar12 + 0x100) = (char)pbVar3;
      }
      else {
        *(undefined4 *)(param_1 + 0x210) = DAT_04005700;
      }
      pbVar8 = (byte *)*param_2;
    }
    iVar12 = *(int *)(param_1 + 0x200);
    if (iVar12 < 0x100) {
      iVar5 = iVar12 + 1;
      *(int *)(param_1 + 0x200) = iVar5;
      *(undefined1 *)(param_1 + iVar12) = 3;
      if (iVar5 == 0x100) goto LAB_0400536c;
      iVar9 = iVar12 + 2;
      *(int *)(param_1 + 0x200) = iVar9;
      cVar17 = (char)uVar18;
      *(char *)(param_1 + iVar5) = cVar17;
      cVar17 = (char)*(undefined4 *)(param_1 + 0x204) - cVar17;
joined_r0x040054d0:
      if (iVar9 != 0x100) {
        *(int *)(param_1 + 0x200) = iVar12 + 3;
        *(char *)(param_1 + iVar9) = cVar17;
        goto LAB_040051e0;
      }
    }
  }
  else {
    if (bVar10 == 0x5b) {
      bVar10 = pbVar8[1];
      iVar14 = *(int *)(param_1 + 0x200);
      uVar18 = *(undefined4 *)(param_1 + 0x204);
      if (bVar10 == 0x5e) {
        pbVar3 = pbVar8 + 2;
        uVar6 = 5;
        *param_2 = pbVar3;
        bVar10 = pbVar8[2];
      }
      else {
        uVar6 = 4;
      }
      while (bVar10 != 0) {
        *param_2 = pbVar3 + 1;
        bVar10 = *pbVar3;
        if (bVar10 == 0x5c) {
          uVar20 = FUN_04004930(param_2);
          uVar6 = (undefined4)(uVar20 >> 0x20);
          iVar12 = *(int *)(param_1 + 0x204);
          if ((uVar20 & 0xff) == 0) {
            if (iVar12 < 0x100) {
              iVar5 = iVar12 + 1;
              bVar19 = iVar5 != 0x100;
              *(int *)(param_1 + 0x204) = iVar5;
              iVar9 = param_1 + iVar12;
              if (bVar19) {
                iVar5 = param_1 + iVar5;
                iVar12 = iVar12 + 2;
              }
              *(char *)(iVar9 + 0x100) = (char)uVar20;
              if (bVar19) {
                *(int *)(param_1 + 0x204) = iVar12;
                *(char *)(iVar5 + 0x100) = (char)(uVar20 >> 8);
                goto LAB_04005584;
              }
            }
          }
          else if (iVar12 < 0x100) {
            *(int *)(param_1 + 0x204) = iVar12 + 1;
            *(char *)(param_1 + iVar12 + 0x100) = (char)uVar20;
            goto LAB_04005584;
          }
LAB_0400557c:
          *(undefined4 *)(param_1 + 0x210) = DAT_04005700;
        }
        else {
          if (bVar10 == 0x5d) {
            iVar12 = *(int *)(param_1 + 0x200);
            if (0xff < iVar12) goto LAB_0400536c;
            iVar5 = iVar12 + 1;
            *(int *)(param_1 + 0x200) = iVar5;
            *(char *)(param_1 + iVar12) = (char)uVar6;
            if (iVar5 == 0x100) goto LAB_0400536c;
            iVar9 = iVar12 + 2;
            *(int *)(param_1 + 0x200) = iVar9;
            cVar17 = (char)uVar18;
            *(char *)(param_1 + iVar5) = cVar17;
            cVar17 = (char)*(undefined4 *)(param_1 + 0x204) - cVar17;
            goto joined_r0x040054d0;
          }
          iVar12 = *(int *)(param_1 + 0x204);
          if (0xff < iVar12) goto LAB_0400557c;
          *(int *)(param_1 + 0x204) = iVar12 + 1;
          *(byte *)(param_1 + iVar12 + 0x100) = bVar10;
        }
LAB_04005584:
        pbVar3 = (byte *)*param_2;
        bVar10 = *pbVar3;
      }
      *(undefined4 *)(param_1 + 0x210) = DAT_04005710;
      goto LAB_040051e0;
    }
    if (bVar10 < 0x5c) {
      if (bVar10 != 0x2e) {
        if (bVar10 == 0x3f) {
          uVar18 = 0xe;
          goto LAB_04005328;
        }
        goto LAB_0400521c;
      }
      iVar14 = *(int *)(param_1 + 0x200);
      if (iVar14 < 0x100) {
        *(int *)(param_1 + 0x200) = iVar14 + 1;
        *(undefined1 *)(param_1 + iVar14) = 2;
        goto LAB_040051e0;
      }
    }
    else {
      if (bVar10 != 0x5e) {
        if (bVar10 == 0x7c) {
          if (0 < iVar16) {
            FUN_04005170(param_1,iVar16);
          }
          iVar16 = iVar15 + 2;
          FUN_04004a9c(param_1,iVar15,3);
          *(undefined1 *)(param_1 + iVar15) = 1;
          iVar12 = *(int *)(param_1 + 0x200) - iVar15;
          if (iVar12 < 0x100) {
            *(char *)(param_1 + iVar15 + 1) = (char)iVar12;
          }
          else {
            *(undefined4 *)(param_1 + 0x210) = DAT_0400570c;
          }
          *(undefined1 *)(param_1 + iVar16) = 0xff;
        }
        else {
          if (bVar10 != 0x5c) goto LAB_0400521c;
          iVar14 = *(int *)(param_1 + 0x200);
          uVar4 = FUN_04004930(param_2);
          iVar12 = *(int *)(param_1 + 0x200);
          if ((uVar4 & 0xff00) == 0) {
            iVar5 = *(int *)(param_1 + 0x204);
            if (iVar12 < 0x100) {
              iVar9 = iVar12 + 1;
              *(int *)(param_1 + 0x200) = iVar9;
              *(undefined1 *)(param_1 + iVar12) = 3;
              if (iVar9 == 0x100) goto LAB_040056e4;
              iVar7 = iVar12 + 2;
              *(int *)(param_1 + 0x200) = iVar7;
              *(char *)(param_1 + iVar9) = (char)iVar5;
              if (iVar7 == 0x100) goto LAB_040056e4;
              *(int *)(param_1 + 0x200) = iVar12 + 3;
              *(undefined1 *)(param_1 + iVar7) = 1;
            }
            else {
LAB_040056e4:
              *(undefined4 *)(param_1 + 0x210) = DAT_04005708;
            }
            if (iVar5 < 0x100) {
              *(int *)(param_1 + 0x204) = iVar5 + 1;
              *(char *)(param_1 + iVar5 + 0x100) = (char)uVar4;
            }
            else {
              *(undefined4 *)(param_1 + 0x210) = DAT_04005700;
            }
          }
          else {
            if (0xff < iVar12) goto LAB_0400536c;
            *(int *)(param_1 + 0x200) = iVar12 + 1;
            *(char *)(param_1 + iVar12) = (char)(uVar4 >> 8);
          }
        }
        goto LAB_040051e0;
      }
      iVar12 = *(int *)(param_1 + 0x200);
      if (iVar12 < 0x100) {
        *(int *)(param_1 + 0x200) = iVar12 + 1;
        *(undefined1 *)(param_1 + iVar12) = 8;
        goto LAB_040051e0;
      }
    }
  }
LAB_0400536c:
  *(undefined4 *)(param_1 + 0x210) = DAT_04005708;
  goto LAB_040051e0;
}

