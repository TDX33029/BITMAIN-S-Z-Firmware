//==================== FUN_0400b0c0 @ 0400b0c0 ====================

uint FUN_0400b0c0(int param_1)

{
  int *piVar1;
  int extraout_r1;
  int extraout_r1_00;
  uint uVar2;
  uint uVar3;
  int iVar4;
  uint unaff_r4;
  uint local_810 [2];
  int local_808;
  int local_804;
  int local_7fc;
  uint local_7f8;
  uint local_7b0;
  
  FUN_04005ea4(local_810,param_1,100);
  piVar1 = DAT_0400b1dc;
  uVar2 = *DAT_0400b1d8;
  uVar3 = local_810[0] << 0x18 | (local_810[0] >> 8 & 0xff) << 0x10 |
          (local_810[0] >> 0x10 & 0xff) << 8 | local_810[0] >> 0x18;
  if (uVar3 >= uVar2 && uVar3 != uVar2) {
    unaff_r4 = 0xffffffff;
  }
  if (uVar2 < uVar3) {
    return unaff_r4;
  }
  if (uVar3 < uVar2) {
    return 0xffffff84;
  }
  if (local_808 == 0) {
    if (local_804 == 0) {
      if (local_7fc != 0) goto LAB_0400b1d0;
      if (local_7f8 == 0) {
        if ((*DAT_0400b1dc != 0) && (FUN_0400c7ec(*DAT_0400b1dc,DAT_0400b1e0), extraout_r1 == 0)) {
          func_0x03fc7fbc(DAT_0400b1e4);
        }
        FUN_0400c7ec(*piVar1,0x1400);
        if (extraout_r1_00 == 0) {
          func_0x03fc7f78(0x23);
        }
        iVar4 = *piVar1;
        uVar2 = local_7b0 << 0x18 | (local_7b0 >> 8 & 0xff) << 0x10 |
                (local_7b0 >> 0x10 & 0xff) << 8 | local_7b0 >> 0x18;
        uVar3 = uVar2 + iVar4;
        FUN_04005ea4(iVar4 + *DAT_0400b1e8,param_1 + 100,uVar2);
        if (*DAT_0400b1ec < uVar3) {
          *DAT_0400b1ec = uVar3;
          return uVar2;
        }
        return uVar2;
      }
    }
    else if (local_7fc != 0) goto LAB_0400b1d0;
    uVar2 = -(local_7f8 << 0x18 | (local_7f8 >> 8 & 0xff) << 0x10 | (local_7f8 >> 0x10 & 0xff) << 8
             | local_7f8 >> 0x18);
  }
  else {
LAB_0400b1d0:
    uVar2 = DAT_0400b1f0;
  }
  return uVar2;
}

