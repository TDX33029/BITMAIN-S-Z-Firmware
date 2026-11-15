//==================== FUN_0000158c @ 0000158c ====================

/* WARNING: Type propagation algorithm not settling */

ushort FUN_0000158c(int param_1)

{
  longlong lVar1;
  ushort uVar2;
  uint uVar3;
  int iVar4;
  ushort uVar5;
  char *pcVar6;
  byte bVar7;
  ushort uVar8;
  int iVar9;
  byte *pbVar10;
  ushort *puVar11;
  int iVar12;
  byte *pbVar13;
  char cVar14;
  byte *pbVar15;
  char local_134 [4];
  undefined4 local_130;
  byte local_12c [4];
  byte local_128 [6];
  ushort local_122;
  ushort local_120;
  uint local_d8;
  undefined2 local_d4;
  uint local_cc;
  int local_c8;
  byte local_c4;
  byte local_c3;
  byte bStack_2b;
  ushort local_2a;
  byte abStack_28 [4];
  
  local_130 = 8;
  if (param_1 == 0) {
    FUN_000014e4(s_0___P___p_0001720a + 0x26,0x1ee);
    DAT_00028e84 = 1;
    uVar5 = 0;
  }
  else {
    DAT_00028e84 = 0;
    func_0xfffff540(param_1,DAT_000019b4,0xffffffff);
    do {
      uVar3 = FUN_00001554(param_1);
    } while ((uVar3 & 0x40) == 0);
    func_0xfffff540(param_1,DAT_000019b8,0xffffffff,0x20);
    iVar4 = *(int *)(param_1 + 0x10);
    iVar9 = 0;
    pcVar6 = *(char **)(param_1 + 100);
    do {
      if (iVar4 == 0x10) {
        cVar14 = (char)*(undefined2 *)pcVar6;
      }
      else {
        cVar14 = *pcVar6;
      }
      local_134[iVar9] = cVar14;
      iVar9 = iVar9 + 1;
    } while (iVar9 != 4);
    if ((((local_134[0] == 'O') && (local_134[1] == 'N')) && (local_134[2] == 'F')) &&
       (local_134[3] == 'I')) {
      func_0xfffff540(param_1,DAT_000019bc,0xffffffff,0);
      do {
        uVar3 = FUN_00001554(param_1);
      } while ((uVar3 & 0x40) == 0);
      iVar9 = 3;
      **(undefined4 **)(param_1 + 0xc) = 0;
      iVar4 = *(int *)(param_1 + 0x10);
      pbVar13 = *(byte **)(param_1 + 100);
      do {
        pbVar15 = local_128;
        do {
          pbVar10 = pbVar15 + 1;
          if (iVar4 == 0x10) {
            bVar7 = (byte)*(undefined2 *)pbVar13;
          }
          else {
            bVar7 = *pbVar13;
          }
          *pbVar15 = bVar7;
          pbVar15 = pbVar10;
        } while (abStack_28 != pbVar10);
        pbVar15 = local_12c + 3;
        uVar5 = 0x4f4e;
        do {
          pbVar15 = pbVar15 + 1;
          iVar12 = 8;
          uVar3 = 0x80;
          uVar8 = uVar5;
          do {
            uVar5 = uVar8 << 1;
            uVar2 = ~uVar8;
            if ((*pbVar15 & uVar3) == 0) {
              uVar2 = uVar8;
            }
            uVar3 = uVar3 >> 1;
            if ((uVar2 & 0x8000) != 0) {
              uVar5 = uVar5 ^ 0x8005;
            }
            iVar12 = iVar12 + -1;
            uVar8 = uVar5;
          } while (iVar12 != 0);
        } while (&bStack_2b != pbVar15);
        if (local_2a == uVar5) {
          *(byte *)(param_1 + 0x28) = local_c4;
          DAT_0001974c = local_c3 >> 4;
          DAT_00019728 = local_c3 & 0xf;
          *(uint *)(param_1 + 0x20) = local_cc;
          DAT_0001971c = DAT_0001974c;
          *(undefined2 *)(param_1 + 0x1c) = local_d4;
          *(uint *)(param_1 + 0x18) = local_d8;
          *(undefined4 *)(param_1 + 0x34) = 0;
          *(undefined4 *)(param_1 + 0x3c) = 0;
          *(int *)(param_1 + 0x24) = local_c8;
          uVar3 = local_c8 * (uint)local_c4;
          *(undefined4 *)(param_1 + 0x44) = 0;
          *(byte *)(param_1 + 0x50) = DAT_00019728;
          DAT_00019740 = DAT_00019728 + DAT_0001974c;
          *(byte *)(param_1 + 0x51) = DAT_0001974c;
          lVar1 = (ulonglong)uVar3 * ((ulonglong)local_d8 * (ulonglong)local_cc & 0xffffffff);
          *(uint *)(param_1 + 0x38) = uVar3;
          DAT_00019710 = DAT_00019740;
          *(uint *)(param_1 + 0x30) = uVar3 * local_cc;
          *(uint *)(param_1 + 0x40) = local_d8 * local_cc;
          *(int *)(param_1 + 0x48) = (int)lVar1;
          *(uint *)(param_1 + 0x4c) =
               uVar3 * (int)((ulonglong)local_d8 * (ulonglong)local_cc >> 0x20) +
               (int)((ulonglong)lVar1 >> 0x20);
          func_0xfffff540(param_1,s__0001974f + 1,0xffffffff,0);
          puVar11 = *(ushort **)(param_1 + 100);
          if (*(int *)(param_1 + 0x10) == 0x10) {
            uVar5 = *puVar11;
            uVar8 = *puVar11;
          }
          else {
            uVar5 = (ushort)(byte)*puVar11;
            uVar8 = (ushort)(byte)*puVar11;
          }
          if (((uVar5 & 0xff) == 0x2c) &&
             (uVar5 = uVar8 & 0xef,
             uVar5 == 0xc3 ||
             ((uVar5 + 0x36 & 0xfd) == 0 ||
             ((uVar8 & 0xff) == 0xf1 || ((uVar8 & 0xed) == 0xa1 || (uVar5 + 0x56 & 0xfd) == 0))))) {
            func_0xfffff540(param_1,DAT_000019c0,0xffffffff,0x90);
            uVar5 = 8;
            pbVar13 = (byte *)((int)&local_130 + 1);
            while( true ) {
              if (*(int *)(param_1 + 0x10) == 0x10) {
                **(ushort **)(param_1 + 100) = uVar5;
              }
              else {
                *(char *)*(ushort **)(param_1 + 100) = (char)uVar5;
              }
              if (local_12c == pbVar13) break;
              uVar5 = (ushort)*pbVar13;
              pbVar13 = pbVar13 + 1;
            }
            do {
              uVar3 = FUN_00001554(param_1);
            } while ((uVar3 & 0x40) == 0);
            func_0xfffff540(param_1,DAT_000019c4,0xffffffff,0x90);
            do {
              uVar3 = FUN_00001554(param_1);
            } while ((uVar3 & 0x40) == 0);
            iVar9 = 0;
            **(undefined4 **)(param_1 + 0xc) = 0;
            iVar4 = *(int *)(param_1 + 0x10);
            pbVar13 = *(byte **)(param_1 + 100);
            do {
              if (iVar4 == 0x10) {
                bVar7 = (byte)*(undefined2 *)pbVar13;
              }
              else {
                bVar7 = *pbVar13;
              }
              local_12c[iVar9] = bVar7;
              iVar9 = iVar9 + 1;
            } while (iVar9 != 4);
            if ((local_12c[0] & 8) != 0) {
              *(undefined1 *)(param_1 + 0x6ecc) = 3;
              if ((local_122 & 1) == 0) {
                *(undefined1 *)(param_1 + 0x29) = 8;
                return local_122 & 1;
              }
              *(undefined1 *)(param_1 + 0x29) = 0x10;
              return 0;
            }
LAB_00001968:
            *(undefined1 *)(param_1 + 0x6ecc) = 2;
          }
          else {
            if (local_d8 - 0x200 < 0x601) goto LAB_00001968;
            *(undefined1 *)(param_1 + 0x6ecc) = 0;
          }
          if ((local_122 & 1) == 0) {
            *(undefined1 *)(param_1 + 0x29) = 8;
          }
          else {
            *(undefined1 *)(param_1 + 0x29) = 0x10;
          }
          *(uint *)(param_1 + 0x58) = local_120 & 1;
          *(uint *)(param_1 + 0x5c) = (local_120 & 3) >> 1;
          return 0;
        }
        iVar9 = iVar9 + -1;
      } while (iVar9 != 0);
    }
    uVar5 = 1;
  }
  return uVar5;
}

