//==================== FUN_04003a68 @ 04003a68 ====================

/* WARNING: Type propagation algorithm not settling */

uint FUN_04003a68(int *param_1,uint param_2,uint param_3,undefined4 *param_4,uint param_5,
                 int param_6,undefined4 param_7)

{
  byte bVar1;
  byte *pbVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  byte *pbVar6;
  byte *pbVar7;
  byte *pbVar8;
  int iVar9;
  byte *pbVar10;
  uint uVar11;
  int iVar12;
  int iVar13;
  uint uVar14;
  int *piVar15;
  undefined4 auStack_40 [2];
  undefined1 auStack_38 [4];
  undefined4 *local_34;
  uint local_30;
  undefined1 *local_2c;
  uint local_28;
  int local_24;
  
  uVar14 = param_1[1];
  iVar13 = -(uVar14 * 4 + 10 & 0xfffffff8);
  local_2c = auStack_38 + iVar13;
  if (param_4 == (undefined4 *)0x0 || param_1 == (int *)0x0) {
    uVar14 = 0xffffffff;
    *DAT_04003d88 = 0x16;
  }
  else {
    local_34 = param_4;
    local_28 = param_3;
    if (uVar14 == 0) {
      uVar14 = 1;
      local_24 = 0;
    }
    else {
      uVar11 = 1;
      local_30 = param_3 & 8;
      iVar5 = 0;
      local_24 = 0;
LAB_04003af4:
      do {
        uVar3 = local_28;
        iVar9 = *param_1 + uVar11 * 0x14;
        if (0 < *(int *)(*param_1 + uVar11 * 0x14)) {
          *(undefined4 *)((int)auStack_40 + iVar13) = param_7;
          iVar4 = FUN_04003210(iVar9 + 4,iVar9 + 8,uVar3,param_6);
          if ((iVar4 != 0 || param_6 < 1) && ((local_30 == 0 || (**(char **)(iVar9 + 4) != '.')))) {
            *(int *)(local_2c + local_24 * 4) = iVar9 + 4;
            iVar4 = FUN_04005b98();
            iVar4 = iVar4 + iVar5 + 2;
            iVar12 = local_24 + 1;
            if (param_2 == 0) {
              iVar5 = FUN_04005b98(*(undefined4 *)(iVar9 + 8));
              iVar4 = iVar4 + iVar5;
            }
            else {
              pbVar10 = *(byte **)(iVar9 + 8);
              bVar1 = *pbVar10;
              while (uVar14 = (uint)bVar1, uVar14 != 0) {
                pbVar10 = pbVar10 + 1;
                bVar1 = *pbVar10;
                if (uVar14 == 0x5c || param_2 == uVar14) {
                  iVar4 = iVar4 + 2;
                }
                else {
                  iVar4 = iVar4 + 1;
                }
              }
            }
            uVar14 = param_1[1];
            uVar11 = uVar11 + 1;
            iVar5 = iVar4 + 2;
            local_24 = iVar12;
            if (uVar14 < uVar11) break;
            goto LAB_04003af4;
          }
          uVar14 = param_1[1];
        }
        uVar11 = uVar11 + 1;
      } while (uVar11 <= uVar14);
      uVar14 = iVar5 + 1;
    }
    func_0x03ffd878(local_2c,local_24,4,DAT_04003d80);
    if (param_5 != 0) {
      if (param_5 < uVar14) {
        FUN_04007548(DAT_04003d84,param_5,uVar14);
        *DAT_04003d88 = 0xc;
        return 0xffffffff;
      }
      uVar14 = param_5;
    }
    pbVar10 = (byte *)*local_34;
    if (pbVar10 == (byte *)0x0) {
      pbVar10 = (byte *)func_0x03fc8dc4(1,uVar14);
      *local_34 = pbVar10;
      if (pbVar10 == (byte *)0x0) {
        *DAT_04003d88 = 0xc;
        return 0xffffffff;
      }
    }
    else {
      FUN_04005e0c(pbVar10,0,uVar14);
    }
    if (local_24 != 0) {
      iVar13 = 0;
      piVar15 = (int *)(local_2c + -4);
      do {
        piVar15 = piVar15 + 1;
        pbVar6 = *(byte **)*piVar15;
        bVar1 = *pbVar6;
        while (bVar1 != 0) {
          *pbVar10 = bVar1;
          pbVar6 = pbVar6 + 1;
          pbVar10 = pbVar10 + 1;
          bVar1 = *pbVar6;
        }
        *pbVar10 = 0x3d;
        pbVar8 = pbVar10 + 1;
        uVar11 = (uint)**(byte **)(*piVar15 + 4);
        pbVar6 = *(byte **)(*piVar15 + 4);
        if (uVar11 == 0) {
          pbVar10 = pbVar10 + 2;
        }
        else {
          do {
            pbVar7 = pbVar8 + 1;
            pbVar10 = pbVar8 + 2;
            pbVar2 = pbVar8;
            if (uVar11 == 0x5c || param_2 == uVar11) {
              pbVar10 = pbVar8 + 3;
              *pbVar8 = 0x5c;
              pbVar2 = pbVar8 + 2;
            }
            pbVar8 = pbVar2;
            if (uVar11 != 0x5c && param_2 != uVar11) {
              pbVar8 = pbVar7;
              pbVar7 = pbVar2;
            }
            *pbVar7 = *pbVar6;
            uVar11 = (uint)pbVar6[1];
            pbVar6 = pbVar6 + 1;
          } while (uVar11 != 0);
        }
        iVar13 = iVar13 + 1;
        *pbVar8 = (byte)param_2;
      } while (iVar13 != local_24);
    }
    *pbVar10 = 0;
  }
  return uVar14;
}

