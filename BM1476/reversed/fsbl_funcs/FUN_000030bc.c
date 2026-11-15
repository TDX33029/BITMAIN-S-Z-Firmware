//==================== FUN_000030bc @ 000030bc ====================

undefined4 FUN_000030bc(undefined4 param_1,undefined4 *param_2,uint param_3)

{
  byte bVar1;
  byte bVar2;
  ushort uVar3;
  int iVar4;
  undefined4 uVar5;
  uint uVar6;
  char *pcVar7;
  int *piVar8;
  char cVar9;
  uint uVar10;
  char *pcVar11;
  int iVar12;
  char *pcVar13;
  int *piVar14;
  uint uVar15;
  int iVar16;
  int iVar17;
  uint extraout_r12;
  uint uVar18;
  uint extraout_r12_00;
  bool bVar19;
  int aiStack_38 [4];
  int iStack_28;
  
  *param_2 = 0;
  iVar4 = FUN_00002b44();
  if (iVar4 < 0) {
    uVar5 = 0xb;
  }
  else {
    pcVar13 = *(char **)(&DAT_00021544 + iVar4 * 4);
    if (pcVar13 == (char *)0x0) {
      uVar5 = 0xc;
    }
    else {
      *param_2 = pcVar13;
      if ((*pcVar13 == '\0') || (uVar6 = FUN_000064b8(pcVar13[1]), (uVar6 & 1) != 0)) {
        *pcVar13 = '\0';
        pcVar13[1] = (char)iVar4;
        uVar6 = FUN_000065cc();
        if ((uVar6 & 1) == 0) {
          if (((param_3 & 0xfe) != 0) && ((uVar6 & 4) != 0)) {
            return 10;
          }
          uVar6 = FUN_00002fd8(pcVar13,0);
          if (uVar6 == 2) {
            piVar8 = aiStack_38;
            pcVar7 = pcVar13 + 0x208;
            do {
              pcVar11 = pcVar7 + 0x10;
              uVar5 = 0;
              if (pcVar7[-6] != '\0') {
                uVar5 = CONCAT31(CONCAT21(*(undefined2 *)pcVar7,pcVar7[-1]),pcVar7[-2]);
              }
              *piVar8 = uVar5;
              piVar8 = piVar8 + 1;
              pcVar7 = pcVar11;
            } while (pcVar11 != pcVar13 + 0x248);
            piVar8 = aiStack_38;
            do {
              piVar14 = piVar8 + 1;
              iVar4 = *piVar8;
              uVar6 = 3;
              if ((iVar4 != 0) &&
                 (uVar6 = FUN_00002fd8(pcVar13,iVar4), uVar18 = extraout_r12_00, uVar6 < 2))
              goto LAB_00003194;
              piVar8 = piVar14;
            } while (&iStack_28 != piVar14);
            if (uVar6 != 4) {
              return 0xd;
            }
          }
          else if (uVar6 != 4) {
            if (uVar6 < 2) {
              iVar4 = 0;
              uVar18 = extraout_r12;
LAB_00003194:
              if (*(short *)(pcVar13 + 0x4b) == 0x200) {
                uVar6 = (uint)*(ushort *)(pcVar13 + 0x56);
                if (uVar6 == 0) {
                  uVar6 = *(uint *)(pcVar13 + 100);
                }
                bVar1 = pcVar13[0x50];
                *(uint *)(pcVar13 + 0x18) = uVar6;
                pcVar13[2] = bVar1;
                if (bVar1 - 1 < 2) {
                  bVar2 = pcVar13[0x4d];
                  *(ushort *)(pcVar13 + 10) = (ushort)bVar2;
                  if ((bVar2 != 0) && ((bVar2 - 1 & bVar2) == 0)) {
                    uVar3 = *(ushort *)(pcVar13 + 0x51);
                    *(ushort *)(pcVar13 + 8) = uVar3;
                    if ((uVar3 & 0xf) == 0) {
                      bVar2 = pcVar13[0x54];
                      uVar10 = (uint)*(ushort *)(pcVar13 + 0x53);
                      bVar19 = uVar10 == 0;
                      if (bVar19) {
                        uVar10 = (uint)(byte)pcVar13[0x61];
                        uVar18 = (uint)*(ushort *)(pcVar13 + 0x62);
                        bVar2 = pcVar13[0x60];
                      }
                      if (bVar19) {
                        uVar10 = (uint)bVar2 | (uVar10 | uVar18 << 8) << 8;
                      }
                      uVar18 = (uint)CONCAT11(pcVar13[0x4f],pcVar13[0x4e]);
                      if (uVar18 != 0) {
                        iVar17 = uVar6 * bVar1;
                        uVar15 = uVar18 + (uVar3 >> 4) + iVar17;
                        if ((uVar15 <= uVar10) &&
                           (pcVar7 = (char *)FUN_00001a50(uVar10 - uVar15),
                           pcVar7 + -1 < (char *)0xffffff4 || pcVar7 == (char *)0xffffff5)) {
                          iVar12 = uVar18 + iVar4;
                          iVar16 = uVar15 + iVar4;
                          pcVar11 = pcVar7 + 2;
                          if (s_PS7_init_Data_Corrupted_0000fff0 + 5 < pcVar7) {
                            *(int *)(pcVar13 + 0x20) = iVar12;
                            *(char **)(pcVar13 + 0x14) = pcVar11;
                            *(int *)(pcVar13 + 0x1c) = iVar4;
                            *(int *)(pcVar13 + 0x28) = iVar16;
                            if (uVar3 != 0 || *(short *)(pcVar13 + 0x6a) != 0) {
                              return 0xd;
                            }
                            *(undefined4 *)(pcVar13 + 0x24) = *(undefined4 *)(pcVar13 + 0x6c);
                            if (uVar6 < (int)pcVar11 * 4 + 0x1ffU >> 9) {
                              return 0xd;
                            }
                            pcVar13[0x10] = -1;
                            pcVar13[0x11] = -1;
                            pcVar13[0x12] = -1;
                            pcVar13[0x13] = -1;
                            pcVar13[0xc] = -1;
                            pcVar13[0xd] = -1;
                            pcVar13[0xe] = -1;
                            pcVar13[0xf] = -1;
                            pcVar13[4] = -0x80;
                            if (((*(short *)(pcVar13 + 0x70) == 1) &&
                                (((iVar4 + 1 == *(int *)(pcVar13 + 0x2c) ||
                                  (iVar4 = FUN_00002f3c(pcVar13), iVar4 == 0)) &&
                                 (pcVar13[4] = '\0', *(short *)(pcVar13 + 0x23e) == -0x55ab)))) &&
                               ((*(int *)(pcVar13 + 0x40) == 0x41615252 &&
                                (*(int *)(pcVar13 + 0x224) == 0x61417272)))) {
                              cVar9 = '\x03';
                              *(undefined4 *)(pcVar13 + 0x10) = *(undefined4 *)(pcVar13 + 0x228);
                              *(uint *)(pcVar13 + 0xc) =
                                   CONCAT31(CONCAT21(*(undefined2 *)(pcVar13 + 0x22e),pcVar13[0x22d]
                                                    ),pcVar13[0x22c]);
                            }
                            else {
                              cVar9 = '\x03';
                            }
                          }
                          else {
                            if (pcVar7 < (char *)0xff6) {
                              *(char **)(pcVar13 + 0x14) = pcVar11;
                              *(int *)(pcVar13 + 0x1c) = iVar4;
                              *(int *)(pcVar13 + 0x20) = iVar12;
                              *(int *)(pcVar13 + 0x28) = iVar16;
                              if (uVar3 == 0) {
                                return 0xd;
                              }
                              *(int *)(pcVar13 + 0x24) = iVar17 + iVar12;
                              iVar4 = ((uint)pcVar11 & 1) + ((uint)((int)pcVar11 * 3) >> 1);
                              cVar9 = '\x01';
                            }
                            else {
                              *(char **)(pcVar13 + 0x14) = pcVar11;
                              *(int *)(pcVar13 + 0x1c) = iVar4;
                              *(int *)(pcVar13 + 0x20) = iVar12;
                              *(int *)(pcVar13 + 0x28) = iVar16;
                              if (uVar3 == 0) {
                                return 0xd;
                              }
                              iVar4 = (int)pcVar11 * 2;
                              *(int *)(pcVar13 + 0x24) = iVar17 + iVar12;
                              cVar9 = '\x02';
                            }
                            if (uVar6 < iVar4 + 0x1ffU >> 9) {
                              return 0xd;
                            }
                            pcVar13[0x10] = -1;
                            pcVar13[0x11] = -1;
                            pcVar13[0x12] = -1;
                            pcVar13[0x13] = -1;
                            pcVar13[0xc] = -1;
                            pcVar13[0xd] = -1;
                            pcVar13[0xe] = -1;
                            pcVar13[0xf] = -1;
                            pcVar13[4] = -0x80;
                          }
                          *pcVar13 = cVar9;
                          DAT_0002154c = DAT_0002154c + 1;
                          *(short *)(pcVar13 + 6) = DAT_0002154c;
                          return 0;
                        }
                      }
                    }
                  }
                }
              }
            }
            return 0xd;
          }
          uVar5 = 1;
        }
        else {
          uVar5 = 3;
        }
      }
      else if ((param_3 & 0xfe) == 0) {
        uVar5 = 0;
      }
      else {
        uVar5 = 0;
        if ((uVar6 & 4) != 0) {
          return 10;
        }
      }
    }
  }
  return uVar5;
}

