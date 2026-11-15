//==================== FUN_000042d4 @ 000042d4 ====================

int FUN_000042d4(undefined4 *param_1,undefined4 param_2,byte param_3)

{
  byte bVar1;
  undefined2 uVar2;
  ushort uVar3;
  int iVar4;
  undefined4 uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  undefined4 *puVar10;
  undefined4 *puVar9;
  byte bVar11;
  int iVar12;
  bool bVar13;
  undefined4 local_54;
  byte *local_50;
  byte *local_4c;
  byte local_46;
  int local_30;
  char local_21;
  
  local_54 = param_2;
  FUN_000027e0(&local_4c,0,0x2c);
  if (param_1 == (undefined4 *)0x0) {
    return 9;
  }
  bVar11 = param_3 & 0x3f;
  iVar4 = FUN_000030bc(&local_54,&local_50,bVar11);
  if (iVar4 != 0) goto LAB_00004320;
  local_4c = local_50;
  iVar4 = FUN_00003f2c(&local_4c,local_54);
  if (iVar4 == 0) {
    if (local_21 < '\0') {
      iVar4 = 6;
      goto LAB_00004320;
    }
    if ((param_3 & 0x1c) == 0) {
      if ((local_46 & 0x10) != 0) {
        iVar4 = 4;
        goto LAB_00004320;
      }
      if (((param_3 & 2) == 0) || ((local_46 & 1) == 0)) {
        iVar12 = *(int *)(local_50 + 0x2c);
        if ((param_3 & 8) == 0) goto LAB_000044dc;
        goto LAB_000044d8;
      }
    }
    else if ((local_46 & 0x11) == 0) {
      if ((param_3 & 4) != 0) {
        iVar4 = 8;
        goto LAB_00004320;
      }
      if ((param_3 & 8) != 0) goto LAB_000043e8;
      iVar12 = *(int *)(local_50 + 0x2c);
      goto LAB_000044dc;
    }
    iVar4 = 7;
  }
  else {
    if (((param_3 & 0x1c) == 0) || (iVar4 != 4)) goto LAB_00004394;
    bVar11 = bVar11 | 8;
    iVar4 = FUN_00003e38(&local_4c);
    if (iVar4 != 0) goto LAB_00004394;
LAB_000043e8:
    iVar4 = local_30;
    uVar6 = (uint)*local_50;
    bVar1 = *(byte *)(local_30 + 0x1b);
    bVar13 = uVar6 == 3;
    if (bVar13) {
      uVar6 = (uint)*(byte *)(local_30 + 0x15);
    }
    uVar8 = (uint)*(ushort *)(local_30 + 0x1a);
    if (bVar13) {
      bVar1 = *(byte *)(local_30 + 0x14);
    }
    if (bVar13) {
      uVar8 = uVar8 | ((uint)bVar1 | uVar6 << 8) << 0x10;
    }
    uVar5 = FUN_00006760();
    *(char *)(iVar4 + 0xe) = (char)uVar5;
    *(char *)(iVar4 + 0xf) = (char)((uint)uVar5 >> 8);
    *(char *)(iVar4 + 0x10) = (char)((uint)uVar5 >> 0x10);
    *(char *)(iVar4 + 0x11) = (char)((uint)uVar5 >> 0x18);
    *(undefined1 *)(local_30 + 0xb) = 0x20;
    *(undefined1 *)(local_30 + 0x1a) = 0;
    *(undefined1 *)(local_30 + 0x1b) = 0;
    if (*local_50 == 3) {
      *(undefined1 *)(local_30 + 0x14) = 0;
      *(undefined1 *)(local_30 + 0x15) = 0;
    }
    *(undefined1 *)(local_30 + 0x1c) = 0;
    *(undefined1 *)(local_30 + 0x1d) = 0;
    *(undefined1 *)(local_30 + 0x1e) = 0;
    *(undefined1 *)(local_30 + 0x1f) = 0;
    local_50[3] = 1;
    if (uVar8 == 0) {
LAB_000044d0:
      iVar12 = *(int *)(local_50 + 0x2c);
    }
    else {
      iVar12 = *(int *)(local_50 + 0x2c);
      iVar4 = FUN_00003a48(&local_4c,uVar8);
      if (iVar4 != 0) {
LAB_00004394:
        *param_1 = 0;
        return iVar4;
      }
      if (iVar12 != *(int *)(local_50 + 0x2c)) {
        iVar4 = FUN_00002f3c(local_50,iVar12);
        *(uint *)(local_50 + 0xc) = uVar8 - 1;
        if (iVar4 != 0) goto LAB_00004394;
        goto LAB_000044d0;
      }
      *(uint *)(local_50 + 0xc) = uVar8 - 1;
    }
LAB_000044d8:
    bVar11 = bVar11 | 0x40;
LAB_000044dc:
    param_1[8] = iVar12;
    param_1[9] = local_30;
    bVar1 = *(byte *)(local_30 + 0x1b);
    uVar6 = (uint)*local_50;
    bVar13 = uVar6 == 3;
    if (bVar13) {
      uVar6 = (uint)*(byte *)(local_30 + 0x15);
    }
    uVar8 = (uint)*(ushort *)(local_30 + 0x1a);
    if (bVar13) {
      bVar1 = *(byte *)(local_30 + 0x14);
    }
    if (bVar13) {
      uVar8 = uVar8 | ((uint)bVar1 | uVar6 << 8) << 0x10;
    }
    param_1[2] = uVar8;
    uVar5 = *(undefined4 *)(local_30 + 0x1c);
    *param_1 = local_50;
    uVar2 = *(undefined2 *)(local_50 + 6);
    *(byte *)(param_1 + 4) = bVar11;
    *(undefined2 *)(param_1 + 1) = uVar2;
    param_1[3] = uVar5;
    *(undefined1 *)((int)param_1 + 0x11) = 0;
    param_1[7] = 0;
    param_1[5] = 0;
    puVar10 = param_1 + 0x10;
    do {
      puVar9 = (undefined4 *)((int)puVar10 + 1);
      *(undefined1 *)puVar10 = 0;
      puVar10 = puVar9;
    } while (puVar9 != param_1 + 0x90);
    if ((bVar11 & 0x20) == 0) {
      return 0;
    }
    uVar6 = param_1[3];
    if (uVar6 == 0) {
      return 0;
    }
    uVar3 = *(ushort *)(local_50 + 10);
    uVar7 = param_1[2];
    param_1[5] = uVar6;
    uVar8 = (uint)uVar3 * 0x200;
    if (uVar8 < uVar6) {
      do {
        uVar6 = uVar6 + (uint)uVar3 * -0x200;
        uVar7 = FUN_000036fc(*param_1);
        if (uVar7 < 2) {
          param_1[6] = uVar7;
          goto LAB_000045e8;
        }
        bVar13 = 0xfffffffe < uVar7;
        if (uVar7 != 0xffffffff) {
          bVar13 = uVar8 <= uVar6;
        }
      } while (bVar13 && (uVar7 != 0xffffffff && uVar6 != uVar8));
      param_1[6] = uVar7;
      if (uVar7 == 0xffffffff) {
        iVar4 = 1;
        goto LAB_00004320;
      }
    }
    else {
      param_1[6] = uVar7;
    }
    if ((uVar6 & 0x1ff) == 0) {
      return 0;
    }
    if ((uVar7 - 2 < *(int *)(local_50 + 0x14) - 2U) &&
       (iVar4 = (uint)*(ushort *)(local_50 + 10) * (uVar7 - 2) + *(int *)(local_50 + 0x28),
       iVar4 != 0)) {
      iVar4 = iVar4 + (uVar6 >> 9);
      param_1[7] = iVar4;
      iVar4 = FUN_0000667c(local_50[1],param_1 + 0x10,iVar4,1);
      if (iVar4 == 0) {
        return 0;
      }
      iVar4 = 1;
    }
    else {
LAB_000045e8:
      iVar4 = 2;
    }
  }
LAB_00004320:
  *param_1 = 0;
  return iVar4;
}

