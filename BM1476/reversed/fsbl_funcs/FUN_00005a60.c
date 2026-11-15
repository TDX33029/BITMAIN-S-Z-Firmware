//==================== FUN_00005a60 @ 00005a60 ====================

undefined1
FUN_00005a60(undefined4 param_1,uint param_2,uint param_3,undefined4 *param_4,uint param_5)

{
  undefined1 uVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  char *pcVar5;
  undefined4 uVar6;
  uint uVar7;
  ushort *puVar8;
  uint uVar9;
  undefined1 uVar10;
  undefined4 *puVar12;
  undefined4 *puVar11;
  undefined1 uVar13;
  uint uVar14;
  uint uVar15;
  uint uVar16;
  uint uVar17;
  uint uVar18;
  undefined4 *puVar19;
  undefined1 uVar20;
  int iVar21;
  bool bVar22;
  int local_58;
  uint local_50;
  int local_4c;
  int local_48;
  uint local_44;
  undefined4 local_34;
  uint local_30;
  uint local_2c [2];
  
  local_34 = param_1;
  uVar3 = FUN_00002b44(&local_34);
  if ((int)uVar3 < 0) {
    return 0xb;
  }
  uVar15 = uVar3 & 0xff;
  if (*(undefined1 **)(&DAT_00021544 + uVar3 * 4) != (undefined1 *)0x0) {
    **(undefined1 **)(&DAT_00021544 + uVar3 * 4) = 0;
  }
  uVar3 = FUN_000065cc(uVar15);
  if ((uVar3 & 1) != 0) {
    return 3;
  }
  if ((uVar3 & 4) != 0) {
    return 10;
  }
  iVar4 = FUN_000066f8(uVar15,3,&local_30);
  if (((iVar4 != 0) || (0x7fff < local_30 - 1)) || ((local_30 & local_30 - 1) != 0)) {
    local_30 = 1;
  }
  if (param_3 != 0) {
    if (0xfffe00 < param_3 - 0x200) {
      return 0x13;
    }
    if ((param_3 - 1 & param_3) != 0) {
      return 0x13;
    }
  }
  uVar3 = param_5 >> 9;
  if (param_4 == (undefined4 *)0x0 || uVar3 == 0) {
    return 0x11;
  }
  iVar4 = FUN_000066f8(uVar15,1,local_2c);
  if (iVar4 != 0) {
    return true;
  }
  if ((param_2 & 8) == 0) {
    if (local_2c[0] < 0x3f) {
      return 0xe;
    }
    local_58 = 0x3f;
    uVar16 = local_2c[0] - 0x3f;
  }
  else {
    local_58 = 0;
    uVar16 = local_2c[0];
  }
  if (0x7f < uVar16) {
    param_3 = param_3 >> 9;
    if (0x80 < param_3) {
      return 0x13;
    }
    uVar14 = param_2 & 2;
    local_2c[0] = uVar16;
    if (uVar14 == 0) {
      if ((param_2 & 1) == 0) {
        return 0x13;
      }
    }
    else if (((param_2 & 7) == 2) || ((param_2 & 1) == 0)) {
      if (param_3 != 0) goto LAB_00005bac;
LAB_00005f58:
      if (uVar16 >> 0x11 == 0) {
        uVar18 = 1;
        param_3 = 0;
      }
      else {
        uVar18 = 2;
        uVar14 = 2;
        puVar8 = DAT_000064b0;
        do {
          if (uVar16 >> 0x11 < uVar14) {
            param_3 = 0;
            break;
          }
          puVar8 = puVar8 + 1;
          uVar14 = (uint)*puVar8;
          uVar18 = uVar18 << 1;
          param_3 = 0;
        } while (uVar14 != 0);
      }
      while( true ) {
        iVar4 = FUN_00001a50(uVar16,uVar18);
        local_50 = iVar4 * 4 + 0x207U >> 9;
        if (0xffeffff < iVar4 - 0xfff6U) {
          return 0xe;
        }
        iVar21 = local_50 + local_58 + 0x20;
        iVar4 = ((local_30 - 1) + iVar21 & -local_30) - iVar21;
        local_4c = local_58 + 0x20 + iVar4;
        iVar4 = iVar4 + 0x20;
        if (uVar16 < (uVar18 * 0x10 - local_58) + iVar21) {
          return 0xe;
        }
        pcVar5 = (char *)FUN_00001a50((uVar16 - iVar4) - local_50,uVar18);
        if (s_PS7_init_Data_Corrupted_0000fff0 + 6 <= pcVar5) break;
        if (param_3 != 0) {
          return 0xe;
        }
        param_3 = uVar18 >> 1;
        uVar14 = param_3;
joined_r0x00005ba8:
        if (uVar14 == 0) {
          return 0xe;
        }
LAB_00005bac:
        uVar18 = param_3;
      }
      local_48 = 3;
      local_44 = 0;
LAB_00005c58:
      puVar12 = param_4;
      do {
        puVar19 = (undefined4 *)((int)puVar12 + 1);
        *(undefined1 *)puVar12 = 0;
        puVar12 = puVar19;
      } while (puVar19 != param_4 + 0x80);
      *(char *)((int)param_4 + 0xf) = (char)((uint)iVar4 >> 8);
      param_4[2] = 0x302e35;
      if (local_48 == 3) {
        uVar20 = 0;
      }
      else {
        uVar20 = 2;
      }
      bVar22 = 0xffff < uVar16;
      uVar1 = (undefined1)(uVar16 >> 8);
      if (bVar22) {
        uVar13 = (char)(uVar16 >> 0x18);
      }
      else {
        uVar13 = uVar1;
      }
      *(undefined1 *)((int)param_4 + 0x11) = 0;
      uVar10 = (undefined1)uVar16;
      if (bVar22) {
        *(undefined1 *)((int)param_4 + 0x23) = uVar13;
      }
      else {
        *(undefined1 *)(param_4 + 5) = uVar13;
      }
      param_4[1] = 0x534f4453;
      if (bVar22) {
        *(undefined1 *)(param_4 + 8) = uVar10;
        uVar10 = (undefined1)(uVar16 >> 0x10);
      }
      *(undefined1 *)(param_4 + 4) = 1;
      if (bVar22) {
        *(undefined1 *)((int)param_4 + 0x21) = uVar1;
        *(undefined1 *)((int)param_4 + 0x22) = uVar10;
      }
      else {
        *(undefined1 *)((int)param_4 + 0x13) = uVar10;
      }
      *param_4 = 0x4d90feeb;
      *(char *)((int)param_4 + 0xe) = (char)iVar4;
      *(char *)((int)param_4 + 0xd) = (char)uVar18;
      *(undefined1 *)(param_4 + 6) = 0x3f;
      *(undefined1 *)((int)param_4 + 0x12) = uVar20;
      *(undefined1 *)(param_4 + 3) = 2;
      *(undefined1 *)(param_4 + 7) = (undefined1)local_58;
      *(undefined1 *)((int)param_4 + 0x15) = 0xf8;
      *(undefined1 *)((int)param_4 + 0x19) = 0;
      *(undefined1 *)((int)param_4 + 0x1b) = 0;
      *(undefined1 *)((int)param_4 + 0x1d) = 0;
      *(undefined1 *)((int)param_4 + 0x1e) = 0;
      *(undefined1 *)((int)param_4 + 0x1f) = 0;
      *(undefined1 *)((int)param_4 + 0x1a) = 0xff;
      uVar20 = (undefined1)(local_50 >> 8);
      if (local_48 == 3) {
        uVar6 = FUN_00006760();
        *(undefined1 *)(param_4 + 9) = (undefined1)local_50;
        *(char *)((int)param_4 + 0x43) = (char)uVar6;
        *(char *)((int)param_4 + 0x45) = (char)((uint)uVar6 >> 0x10);
        *(char *)(param_4 + 0x11) = (char)((uint)uVar6 >> 8);
        *(undefined4 *)((int)param_4 + 0x47) = 0x4e204f4e;
        *(char *)((int)param_4 + 0x46) = (char)((uint)uVar6 >> 0x18);
        *(undefined4 *)((int)param_4 + 0x4b) = 0x20454d41;
        *(undefined4 *)((int)param_4 + 0x53) = 0x32335441;
        *(char *)((int)param_4 + 0x26) = (char)(local_50 >> 0x10);
        *(undefined1 *)((int)param_4 + 0x2d) = 0;
        *(undefined1 *)((int)param_4 + 0x2e) = 0;
        *(undefined1 *)((int)param_4 + 0x2f) = 0;
        *(undefined1 *)((int)param_4 + 0x31) = 0;
        *(undefined1 *)((int)param_4 + 0x33) = 0;
        *(undefined4 *)((int)param_4 + 0x4f) = 0x46202020;
        *(undefined1 *)((int)param_4 + 0x25) = uVar20;
        *(char *)((int)param_4 + 0x27) = (char)(local_50 >> 0x18);
        *(undefined1 *)(param_4 + 0x10) = 0x80;
        *(undefined1 *)((int)param_4 + 0x42) = 0x29;
        *(undefined1 *)(param_4 + 0xb) = 2;
        *(undefined1 *)((int)param_4 + 0x32) = 6;
        *(undefined1 *)((int)param_4 + 0x59) = 0x20;
        *(undefined1 *)((int)param_4 + 0x1fe) = 0x55;
        *(undefined1 *)(param_4 + 0xc) = 1;
        *(undefined2 *)((int)param_4 + 0x57) = 0x2020;
        *(undefined1 *)((int)param_4 + 0x1ff) = 0xaa;
        iVar4 = FUN_0000676c(uVar15,param_4,local_58,1);
        if (iVar4 != 0) {
          return true;
        }
        FUN_0000676c(uVar15,param_4,local_58 + 6,1);
        puVar12 = param_4;
        do {
          puVar11 = (undefined4 *)((int)puVar12 + 1);
          *(undefined1 *)puVar12 = 0;
          puVar12 = puVar11;
        } while (puVar19 != puVar11);
        pcVar5 = pcVar5 + -1;
        *(char *)(param_4 + 0x7a) = (char)pcVar5;
        *(char *)((int)param_4 + 0x1e9) = (char)((uint)pcVar5 >> 8);
        *(char *)((int)param_4 + 0x1ea) = (char)((uint)pcVar5 >> 0x10);
        *(char *)((int)param_4 + 0x1eb) = (char)((uint)pcVar5 >> 0x18);
        *(undefined1 *)param_4 = 0x52;
        *(undefined1 *)((int)param_4 + 1) = 0x52;
        *(undefined1 *)((int)param_4 + 3) = 0x41;
        *(undefined1 *)((int)param_4 + 0x1e6) = 0x41;
        *(undefined1 *)(param_4 + 0x79) = 0x72;
        *(undefined1 *)((int)param_4 + 0x1e5) = 0x72;
        *(undefined1 *)(param_4 + 0x7b) = 2;
        *(undefined1 *)((int)param_4 + 0x1fe) = 0x55;
        *(undefined1 *)((int)param_4 + 0x1ed) = 0;
        *(undefined1 *)((int)param_4 + 0x1ee) = 0;
        *(undefined1 *)((int)param_4 + 0x1ef) = 0;
        *(undefined1 *)((int)param_4 + 2) = 0x61;
        *(undefined1 *)((int)param_4 + 0x1e7) = 0x61;
        *(undefined1 *)((int)param_4 + 0x1ff) = 0xaa;
        FUN_0000676c(uVar15,param_4,local_58 + 7,1);
        FUN_0000676c(uVar15,param_4,local_58 + 1);
      }
      else {
        uVar6 = FUN_00006760();
        *(char *)((int)param_4 + 0x27) = (char)uVar6;
        *(char *)(param_4 + 10) = (char)((uint)uVar6 >> 8);
        *(char *)((int)param_4 + 0x2a) = (char)((uint)uVar6 >> 0x18);
        *(char *)((int)param_4 + 0x29) = (char)((uint)uVar6 >> 0x10);
        *(undefined4 *)((int)param_4 + 0x2f) = 0x20454d41;
        *(undefined4 *)((int)param_4 + 0x33) = 0x46202020;
        *(undefined4 *)((int)param_4 + 0x2b) = 0x4e204f4e;
        *(undefined4 *)((int)param_4 + 0x37) = 0x20205441;
        *(undefined1 *)(param_4 + 9) = 0x80;
        *(undefined1 *)((int)param_4 + 0x26) = 0x29;
        *(undefined1 *)((int)param_4 + 0x17) = uVar20;
        *(undefined1 *)((int)param_4 + 0x3d) = 0x20;
        *(undefined1 *)((int)param_4 + 0x1fe) = 0x55;
        *(undefined1 *)((int)param_4 + 0x16) = (undefined1)local_50;
        *(undefined2 *)((int)param_4 + 0x3b) = 0x2020;
        *(undefined1 *)((int)param_4 + 0x1ff) = 0xaa;
        iVar4 = FUN_0000676c(uVar15,param_4,local_58,1);
        if (iVar4 != 0) {
          return true;
        }
      }
      puVar12 = param_4;
      do {
        puVar11 = (undefined4 *)((int)puVar12 + 1);
        *(undefined1 *)puVar12 = 0;
        puVar12 = puVar11;
      } while ((undefined4 *)((int)param_4 + (param_5 & 0xfffffe00)) != puVar11);
      if (local_48 == 3) {
        *(undefined1 *)param_4 = 0xf8;
        *(undefined1 *)((int)param_4 + 1) = 0xff;
        *(undefined1 *)((int)param_4 + 2) = 0xff;
        *(undefined1 *)((int)param_4 + 3) = 0xff;
        *(undefined1 *)(param_4 + 1) = 0xff;
        *(undefined1 *)((int)param_4 + 5) = 0xff;
        *(undefined1 *)((int)param_4 + 6) = 0xff;
        *(undefined1 *)((int)param_4 + 7) = 0xff;
        *(undefined1 *)(param_4 + 2) = 0xff;
        *(undefined1 *)((int)param_4 + 9) = 0xff;
        *(undefined1 *)((int)param_4 + 10) = 0xff;
        *(undefined1 *)((int)param_4 + 0xb) = 0xf;
      }
      else {
        if (local_48 == 1) {
          uVar20 = 0;
        }
        else {
          uVar20 = 0xff;
        }
        *(undefined1 *)param_4 = 0xf8;
        *(undefined1 *)((int)param_4 + 3) = uVar20;
        *(undefined1 *)((int)param_4 + 1) = 0xff;
        *(undefined1 *)((int)param_4 + 2) = 0xff;
      }
      while( true ) {
        if (local_50 < uVar3) {
          uVar16 = local_50;
        }
        else {
          uVar16 = uVar3;
        }
        iVar4 = FUN_0000676c(uVar15,param_4,local_4c,uVar16);
        if (iVar4 != 0) break;
        puVar12 = param_4;
        do {
          puVar11 = (undefined4 *)((int)puVar12 + 1);
          *(undefined1 *)puVar12 = 0;
          puVar12 = puVar11;
        } while (puVar19 != puVar11);
        local_50 = local_50 - uVar16;
        local_4c = local_4c + uVar16;
        if (local_50 == 0) {
          if (local_48 == 3) {
            local_44 = uVar18;
          }
          while( true ) {
            if (local_44 < uVar3) {
              uVar16 = local_44;
            }
            else {
              uVar16 = uVar3;
            }
            iVar4 = FUN_0000676c(uVar15,param_4,local_4c,uVar16);
            uVar14 = local_2c[0];
            if (iVar4 != 0) break;
            local_44 = local_44 - uVar16;
            local_4c = local_4c + uVar16;
            if (local_44 == 0) {
              if (local_48 == 3) {
                uVar20 = 0xc;
              }
              else if (local_2c[0] < 0x10000) {
                if (local_48 == 2) {
                  uVar20 = 4;
                }
                else {
                  uVar20 = 1;
                }
              }
              else {
                uVar20 = 6;
              }
              puVar12 = param_4;
              if ((param_2 & 8) == 0) {
                do {
                  puVar19 = (undefined4 *)((int)puVar12 + 1);
                  *(undefined1 *)puVar12 = 0;
                  puVar12 = puVar19;
                } while (puVar11 != puVar19);
                *(undefined1 *)((int)param_4 + 0x1c2) = uVar20;
                *(undefined1 *)((int)param_4 + 0x1fe) = 0x55;
                *(undefined1 *)((int)param_4 + 0x1ff) = 0xaa;
                *(undefined1 *)((int)param_4 + 0x1be) = 0;
                *(undefined1 *)((int)param_4 + 0x1c1) = 0;
                *(undefined1 *)((int)param_4 + 0x1bf) = 1;
                *(undefined1 *)(param_4 + 0x70) = 1;
                uVar3 = FUN_00001a50(local_2c[0] + local_58,0x3ec1);
                *(char *)((int)param_4 + 0x1c5) = (char)uVar3;
                *(char *)((int)param_4 + 0x1ca) = (char)uVar14;
                *(undefined1 *)((int)param_4 + 0x1c6) = (undefined1)local_58;
                *(byte *)(param_4 + 0x71) = (byte)(uVar3 >> 2) | 0x3f;
                *(char *)((int)param_4 + 0x1cb) = (char)(uVar14 >> 8);
                *(undefined1 *)((int)param_4 + 0x1c7) = 0;
                *(undefined1 *)(param_4 + 0x72) = 0;
                *(undefined1 *)((int)param_4 + 0x1c9) = 0;
                *(char *)(param_4 + 0x73) = (char)(uVar14 >> 0x10);
                *(char *)((int)param_4 + 0x1cd) = (char)(uVar14 >> 0x18);
                *(undefined1 *)((int)param_4 + 0x1c3) = 0xfe;
                iVar4 = FUN_0000676c(uVar15,param_4,0,1);
                if (iVar4 != 0) {
                  return true;
                }
              }
              iVar4 = FUN_000066f8(uVar15,0);
              return iVar4 != 0;
            }
          }
          return true;
        }
      }
      return true;
    }
    uVar2 = -local_30;
    iVar4 = local_30 - 1;
    local_4c = local_58 + 1;
    uVar17 = uVar15;
LAB_00005fdc:
    do {
      uVar18 = param_3;
      if (param_3 == 0) {
        if (uVar16 >> 0xc == 0) {
          uVar18 = 1;
        }
        else {
          uVar18 = 2;
          uVar7 = 4;
          puVar8 = DAT_000064b4;
          do {
            if (uVar16 >> 0xc < uVar7) break;
            puVar8 = puVar8 + 1;
            uVar7 = (uint)*puVar8;
            uVar18 = uVar18 << 1;
          } while (uVar7 != 0);
        }
      }
      uVar7 = FUN_00001a50(uVar16,uVar18);
      if (uVar7 < 0xff6) {
        uVar17 = 1;
        uVar9 = uVar7 * 3 + 1 >> 1;
      }
      else {
        uVar9 = (uVar7 + 2) * 2;
      }
      if (uVar7 < 0xff6) {
        uVar9 = uVar9 + 3;
      }
      else {
        uVar17 = 2;
      }
      local_50 = uVar9 + 0x1ff >> 9;
      iVar21 = local_50 + 0x20 + local_4c;
      local_50 = ((iVar4 + iVar21 & uVar2) - iVar21) + local_50;
      if (uVar16 < (uVar18 * 0x10 - local_58) + iVar21) {
        return 0xe;
      }
      pcVar5 = (char *)FUN_00001a50((uVar16 - local_50) + -0x21,uVar18);
      if (uVar17 != 2) {
        if ((char *)0xff5 < pcVar5) {
          return 0xe;
        }
        local_48 = 1;
        iVar4 = 1;
        local_44 = 0x20;
        goto LAB_00005c58;
      }
      if (pcVar5 < s_PS7_init_Data_Corrupted_0000fff0 + 6) {
        if ((char *)0xff5 < pcVar5) {
          iVar4 = 1;
          local_44 = 0x20;
          local_48 = 2;
          goto LAB_00005c58;
        }
        if (param_3 != 0) {
          return 0xe;
        }
        param_3 = uVar18 << 1;
      }
      else {
        if (param_3 != 0) goto joined_r0x00005ba8;
        param_3 = uVar18 * 2;
        if (param_3 < 0x41) goto LAB_00005fdc;
        if (uVar14 != 0) goto LAB_00005f58;
      }
    } while (param_3 < 0x81);
  }
  return 0xe;
}

