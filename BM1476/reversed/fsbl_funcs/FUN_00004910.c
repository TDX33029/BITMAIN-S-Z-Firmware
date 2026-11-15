//==================== FUN_00004910 @ 00004910 ====================

uint FUN_00004910(int param_1,undefined4 *param_2,uint param_3,int *param_4)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  undefined4 *puVar6;
  byte bVar7;
  int iVar8;
  undefined4 *puVar9;
  undefined4 *puVar10;
  uint uVar11;
  uint uVar12;
  undefined4 *puVar13;
  undefined1 *puVar14;
  int local_2c [2];
  
  *param_4 = 0;
  uVar1 = FUN_00002bbc(param_1,local_2c);
  if ((uVar1 == 0) && (uVar1 = (uint)*(byte *)(param_1 + 0x11), uVar1 == 0)) {
    bVar7 = *(byte *)(param_1 + 0x10);
    if ((bVar7 & 2) == 0) {
      uVar1 = 7;
    }
    else {
      uVar4 = *(uint *)(param_1 + 0x14);
      if (CARRY4(uVar4,param_3)) {
        param_3 = ~uVar4;
      }
      if (param_3 != 0) {
        iVar8 = param_1 + 0x40;
        do {
          uVar12 = uVar4 & 0x1ff;
          if (uVar12 == 0) {
            uVar12 = *(ushort *)(local_2c[0] + 10) - 1 & uVar4 >> 9;
            if (uVar12 == 0) {
              if (uVar4 == 0) {
                iVar2 = *(int *)(param_1 + 8);
                if (iVar2 == 0) {
                  iVar2 = FUN_00003868(param_1,0);
                  goto LAB_00004ba0;
                }
              }
              else {
                iVar2 = FUN_00003868(param_1,*(undefined4 *)(param_1 + 0x18));
LAB_00004ba0:
                if (iVar2 == 0) break;
              }
              if (iVar2 == 1) goto LAB_00004c10;
              if (iVar2 == -1) goto LAB_00004a3c;
              *(int *)(param_1 + 0x18) = iVar2;
              if (*(int *)(param_1 + 8) == 0) {
                *(int *)(param_1 + 8) = iVar2;
              }
            }
            if (*(char *)(param_1 + 0x10) < '\0') {
              iVar2 = FUN_0000676c(*(undefined1 *)(local_2c[0] + 1),iVar8,
                                   *(undefined4 *)(param_1 + 0x1c),1);
              if (iVar2 != 0) goto LAB_00004a3c;
              *(byte *)(param_1 + 0x10) = *(byte *)(param_1 + 0x10) & 0x7f;
            }
            uVar4 = *(int *)(param_1 + 0x18) - 2;
            if (*(int *)(local_2c[0] + 0x14) - 2U <= uVar4) {
LAB_00004c10:
              *(undefined1 *)(param_1 + 0x11) = 2;
              return 2;
            }
            uVar5 = (uint)*(ushort *)(local_2c[0] + 10);
            iVar2 = uVar5 * uVar4 + *(int *)(local_2c[0] + 0x28);
            if (iVar2 == 0) goto LAB_00004c10;
            uVar11 = param_3 >> 9;
            iVar2 = uVar12 + iVar2;
            if (uVar11 == 0) {
              uVar4 = *(uint *)(param_1 + 0x14);
              if ((*(int *)(param_1 + 0x1c) != iVar2) && (uVar4 < *(uint *)(param_1 + 0xc))) {
                iVar3 = FUN_0000667c(*(undefined1 *)(local_2c[0] + 1),iVar8,iVar2,1);
                if (iVar3 != 0) goto LAB_00004a3c;
                uVar4 = *(uint *)(param_1 + 0x14);
              }
              uVar12 = uVar4 & 0x1ff;
              *(int *)(param_1 + 0x1c) = iVar2;
              goto LAB_00004a74;
            }
            if (uVar5 < uVar12 + uVar11) {
              uVar11 = uVar5 - uVar12;
            }
            iVar3 = FUN_0000676c(*(undefined1 *)(local_2c[0] + 1),param_2,iVar2,uVar11);
            if (iVar3 != 0) {
LAB_00004a3c:
              *(undefined1 *)(param_1 + 0x11) = 1;
              return 1;
            }
            uVar4 = *(int *)(param_1 + 0x1c) - iVar2;
            if (uVar4 < uVar11) {
              puVar6 = param_2 + uVar4 * 0x80 + -1;
              puVar13 = (undefined4 *)(param_1 + 0x40);
              do {
                puVar6 = puVar6 + 1;
                puVar10 = puVar13 + 1;
                *puVar13 = *puVar6;
                puVar13 = puVar10;
              } while (puVar10 != (undefined4 *)(param_1 + 0x240));
              *(byte *)(param_1 + 0x10) = *(byte *)(param_1 + 0x10) & 0x7f;
            }
            uVar11 = uVar11 << 9;
            uVar4 = *(uint *)(param_1 + 0x14);
          }
          else {
LAB_00004a74:
            puVar13 = (undefined4 *)(iVar8 + uVar12);
            uVar11 = 0x200 - uVar12;
            if (param_3 <= 0x200 - uVar12) {
              uVar11 = param_3;
            }
            if (uVar11 < 4) {
              uVar12 = uVar11;
              puVar6 = param_2;
LAB_00004b28:
              puVar14 = (undefined1 *)((int)puVar13 + -1);
              puVar13 = puVar6;
              do {
                puVar10 = (undefined4 *)((int)puVar13 + 1);
                puVar14 = puVar14 + 1;
                *puVar14 = *(undefined1 *)puVar13;
                puVar13 = puVar10;
              } while (puVar10 != (undefined4 *)((int)puVar6 + uVar12));
              uVar4 = *(uint *)(param_1 + 0x14);
            }
            else {
              uVar12 = uVar11 - 4 >> 2;
              puVar6 = param_2;
              puVar10 = puVar13;
              do {
                puVar9 = puVar6 + 1;
                *puVar10 = *puVar6;
                puVar6 = puVar9;
                puVar10 = puVar10 + 1;
              } while (param_2 + uVar12 + 1 != puVar9);
              iVar2 = uVar12 + 1;
              puVar13 = puVar13 + iVar2;
              uVar12 = uVar11 & 3;
              puVar6 = param_2 + iVar2;
              if ((uVar11 & 3) != 0) goto LAB_00004b28;
            }
            *(byte *)(param_1 + 0x10) =
                 ~((byte)~(byte)(((uint)*(byte *)(param_1 + 0x10) << 0x19) >> 0x18) >> 1);
          }
          uVar4 = uVar4 + uVar11;
          param_2 = (undefined4 *)((int)param_2 + uVar11);
          uVar12 = *(uint *)(param_1 + 0xc);
          if (*(uint *)(param_1 + 0xc) < uVar4) {
            uVar12 = uVar4;
          }
          param_3 = param_3 - uVar11;
          *param_4 = *param_4 + uVar11;
          *(uint *)(param_1 + 0xc) = uVar12;
          *(uint *)(param_1 + 0x14) = uVar4;
        } while (param_3 != 0);
        bVar7 = *(byte *)(param_1 + 0x10);
      }
      *(byte *)(param_1 + 0x10) = bVar7 | 0x40;
    }
  }
  return uVar1;
}

