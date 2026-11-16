//==================== FUN_00002200 @ 00002200 ====================

void FUN_00002200(uint *param_1,uint *param_2,uint param_3)

{
  byte *pbVar1;
  byte *pbVar2;
  uint *puVar3;
  int *piVar4;
  int *piVar5;
  undefined1 uVar6;
  undefined2 uVar7;
  ushort uVar8;
  uint uVar9;
  int iVar10;
  uint uVar11;
  uint uVar12;
  uint uVar13;
  uint uVar14;
  uint uVar15;
  uint uVar16;
  uint uVar17;
  int iVar18;
  undefined2 *puVar19;
  undefined2 *puVar20;
  uint *puVar21;
  int *piVar22;
  int *piVar23;
  uint *puVar24;
  bool bVar25;
  bool bVar26;
  undefined8 uVar27;
  undefined8 uVar28;
  undefined8 uVar29;
  undefined8 uVar30;
  undefined8 uVar31;
  undefined8 uVar32;
  undefined8 uVar33;
  undefined8 uVar34;
  
  if (0x3f < (int)param_3) {
    uVar9 = (uint)param_1 & 7;
    if (uVar9 == ((uint)param_2 & 7)) {
      if (((uint)param_1 & 7) != 0) {
        uVar11 = (int)param_1 * -0x20000000;
        param_3 = param_3 - (uVar11 >> 0x1d);
        puVar24 = param_1;
        puVar21 = param_2;
        if ((int)uVar11 < 0) {
          puVar21 = param_2 + 1;
          uVar9 = *param_2;
          puVar24 = param_1 + 1;
          *param_1 = uVar9;
        }
        uVar8 = (ushort)uVar9;
        bVar26 = (bool)((byte)(uVar11 >> 0x1e) & 1);
        bVar25 = ((uint)param_1 & 1) != 0;
        puVar3 = puVar21;
        if (bVar26) {
          puVar3 = (uint *)((int)puVar21 + 2);
          uVar8 = (ushort)*puVar21;
        }
        uVar6 = 0;
        param_2 = puVar3;
        if (bVar25) {
          param_2 = (uint *)((int)puVar3 + 1);
          uVar6 = (undefined1)*puVar3;
        }
        puVar21 = puVar24;
        if (bVar26) {
          puVar21 = (uint *)((int)puVar24 + 2);
          *(ushort *)puVar24 = uVar8;
        }
        param_1 = puVar21;
        if (bVar25) {
          param_1 = (uint *)((int)puVar21 + 1);
          *(undefined1 *)puVar21 = uVar6;
        }
      }
      uVar9 = param_3 - 0x40;
      if (0x3f < (int)param_3) {
        uVar11 = uVar9;
        if (0x1ff < (int)uVar9) {
          uVar30 = *(undefined8 *)param_2;
          uVar31 = *(undefined8 *)(param_2 + 0x10);
          uVar32 = *(undefined8 *)(param_2 + 0x20);
          uVar33 = *(undefined8 *)(param_2 + 0x30);
          uVar34 = *(undefined8 *)(param_2 + 0x40);
          uVar27 = *(undefined8 *)(param_2 + 2);
          uVar28 = *(undefined8 *)(param_2 + 4);
          uVar29 = *(undefined8 *)(param_2 + 6);
          param_2 = param_2 + 8;
          iVar18 = param_3 - 0x2c0;
          iVar10 = iVar18;
          if (0x27f < (int)uVar9) {
            do {
              *(undefined8 *)param_1 = uVar30;
              uVar30 = *(undefined8 *)param_2;
              *(undefined8 *)(param_1 + 2) = uVar27;
              uVar27 = *(undefined8 *)(param_2 + 2);
              *(undefined8 *)(param_1 + 4) = uVar28;
              uVar28 = *(undefined8 *)(param_2 + 4);
              *(undefined8 *)(param_1 + 6) = uVar29;
              uVar29 = *(undefined8 *)(param_2 + 6);
              *(undefined8 *)(param_1 + 8) = uVar30;
              uVar30 = *(undefined8 *)(param_2 + 0x48);
              *(undefined8 *)(param_1 + 10) = uVar27;
              uVar27 = *(undefined8 *)(param_2 + 10);
              *(undefined8 *)(param_1 + 0xc) = uVar28;
              uVar28 = *(undefined8 *)(param_2 + 0xc);
              *(undefined8 *)(param_1 + 0xe) = uVar29;
              uVar29 = *(undefined8 *)(param_2 + 0xe);
              *(undefined8 *)(param_1 + 0x10) = uVar31;
              uVar31 = *(undefined8 *)(param_2 + 0x10);
              *(undefined8 *)(param_1 + 0x12) = uVar27;
              uVar27 = *(undefined8 *)(param_2 + 0x12);
              *(undefined8 *)(param_1 + 0x14) = uVar28;
              uVar28 = *(undefined8 *)(param_2 + 0x14);
              *(undefined8 *)(param_1 + 0x16) = uVar29;
              uVar29 = *(undefined8 *)(param_2 + 0x16);
              *(undefined8 *)(param_1 + 0x18) = uVar31;
              uVar31 = *(undefined8 *)(param_2 + 0x58);
              *(undefined8 *)(param_1 + 0x1a) = uVar27;
              uVar27 = *(undefined8 *)(param_2 + 0x1a);
              *(undefined8 *)(param_1 + 0x1c) = uVar28;
              uVar28 = *(undefined8 *)(param_2 + 0x1c);
              *(undefined8 *)(param_1 + 0x1e) = uVar29;
              uVar29 = *(undefined8 *)(param_2 + 0x1e);
              *(undefined8 *)(param_1 + 0x20) = uVar32;
              uVar32 = *(undefined8 *)(param_2 + 0x20);
              *(undefined8 *)(param_1 + 0x22) = uVar27;
              uVar27 = *(undefined8 *)(param_2 + 0x22);
              *(undefined8 *)(param_1 + 0x24) = uVar28;
              uVar28 = *(undefined8 *)(param_2 + 0x24);
              *(undefined8 *)(param_1 + 0x26) = uVar29;
              uVar29 = *(undefined8 *)(param_2 + 0x26);
              *(undefined8 *)(param_1 + 0x28) = uVar32;
              uVar32 = *(undefined8 *)(param_2 + 0x68);
              *(undefined8 *)(param_1 + 0x2a) = uVar27;
              uVar27 = *(undefined8 *)(param_2 + 0x2a);
              *(undefined8 *)(param_1 + 0x2c) = uVar28;
              uVar28 = *(undefined8 *)(param_2 + 0x2c);
              *(undefined8 *)(param_1 + 0x2e) = uVar29;
              uVar29 = *(undefined8 *)(param_2 + 0x2e);
              *(undefined8 *)(param_1 + 0x30) = uVar33;
              uVar33 = *(undefined8 *)(param_2 + 0x30);
              *(undefined8 *)(param_1 + 0x32) = uVar27;
              uVar27 = *(undefined8 *)(param_2 + 0x32);
              *(undefined8 *)(param_1 + 0x34) = uVar28;
              uVar28 = *(undefined8 *)(param_2 + 0x34);
              *(undefined8 *)(param_1 + 0x36) = uVar29;
              uVar29 = *(undefined8 *)(param_2 + 0x36);
              *(undefined8 *)(param_1 + 0x38) = uVar33;
              uVar33 = *(undefined8 *)(param_2 + 0x78);
              *(undefined8 *)(param_1 + 0x3a) = uVar27;
              uVar27 = *(undefined8 *)(param_2 + 0x3a);
              *(undefined8 *)(param_1 + 0x3c) = uVar28;
              uVar28 = *(undefined8 *)(param_2 + 0x3c);
              *(undefined8 *)(param_1 + 0x3e) = uVar29;
              uVar29 = *(undefined8 *)(param_2 + 0x3e);
              *(undefined8 *)(param_1 + 0x40) = uVar34;
              uVar34 = *(undefined8 *)(param_2 + 0x40);
              *(undefined8 *)(param_1 + 0x42) = uVar27;
              uVar27 = *(undefined8 *)(param_2 + 0x42);
              *(undefined8 *)(param_1 + 0x44) = uVar28;
              uVar28 = *(undefined8 *)(param_2 + 0x44);
              *(undefined8 *)(param_1 + 0x46) = uVar29;
              uVar29 = *(undefined8 *)(param_2 + 0x46);
              *(undefined8 *)(param_1 + 0x48) = uVar34;
              uVar34 = *(undefined8 *)(param_2 + 0x88);
              *(undefined8 *)(param_1 + 0x4a) = uVar27;
              uVar27 = *(undefined8 *)(param_2 + 0x4a);
              *(undefined8 *)(param_1 + 0x4c) = uVar28;
              uVar28 = *(undefined8 *)(param_2 + 0x4c);
              *(undefined8 *)(param_1 + 0x4e) = uVar29;
              uVar29 = *(undefined8 *)(param_2 + 0x4e);
              param_1 = param_1 + 0x50;
              param_2 = param_2 + 0x50;
              iVar18 = iVar10 + -0x140;
              bVar25 = 0x13f < iVar10;
              iVar10 = iVar18;
            } while (bVar25);
          }
          *(undefined8 *)param_1 = uVar30;
          uVar30 = *(undefined8 *)param_2;
          *(undefined8 *)(param_1 + 2) = uVar27;
          uVar27 = *(undefined8 *)(param_2 + 2);
          *(undefined8 *)(param_1 + 4) = uVar28;
          uVar28 = *(undefined8 *)(param_2 + 4);
          *(undefined8 *)(param_1 + 6) = uVar29;
          uVar29 = *(undefined8 *)(param_2 + 6);
          *(undefined8 *)(param_1 + 8) = uVar30;
          *(undefined8 *)(param_1 + 10) = uVar27;
          uVar27 = *(undefined8 *)(param_2 + 10);
          *(undefined8 *)(param_1 + 0xc) = uVar28;
          uVar28 = *(undefined8 *)(param_2 + 0xc);
          *(undefined8 *)(param_1 + 0xe) = uVar29;
          uVar29 = *(undefined8 *)(param_2 + 0xe);
          *(undefined8 *)(param_1 + 0x10) = uVar31;
          uVar30 = *(undefined8 *)(param_2 + 0x10);
          *(undefined8 *)(param_1 + 0x12) = uVar27;
          uVar27 = *(undefined8 *)(param_2 + 0x12);
          *(undefined8 *)(param_1 + 0x14) = uVar28;
          uVar28 = *(undefined8 *)(param_2 + 0x14);
          *(undefined8 *)(param_1 + 0x16) = uVar29;
          uVar29 = *(undefined8 *)(param_2 + 0x16);
          *(undefined8 *)(param_1 + 0x18) = uVar30;
          *(undefined8 *)(param_1 + 0x1a) = uVar27;
          uVar27 = *(undefined8 *)(param_2 + 0x1a);
          *(undefined8 *)(param_1 + 0x1c) = uVar28;
          uVar28 = *(undefined8 *)(param_2 + 0x1c);
          *(undefined8 *)(param_1 + 0x1e) = uVar29;
          uVar29 = *(undefined8 *)(param_2 + 0x1e);
          *(undefined8 *)(param_1 + 0x20) = uVar32;
          uVar30 = *(undefined8 *)(param_2 + 0x20);
          *(undefined8 *)(param_1 + 0x22) = uVar27;
          uVar27 = *(undefined8 *)(param_2 + 0x22);
          *(undefined8 *)(param_1 + 0x24) = uVar28;
          uVar28 = *(undefined8 *)(param_2 + 0x24);
          *(undefined8 *)(param_1 + 0x26) = uVar29;
          uVar29 = *(undefined8 *)(param_2 + 0x26);
          *(undefined8 *)(param_1 + 0x28) = uVar30;
          *(undefined8 *)(param_1 + 0x2a) = uVar27;
          uVar27 = *(undefined8 *)(param_2 + 0x2a);
          *(undefined8 *)(param_1 + 0x2c) = uVar28;
          uVar28 = *(undefined8 *)(param_2 + 0x2c);
          *(undefined8 *)(param_1 + 0x2e) = uVar29;
          uVar29 = *(undefined8 *)(param_2 + 0x2e);
          *(undefined8 *)(param_1 + 0x30) = uVar33;
          uVar30 = *(undefined8 *)(param_2 + 0x30);
          *(undefined8 *)(param_1 + 0x32) = uVar27;
          uVar27 = *(undefined8 *)(param_2 + 0x32);
          *(undefined8 *)(param_1 + 0x34) = uVar28;
          uVar28 = *(undefined8 *)(param_2 + 0x34);
          *(undefined8 *)(param_1 + 0x36) = uVar29;
          uVar29 = *(undefined8 *)(param_2 + 0x36);
          *(undefined8 *)(param_1 + 0x38) = uVar30;
          *(undefined8 *)(param_1 + 0x3a) = uVar27;
          uVar27 = *(undefined8 *)(param_2 + 0x3a);
          *(undefined8 *)(param_1 + 0x3c) = uVar28;
          uVar28 = *(undefined8 *)(param_2 + 0x3c);
          *(undefined8 *)(param_1 + 0x3e) = uVar29;
          uVar29 = *(undefined8 *)(param_2 + 0x3e);
          *(undefined8 *)(param_1 + 0x40) = uVar34;
          uVar30 = *(undefined8 *)(param_2 + 0x40);
          *(undefined8 *)(param_1 + 0x42) = uVar27;
          uVar27 = *(undefined8 *)(param_2 + 0x42);
          *(undefined8 *)(param_1 + 0x44) = uVar28;
          uVar28 = *(undefined8 *)(param_2 + 0x44);
          *(undefined8 *)(param_1 + 0x46) = uVar29;
          uVar29 = *(undefined8 *)(param_2 + 0x46);
          *(undefined8 *)(param_1 + 0x48) = uVar30;
          param_2 = param_2 + 0x48;
          *(undefined8 *)(param_1 + 0x4a) = uVar27;
          *(undefined8 *)(param_1 + 0x4c) = uVar28;
          *(undefined8 *)(param_1 + 0x4e) = uVar29;
          param_1 = param_1 + 0x50;
          uVar11 = iVar18 + 0x140;
        }
        do {
          uVar9 = uVar11 - 0x40;
          uVar28 = *(undefined8 *)(param_2 + 2);
          *(undefined8 *)param_1 = *(undefined8 *)param_2;
          uVar27 = *(undefined8 *)(param_2 + 4);
          *(undefined8 *)(param_1 + 2) = uVar28;
          uVar28 = *(undefined8 *)(param_2 + 6);
          *(undefined8 *)(param_1 + 4) = uVar27;
          uVar27 = *(undefined8 *)(param_2 + 8);
          *(undefined8 *)(param_1 + 6) = uVar28;
          uVar28 = *(undefined8 *)(param_2 + 10);
          *(undefined8 *)(param_1 + 8) = uVar27;
          uVar27 = *(undefined8 *)(param_2 + 0xc);
          *(undefined8 *)(param_1 + 10) = uVar28;
          uVar28 = *(undefined8 *)(param_2 + 0xe);
          *(undefined8 *)(param_1 + 0xc) = uVar27;
          param_2 = param_2 + 0x10;
          *(undefined8 *)(param_1 + 0xe) = uVar28;
          param_1 = param_1 + 0x10;
          bVar25 = 0x3f < (int)uVar11;
          uVar11 = uVar9;
        } while (bVar25);
        if ((uVar9 & 0x3f) == 0) {
          return;
        }
      }
      uVar11 = uVar9 & 0x38;
      piVar22 = (int *)((int)param_1 + uVar11);
      piVar4 = (int *)((int)param_2 + uVar11);
      iVar10 = 0x34 - uVar11;
      switch(uVar11) {
      case 0x38:
        *(undefined8 *)(piVar22 + -0xe) = *(undefined8 *)(piVar4 + -0xe);
      case 0x30:
        *(undefined8 *)(piVar22 + -0xc) = *(undefined8 *)(piVar4 + -0xc);
      case 0x28:
        *(undefined8 *)(piVar22 + -10) = *(undefined8 *)(piVar4 + -10);
      case 0x20:
        *(undefined8 *)(piVar22 + -8) = *(undefined8 *)(piVar4 + -8);
      case 0x18:
        *(undefined8 *)(piVar22 + -6) = *(undefined8 *)(piVar4 + -6);
      case 0x10:
        *(undefined8 *)(piVar22 + -4) = *(undefined8 *)(piVar4 + -4);
      case 8:
        *(undefined8 *)(piVar22 + -2) = *(undefined8 *)(piVar4 + -2);
      case 0:
        piVar23 = piVar22;
        piVar5 = piVar4;
        if ((uVar9 & 4) != 0) {
          piVar5 = piVar4 + 1;
          iVar10 = *piVar4;
          piVar23 = piVar22 + 1;
          *piVar22 = iVar10;
        }
        uVar7 = (undefined2)iVar10;
        bVar26 = (bool)((byte)(uVar9 >> 1) & 1);
        bVar25 = (uVar9 & 1) != 0;
        piVar4 = piVar5;
        if (bVar26) {
          piVar4 = (int *)((int)piVar5 + 2);
          uVar7 = (undefined2)*piVar5;
        }
        uVar6 = 0;
        if (bVar25) {
          uVar6 = (undefined1)*piVar4;
        }
        piVar4 = piVar23;
        if (bVar26) {
          piVar4 = (int *)((int)piVar23 + 2);
          *(undefined2 *)piVar23 = uVar7;
        }
        if (bVar25) {
          *(undefined1 *)piVar4 = uVar6;
        }
        return;
      }
    }
    HintPreloadData(param_2);
    HintPreloadData(param_2 + 0x10);
    HintPreloadData(param_2 + 0x20);
    if (((uint)param_1 & 7) != 0) {
      uVar11 = (int)param_1 * -0x20000000;
      param_3 = param_3 - (uVar11 >> 0x1d);
      puVar24 = param_1;
      puVar21 = param_2;
      if ((int)uVar11 < 0) {
        puVar21 = param_2 + 1;
        uVar9 = *param_2;
        puVar24 = param_1 + 1;
        *param_1 = uVar9;
      }
      uVar6 = (undefined1)uVar9;
      bVar26 = (bool)((byte)(uVar11 >> 0x1e) & 1);
      uVar9 = (int)param_1 * -0x80000000;
      bVar25 = uVar9 != 0;
      puVar3 = puVar21;
      if (bVar25) {
        puVar3 = (uint *)((int)puVar21 + 1);
        uVar6 = (undefined1)*puVar21;
      }
      param_2 = puVar3;
      if (bVar26) {
        param_2 = (uint *)((int)puVar3 + 2);
        uVar9 = (uint)(ushort)*puVar3;
      }
      puVar21 = puVar24;
      if (bVar25) {
        puVar21 = (uint *)((int)puVar24 + 1);
        *(undefined1 *)puVar24 = uVar6;
      }
      param_1 = puVar21;
      if (bVar26) {
        param_1 = (uint *)((int)puVar21 + 2);
        *(ushort *)puVar21 = (ushort)uVar9;
      }
    }
    HintPreloadData(param_2 + 0x30);
    param_3 = param_3 - 0x40;
    if (-1 < (int)param_3) {
      HintPreloadData(param_2 + 0x40);
      uVar9 = *param_2;
      uVar11 = param_2[1];
      uVar12 = param_2[2];
      uVar13 = param_2[3];
      uVar14 = param_2[4];
      uVar15 = param_2[5];
      uVar16 = param_2[6];
      param_2 = param_2 + 7;
      uVar17 = *param_2;
      param_1 = param_1 + -2;
      while( true ) {
        bVar25 = param_3 < 0x40;
        param_3 = param_3 - 0x40;
        param_1[2] = uVar9;
        param_1[3] = uVar11;
        uVar9 = param_2[1];
        uVar11 = param_2[2];
        param_1[4] = uVar12;
        param_1[5] = uVar13;
        uVar12 = param_2[3];
        uVar13 = param_2[4];
        param_1[6] = uVar14;
        param_1[7] = uVar15;
        uVar14 = param_2[5];
        uVar15 = param_2[6];
        param_1[8] = uVar16;
        param_1[9] = uVar17;
        uVar16 = param_2[7];
        uVar17 = param_2[8];
        if (bVar25) break;
        HintPreloadData(param_2 + 0x49);
        param_1[10] = uVar9;
        param_1[0xb] = uVar11;
        uVar9 = param_2[9];
        uVar11 = param_2[10];
        param_1[0xc] = uVar12;
        param_1[0xd] = uVar13;
        uVar12 = param_2[0xb];
        uVar13 = param_2[0xc];
        param_1[0xe] = uVar14;
        param_1[0xf] = uVar15;
        uVar14 = param_2[0xd];
        uVar15 = param_2[0xe];
        param_1[0x10] = uVar16;
        param_1[0x11] = uVar17;
        uVar16 = param_2[0xf];
        param_2 = param_2 + 0x10;
        uVar17 = *param_2;
        param_1 = param_1 + 0x10;
      }
      param_1[10] = uVar9;
      param_1[0xb] = uVar11;
      param_2 = param_2 + 9;
      param_1[0xc] = uVar12;
      param_1[0xd] = uVar13;
      param_1[0xe] = uVar14;
      param_1[0xf] = uVar15;
      param_1[0x10] = uVar16;
      param_1[0x11] = uVar17;
      param_1 = param_1 + 0x12;
      param_3 = param_3 & 0x3f;
      if (param_3 == 0) {
        return;
      }
    }
  }
  uVar9 = param_3 & 0x3c;
  puVar19 = (undefined2 *)((int)param_1 + uVar9);
  pbVar1 = (byte *)((int)param_2 + uVar9);
  iVar10 = 0x3a - uVar9;
  switch(uVar9) {
  case 0x3c:
    *(undefined4 *)(puVar19 + -0x1e) = *(undefined4 *)(pbVar1 + -0x3c);
  case 0x38:
    *(undefined4 *)(puVar19 + -0x1c) = *(undefined4 *)(pbVar1 + -0x38);
  case 0x34:
    *(undefined4 *)(puVar19 + -0x1a) = *(undefined4 *)(pbVar1 + -0x34);
  case 0x30:
    *(undefined4 *)(puVar19 + -0x18) = *(undefined4 *)(pbVar1 + -0x30);
  case 0x2c:
    *(undefined4 *)(puVar19 + -0x16) = *(undefined4 *)(pbVar1 + -0x2c);
  case 0x28:
    *(undefined4 *)(puVar19 + -0x14) = *(undefined4 *)(pbVar1 + -0x28);
  case 0x24:
    *(undefined4 *)(puVar19 + -0x12) = *(undefined4 *)(pbVar1 + -0x24);
  case 0x20:
    *(undefined4 *)(puVar19 + -0x10) = *(undefined4 *)(pbVar1 + -0x20);
  case 0x1c:
    *(undefined4 *)(puVar19 + -0xe) = *(undefined4 *)(pbVar1 + -0x1c);
  case 0x18:
    *(undefined4 *)(puVar19 + -0xc) = *(undefined4 *)(pbVar1 + -0x18);
  case 0x14:
    *(undefined4 *)(puVar19 + -10) = *(undefined4 *)(pbVar1 + -0x14);
  case 0x10:
    *(undefined4 *)(puVar19 + -8) = *(undefined4 *)(pbVar1 + -0x10);
  case 0xc:
    *(undefined4 *)(puVar19 + -6) = *(undefined4 *)(pbVar1 + -0xc);
  case 8:
    *(undefined4 *)(puVar19 + -4) = *(undefined4 *)(pbVar1 + -8);
  case 4:
    iVar10 = *(int *)(pbVar1 + -4);
    *(int *)(puVar19 + -2) = iVar10;
  case 0:
    uVar7 = (undefined2)iVar10;
    bVar26 = (bool)((byte)(param_3 >> 1) & 1);
    bVar25 = (param_3 & 1) != 0;
    pbVar2 = pbVar1;
    if (bVar26) {
      pbVar2 = pbVar1 + 2;
      uVar7 = *(undefined2 *)pbVar1;
    }
    if (bVar25) {
      pbVar2 = (byte *)(uint)*pbVar2;
    }
    puVar20 = puVar19;
    if (bVar26) {
      puVar20 = puVar19 + 1;
      *puVar19 = uVar7;
    }
    if (bVar25) {
      *(char *)puVar20 = (char)pbVar2;
    }
    return;
  }
}

