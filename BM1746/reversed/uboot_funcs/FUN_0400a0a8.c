//==================== FUN_0400a0a8 @ 0400a0a8 ====================

void FUN_0400a0a8(int param_1,uint param_2)

{
  ushort uVar1;
  ushort uVar2;
  ushort uVar3;
  int iVar4;
  ushort uVar5;
  byte *pbVar6;
  uint uVar7;
  ushort uVar8;
  ushort uVar9;
  int local_1c [2];
  
  *DAT_0400a3bc = param_1;
  *DAT_0400a3c0 = param_2;
  if (param_2 < 0xe) {
    return;
  }
  if ((code *)*DAT_0400a3c4 == (code *)0x0) {
    uVar8 = 0xfff;
    uVar2 = *DAT_0400a3c8 >> 8;
    uVar1 = *DAT_0400a3c8 << 8;
    uVar9 = uVar2 | uVar1;
    uVar3 = *DAT_0400a3cc >> 8 | *DAT_0400a3cc << 8;
    if (uVar9 == 0xffff) {
      uVar9 = 0xfff;
    }
    if (uVar3 == 0xffff) {
      uVar3 = 0xfff;
    }
    uVar5 = *(ushort *)(param_1 + 0xc) >> 8 | *(ushort *)(param_1 + 0xc) << 8;
    if (uVar5 < 0x5ea) {
      pbVar6 = (byte *)(param_1 + 0x16);
      param_2 = param_2 - 0x16;
      uVar5 = *(ushort *)(param_1 + 0x14) >> 8 | *(ushort *)(param_1 + 0x14) << 8;
    }
    else if (uVar5 == 0x8100) {
      if (param_2 < 0x12) {
        return;
      }
      if ((uVar2 | uVar1 & 0xfff) == 0xfff) {
        return;
      }
      pbVar6 = (byte *)(param_1 + 0x12);
      param_2 = param_2 - 0x12;
      uVar8 = *(ushort *)(param_1 + 0xe) >> 8 | (*(ushort *)(param_1 + 0xe) & 0xf) << 8;
      uVar5 = *(ushort *)(param_1 + 0x10) >> 8 | *(ushort *)(param_1 + 0x10) << 8;
    }
    else {
      pbVar6 = (byte *)(param_1 + 0xe);
      param_2 = param_2 - 0xe;
    }
    if ((uVar9 & 0xfff) != 0xfff) {
      if (uVar8 == 0xfff) {
        uVar8 = uVar3 & 0xfff;
      }
      if ((uVar9 & 0xfff) != uVar8) {
        return;
      }
    }
    if (uVar5 == 0x800) {
      if ((((0x1b < param_2) &&
           (uVar7 = (uint)(*(ushort *)(pbVar6 + 2) >> 8) | (*(ushort *)(pbVar6 + 2) & 0xff) << 8,
           (int)uVar7 <= (int)param_2)) && ((*pbVar6 & 0xf0) == 0x40)) &&
         ((((*pbVar6 & 0xf) < 6 && (iVar4 = FUN_04007cc8(pbVar6,0x14), iVar4 != 0)) &&
          ((FUN_04005ea4(local_1c,pbVar6 + 0x10,4),
           local_1c[0] == -1 || (*DAT_0400a3d0 == local_1c[0] || *DAT_0400a3d0 == 0) &&
           (FUN_04005ea4(local_1c,pbVar6 + 0xc,4),
           pbVar6 != (byte *)0x0 &&
           (*(ushort *)(pbVar6 + 6) >> 8 == 0 && (*(ushort *)(pbVar6 + 6) & 0x3f) == 0))))))) {
        if (pbVar6[9] == 1) {
          if (pbVar6[0x14] == 5) {
            if (pbVar6[0x15] == 1) {
              FUN_04007548(DAT_0400a3d4,pbVar6 + 0x18);
            }
          }
          else {
            FUN_0400b900(param_1,pbVar6,uVar7);
            if ((code *)*DAT_0400a3d8 != (code *)0x0) {
              (*(code *)*DAT_0400a3d8)
                        (pbVar6[0x14],pbVar6[0x15],
                         *(ushort *)(pbVar6 + 0x16) >> 8 | *(ushort *)(pbVar6 + 0x16) << 8,
                         local_1c[0],
                         *(ushort *)(pbVar6 + 0x14) >> 8 | *(ushort *)(pbVar6 + 0x14) << 8,
                         pbVar6 + 0x18,
                         *(ushort *)(pbVar6 + 0x18) >> 8 | *(ushort *)(pbVar6 + 0x18) << 8);
            }
          }
        }
        else if (pbVar6[9] == 0x11) {
          (*(code *)*DAT_0400a3dc)
                    (pbVar6 + 0x1c,*(ushort *)(pbVar6 + 0x16) >> 8 | *(ushort *)(pbVar6 + 0x16) << 8
                     ,local_1c[0],*(ushort *)(pbVar6 + 0x14) >> 8 | *(ushort *)(pbVar6 + 0x14) << 8,
                     ((uint)(*(ushort *)(pbVar6 + 0x18) >> 8) |
                     (*(ushort *)(pbVar6 + 0x18) & 0xff) << 8) - 8);
        }
      }
    }
    else if (uVar5 == 0x806) {
      FUN_04007efc(param_1,pbVar6);
    }
  }
  else {
    (*(code *)*DAT_0400a3c4)();
  }
  return;
}

