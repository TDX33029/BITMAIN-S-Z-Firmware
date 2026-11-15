//==================== FUN_04003d8c @ 04003d8c ====================

undefined4
FUN_04003d8c(int *param_1,undefined4 param_2,uint param_3,byte *param_4,uint param_5,int param_6,
            int param_7,undefined4 param_8)

{
  bool bVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  byte *pbVar4;
  byte *pbVar5;
  undefined4 uVar6;
  uint uVar7;
  byte bVar8;
  byte *pbVar9;
  byte *pbVar10;
  int iVar11;
  byte *pbVar12;
  byte *pbVar13;
  int *piVar14;
  byte *pbVar15;
  byte *pbVar16;
  int iVar17;
  byte *pbVar18;
  bool bVar19;
  bool bVar20;
  int aiStack_58 [4];
  undefined1 auStack_48 [4];
  byte *local_44;
  byte *local_40;
  undefined1 *local_3c;
  int local_34;
  byte *local_30;
  byte *local_2c;
  undefined4 uStack_28;
  undefined4 uStack_24;
  
  iVar17 = -(param_7 * 4 + 10U & 0xfffffff8);
  local_3c = auStack_48 + iVar17;
  if (param_1 == (int *)0x0) {
    *DAT_04004214 = 0x16;
    return 0;
  }
  local_40 = (byte *)func_0x03fc87b0(param_3 + 1);
  if (local_40 == (byte *)0x0) {
    *DAT_04004214 = 0xc;
    return 0;
  }
  pbVar5 = (byte *)FUN_04005ea4(local_40,param_2,param_3);
  local_40[param_3] = 0;
  pbVar16 = local_40 + param_3;
  if (param_7 != 0) {
    pbVar5 = (byte *)FUN_04005ea4(local_3c,param_8,param_7 * 4);
  }
  iVar11 = *param_1;
  if ((param_5 & 1) == 0) {
    if (iVar11 != 0) {
      pbVar5 = (byte *)FUN_0400339c(param_1);
      iVar11 = *param_1;
      goto LAB_04003e10;
    }
  }
  else {
LAB_04003e10:
    if (iVar11 != 0) goto LAB_04003e18;
  }
  uVar7 = (param_3 >> 3) + 0x40;
  if (0x1ff < uVar7) {
    uVar7 = 0x200;
  }
  pbVar5 = (byte *)FUN_04003300(uVar7,param_1);
  if (pbVar5 == (byte *)0x0) {
    func_0x03fc8570(local_40);
    return 0;
  }
LAB_04003e18:
  if (param_3 == 0) {
    func_0x03fc8570(local_40);
    return 1;
  }
  pbVar12 = (byte *)(uint)*local_40;
  if (((param_6 != 0) && (local_40 < pbVar16)) && (pbVar12 != (byte *)0x0)) {
    pbVar5 = (byte *)0x0;
    pbVar9 = local_40;
    do {
      if (((pbVar12 == (byte *)0xd) && (pbVar9 < local_40 + (param_3 - 1))) && (pbVar9[1] == 10)) {
        pbVar5 = pbVar5 + 1;
      }
      else {
        pbVar9[-(int)pbVar5] = (byte)pbVar12;
      }
      if (pbVar16 + -1 == pbVar9) break;
      pbVar9 = pbVar9 + 1;
      pbVar12 = (byte *)(uint)*pbVar9;
    } while (pbVar12 != (byte *)0x0);
    param_3 = param_3 - (int)pbVar5;
    pbVar12 = (byte *)(uint)*local_40;
  }
  pbVar9 = local_40 + param_3;
  pbVar16 = local_40;
  do {
    while (pbVar13 = pbVar16 + 1, pbVar12 == (byte *)0x9 || pbVar12 == (byte *)0x20) {
      pbVar12 = (byte *)(uint)pbVar16[1];
      pbVar16 = pbVar13;
    }
    if (pbVar12 == (byte *)0x23) {
      if (param_4 != (byte *)0x23) {
        while (pbVar12 = pbVar13,
              (byte *)(uint)*pbVar12 != (byte *)0x0 && (byte *)(uint)*pbVar12 != param_4) {
          pbVar13 = pbVar12 + 1;
          pbVar16 = pbVar12;
        }
        pbVar13 = pbVar16 + 2;
      }
LAB_04003f5c:
    }
    else {
      pbVar10 = pbVar16;
      if (pbVar12 != (byte *)0x0 && pbVar12 != (byte *)0x3d) {
        if (param_4 != pbVar12) {
          do {
            pbVar10 = pbVar13;
            pbVar13 = pbVar10 + 1;
            pbVar12 = (byte *)(uint)*pbVar10;
            if (pbVar12 == (byte *)0x3d || pbVar12 == (byte *)0x0) goto LAB_04003f1c;
          } while (pbVar12 != param_4);
          pbVar12 = param_4;
        }
LAB_04003ff0:
        pbVar15 = pbVar13;
        if (pbVar12 == (byte *)0x3d) {
          *pbVar10 = 0;
          pbVar15 = pbVar13 + 1;
          pbVar10 = pbVar13;
        }
LAB_04003f24:
        *pbVar10 = 0;
        pbVar13 = pbVar15;
        if (param_7 != 0) {
          iVar11 = FUN_040030f8(pbVar16,param_7,local_3c);
          pbVar5 = (byte *)0x0;
          if (iVar11 == 0) goto LAB_04003f5c;
        }
        pbVar5 = (byte *)FUN_04003954(pbVar16,param_1,param_5);
        goto LAB_04003f5c;
      }
LAB_04003f1c:
      pbVar15 = pbVar13;
      if (pbVar12 == (byte *)0x0) goto LAB_04003f24;
      pbVar15 = (byte *)(uint)pbVar10[1];
      bVar19 = pbVar15 == (byte *)0x0;
      bVar1 = pbVar15 == param_4;
      bVar20 = param_4 == pbVar12;
      if (!bVar20 && (!bVar19 && !bVar1)) {
        pbVar5 = pbVar13;
      }
      if (!bVar20 && (!bVar19 && !bVar1)) {
        *pbVar10 = bVar20 || (bVar19 || bVar1);
      }
      if (!bVar20 && (!bVar19 && !bVar1)) {
        pbVar12 = pbVar13;
      }
      if (!bVar20 && (!bVar19 && !bVar1)) {
        local_44 = pbVar13;
      }
      pbVar4 = local_44;
      if (bVar20 || (bVar19 || bVar1)) goto LAB_04003ff0;
      do {
        pbVar10 = pbVar12 + 1;
        pbVar18 = pbVar12 + 2;
        pbVar13 = pbVar12;
        if (pbVar15 == (byte *)0x5c) {
          bVar8 = pbVar12[1];
          local_44 = pbVar12 + 3;
          if (bVar8 == 0) {
            bVar8 = *pbVar12;
            pbVar12 = pbVar10;
            local_44 = pbVar18;
          }
          else {
            pbVar13 = pbVar10;
            pbVar12 = pbVar18;
          }
        }
        else {
          bVar8 = *pbVar12;
          pbVar12 = pbVar10;
          local_44 = pbVar18;
        }
        pbVar10 = pbVar5 + 1;
        *pbVar5 = bVar8;
        pbVar15 = (byte *)(uint)pbVar13[1];
        pbVar5 = pbVar10;
      } while (pbVar15 != (byte *)0x0 && pbVar15 != param_4);
      *pbVar10 = 0;
      if (*pbVar16 == 0) {
        *DAT_04004214 = 0x16;
        func_0x03fc8570(local_40);
        return 0;
      }
      if ((param_7 != 0) && (iVar11 = FUN_040030f8(pbVar16,param_7,local_3c), iVar11 == 0)) {
        pbVar5 = (byte *)0x0;
        pbVar13 = local_44;
        goto LAB_04003f5c;
      }
      local_2c = pbVar4;
      local_30 = pbVar16;
      *(uint *)((int)aiStack_58 + iVar17 + 0xc) = param_5;
      *(int **)((int)aiStack_58 + iVar17 + 8) = param_1;
      uVar3 = uStack_24;
      uVar2 = uStack_28;
      pbVar12 = local_2c;
      pbVar5 = local_30;
      *(undefined4 *)((int)aiStack_58 + iVar17) = 1;
      *(int **)((int)aiStack_58 + iVar17 + 4) = &local_34;
      pbVar5 = (byte *)FUN_040034a8(pbVar5,pbVar12,uVar2,uVar3);
      if (local_34 == 0) {
        pbVar5 = (byte *)FUN_04007548(DAT_04004220,pbVar16,pbVar4);
      }
      pbVar13 = local_44;
    }
    if (pbVar9 <= pbVar13) break;
    pbVar12 = (byte *)(uint)*pbVar13;
    pbVar16 = pbVar13;
  } while (pbVar12 != (byte *)0x0);
  func_0x03fc8570(local_40);
  uVar3 = DAT_0400421c;
  uVar2 = DAT_04004218;
  if (0 < param_7) {
    iVar17 = 0;
    piVar14 = (int *)(local_3c + -4);
    do {
      piVar14 = piVar14 + 1;
      if (*piVar14 != 0) {
        iVar11 = FUN_04003954(*piVar14,param_1,param_5);
        uVar6 = uVar3;
        if (iVar11 != 0) {
          uVar6 = uVar2;
        }
        FUN_04007548(uVar6,*piVar14);
      }
      iVar17 = iVar17 + 1;
    } while (param_7 != iVar17);
  }
  return 1;
}

