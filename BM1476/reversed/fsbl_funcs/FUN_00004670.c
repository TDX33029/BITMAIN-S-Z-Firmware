//==================== FUN_00004670 @ 00004670 ====================

uint FUN_00004670(undefined4 *param_1,undefined4 *param_2,uint param_3,int *param_4)

{
  undefined1 uVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  undefined4 *puVar6;
  undefined4 *puVar7;
  undefined1 *puVar8;
  uint uVar9;
  undefined4 *puVar10;
  uint uVar11;
  uint uVar12;
  undefined4 *puVar13;
  undefined4 *puVar14;
  uint uVar15;
  int local_2c [2];
  
  *param_4 = 0;
  uVar2 = FUN_00002bbc(param_1,local_2c);
  if ((uVar2 == 0) && (uVar2 = (uint)*(byte *)((int)param_1 + 0x11), uVar2 == 0)) {
    if ((*(byte *)(param_1 + 4) & 1) == 0) {
      uVar2 = 7;
    }
    else {
      uVar15 = param_1[5];
      uVar9 = param_1[3] - uVar15;
      if (param_3 <= param_1[3] - uVar15) {
        uVar9 = param_3;
      }
      if (uVar9 != 0) {
        puVar10 = param_1 + 0x10;
        do {
          uVar12 = uVar15 & 0x1ff;
          if (uVar12 == 0) {
            uVar12 = *(ushort *)(local_2c[0] + 10) - 1 & uVar15 >> 9;
            if (uVar12 == 0) {
              if (uVar15 == 0) {
                uVar15 = param_1[2];
              }
              else {
                uVar15 = FUN_000036fc(*param_1,param_1[6]);
              }
              if (uVar15 < 2) goto LAB_00004904;
              if (uVar15 == 0xffffffff) goto LAB_00004758;
              param_1[6] = uVar15;
            }
            else {
              uVar15 = param_1[6];
            }
            if (*(int *)(local_2c[0] + 0x14) - 2U <= uVar15 - 2) {
LAB_00004904:
              *(undefined1 *)((int)param_1 + 0x11) = 2;
              return 2;
            }
            uVar5 = (uint)*(ushort *)(local_2c[0] + 10);
            iVar3 = uVar5 * (uVar15 - 2) + *(int *)(local_2c[0] + 0x28);
            if (iVar3 == 0) goto LAB_00004904;
            uVar11 = uVar9 >> 9;
            iVar3 = uVar12 + iVar3;
            if (uVar11 == 0) {
              if (param_1[7] != iVar3) {
                uVar1 = *(undefined1 *)(local_2c[0] + 1);
                if (*(char *)(param_1 + 4) < '\0') {
                  iVar4 = FUN_0000676c(uVar1,puVar10,param_1[7],1);
                  if (iVar4 != 0) goto LAB_00004758;
                  *(byte *)(param_1 + 4) = *(byte *)(param_1 + 4) & 0x7f;
                  uVar1 = *(undefined1 *)(local_2c[0] + 1);
                }
                iVar4 = FUN_0000667c(uVar1,puVar10,iVar3,1);
                if (iVar4 != 0) goto LAB_00004758;
              }
              uVar15 = param_1[5];
              param_1[7] = iVar3;
              uVar12 = uVar15 & 0x1ff;
              goto LAB_000047ac;
            }
            if (uVar5 < uVar12 + uVar11) {
              uVar11 = uVar5 - uVar12;
            }
            iVar4 = FUN_0000667c(*(undefined1 *)(local_2c[0] + 1),param_2,iVar3,uVar11);
            if (iVar4 != 0) {
LAB_00004758:
              *(undefined1 *)((int)param_1 + 0x11) = 1;
              return 1;
            }
            if ((*(char *)(param_1 + 4) < '\0') &&
               (iVar4 = param_1[7], (uint)(iVar4 - iVar3) < uVar11)) {
              puVar6 = param_1 + 0xf;
              puVar13 = param_2 + (iVar4 - iVar3) * 0x80;
              do {
                puVar6 = puVar6 + 1;
                puVar14 = puVar13 + 1;
                *puVar13 = *puVar6;
                puVar13 = puVar14;
              } while (puVar14 != param_2 + (iVar4 - iVar3) * 0x80 + 0x80);
            }
            uVar11 = uVar11 << 9;
            uVar15 = param_1[5];
          }
          else {
LAB_000047ac:
            puVar13 = (undefined4 *)((int)puVar10 + uVar12);
            uVar11 = 0x200 - uVar12;
            if (uVar9 <= 0x200 - uVar12) {
              uVar11 = uVar9;
            }
            if (uVar11 < 4) {
              uVar12 = uVar11;
              puVar6 = param_2;
            }
            else {
              uVar12 = uVar11 - 4 >> 2;
              puVar6 = puVar13;
              puVar14 = param_2;
              do {
                puVar7 = puVar6 + 1;
                *puVar14 = *puVar6;
                puVar6 = puVar7;
                puVar14 = puVar14 + 1;
              } while (puVar13 + uVar12 + 1 != puVar7);
              iVar3 = uVar12 + 1;
              puVar13 = puVar13 + iVar3;
              uVar12 = uVar11 & 3;
              puVar6 = param_2 + iVar3;
              if ((uVar11 & 3) == 0) goto LAB_00004804;
            }
            puVar8 = (undefined1 *)((int)puVar6 + -1);
            puVar6 = puVar13;
            do {
              puVar14 = (undefined4 *)((int)puVar6 + 1);
              puVar8 = puVar8 + 1;
              *puVar8 = *(undefined1 *)puVar6;
              puVar6 = puVar14;
            } while (puVar14 != (undefined4 *)((int)puVar13 + uVar12));
            uVar15 = param_1[5];
          }
LAB_00004804:
          uVar9 = uVar9 - uVar11;
          uVar15 = uVar15 + uVar11;
          param_2 = (undefined4 *)((int)param_2 + uVar11);
          *param_4 = *param_4 + uVar11;
          param_1[5] = uVar15;
        } while (uVar9 != 0);
      }
    }
  }
  return uVar2;
}

