//==================== FUN_04000090 @ 04000090 ====================

void FUN_04000090(int param_1,uint3 *param_2)

{
  uint3 uVar1;
  uint3 uVar2;
  uint3 uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  uint uVar10;
  uint uVar11;
  uint uVar12;
  uint uVar13;
  uint uVar14;
  uint uVar15;
  uint uVar16;
  uint uVar17;
  uint uVar18;
  uint uVar19;
  uint uVar20;
  uint uVar21;
  uint uVar22;
  uint uVar23;
  uint uVar24;
  uint uVar25;
  uint uVar26;
  uint uVar27;
  uint uVar28;
  uint uVar29;
  uint uVar30;
  uint uVar31;
  uint uVar32;
  uint uVar33;
  uint uVar34;
  uint uVar35;
  uint uVar36;
  uint uVar37;
  uint uVar38;
  uint uVar39;
  uint uVar40;
  uint uVar41;
  uint uVar42;
  uint uVar43;
  uint uVar44;
  uint uVar45;
  uint uVar46;
  uint uVar47;
  uint uVar48;
  int iVar49;
  uint uVar50;
  uint uVar51;
  uint uVar52;
  uint uVar53;
  uint uVar54;
  uint uVar55;
  
  uVar1 = *param_2;
  uVar47 = (uint)uVar1 << 0x18 | ((uVar1 & 0xff00) >> 8) << 0x10 | (uint)(uVar1 >> 0x10) << 8 |
           (uint)*(byte *)((int)param_2 + 3);
  uVar8 = *(uint *)(param_2 + 1);
  uVar1 = param_2[2];
  uVar5 = uVar8 << 0x18 | (uVar8 >> 8 & 0xff) << 0x10;
  uVar43 = uVar5 | (uVar8 >> 0x10 & 0xff) << 8 | uVar8 >> 0x18;
  uVar2 = param_2[3];
  uVar7 = (uint)uVar1 << 0x18 | ((uVar1 & 0xff00) >> 8) << 0x10;
  uVar52 = uVar7 | (uint)(uVar1 >> 0x10) << 8 | (uint)*(byte *)((int)param_2 + 0xb);
  uVar15 = *(uint *)(param_2 + 4);
  uVar34 = uVar15 << 0x18 | (uVar15 >> 8 & 0xff) << 0x10;
  uVar13 = uVar34 | (uVar15 >> 0x10 & 0xff) << 8 | uVar15 >> 0x18;
  uVar3 = param_2[6];
  uVar27 = (uint)uVar2 << 0x18 | ((uVar2 & 0xff00) >> 8) << 0x10;
  uVar9 = uVar27 | (uint)(uVar2 >> 0x10) << 8 | (uint)*(byte *)((int)param_2 + 0xf);
  uVar1 = param_2[7];
  uVar2 = param_2[8];
  uVar16 = (uint)param_2[5] << 0x18 | ((param_2[5] & 0xff00) >> 8) << 0x10;
  uVar53 = uVar16 | (uint)*(byte *)((int)param_2 + 0x16) << 8 | (uint)*(byte *)((int)param_2 + 0x17)
  ;
  uVar37 = (uint)uVar3 << 0x18 | ((uVar3 & 0xff00) >> 8) << 0x10;
  uVar48 = uVar37 | (uint)(uVar3 >> 0x10) << 8 | (uint)*(byte *)((int)param_2 + 0x1b);
  uVar3 = param_2[9];
  uVar17 = (uint)uVar1 << 0x18 | ((uVar1 & 0xff00) >> 8) << 0x10;
  uVar24 = uVar17 | (uint)(uVar1 >> 0x10) << 8 | (uint)*(byte *)((int)param_2 + 0x1f);
  uVar28 = (uint)uVar2 << 0x18 | ((uVar2 & 0xff00) >> 8) << 0x10;
  uVar44 = uVar28 | (uint)(uVar2 >> 0x10) << 8 | (uint)*(byte *)((int)param_2 + 0x23);
  uVar1 = param_2[10];
  uVar2 = param_2[0xb];
  uVar22 = (uint)uVar3 << 0x18 | ((uVar3 & 0xff00) >> 8) << 0x10;
  uVar30 = uVar22 | (uint)(uVar3 >> 0x10) << 8 | (uint)*(byte *)((int)param_2 + 0x27);
  uVar55 = *(uint *)(param_2 + 0xc);
  uVar23 = (uint)uVar1 << 0x18 | ((uVar1 & 0xff00) >> 8) << 0x10;
  uVar18 = uVar23 | (uint)(uVar1 >> 0x10) << 8 | (uint)*(byte *)((int)param_2 + 0x2b);
  uVar51 = (uint)uVar2 << 0x18 | ((uVar2 & 0xff00) >> 8) << 0x10;
  uVar45 = uVar51 | (uint)(uVar2 >> 0x10) << 8 | (uint)*(byte *)((int)param_2 + 0x2f);
  uVar29 = uVar55 << 0x18 | (uVar55 >> 8 & 0xff) << 0x10;
  uVar10 = uVar29 | (uVar55 >> 0x10 & 0xff) << 8 | uVar55 >> 0x18;
  uVar42 = *(uint *)(param_2 + 0xe);
  uVar41 = *(uint *)(param_2 + 0xd);
  uVar1 = param_2[0xf];
  uVar31 = *(uint *)(param_1 + 0x20);
  uVar38 = *(uint *)(param_1 + 8);
  uVar36 = *(uint *)(param_1 + 0x18);
  uVar32 = *(uint *)(param_1 + 0xc);
  uVar25 = *(uint *)(param_1 + 0x1c);
  uVar33 = *(uint *)(param_1 + 0x10);
  iVar49 = ((uVar36 >> 0xb | uVar36 << 0x15) ^ (uVar36 >> 6 | uVar36 << 0x1a) ^
           (uVar36 >> 0x19 | uVar36 << 7)) + *(int *)(param_1 + 0x24) + 0x428a2f98 +
           ((uVar25 ^ uVar31) & uVar36 ^ uVar31) + uVar47;
  uVar26 = ((uVar38 >> 0xd | uVar38 << 0x13) ^ (uVar38 >> 2 | uVar38 << 0x1e) ^
           (uVar38 >> 0x16 | uVar38 << 10)) + ((uVar38 | uVar32) & uVar33 | uVar38 & uVar32) +
           iVar49;
  uVar4 = *(int *)(param_1 + 0x14) + iVar49;
  iVar49 = ((uVar36 ^ uVar25) & uVar4 ^ uVar25) + uVar31 + 0x71374491 + uVar43 +
           ((uVar4 >> 0xb | uVar4 * 0x200000) ^ (uVar4 >> 6 | uVar4 * 0x4000000) ^
           (uVar4 >> 0x19 | uVar4 * 0x80));
  uVar14 = ((uVar26 >> 0xd | uVar26 * 0x80000) ^ (uVar26 >> 2 | uVar26 * 0x40000000) ^
           (uVar26 >> 0x16 | uVar26 * 0x400)) + ((uVar38 | uVar26) & uVar32 | uVar38 & uVar26) +
           iVar49;
  uVar11 = uVar33 + iVar49;
  iVar49 = ((uVar36 ^ uVar4) & uVar11 ^ uVar36) + uVar25 + 0xb5c0fbcf + uVar52 +
           ((uVar11 >> 0xb | uVar11 * 0x200000) ^ (uVar11 >> 6 | uVar11 * 0x4000000) ^
           (uVar11 >> 0x19 | uVar11 * 0x80));
  uVar35 = uVar41 << 0x18 | (uVar41 >> 8 & 0xff) << 0x10;
  uVar19 = uVar35 | (uVar41 >> 0x10 & 0xff) << 8 | uVar41 >> 0x18;
  uVar39 = uVar42 << 0x18 | (uVar42 >> 8 & 0xff) << 0x10;
  uVar6 = uVar39 | (uVar42 >> 0x10 & 0xff) << 8;
  uVar20 = uVar6 | uVar42 >> 0x18;
  uVar40 = (uint)uVar1 << 0x18 | ((uVar1 & 0xff00) >> 8) << 0x10;
  uVar12 = uVar40 | (uint)(uVar1 >> 0x10) << 8;
  uVar21 = uVar12 | *(byte *)((int)param_2 + 0x3f);
  uVar46 = ((uVar14 >> 0xd | uVar14 * 0x80000) ^ (uVar14 >> 2 | uVar14 * 0x40000000) ^
           (uVar14 >> 0x16 | uVar14 * 0x400)) + ((uVar26 | uVar14) & uVar38 | uVar26 & uVar14) +
           iVar49;
  uVar54 = uVar32 + iVar49;
  iVar49 = (uVar4 ^ (uVar4 ^ uVar11) & uVar54) + uVar36 + 0xe9b5dba5 + uVar9 +
           ((uVar54 >> 0xb | uVar54 * 0x200000) ^ (uVar54 >> 6 | uVar54 * 0x4000000) ^
           (uVar54 >> 0x19 | uVar54 * 0x80));
  uVar5 = ((uVar39 >> 0x13 | uVar20 << 0xd) ^ (uVar39 >> 0x11 | uVar20 << 0xf) ^ uVar6 >> 10) +
          uVar47 + uVar30 +
          ((uVar5 >> 0x12 | uVar43 << 0xe) ^ (uVar43 >> 7 | (uVar8 >> 0x18) << 0x19) ^ uVar43 >> 3);
  uVar8 = ((uVar46 >> 0xd | uVar46 * 0x80000) ^ (uVar46 >> 2 | uVar46 * 0x40000000) ^
          (uVar46 >> 0x16 | uVar46 * 0x400)) + ((uVar14 | uVar46) & uVar26 | uVar14 & uVar46) +
          iVar49;
  uVar6 = uVar38 + iVar49;
  iVar49 = ((uVar11 ^ uVar54) & uVar6 ^ uVar11) + uVar13 + 0x3956c25b + uVar4 +
           ((uVar6 >> 0xb | uVar6 * 0x200000) ^ (uVar6 >> 6 | uVar6 * 0x4000000) ^
           (uVar6 >> 0x19 | uVar6 * 0x80));
  uVar26 = uVar26 + iVar49;
  uVar47 = ((uVar8 >> 0xd | uVar8 * 0x80000) ^ (uVar8 >> 2 | uVar8 * 0x40000000) ^
           (uVar8 >> 0x16 | uVar8 * 0x400)) + ((uVar46 | uVar8) & uVar14 | uVar46 & uVar8) + iVar49;
  uVar43 = ((uVar40 >> 0x13 | uVar21 << 0xd) ^ (uVar40 >> 0x11 | uVar21 << 0xf) ^ uVar12 >> 10) +
           uVar43 + uVar18 +
           ((uVar7 >> 0x12 | uVar52 << 0xe) ^
            (uVar52 >> 7 | (uint)*(byte *)((int)param_2 + 0xb) << 0x19) ^ uVar52 >> 3);
  iVar49 = ((uVar54 ^ uVar6) & uVar26 ^ uVar54) + uVar53 + 0x59f111f1 + uVar11 +
           ((uVar26 >> 0xb | uVar26 * 0x200000) ^ (uVar26 >> 6 | uVar26 * 0x4000000) ^
           (uVar26 >> 0x19 | uVar26 * 0x80));
  uVar14 = uVar14 + iVar49;
  uVar50 = ((uVar47 >> 0xd | uVar47 * 0x80000) ^ (uVar47 >> 2 | uVar47 * 0x40000000) ^
           (uVar47 >> 0x16 | uVar47 * 0x400)) + ((uVar8 | uVar47) & uVar46 | uVar8 & uVar47) +
           iVar49;
  iVar49 = ((uVar6 ^ uVar26) & uVar14 ^ uVar6) + uVar48 + 0x923f82a4 + uVar54 +
           ((uVar14 >> 0xb | uVar14 * 0x200000) ^ (uVar14 >> 6 | uVar14 * 0x4000000) ^
           (uVar14 >> 0x19 | uVar14 * 0x80));
  uVar46 = uVar46 + iVar49;
  uVar12 = ((uVar50 >> 0xd | uVar50 * 0x80000) ^ (uVar50 >> 2 | uVar50 * 0x40000000) ^
           (uVar50 >> 0x16 | uVar50 * 0x400)) + ((uVar47 | uVar50) & uVar8 | uVar47 & uVar50) +
           iVar49;
  uVar4 = ((uVar27 >> 0x12 | uVar9 << 0xe) ^
           (uVar9 >> 7 | (uint)*(byte *)((int)param_2 + 0xf) << 0x19) ^ uVar9 >> 3) +
          uVar52 + uVar45 +
          ((uVar5 >> 0x13 | uVar5 * 0x2000) ^ (uVar5 >> 0x11 | uVar5 * 0x8000) ^ uVar5 >> 10);
  iVar49 = ((uVar26 ^ uVar14) & uVar46 ^ uVar26) + uVar24 + 0xab1c5ed5 + uVar6 +
           ((uVar46 >> 0xb | uVar46 * 0x200000) ^ (uVar46 >> 6 | uVar46 * 0x4000000) ^
           (uVar46 >> 0x19 | uVar46 * 0x80));
  uVar8 = uVar8 + iVar49;
  uVar11 = ((uVar12 >> 0xd | uVar12 * 0x80000) ^ (uVar12 >> 2 | uVar12 * 0x40000000) ^
           (uVar12 >> 0x16 | uVar12 * 0x400)) + ((uVar50 | uVar12) & uVar47 | uVar50 & uVar12) +
           iVar49;
  iVar49 = ((uVar14 ^ uVar46) & uVar8 ^ uVar14) + uVar44 + 0xd807aa98 + uVar26 +
           ((uVar8 >> 0xb | uVar8 * 0x200000) ^ (uVar8 >> 6 | uVar8 * 0x4000000) ^
           (uVar8 >> 0x19 | uVar8 * 0x80));
  uVar47 = uVar47 + iVar49;
  uVar27 = ((uVar11 >> 0xd | uVar11 * 0x80000) ^ (uVar11 >> 2 | uVar11 * 0x40000000) ^
           (uVar11 >> 0x16 | uVar11 * 0x400)) + ((uVar12 | uVar11) & uVar50 | uVar12 & uVar11) +
           iVar49;
  uVar6 = ((uVar34 >> 0x12 | uVar13 << 0xe) ^ (uVar13 >> 7 | (uVar15 >> 0x18) << 0x19) ^ uVar13 >> 3
          ) + uVar9 + uVar10 +
          ((uVar43 >> 0x13 | uVar43 * 0x2000) ^ (uVar43 >> 0x11 | uVar43 * 0x8000) ^ uVar43 >> 10);
  iVar49 = ((uVar46 ^ uVar8) & uVar47 ^ uVar46) + uVar30 + 0x12835b01 + uVar14 +
           ((uVar47 >> 0xb | uVar47 * 0x200000) ^ (uVar47 >> 6 | uVar47 * 0x4000000) ^
           (uVar47 >> 0x19 | uVar47 * 0x80));
  uVar50 = uVar50 + iVar49;
  uVar7 = ((uVar27 >> 0xd | uVar27 * 0x80000) ^ (uVar27 >> 2 | uVar27 * 0x40000000) ^
          (uVar27 >> 0x16 | uVar27 * 0x400)) + ((uVar11 | uVar27) & uVar12 | uVar11 & uVar27) +
          iVar49;
  iVar49 = ((uVar8 ^ uVar47) & uVar50 ^ uVar8) + uVar18 + 0x243185be + uVar46 +
           ((uVar50 >> 0xb | uVar50 * 0x200000) ^ (uVar50 >> 6 | uVar50 * 0x4000000) ^
           (uVar50 >> 0x19 | uVar50 * 0x80));
  uVar12 = uVar12 + iVar49;
  uVar34 = ((uVar7 >> 0xd | uVar7 * 0x80000) ^ (uVar7 >> 2 | uVar7 * 0x40000000) ^
           (uVar7 >> 0x16 | uVar7 * 0x400)) + ((uVar27 | uVar7) & uVar11 | uVar27 & uVar7) + iVar49;
  iVar49 = ((uVar47 ^ uVar50) & uVar12 ^ uVar47) + uVar45 + 0x550c7dc3 + uVar8 +
           ((uVar12 >> 0xb | uVar12 * 0x200000) ^ (uVar12 >> 6 | uVar12 * 0x4000000) ^
           (uVar12 >> 0x19 | uVar12 * 0x80));
  uVar8 = ((uVar34 >> 0xd | uVar34 * 0x80000) ^ (uVar34 >> 2 | uVar34 * 0x40000000) ^
          (uVar34 >> 0x16 | uVar34 * 0x400)) + ((uVar7 | uVar34) & uVar27 | uVar7 & uVar34) + iVar49
  ;
  uVar11 = uVar11 + iVar49;
  iVar49 = ((uVar50 ^ uVar12) & uVar11 ^ uVar50) + uVar10 + 0x72be5d74 + uVar47 +
           ((uVar11 >> 0xb | uVar11 * 0x200000) ^ (uVar11 >> 6 | uVar11 * 0x4000000) ^
           (uVar11 >> 0x19 | uVar11 * 0x80));
  uVar27 = uVar27 + iVar49;
  uVar26 = ((uVar8 >> 0xd | uVar8 * 0x80000) ^ (uVar8 >> 2 | uVar8 * 0x40000000) ^
           (uVar8 >> 0x16 | uVar8 * 0x400)) + ((uVar34 | uVar8) & uVar7 | uVar34 & uVar8) + iVar49;
  uVar15 = ((uVar16 >> 0x12 | uVar53 << 0xe) ^
            (uVar53 >> 7 | (uint)*(byte *)((int)param_2 + 0x17) << 0x19) ^ uVar53 >> 3) +
           uVar13 + uVar19 +
           ((uVar4 >> 0x13 | uVar4 * 0x2000) ^ (uVar4 >> 0x11 | uVar4 * 0x8000) ^ uVar4 >> 10);
  iVar49 = ((uVar12 ^ uVar11) & uVar27 ^ uVar12) + uVar19 + 0x80deb1fe + uVar50 +
           ((uVar27 >> 0xb | uVar27 * 0x200000) ^ (uVar27 >> 6 | uVar27 * 0x4000000) ^
           (uVar27 >> 0x19 | uVar27 * 0x80));
  uVar7 = uVar7 + iVar49;
  uVar16 = ((uVar26 >> 0xd | uVar26 * 0x80000) ^ (uVar26 >> 2 | uVar26 * 0x40000000) ^
           (uVar26 >> 0x16 | uVar26 * 0x400)) + ((uVar8 | uVar26) & uVar34 | uVar8 & uVar26) +
           iVar49;
  iVar49 = ((uVar11 ^ uVar27) & uVar7 ^ uVar11) + uVar20 + 0x9bdc06a7 + uVar12 +
           ((uVar7 >> 0xb | uVar7 * 0x200000) ^ (uVar7 >> 6 | uVar7 * 0x4000000) ^
           (uVar7 >> 0x19 | uVar7 * 0x80));
  uVar34 = uVar34 + iVar49;
  uVar9 = ((uVar16 >> 0xd | uVar16 * 0x80000) ^ (uVar16 >> 2 | uVar16 * 0x40000000) ^
          (uVar16 >> 0x16 | uVar16 * 0x400)) + ((uVar26 | uVar16) & uVar8 | uVar26 & uVar16) +
          iVar49;
  iVar49 = ((uVar27 ^ uVar7) & uVar34 ^ uVar27) + uVar21 + 0xc19bf174 + uVar11 +
           ((uVar34 >> 0xb | uVar34 * 0x200000) ^ (uVar34 >> 6 | uVar34 * 0x4000000) ^
           (uVar34 >> 0x19 | uVar34 * 0x80));
  uVar8 = uVar8 + iVar49;
  uVar12 = ((uVar9 >> 0xd | uVar9 * 0x80000) ^ (uVar9 >> 2 | uVar9 * 0x40000000) ^
           (uVar9 >> 0x16 | uVar9 * 0x400)) + ((uVar16 | uVar9) & uVar26 | uVar16 & uVar9) + iVar49;
  uVar46 = ((uVar37 >> 0x12 | uVar48 << 0xe) ^
            (uVar48 >> 7 | (uint)*(byte *)((int)param_2 + 0x1b) << 0x19) ^ uVar48 >> 3) +
           uVar53 + uVar20 +
           ((uVar6 >> 0x13 | uVar6 * 0x2000) ^ (uVar6 >> 0x11 | uVar6 * 0x8000) ^ uVar6 >> 10);
  iVar49 = ((uVar7 ^ uVar34) & uVar8 ^ uVar7) + uVar5 + 0xe49b69c1 + uVar27 +
           ((uVar8 >> 0xb | uVar8 * 0x200000) ^ (uVar8 >> 6 | uVar8 * 0x4000000) ^
           (uVar8 >> 0x19 | uVar8 * 0x80));
  uVar26 = uVar26 + iVar49;
  uVar14 = ((uVar12 >> 0xd | uVar12 * 0x80000) ^ (uVar12 >> 2 | uVar12 * 0x40000000) ^
           (uVar12 >> 0x16 | uVar12 * 0x400)) + ((uVar9 | uVar12) & uVar16 | uVar9 & uVar12) +
           iVar49;
  iVar49 = ((uVar34 ^ uVar8) & uVar26 ^ uVar34) + uVar43 + 0xefbe4786 + uVar7 +
           ((uVar26 >> 0xb | uVar26 * 0x200000) ^ (uVar26 >> 6 | uVar26 * 0x4000000) ^
           (uVar26 >> 0x19 | uVar26 * 0x80));
  uVar16 = uVar16 + iVar49;
  uVar7 = ((uVar14 >> 0xd | uVar14 * 0x80000) ^ (uVar14 >> 2 | uVar14 * 0x40000000) ^
          (uVar14 >> 0x16 | uVar14 * 0x400)) + ((uVar12 | uVar14) & uVar9 | uVar12 & uVar14) +
          iVar49;
  iVar49 = ((uVar8 ^ uVar26) & uVar16 ^ uVar8) + uVar4 + 0xfc19dc6 + uVar34 +
           ((uVar16 >> 0xb | uVar16 * 0x200000) ^ (uVar16 >> 6 | uVar16 * 0x4000000) ^
           (uVar16 >> 0x19 | uVar16 * 0x80));
  uVar9 = uVar9 + iVar49;
  uVar34 = ((uVar7 >> 0xd | uVar7 * 0x80000) ^ (uVar7 >> 2 | uVar7 * 0x40000000) ^
           (uVar7 >> 0x16 | uVar7 * 0x400)) + ((uVar14 | uVar7) & uVar12 | uVar14 & uVar7) + iVar49;
  iVar49 = ((uVar26 ^ uVar16) & uVar9 ^ uVar26) + uVar6 + 0x240ca1cc + uVar8 +
           ((uVar9 >> 0xb | uVar9 * 0x200000) ^ (uVar9 >> 6 | uVar9 * 0x4000000) ^
           (uVar9 >> 0x19 | uVar9 * 0x80));
  uVar11 = ((uVar17 >> 0x12 | uVar24 << 0xe) ^
            (uVar24 >> 7 | (uint)*(byte *)((int)param_2 + 0x1f) << 0x19) ^ uVar24 >> 3) +
           uVar48 + uVar21 +
           ((uVar15 >> 0x13 | uVar15 * 0x2000) ^ (uVar15 >> 0x11 | uVar15 * 0x8000) ^ uVar15 >> 10);
  uVar12 = uVar12 + iVar49;
  uVar27 = ((uVar34 >> 0xd | uVar34 * 0x80000) ^ (uVar34 >> 2 | uVar34 * 0x40000000) ^
           (uVar34 >> 0x16 | uVar34 * 0x400)) + ((uVar7 | uVar34) & uVar14 | uVar7 & uVar34) +
           iVar49;
  iVar49 = ((uVar16 ^ uVar9) & uVar12 ^ uVar16) + uVar15 + 0x2de92c6f + uVar26 +
           ((uVar12 >> 0xb | uVar12 * 0x200000) ^ (uVar12 >> 6 | uVar12 * 0x4000000) ^
           (uVar12 >> 0x19 | uVar12 * 0x80));
  uVar14 = uVar14 + iVar49;
  uVar13 = ((uVar27 >> 0xd | uVar27 * 0x80000) ^ (uVar27 >> 2 | uVar27 * 0x40000000) ^
           (uVar27 >> 0x16 | uVar27 * 0x400)) + ((uVar34 | uVar27) & uVar7 | uVar27 & uVar34) +
           iVar49;
  iVar49 = ((uVar9 ^ uVar12) & uVar14 ^ uVar9) + uVar46 + 0x4a7484aa + uVar16 +
           ((uVar14 >> 0xb | uVar14 * 0x200000) ^ (uVar14 >> 6 | uVar14 * 0x4000000) ^
           (uVar14 >> 0x19 | uVar14 * 0x80));
  uVar7 = uVar7 + iVar49;
  uVar47 = ((uVar13 >> 0xd | uVar13 * 0x80000) ^ (uVar13 >> 2 | uVar13 * 0x40000000) ^
           (uVar13 >> 0x16 | uVar13 * 0x400)) + ((uVar27 | uVar13) & uVar34 | uVar27 & uVar13) +
           iVar49;
  iVar49 = ((uVar12 ^ uVar14) & uVar7 ^ uVar12) + uVar11 + 0x5cb0a9dc + uVar9 +
           ((uVar7 >> 0xb | uVar7 * 0x200000) ^ (uVar7 >> 6 | uVar7 * 0x4000000) ^
           (uVar7 >> 0x19 | uVar7 * 0x80));
  uVar34 = uVar34 + iVar49;
  uVar37 = ((uVar47 >> 0xd | uVar47 * 0x80000) ^ (uVar47 >> 2 | uVar47 * 0x40000000) ^
           (uVar47 >> 0x16 | uVar47 * 0x400)) + ((uVar13 | uVar47) & uVar27 | uVar13 & uVar47) +
           iVar49;
  uVar8 = ((uVar28 >> 0x12 | uVar44 << 0xe) ^
           (uVar44 >> 7 | (uint)*(byte *)((int)param_2 + 0x23) << 0x19) ^ uVar44 >> 3) + uVar24 +
          uVar5 + ((uVar46 >> 0x13 | uVar46 * 0x2000) ^ (uVar46 >> 0x11 | uVar46 * 0x8000) ^
                  uVar46 >> 10);
  iVar49 = ((uVar14 ^ uVar7) & uVar34 ^ uVar14) + uVar8 + 0x76f988da + uVar12 +
           ((uVar34 >> 0xb | uVar34 * 0x200000) ^ (uVar34 >> 6 | uVar34 * 0x4000000) ^
           (uVar34 >> 0x19 | uVar34 * 0x80));
  uVar27 = uVar27 + iVar49;
  uVar17 = ((uVar37 >> 0xd | uVar37 * 0x80000) ^ (uVar37 >> 2 | uVar37 * 0x40000000) ^
           (uVar37 >> 0x16 | uVar37 * 0x400)) + ((uVar47 | uVar37) & uVar13 | uVar47 & uVar37) +
           iVar49;
  uVar12 = ((uVar22 >> 0x12 | uVar30 << 0xe) ^
            (uVar30 >> 7 | (uint)*(byte *)((int)param_2 + 0x27) << 0x19) ^ uVar30 >> 3) + uVar44 +
           uVar43 + ((uVar11 >> 0x13 | uVar11 * 0x2000) ^ (uVar11 >> 0x11 | uVar11 * 0x8000) ^
                    uVar11 >> 10);
  iVar49 = ((uVar7 ^ uVar34) & uVar27 ^ uVar7) + uVar12 + 0x983e5152 + uVar14 +
           ((uVar27 >> 0xb | uVar27 * 0x200000) ^ (uVar27 >> 6 | uVar27 * 0x4000000) ^
           (uVar27 >> 0x19 | uVar27 * 0x80));
  uVar13 = uVar13 + iVar49;
  uVar28 = ((uVar17 >> 0xd | uVar17 * 0x80000) ^ (uVar17 >> 2 | uVar17 * 0x40000000) ^
           (uVar17 >> 0x16 | uVar17 * 0x400)) + ((uVar37 | uVar17) & uVar47 | uVar37 & uVar17) +
           iVar49;
  uVar26 = ((uVar23 >> 0x12 | uVar18 << 0xe) ^
            (uVar18 >> 7 | (uint)*(byte *)((int)param_2 + 0x2b) << 0x19) ^ uVar18 >> 3) + uVar30 +
           uVar4 + ((uVar8 >> 0x13 | uVar8 * 0x2000) ^ (uVar8 >> 0x11 | uVar8 * 0x8000) ^
                   uVar8 >> 10);
  iVar49 = ((uVar34 ^ uVar27) & uVar13 ^ uVar34) + uVar26 + 0xa831c66d + uVar7 +
           ((uVar13 >> 0xb | uVar13 * 0x200000) ^ (uVar13 >> 6 | uVar13 * 0x4000000) ^
           (uVar13 >> 0x19 | uVar13 * 0x80));
  uVar47 = uVar47 + iVar49;
  uVar9 = ((uVar28 >> 0xd | uVar28 * 0x80000) ^ (uVar28 >> 2 | uVar28 * 0x40000000) ^
          (uVar28 >> 0x16 | uVar28 * 0x400)) + ((uVar17 | uVar28) & uVar37 | uVar17 & uVar28) +
          iVar49;
  uVar22 = ((uVar51 >> 0x12 | uVar45 << 0xe) ^
            (uVar45 >> 7 | (uint)*(byte *)((int)param_2 + 0x2f) << 0x19) ^ uVar45 >> 3) + uVar18 +
           uVar6 + ((uVar12 >> 0x13 | uVar12 * 0x2000) ^ (uVar12 >> 0x11 | uVar12 * 0x8000) ^
                   uVar12 >> 10);
  iVar49 = ((uVar27 ^ uVar13) & uVar47 ^ uVar27) + uVar22 + 0xb00327c8 + uVar34 +
           ((uVar47 >> 0xb | uVar47 * 0x200000) ^ (uVar47 >> 6 | uVar47 * 0x4000000) ^
           (uVar47 >> 0x19 | uVar47 * 0x80));
  uVar37 = uVar37 + iVar49;
  uVar7 = ((uVar9 >> 0xd | uVar9 * 0x80000) ^ (uVar9 >> 2 | uVar9 * 0x40000000) ^
          (uVar9 >> 0x16 | uVar9 * 0x400)) + ((uVar28 | uVar9) & uVar17 | uVar28 & uVar9) + iVar49;
  uVar14 = ((uVar29 >> 0x12 | uVar10 << 0xe) ^ (uVar10 >> 7 | (uVar55 >> 0x18) << 0x19) ^
           uVar10 >> 3) + uVar45 + uVar15 +
           ((uVar26 >> 0x13 | uVar26 * 0x2000) ^ (uVar26 >> 0x11 | uVar26 * 0x8000) ^ uVar26 >> 10);
  iVar49 = ((uVar13 ^ uVar47) & uVar37 ^ uVar13) + uVar14 + 0xbf597fc7 + uVar27 +
           ((uVar37 >> 0xb | uVar37 * 0x200000) ^ (uVar37 >> 6 | uVar37 * 0x4000000) ^
           (uVar37 >> 0x19 | uVar37 * 0x80));
  uVar17 = uVar17 + iVar49;
  uVar55 = ((uVar7 >> 0xd | uVar7 * 0x80000) ^ (uVar7 >> 2 | uVar7 * 0x40000000) ^
           (uVar7 >> 0x16 | uVar7 * 0x400)) + ((uVar9 | uVar7) & uVar28 | uVar9 & uVar7) + iVar49;
  uVar27 = ((uVar35 >> 0x12 | uVar19 << 0xe) ^ (uVar19 >> 7 | (uVar41 >> 0x18) << 0x19) ^
           uVar19 >> 3) + uVar10 + uVar46 +
           ((uVar22 >> 0x13 | uVar22 * 0x2000) ^ (uVar22 >> 0x11 | uVar22 * 0x8000) ^ uVar22 >> 10);
  iVar49 = ((uVar47 ^ uVar37) & uVar17 ^ uVar47) + uVar27 + 0xc6e00bf3 + uVar13 +
           ((uVar17 >> 0xb | uVar17 * 0x200000) ^ (uVar17 >> 6 | uVar17 * 0x4000000) ^
           (uVar17 >> 0x19 | uVar17 * 0x80));
  uVar28 = uVar28 + iVar49;
  uVar34 = ((uVar55 >> 0xd | uVar55 * 0x80000) ^ (uVar55 >> 2 | uVar55 * 0x40000000) ^
           (uVar55 >> 0x16 | uVar55 * 0x400)) + ((uVar7 | uVar55) & uVar9 | uVar7 & uVar55) + iVar49
  ;
  uVar16 = ((uVar39 >> 0x12 | uVar20 << 0xe) ^ (uVar20 >> 7 | (uVar42 >> 0x18) << 0x19) ^
           uVar20 >> 3) + uVar19 + uVar11 +
           ((uVar14 >> 0x13 | uVar14 * 0x2000) ^ (uVar14 >> 0x11 | uVar14 * 0x8000) ^ uVar14 >> 10);
  iVar49 = ((uVar37 ^ uVar17) & uVar28 ^ uVar37) + uVar16 + 0xd5a79147 + uVar47 +
           ((uVar28 >> 0xb | uVar28 * 0x200000) ^ (uVar28 >> 6 | uVar28 * 0x4000000) ^
           (uVar28 >> 0x19 | uVar28 * 0x80));
  uVar9 = uVar9 + iVar49;
  uVar23 = ((uVar34 >> 0xd | uVar34 * 0x80000) ^ (uVar34 >> 2 | uVar34 * 0x40000000) ^
           (uVar34 >> 0x16 | uVar34 * 0x400)) + ((uVar55 | uVar34) & uVar7 | uVar55 & uVar34) +
           iVar49;
  uVar13 = ((uVar40 >> 0x12 | uVar21 << 0xe) ^
            (uVar21 >> 7 | (uint)*(byte *)((int)param_2 + 0x3f) << 0x19) ^ uVar21 >> 3) + uVar20 +
           uVar8 + ((uVar27 >> 0x13 | uVar27 * 0x2000) ^ (uVar27 >> 0x11 | uVar27 * 0x8000) ^
                   uVar27 >> 10);
  iVar49 = ((uVar17 ^ uVar28) & uVar9 ^ uVar17) + uVar13 + 0x6ca6351 + uVar37 +
           ((uVar9 >> 0xb | uVar9 * 0x200000) ^ (uVar9 >> 6 | uVar9 * 0x4000000) ^
           (uVar9 >> 0x19 | uVar9 * 0x80));
  uVar7 = uVar7 + iVar49;
  uVar51 = ((uVar23 >> 0xd | uVar23 * 0x80000) ^ (uVar23 >> 2 | uVar23 * 0x40000000) ^
           (uVar23 >> 0x16 | uVar23 * 0x400)) + ((uVar34 | uVar23) & uVar55 | uVar34 & uVar23) +
           iVar49;
  uVar37 = ((uVar5 >> 0x12 | uVar5 * 0x4000) ^ (uVar5 >> 7 | uVar5 * 0x2000000) ^ uVar5 >> 3) +
           uVar21 + uVar12 +
           ((uVar16 >> 0x13 | uVar16 * 0x2000) ^ (uVar16 >> 0x11 | uVar16 * 0x8000) ^ uVar16 >> 10);
  iVar49 = ((uVar28 ^ uVar9) & uVar7 ^ uVar28) + uVar37 + 0x14292967 + uVar17 +
           ((uVar7 >> 0xb | uVar7 * 0x200000) ^ (uVar7 >> 6 | uVar7 * 0x4000000) ^
           (uVar7 >> 0x19 | uVar7 * 0x80));
  uVar55 = uVar55 + iVar49;
  uVar17 = ((uVar51 >> 0xd | uVar51 * 0x80000) ^ (uVar51 >> 2 | uVar51 * 0x40000000) ^
           (uVar51 >> 0x16 | uVar51 * 0x400)) + ((uVar23 | uVar51) & uVar34 | uVar23 & uVar51) +
           iVar49;
  uVar10 = ((uVar43 >> 0x12 | uVar43 * 0x4000) ^ (uVar43 >> 7 | uVar43 * 0x2000000) ^ uVar43 >> 3) +
           uVar5 + uVar26 +
           ((uVar13 >> 0x13 | uVar13 * 0x2000) ^ (uVar13 >> 0x11 | uVar13 * 0x8000) ^ uVar13 >> 10);
  iVar49 = ((uVar9 ^ uVar7) & uVar55 ^ uVar9) + uVar10 + 0x27b70a85 + uVar28 +
           ((uVar55 >> 0xb | uVar55 * 0x200000) ^ (uVar55 >> 6 | uVar55 * 0x4000000) ^
           (uVar55 >> 0x19 | uVar55 * 0x80));
  uVar34 = uVar34 + iVar49;
  uVar28 = ((uVar17 >> 0xd | uVar17 * 0x80000) ^ (uVar17 >> 2 | uVar17 * 0x40000000) ^
           (uVar17 >> 0x16 | uVar17 * 0x400)) + ((uVar51 | uVar17) & uVar23 | uVar51 & uVar17) +
           iVar49;
  uVar39 = ((uVar4 >> 0x12 | uVar4 * 0x4000) ^ (uVar4 >> 7 | uVar4 * 0x2000000) ^ uVar4 >> 3) +
           uVar43 + uVar22 +
           ((uVar37 >> 0x13 | uVar37 * 0x2000) ^ (uVar37 >> 0x11 | uVar37 * 0x8000) ^ uVar37 >> 10);
  iVar49 = ((uVar7 ^ uVar55) & uVar34 ^ uVar7) + uVar39 + 0x2e1b2138 + uVar9 +
           ((uVar34 >> 0xb | uVar34 * 0x200000) ^ (uVar34 >> 6 | uVar34 * 0x4000000) ^
           (uVar34 >> 0x19 | uVar34 * 0x80));
  uVar23 = uVar23 + iVar49;
  uVar29 = ((uVar28 >> 0xd | uVar28 * 0x80000) ^ (uVar28 >> 2 | uVar28 * 0x40000000) ^
           (uVar28 >> 0x16 | uVar28 * 0x400)) + ((uVar17 | uVar28) & uVar51 | uVar17 & uVar28) +
           iVar49;
  uVar40 = ((uVar6 >> 0x12 | uVar6 * 0x4000) ^ (uVar6 >> 7 | uVar6 * 0x2000000) ^ uVar6 >> 3) +
           uVar4 + uVar14 +
           ((uVar10 >> 0x13 | uVar10 * 0x2000) ^ (uVar10 >> 0x11 | uVar10 * 0x8000) ^ uVar10 >> 10);
  iVar49 = ((uVar55 ^ uVar34) & uVar23 ^ uVar55) + uVar40 + 0x4d2c6dfc + uVar7 +
           ((uVar23 >> 0xb | uVar23 * 0x200000) ^ (uVar23 >> 6 | uVar23 * 0x4000000) ^
           (uVar23 >> 0x19 | uVar23 * 0x80));
  uVar51 = uVar51 + iVar49;
  uVar35 = ((uVar29 >> 0xd | uVar29 * 0x80000) ^ (uVar29 >> 2 | uVar29 * 0x40000000) ^
           (uVar29 >> 0x16 | uVar29 * 0x400)) + ((uVar28 | uVar29) & uVar17 | uVar28 & uVar29) +
           iVar49;
  uVar6 = ((uVar15 >> 0x12 | uVar15 * 0x4000) ^ (uVar15 >> 7 | uVar15 * 0x2000000) ^ uVar15 >> 3) +
          uVar6 + uVar27 +
          ((uVar39 >> 0x13 | uVar39 * 0x2000) ^ (uVar39 >> 0x11 | uVar39 * 0x8000) ^ uVar39 >> 10);
  iVar49 = ((uVar34 ^ uVar23) & uVar51 ^ uVar34) + uVar6 + 0x53380d13 + uVar55 +
           ((uVar51 >> 0xb | uVar51 * 0x200000) ^ (uVar51 >> 6 | uVar51 * 0x4000000) ^
           (uVar51 >> 0x19 | uVar51 * 0x80));
  uVar17 = uVar17 + iVar49;
  uVar5 = ((uVar35 >> 0xd | uVar35 * 0x80000) ^ (uVar35 >> 2 | uVar35 * 0x40000000) ^
          (uVar35 >> 0x16 | uVar35 * 0x400)) + ((uVar29 | uVar35) & uVar28 | uVar29 & uVar35) +
          iVar49;
  uVar15 = ((uVar46 >> 0x12 | uVar46 * 0x4000) ^ (uVar46 >> 7 | uVar46 * 0x2000000) ^ uVar46 >> 3) +
           uVar15 + uVar16 +
           ((uVar40 >> 0x13 | uVar40 * 0x2000) ^ (uVar40 >> 0x11 | uVar40 * 0x8000) ^ uVar40 >> 10);
  iVar49 = ((uVar23 ^ uVar51) & uVar17 ^ uVar23) + uVar15 + 0x650a7354 + uVar34 +
           ((uVar17 >> 0xb | uVar17 * 0x200000) ^ (uVar17 >> 6 | uVar17 * 0x4000000) ^
           (uVar17 >> 0x19 | uVar17 * 0x80));
  uVar28 = uVar28 + iVar49;
  uVar4 = ((uVar5 >> 0xd | uVar5 * 0x80000) ^ (uVar5 >> 2 | uVar5 * 0x40000000) ^
          (uVar5 >> 0x16 | uVar5 * 0x400)) + ((uVar35 | uVar5) & uVar29 | uVar35 & uVar5) + iVar49;
  uVar55 = ((uVar11 >> 0x12 | uVar11 * 0x4000) ^ (uVar11 >> 7 | uVar11 * 0x2000000) ^ uVar11 >> 3) +
           uVar46 + uVar13 +
           ((uVar6 >> 0x13 | uVar6 * 0x2000) ^ (uVar6 >> 0x11 | uVar6 * 0x8000) ^ uVar6 >> 10);
  iVar49 = ((uVar51 ^ uVar17) & uVar28 ^ uVar51) + uVar55 + 0x766a0abb + uVar23 +
           ((uVar28 >> 0xb | uVar28 * 0x200000) ^ (uVar28 >> 6 | uVar28 * 0x4000000) ^
           (uVar28 >> 0x19 | uVar28 * 0x80));
  uVar29 = uVar29 + iVar49;
  uVar7 = ((uVar4 >> 0xd | uVar4 * 0x80000) ^ (uVar4 >> 2 | uVar4 * 0x40000000) ^
          (uVar4 >> 0x16 | uVar4 * 0x400)) + ((uVar5 | uVar4) & uVar35 | uVar5 & uVar4) + iVar49;
  uVar41 = ((uVar8 >> 0x12 | uVar8 * 0x4000) ^ (uVar8 >> 7 | uVar8 * 0x2000000) ^ uVar8 >> 3) +
           uVar11 + uVar37 +
           ((uVar15 >> 0x13 | uVar15 * 0x2000) ^ (uVar15 >> 0x11 | uVar15 * 0x8000) ^ uVar15 >> 10);
  iVar49 = ((uVar17 ^ uVar28) & uVar29 ^ uVar17) + uVar41 + 0x81c2c92e + uVar51 +
           ((uVar29 >> 0xb | uVar29 * 0x200000) ^ (uVar29 >> 6 | uVar29 * 0x4000000) ^
           (uVar29 >> 0x19 | uVar29 * 0x80));
  uVar35 = uVar35 + iVar49;
  uVar51 = ((uVar7 >> 0xd | uVar7 * 0x80000) ^ (uVar7 >> 2 | uVar7 * 0x40000000) ^
           (uVar7 >> 0x16 | uVar7 * 0x400)) + ((uVar4 | uVar7) & uVar5 | uVar4 & uVar7) + iVar49;
  uVar9 = ((uVar12 >> 0x12 | uVar12 * 0x4000) ^ (uVar12 >> 7 | uVar12 * 0x2000000) ^ uVar12 >> 3) +
          uVar8 + uVar10 +
          ((uVar55 >> 0x13 | uVar55 * 0x2000) ^ (uVar55 >> 0x11 | uVar55 * 0x8000) ^ uVar55 >> 10);
  iVar49 = ((uVar28 ^ uVar29) & uVar35 ^ uVar28) + uVar9 + 0x92722c85 + uVar17 +
           ((uVar35 >> 0xb | uVar35 * 0x200000) ^ (uVar35 >> 6 | uVar35 * 0x4000000) ^
           (uVar35 >> 0x19 | uVar35 * 0x80));
  uVar5 = uVar5 + iVar49;
  uVar18 = ((uVar51 >> 0xd | uVar51 * 0x80000) ^ (uVar51 >> 2 | uVar51 * 0x40000000) ^
           (uVar51 >> 0x16 | uVar51 * 0x400)) + ((uVar7 | uVar51) & uVar4 | uVar7 & uVar51) + iVar49
  ;
  uVar12 = ((uVar26 >> 0x12 | uVar26 * 0x4000) ^ (uVar26 >> 7 | uVar26 * 0x2000000) ^ uVar26 >> 3) +
           uVar12 + uVar39 +
           ((uVar41 >> 0x13 | uVar41 * 0x2000) ^ (uVar41 >> 0x11 | uVar41 * 0x8000) ^ uVar41 >> 10);
  iVar49 = ((uVar29 ^ uVar35) & uVar5 ^ uVar29) + uVar12 + 0xa2bfe8a1 + uVar28 +
           ((uVar5 >> 0xb | uVar5 * 0x200000) ^ (uVar5 >> 6 | uVar5 * 0x4000000) ^
           (uVar5 >> 0x19 | uVar5 * 0x80));
  uVar4 = uVar4 + iVar49;
  uVar34 = ((uVar18 >> 0xd | uVar18 * 0x80000) ^ (uVar18 >> 2 | uVar18 * 0x40000000) ^
           (uVar18 >> 0x16 | uVar18 * 0x400)) + ((uVar51 | uVar18) & uVar7 | uVar51 & uVar18) +
           iVar49;
  uVar23 = ((uVar22 >> 0x12 | uVar22 * 0x4000) ^ (uVar22 >> 7 | uVar22 * 0x2000000) ^ uVar22 >> 3) +
           uVar26 + uVar40 +
           ((uVar9 >> 0x13 | uVar9 * 0x2000) ^ (uVar9 >> 0x11 | uVar9 * 0x8000) ^ uVar9 >> 10);
  iVar49 = ((uVar35 ^ uVar5) & uVar4 ^ uVar35) + uVar23 + 0xa81a664b + uVar29 +
           ((uVar4 >> 0xb | uVar4 * 0x200000) ^ (uVar4 >> 6 | uVar4 * 0x4000000) ^
           (uVar4 >> 0x19 | uVar4 * 0x80));
  uVar7 = uVar7 + iVar49;
  uVar17 = ((uVar34 >> 0xd | uVar34 * 0x80000) ^ (uVar34 >> 2 | uVar34 * 0x40000000) ^
           (uVar34 >> 0x16 | uVar34 * 0x400)) + ((uVar18 | uVar34) & uVar51 | uVar18 & uVar34) +
           iVar49;
  uVar8 = ((uVar14 >> 0x12 | uVar14 * 0x4000) ^ (uVar14 >> 7 | uVar14 * 0x2000000) ^ uVar14 >> 3) +
          uVar22 + uVar6 +
          ((uVar12 >> 0x13 | uVar12 * 0x2000) ^ (uVar12 >> 0x11 | uVar12 * 0x8000) ^ uVar12 >> 10);
  iVar49 = ((uVar5 ^ uVar4) & uVar7 ^ uVar5) + uVar8 + 0xc24b8b70 + uVar35 +
           ((uVar7 >> 0xb | uVar7 * 0x200000) ^ (uVar7 >> 6 | uVar7 * 0x4000000) ^
           (uVar7 >> 0x19 | uVar7 * 0x80));
  uVar51 = uVar51 + iVar49;
  uVar28 = ((uVar17 >> 0xd | uVar17 * 0x80000) ^ (uVar17 >> 2 | uVar17 * 0x40000000) ^
           (uVar17 >> 0x16 | uVar17 * 0x400)) + ((uVar34 | uVar17) & uVar18 | uVar34 & uVar17) +
           iVar49;
  uVar42 = ((uVar27 >> 0x12 | uVar27 * 0x4000) ^ (uVar27 >> 7 | uVar27 * 0x2000000) ^ uVar27 >> 3) +
           uVar14 + uVar15 +
           ((uVar23 >> 0x13 | uVar23 * 0x2000) ^ (uVar23 >> 0x11 | uVar23 * 0x8000) ^ uVar23 >> 10);
  iVar49 = ((uVar4 ^ uVar7) & uVar51 ^ uVar4) + uVar42 + 0xc76c51a3 + uVar5 +
           ((uVar51 >> 0xb | uVar51 * 0x200000) ^ (uVar51 >> 6 | uVar51 * 0x4000000) ^
           (uVar51 >> 0x19 | uVar51 * 0x80));
  uVar18 = uVar18 + iVar49;
  uVar11 = ((uVar28 >> 0xd | uVar28 * 0x80000) ^ (uVar28 >> 2 | uVar28 * 0x40000000) ^
           (uVar28 >> 0x16 | uVar28 * 0x400)) + ((uVar17 | uVar28) & uVar34 | uVar17 & uVar28) +
           iVar49;
  uVar29 = ((uVar16 >> 0x12 | uVar16 * 0x4000) ^ (uVar16 >> 7 | uVar16 * 0x2000000) ^ uVar16 >> 3) +
           uVar27 + uVar55 +
           ((uVar8 >> 0x13 | uVar8 * 0x2000) ^ (uVar8 >> 0x11 | uVar8 * 0x8000) ^ uVar8 >> 10);
  iVar49 = ((uVar7 ^ uVar51) & uVar18 ^ uVar7) + uVar29 + 0xd192e819 + uVar4 +
           ((uVar18 >> 0xb | uVar18 * 0x200000) ^ (uVar18 >> 6 | uVar18 * 0x4000000) ^
           (uVar18 >> 0x19 | uVar18 * 0x80));
  uVar34 = uVar34 + iVar49;
  uVar5 = ((uVar11 >> 0xd | uVar11 * 0x80000) ^ (uVar11 >> 2 | uVar11 * 0x40000000) ^
          (uVar11 >> 0x16 | uVar11 * 0x400)) + ((uVar28 | uVar11) & uVar17 | uVar28 & uVar11) +
          iVar49;
  uVar19 = ((uVar13 >> 0x12 | uVar13 * 0x4000) ^ (uVar13 >> 7 | uVar13 * 0x2000000) ^ uVar13 >> 3) +
           uVar16 + uVar41 +
           ((uVar42 >> 0x13 | uVar42 * 0x2000) ^ (uVar42 >> 0x11 | uVar42 * 0x8000) ^ uVar42 >> 10);
  iVar49 = ((uVar51 ^ uVar18) & uVar34 ^ uVar51) + uVar19 + 0xd6990624 + uVar7 +
           ((uVar34 >> 0xb | uVar34 * 0x200000) ^ (uVar34 >> 6 | uVar34 * 0x4000000) ^
           (uVar34 >> 0x19 | uVar34 * 0x80));
  uVar17 = uVar17 + iVar49;
  uVar27 = ((uVar5 >> 0xd | uVar5 * 0x80000) ^ (uVar5 >> 2 | uVar5 * 0x40000000) ^
           (uVar5 >> 0x16 | uVar5 * 0x400)) + ((uVar11 | uVar5) & uVar28 | uVar11 & uVar5) + iVar49;
  uVar4 = ((uVar37 >> 0x12 | uVar37 * 0x4000) ^ (uVar37 >> 7 | uVar37 * 0x2000000) ^ uVar37 >> 3) +
          uVar13 + uVar9 +
          ((uVar29 >> 0x13 | uVar29 * 0x2000) ^ (uVar29 >> 0x11 | uVar29 * 0x8000) ^ uVar29 >> 10);
  iVar49 = ((uVar18 ^ uVar34) & uVar17 ^ uVar18) + uVar4 + 0xf40e3585 + uVar51 +
           ((uVar17 >> 0xb | uVar17 * 0x200000) ^ (uVar17 >> 6 | uVar17 * 0x4000000) ^
           (uVar17 >> 0x19 | uVar17 * 0x80));
  uVar28 = uVar28 + iVar49;
  uVar7 = ((uVar27 >> 0xd | uVar27 * 0x80000) ^ (uVar27 >> 2 | uVar27 * 0x40000000) ^
          (uVar27 >> 0x16 | uVar27 * 0x400)) + ((uVar5 | uVar27) & uVar11 | uVar5 & uVar27) + iVar49
  ;
  uVar26 = ((uVar10 >> 0x12 | uVar10 * 0x4000) ^ (uVar10 >> 7 | uVar10 * 0x2000000) ^ uVar10 >> 3) +
           uVar37 + uVar12 +
           ((uVar19 >> 0x13 | uVar19 * 0x2000) ^ (uVar19 >> 0x11 | uVar19 * 0x8000) ^ uVar19 >> 10);
  iVar49 = ((uVar34 ^ uVar17) & uVar28 ^ uVar34) + uVar26 + 0x106aa070 + uVar18 +
           ((uVar28 >> 0xb | uVar28 * 0x200000) ^ (uVar28 >> 6 | uVar28 * 0x4000000) ^
           (uVar28 >> 0x19 | uVar28 * 0x80));
  uVar11 = uVar11 + iVar49;
  uVar22 = ((uVar7 >> 0xd | uVar7 * 0x80000) ^ (uVar7 >> 2 | uVar7 * 0x40000000) ^
           (uVar7 >> 0x16 | uVar7 * 0x400)) + ((uVar27 | uVar7) & uVar5 | uVar27 & uVar7) + iVar49;
  uVar13 = ((uVar39 >> 0x12 | uVar39 * 0x4000) ^ (uVar39 >> 7 | uVar39 * 0x2000000) ^ uVar39 >> 3) +
           uVar10 + uVar23 +
           ((uVar4 >> 0x13 | uVar4 * 0x2000) ^ (uVar4 >> 0x11 | uVar4 * 0x8000) ^ uVar4 >> 10);
  iVar49 = ((uVar17 ^ uVar28) & uVar11 ^ uVar17) + uVar13 + 0x19a4c116 + uVar34 +
           ((uVar11 >> 0xb | uVar11 * 0x200000) ^ (uVar11 >> 6 | uVar11 * 0x4000000) ^
           (uVar11 >> 0x19 | uVar11 * 0x80));
  uVar5 = uVar5 + iVar49;
  uVar10 = ((uVar22 >> 0xd | uVar22 * 0x80000) ^ (uVar22 >> 2 | uVar22 * 0x40000000) ^
           (uVar22 >> 0x16 | uVar22 * 0x400)) + ((uVar7 | uVar22) & uVar27 | uVar7 & uVar22) +
           iVar49;
  uVar34 = ((uVar40 >> 0x12 | uVar40 * 0x4000) ^ (uVar40 >> 7 | uVar40 * 0x2000000) ^ uVar40 >> 3) +
           uVar39 + uVar8 +
           ((uVar26 >> 0x13 | uVar26 * 0x2000) ^ (uVar26 >> 0x11 | uVar26 * 0x8000) ^ uVar26 >> 10);
  iVar49 = ((uVar28 ^ uVar11) & uVar5 ^ uVar28) + uVar34 + 0x1e376c08 + uVar17 +
           ((uVar5 >> 0xb | uVar5 * 0x200000) ^ (uVar5 >> 6 | uVar5 * 0x4000000) ^
           (uVar5 >> 0x19 | uVar5 * 0x80));
  uVar35 = ((uVar10 >> 0xd | uVar10 * 0x80000) ^ (uVar10 >> 2 | uVar10 * 0x40000000) ^
           (uVar10 >> 0x16 | uVar10 * 0x400)) + ((uVar22 | uVar10) & uVar7 | uVar22 & uVar10) +
           iVar49;
  uVar27 = uVar27 + iVar49;
  uVar16 = ((uVar6 >> 0x12 | uVar6 * 0x4000) ^ (uVar6 >> 7 | uVar6 * 0x2000000) ^ uVar6 >> 3) +
           uVar40 + uVar42 +
           ((uVar13 >> 0x13 | uVar13 * 0x2000) ^ (uVar13 >> 0x11 | uVar13 * 0x8000) ^ uVar13 >> 10);
  iVar49 = ((uVar11 ^ uVar5) & uVar27 ^ uVar11) + uVar16 + 0x2748774c + uVar28 +
           ((uVar27 >> 0xb | uVar27 * 0x200000) ^ (uVar27 >> 6 | uVar27 * 0x4000000) ^
           (uVar27 >> 0x19 | uVar27 * 0x80));
  uVar7 = uVar7 + iVar49;
  uVar14 = ((uVar35 >> 0xd | uVar35 * 0x80000) ^ (uVar35 >> 2 | uVar35 * 0x40000000) ^
           (uVar35 >> 0x16 | uVar35 * 0x400)) + ((uVar10 | uVar35) & uVar22 | uVar10 & uVar35) +
           iVar49;
  uVar51 = ((uVar15 >> 0x12 | uVar15 * 0x4000) ^ (uVar15 >> 7 | uVar15 * 0x2000000) ^ uVar15 >> 3) +
           uVar6 + uVar29 +
           ((uVar34 >> 0x13 | uVar34 * 0x2000) ^ (uVar34 >> 0x11 | uVar34 * 0x8000) ^ uVar34 >> 10);
  iVar49 = ((uVar5 ^ uVar27) & uVar7 ^ uVar5) + uVar51 + 0x34b0bcb5 + uVar11 +
           ((uVar7 >> 0xb | uVar7 * 0x200000) ^ (uVar7 >> 6 | uVar7 * 0x4000000) ^
           (uVar7 >> 0x19 | uVar7 * 0x80));
  uVar22 = uVar22 + iVar49;
  uVar6 = ((uVar14 >> 0xd | uVar14 * 0x80000) ^ (uVar14 >> 2 | uVar14 * 0x40000000) ^
          (uVar14 >> 0x16 | uVar14 * 0x400)) + ((uVar35 | uVar14) & uVar10 | uVar35 & uVar14) +
          iVar49;
  uVar37 = ((uVar55 >> 0x12 | uVar55 * 0x4000) ^ (uVar55 >> 7 | uVar55 * 0x2000000) ^ uVar55 >> 3) +
           uVar15 + uVar19 +
           ((uVar16 >> 0x13 | uVar16 * 0x2000) ^ (uVar16 >> 0x11 | uVar16 * 0x8000) ^ uVar16 >> 10);
  iVar49 = ((uVar27 ^ uVar7) & uVar22 ^ uVar27) + uVar37 + 0x391c0cb3 + uVar5 +
           ((uVar22 >> 0xb | uVar22 * 0x200000) ^ (uVar22 >> 6 | uVar22 * 0x4000000) ^
           (uVar22 >> 0x19 | uVar22 * 0x80));
  uVar10 = uVar10 + iVar49;
  uVar15 = ((uVar6 >> 0xd | uVar6 * 0x80000) ^ (uVar6 >> 2 | uVar6 * 0x40000000) ^
           (uVar6 >> 0x16 | uVar6 * 0x400)) + ((uVar14 | uVar6) & uVar35 | uVar14 & uVar6) + iVar49;
  uVar17 = ((uVar41 >> 0x12 | uVar41 * 0x4000) ^ (uVar41 >> 7 | uVar41 * 0x2000000) ^ uVar41 >> 3) +
           uVar55 + uVar4 +
           ((uVar51 >> 0x13 | uVar51 * 0x2000) ^ (uVar51 >> 0x11 | uVar51 * 0x8000) ^ uVar51 >> 10);
  iVar49 = ((uVar7 ^ uVar22) & uVar10 ^ uVar7) + uVar17 + 0x4ed8aa4a + uVar27 +
           ((uVar10 >> 0xb | uVar10 * 0x200000) ^ (uVar10 >> 6 | uVar10 * 0x4000000) ^
           (uVar10 >> 0x19 | uVar10 * 0x80));
  uVar35 = uVar35 + iVar49;
  uVar5 = ((uVar15 >> 0xd | uVar15 * 0x80000) ^ (uVar15 >> 2 | uVar15 * 0x40000000) ^
          (uVar15 >> 0x16 | uVar15 * 0x400)) + ((uVar6 | uVar15) & uVar14 | uVar6 & uVar15) + iVar49
  ;
  uVar28 = ((uVar9 >> 0x12 | uVar9 * 0x4000) ^ (uVar9 >> 7 | uVar9 * 0x2000000) ^ uVar9 >> 3) +
           uVar41 + uVar26 +
           ((uVar37 >> 0x13 | uVar37 * 0x2000) ^ (uVar37 >> 0x11 | uVar37 * 0x8000) ^ uVar37 >> 10);
  iVar49 = ((uVar22 ^ uVar10) & uVar35 ^ uVar22) + uVar28 + 0x5b9cca4f + uVar7 +
           ((uVar35 >> 0xb | uVar35 * 0x200000) ^ (uVar35 >> 6 | uVar35 * 0x4000000) ^
           (uVar35 >> 0x19 | uVar35 * 0x80));
  uVar14 = uVar14 + iVar49;
  uVar7 = ((uVar5 >> 0xd | uVar5 * 0x80000) ^ (uVar5 >> 2 | uVar5 * 0x40000000) ^
          (uVar5 >> 0x16 | uVar5 * 0x400)) + ((uVar15 | uVar5) & uVar6 | uVar15 & uVar5) + iVar49;
  uVar39 = ((uVar12 >> 0x12 | uVar12 * 0x4000) ^ (uVar12 >> 7 | uVar12 * 0x2000000) ^ uVar12 >> 3) +
           uVar9 + uVar13 +
           ((uVar17 >> 0x13 | uVar17 * 0x2000) ^ (uVar17 >> 0x11 | uVar17 * 0x8000) ^ uVar17 >> 10);
  iVar49 = ((uVar10 ^ uVar35) & uVar14 ^ uVar10) + uVar39 + 0x682e6ff3 + uVar22 +
           ((uVar14 >> 0xb | uVar14 * 0x200000) ^ (uVar14 >> 6 | uVar14 * 0x4000000) ^
           (uVar14 >> 0x19 | uVar14 * 0x80));
  uVar6 = uVar6 + iVar49;
  uVar40 = ((uVar7 >> 0xd | uVar7 * 0x80000) ^ (uVar7 >> 2 | uVar7 * 0x40000000) ^
           (uVar7 >> 0x16 | uVar7 * 0x400)) + ((uVar5 | uVar7) & uVar15 | uVar5 & uVar7) + iVar49;
  uVar34 = ((uVar23 >> 0x12 | uVar23 * 0x4000) ^ (uVar23 >> 7 | uVar23 * 0x2000000) ^ uVar23 >> 3) +
           uVar12 + uVar34 +
           ((uVar28 >> 0x13 | uVar28 * 0x2000) ^ (uVar28 >> 0x11 | uVar28 * 0x8000) ^ uVar28 >> 10);
  iVar49 = ((uVar35 ^ uVar14) & uVar6 ^ uVar35) + uVar34 + 0x748f82ee + uVar10 +
           ((uVar6 >> 0xb | uVar6 * 0x200000) ^ (uVar6 >> 6 | uVar6 * 0x4000000) ^
           (uVar6 >> 0x19 | uVar6 * 0x80));
  uVar15 = uVar15 + iVar49;
  uVar22 = ((uVar40 >> 0xd | uVar40 * 0x80000) ^ (uVar40 >> 2 | uVar40 * 0x40000000) ^
           (uVar40 >> 0x16 | uVar40 * 0x400)) + ((uVar7 | uVar40) & uVar5 | uVar7 & uVar40) + iVar49
  ;
  uVar27 = ((uVar8 >> 0x12 | uVar8 * 0x4000) ^ (uVar8 >> 7 | uVar8 * 0x2000000) ^ uVar8 >> 3) +
           uVar23 + uVar16 +
           ((uVar39 >> 0x13 | uVar39 * 0x2000) ^ (uVar39 >> 0x11 | uVar39 * 0x8000) ^ uVar39 >> 10);
  iVar49 = ((uVar14 ^ uVar6) & uVar15 ^ uVar14) + uVar27 + 0x78a5636f + uVar35 +
           ((uVar15 >> 0xb | uVar15 * 0x200000) ^ (uVar15 >> 6 | uVar15 * 0x4000000) ^
           (uVar15 >> 0x19 | uVar15 * 0x80));
  uVar5 = uVar5 + iVar49;
  uVar16 = ((uVar22 >> 0xd | uVar22 * 0x80000) ^ (uVar22 >> 2 | uVar22 * 0x40000000) ^
           (uVar22 >> 0x16 | uVar22 * 0x400)) + ((uVar40 | uVar22) & uVar7 | uVar40 & uVar22) +
           iVar49;
  uVar51 = ((uVar42 >> 0x12 | uVar42 * 0x4000) ^ (uVar42 >> 7 | uVar42 * 0x2000000) ^ uVar42 >> 3) +
           uVar8 + uVar51 +
           ((uVar34 >> 0x13 | uVar34 * 0x2000) ^ (uVar34 >> 0x11 | uVar34 * 0x8000) ^ uVar34 >> 10);
  iVar49 = ((uVar6 ^ uVar15) & uVar5 ^ uVar6) + uVar51 + 0x84c87814 + uVar14 +
           ((uVar5 >> 0xb | uVar5 * 0x200000) ^ (uVar5 >> 6 | uVar5 * 0x4000000) ^
           (uVar5 >> 0x19 | uVar5 * 0x80));
  uVar7 = uVar7 + iVar49;
  uVar23 = ((uVar16 >> 0xd | uVar16 * 0x80000) ^ (uVar16 >> 2 | uVar16 * 0x40000000) ^
           (uVar16 >> 0x16 | uVar16 * 0x400)) + ((uVar22 | uVar16) & uVar40 | uVar22 & uVar16) +
           iVar49;
  uVar37 = ((uVar29 >> 0x12 | uVar29 * 0x4000) ^ (uVar29 >> 7 | uVar29 * 0x2000000) ^ uVar29 >> 3) +
           uVar42 + uVar37 +
           ((uVar27 >> 0x13 | uVar27 * 0x2000) ^ (uVar27 >> 0x11 | uVar27 * 0x8000) ^ uVar27 >> 10);
  iVar49 = ((uVar15 ^ uVar5) & uVar7 ^ uVar15) + uVar37 + 0x8cc70208 + uVar6 +
           ((uVar7 >> 0xb | uVar7 * 0x200000) ^ (uVar7 >> 6 | uVar7 * 0x4000000) ^
           (uVar7 >> 0x19 | uVar7 * 0x80));
  uVar35 = ((uVar23 >> 0xd | uVar23 * 0x80000) ^ (uVar23 >> 2 | uVar23 * 0x40000000) ^
           (uVar23 >> 0x16 | uVar23 * 0x400)) + ((uVar16 | uVar23) & uVar22 | uVar16 & uVar23) +
           iVar49;
  uVar40 = uVar40 + iVar49;
  uVar27 = ((uVar19 >> 0x12 | uVar19 * 0x4000) ^ (uVar19 >> 7 | uVar19 * 0x2000000) ^ uVar19 >> 3) +
           uVar29 + uVar17 +
           ((uVar51 >> 0x13 | uVar51 * 0x2000) ^ (uVar51 >> 0x11 | uVar51 * 0x8000) ^ uVar51 >> 10);
  iVar49 = ((uVar5 ^ uVar7) & uVar40 ^ uVar5) + uVar27 + 0x90befffa + uVar15 +
           ((uVar40 >> 0xb | uVar40 * 0x200000) ^ (uVar40 >> 6 | uVar40 * 0x4000000) ^
           (uVar40 >> 0x19 | uVar40 * 0x80));
  uVar17 = ((uVar35 >> 0xd | uVar35 * 0x80000) ^ (uVar35 >> 2 | uVar35 * 0x40000000) ^
           (uVar35 >> 0x16 | uVar35 * 0x400)) + ((uVar23 | uVar35) & uVar16 | uVar23 & uVar35) +
           iVar49;
  uVar22 = uVar22 + iVar49;
  uVar37 = ((uVar4 >> 0x12 | uVar4 * 0x4000) ^ (uVar4 >> 7 | uVar4 * 0x2000000) ^ uVar4 >> 3) +
           uVar19 + uVar28 +
           ((uVar37 >> 0x13 | uVar37 * 0x2000) ^ (uVar37 >> 0x11 | uVar37 * 0x8000) ^ uVar37 >> 10);
  iVar49 = ((uVar7 ^ uVar40) & uVar22 ^ uVar7) + uVar37 + 0xa4506ceb + uVar5 +
           ((uVar22 >> 0xb | uVar22 * 0x200000) ^ (uVar22 >> 6 | uVar22 * 0x4000000) ^
           (uVar22 >> 0x19 | uVar22 * 0x80));
  uVar16 = uVar16 + iVar49;
  uVar5 = ((uVar17 >> 0xd | uVar17 * 0x80000) ^ (uVar17 >> 2 | uVar17 * 0x40000000) ^
          (uVar17 >> 0x16 | uVar17 * 0x400)) + ((uVar35 | uVar17) & uVar23 | uVar35 & uVar17) +
          iVar49;
  iVar49 = ((uVar26 >> 0x12 | uVar26 * 0x4000) ^ (uVar26 >> 7 | uVar26 * 0x2000000) ^ uVar26 >> 3) +
           uVar4 + 0xbef9a3f7 + uVar39 +
           ((uVar27 >> 0x13 | uVar27 * 0x2000) ^ (uVar27 >> 0x11 | uVar27 * 0x8000) ^ uVar27 >> 10)
           + uVar7 + ((uVar40 ^ uVar22) & uVar16 ^ uVar40) +
           ((uVar16 >> 0xb | uVar16 * 0x200000) ^ (uVar16 >> 6 | uVar16 * 0x4000000) ^
           (uVar16 >> 0x19 | uVar16 * 0x80));
  uVar23 = uVar23 + iVar49;
  uVar7 = ((uVar5 >> 0xd | uVar5 * 0x80000) ^ (uVar5 >> 2 | uVar5 * 0x40000000) ^
          (uVar5 >> 0x16 | uVar5 * 0x400)) + ((uVar17 | uVar5) & uVar35 | uVar17 & uVar5) + iVar49;
  iVar49 = ((uVar13 >> 0x12 | uVar13 * 0x4000) ^ (uVar13 >> 7 | uVar13 * 0x2000000) ^ uVar13 >> 3) +
           uVar26 + 0xc67178f2 + uVar34 +
           ((uVar37 >> 0x13 | uVar37 * 0x2000) ^ (uVar37 >> 0x11 | uVar37 * 0x8000) ^ uVar37 >> 10)
           + uVar40 + ((uVar22 ^ uVar16) & uVar23 ^ uVar22) +
           ((uVar23 >> 0xb | uVar23 * 0x200000) ^ (uVar23 >> 6 | uVar23 * 0x4000000) ^
           (uVar23 >> 0x19 | uVar23 * 0x80));
  *(uint *)(param_1 + 0x24) = *(int *)(param_1 + 0x24) + uVar22;
  *(uint *)(param_1 + 0x14) = *(int *)(param_1 + 0x14) + uVar17;
  *(uint *)(param_1 + 0x20) = uVar31 + uVar16;
  *(uint *)(param_1 + 0x10) = uVar33 + uVar5;
  *(uint *)(param_1 + 0xc) = uVar32 + uVar7;
  *(uint *)(param_1 + 0x1c) = uVar25 + uVar23;
  *(uint *)(param_1 + 8) =
       ((uVar7 >> 0xd | uVar7 * 0x80000) ^ (uVar7 >> 2 | uVar7 * 0x40000000) ^
       (uVar7 >> 0x16 | uVar7 * 0x400)) + ((uVar5 | uVar7) & uVar17 | uVar5 & uVar7) + uVar38 +
       iVar49;
  *(uint *)(param_1 + 0x18) = uVar36 + uVar35 + iVar49;
  return;
}



//==================== FUN_040028a8 @ 040028a8 ====================

void FUN_040028a8(uint *param_1,int param_2,uint param_3)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  
  uVar4 = *param_1;
  uVar5 = uVar4 & 0x3f;
  *param_1 = uVar4 + param_3;
  uVar2 = 0x40 - uVar5;
  if (CARRY4(uVar4,param_3)) {
    param_1[1] = param_1[1] + 1;
  }
  if (uVar5 != 0 && uVar2 <= param_3) {
    param_3 = param_3 - uVar2;
    FUN_04005ea4((int)(param_1 + 10) + uVar5,param_2,uVar2);
    param_2 = param_2 + uVar2;
    uVar5 = 0;
    FUN_04000090(param_1,param_1 + 10);
  }
  if (0x3f < param_3) {
    iVar3 = param_2 + (param_3 - 0x40 & 0xffffffc0) + 0x40;
    iVar1 = param_2;
    do {
      param_2 = iVar1 + 0x40;
      FUN_04000090(param_1,iVar1);
      iVar1 = param_2;
    } while (param_2 != iVar3);
    param_3 = param_3 & 0x3f;
  }
  if (param_3 == 0) {
    return;
  }
  FUN_04005ea4((int)param_1 + uVar5 + 0x28,param_2,param_3);
  return;
}



//==================== FUN_040029f4 @ 040029f4 ====================

void FUN_040029f4(uint *param_1,undefined1 *param_2)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  uint uVar4;
  undefined1 local_18;
  undefined1 local_17;
  undefined1 local_16;
  byte local_15;
  undefined1 local_14;
  undefined1 local_13;
  undefined1 local_12;
  undefined1 local_11;
  
  uVar4 = *param_1;
  uVar1 = param_1[1];
  uVar2 = uVar4 & 0x3f;
  local_15 = (byte)(uVar4 >> 0x1d) | (byte)(uVar1 << 3);
  local_11 = (undefined1)(uVar4 << 3);
  local_14 = (undefined1)((uVar4 & 0x1fffffff) >> 0x15);
  local_17 = (undefined1)((uVar1 & 0x1fffffff) >> 0xd);
  if (uVar2 < 0x38) {
    iVar3 = 0x38 - uVar2;
  }
  else {
    iVar3 = 0x78 - uVar2;
  }
  local_16 = (undefined1)((uVar1 & 0x1fffffff) >> 5);
  local_13 = (undefined1)((uVar4 & 0x1fffffff) >> 0xd);
  local_12 = (undefined1)((uVar4 & 0x1fffffff) >> 5);
  local_18 = (undefined1)((uVar1 & 0x1fffffff) >> 0x15);
  FUN_040028a8(param_1,DAT_04002ba4,iVar3);
  FUN_040028a8(param_1,&local_18,8);
  *param_2 = *(undefined1 *)((int)param_1 + 0xb);
  param_2[1] = (char)*(undefined2 *)((int)param_1 + 10);
  param_2[2] = (char)(param_1[2] >> 8);
  param_2[3] = (char)param_1[2];
  param_2[4] = *(undefined1 *)((int)param_1 + 0xf);
  param_2[5] = (char)*(undefined2 *)((int)param_1 + 0xe);
  param_2[6] = (char)(param_1[3] >> 8);
  param_2[7] = (char)param_1[3];
  param_2[8] = *(undefined1 *)((int)param_1 + 0x13);
  param_2[9] = (char)*(undefined2 *)((int)param_1 + 0x12);
  param_2[10] = (char)(param_1[4] >> 8);
  param_2[0xb] = (char)param_1[4];
  param_2[0xc] = *(undefined1 *)((int)param_1 + 0x17);
  param_2[0xd] = (char)*(undefined2 *)((int)param_1 + 0x16);
  param_2[0xe] = (char)(param_1[5] >> 8);
  param_2[0xf] = (char)param_1[5];
  param_2[0x10] = *(undefined1 *)((int)param_1 + 0x1b);
  param_2[0x11] = (char)*(undefined2 *)((int)param_1 + 0x1a);
  param_2[0x12] = (char)(param_1[6] >> 8);
  param_2[0x13] = (char)param_1[6];
  param_2[0x14] = *(undefined1 *)((int)param_1 + 0x1f);
  param_2[0x15] = (char)*(undefined2 *)((int)param_1 + 0x1e);
  param_2[0x16] = (char)(param_1[7] >> 8);
  param_2[0x17] = (char)param_1[7];
  param_2[0x18] = *(undefined1 *)((int)param_1 + 0x23);
  param_2[0x19] = (char)*(undefined2 *)((int)param_1 + 0x22);
  param_2[0x1a] = (char)(param_1[8] >> 8);
  param_2[0x1b] = (char)param_1[8];
  param_2[0x1c] = *(undefined1 *)((int)param_1 + 0x27);
  param_2[0x1d] = (char)*(undefined2 *)((int)param_1 + 0x26);
  param_2[0x1e] = (char)(param_1[9] >> 8);
  param_2[0x1f] = (char)param_1[9];
  return;
}



//==================== FUN_04002c50 @ 04002c50 ====================

uint FUN_04002c50(void)

{
  int iVar1;
  uint uVar2;
  int in_r3;
  uint uVar3;
  int iVar4;
  uint in_stack_00000000;
  uint in_stack_00000004;
  uint *in_stack_00000008;
  uint local_14;
  
  if (((in_stack_00000000 < 2) && (in_stack_00000004 < 2)) &&
     (iVar1 = func_0x03ffae84(), iVar1 != 0)) {
    in_r3 = in_r3 * (in_stack_00000004 + in_stack_00000000);
    iVar4 = iVar1 + in_r3 * 4;
    if (iVar4 + in_stack_00000000 * 4 + in_stack_00000004 * 4 <= iVar1 + (local_14 & 0xfffffffc)) {
      if (in_stack_00000000 == 0) {
        uVar2 = 0;
      }
      else {
        uVar2 = *(uint *)(iVar1 + in_r3 * 4);
        uVar2 = uVar2 << 0x18 | (uVar2 >> 8 & 0xff) << 0x10 | (uVar2 >> 0x10 & 0xff) << 8 |
                uVar2 >> 0x18;
      }
      if (in_stack_00000008 == (uint *)0x0) {
        return uVar2;
      }
      uVar3 = 0;
      if (in_stack_00000004 != 0) {
        uVar3 = *(uint *)(iVar4 + in_stack_00000000 * 4);
        uVar3 = uVar3 << 0x18 | (uVar3 >> 8 & 0xff) << 0x10 | (uVar3 >> 0x10 & 0xff) << 8 |
                uVar3 >> 0x18;
      }
      *in_stack_00000008 = uVar3;
      return uVar2;
    }
  }
  return 0xffffffff;
}



//==================== FUN_04002ce8 @ 04002ce8 ====================

void FUN_04002ce8(void)

{
  FUN_04002c50();
  return;
}



//==================== FUN_04002f50 @ 04002f50 ====================

undefined4 FUN_04002f50(void)

{
  uint *puVar1;
  undefined4 uVar2;
  uint *puVar4;
  uint *puVar3;
  uint uVar5;
  int in_r3;
  uint *puVar6;
  int in_stack_00000000;
  int local_14;
  
  puVar1 = (uint *)func_0x03ffae84();
  if (puVar1 == (uint *)0x0) {
    uVar2 = 0xffffffff;
  }
  else if (local_14 < in_stack_00000000 * 4) {
    uVar2 = 0xfffffff4;
  }
  else {
    if (0 < in_stack_00000000) {
      puVar6 = (uint *)(in_r3 + -4);
      puVar3 = puVar1;
      do {
        puVar4 = puVar3 + 1;
        uVar5 = *puVar3;
        puVar6 = puVar6 + 1;
        *puVar6 = uVar5 << 0x18 | (uVar5 >> 8 & 0xff) << 0x10 | (uVar5 >> 0x10 & 0xff) << 8 |
                  uVar5 >> 0x18;
        puVar3 = puVar4;
      } while (puVar4 != puVar1 + in_stack_00000000);
    }
    uVar2 = 0;
  }
  return uVar2;
}



//==================== FUN_04002fbc @ 04002fbc ====================

bool FUN_04002fbc(void)

{
  int iVar1;
  
  iVar1 = func_0x03ffae84();
  return iVar1 != 0;
}



//==================== FUN_040030a8 @ 040030a8 ====================

void FUN_040030a8(int *param_1,int *param_2,undefined4 *param_3,int param_4)

{
  int iVar1;
  
  func_0x03fc8570(*param_3);
  func_0x03fc8570(param_3[1]);
  iVar1 = *param_1;
  param_3[2] = 0;
  param_3[3] = 0;
  *(undefined4 *)(iVar1 + param_4 * 0x14) = 0xffffffff;
  *param_2 = *param_2 + -1;
  return;
}



//==================== FUN_040030f8 @ 040030f8 ====================

undefined4 FUN_040030f8(undefined4 param_1,int param_2,int *param_3)

{
  int iVar1;
  int *piVar3;
  int *piVar2;
  undefined4 uVar4;
  
  if (param_2 < 1) {
    uVar4 = 0;
  }
  else {
    uVar4 = 0;
    piVar2 = param_3;
    do {
      piVar3 = piVar2 + 1;
      if ((*piVar2 != 0) && (iVar1 = FUN_04005a60(param_1), iVar1 == 0)) {
        uVar4 = 1;
        *piVar2 = 0;
      }
      piVar2 = piVar3;
    } while (piVar3 != param_3 + param_2);
  }
  return uVar4;
}



//==================== FUN_04003148 @ 04003148 ====================

bool FUN_04003148(uint param_1,undefined4 param_2,undefined4 param_3,int param_4)

{
  int iVar1;
  undefined4 uVar2;
  undefined1 auStack_98 [128];
  
  param_1 = param_1 & 0x1c0;
  if (param_1 == 0x80) {
    iVar1 = FUN_04006044(param_2,param_3);
    return iVar1 != 0;
  }
  if (param_1 == 0x100) {
    iVar1 = *(int *)(param_4 + 0x208);
    uVar2 = FUN_04005b98(param_2);
    iVar1 = FUN_0400583c(param_4,param_2,uVar2,auStack_98 + (0x80 - (iVar1 * 8 + 0x18)));
    return iVar1 != 0;
  }
  if (param_1 == 0x40) {
    iVar1 = FUN_04005a60(param_2,param_3);
    return iVar1 == 0;
  }
  FUN_04007548(DAT_0400320c,param_1);
  return false;
}



//==================== FUN_04003210 @ 04003210 ====================

undefined4
FUN_04003210(undefined4 *param_1,undefined4 *param_2,uint param_3,int param_4,int param_5)

{
  int iVar1;
  undefined4 *puVar2;
  int iVar3;
  undefined1 auStack_234 [528];
  undefined4 local_24;
  
  if (0 < param_4) {
    iVar3 = 0;
    puVar2 = (undefined4 *)(param_5 + -4);
    do {
      while( true ) {
        puVar2 = puVar2 + 1;
        iVar1 = FUN_04005718(auStack_234,*puVar2);
        if (iVar1 == 0) {
          FUN_04007548(DAT_040032fc,local_24);
          return 0;
        }
        if (((param_3 & 0x10) != 0) &&
           (iVar1 = FUN_04003148(param_3,*param_1,*puVar2,auStack_234), iVar1 != 0)) {
          return 1;
        }
        if ((param_3 & 0x20) != 0) break;
        iVar3 = iVar3 + 1;
        if (iVar3 == param_4) {
          return 0;
        }
      }
      iVar1 = FUN_04003148(param_3,*param_2,*puVar2,auStack_234);
      if (iVar1 != 0) {
        return 1;
      }
      iVar3 = iVar3 + 1;
    } while (iVar3 != param_4);
  }
  return 0;
}



//==================== FUN_04003300 @ 04003300 ====================

bool FUN_04003300(uint param_1,int *param_2)

{
  uint extraout_r1;
  uint uVar1;
  uint uVar2;
  int iVar3;
  
  param_1 = param_1 | 1;
  do {
    uVar1 = param_1 * -0x55555555;
    uVar2 = param_1 % 3;
    if (param_1 < 10) {
LAB_04003364:
      if (uVar2 != 0) {
        param_2[1] = param_1;
        param_2[2] = 0;
        iVar3 = func_0x03fc8dc4(param_1 + 1,0x14);
        *param_2 = iVar3;
        return iVar3 != 0;
      }
    }
    else if (uVar2 != 0) {
      iVar3 = 3;
      do {
        iVar3 = iVar3 + 2;
        FUN_0400c7d4(param_1,iVar3,uVar1);
        uVar1 = iVar3 * iVar3;
        uVar2 = extraout_r1;
        if (param_1 <= uVar1) goto LAB_04003364;
      } while (extraout_r1 != 0);
    }
    param_1 = param_1 + 2;
  } while( true );
}



//==================== FUN_0400339c @ 0400339c ====================

void FUN_0400339c(int *param_1)

{
  int *piVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  uint uVar5;
  int iVar6;
  
  iVar2 = *param_1;
  uVar3 = param_1[1];
  if (uVar3 != 0) {
    iVar4 = 0x14;
    uVar5 = 1;
    do {
      piVar1 = (int *)(iVar2 + iVar4);
      iVar6 = iVar2 + iVar4;
      uVar5 = uVar5 + 1;
      iVar4 = iVar4 + 0x14;
      if (0 < *piVar1) {
        func_0x03fc8570(*(undefined4 *)(iVar6 + 4));
        func_0x03fc8570(*(undefined4 *)(iVar6 + 8));
        iVar2 = *param_1;
        uVar3 = param_1[1];
      }
    } while (uVar5 <= uVar3);
  }
  func_0x03fc8570();
  *param_1 = 0;
  return;
}



//==================== FUN_040033fc @ 040033fc ====================

uint FUN_040033fc(undefined4 param_1,int param_2,int *param_3,int *param_4)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  
  uVar4 = param_2 + 1;
  uVar1 = FUN_04005b98();
  uVar5 = param_4[1];
  if (uVar4 < uVar5) {
    iVar3 = uVar4 * 0x14;
    do {
      if (0 < *(int *)(*param_4 + iVar3)) {
        iVar2 = FUN_04005a90(param_1,*(undefined4 *)(*param_4 + iVar3 + 4),uVar1);
        if (iVar2 == 0) {
          *param_3 = *param_4 + iVar3 + 4;
          return uVar4;
        }
        uVar5 = param_4[1];
      }
      uVar4 = uVar4 + 1;
      iVar3 = iVar3 + 0x14;
    } while (uVar4 < uVar5);
  }
  *DAT_040034a4 = 3;
  *param_3 = 0;
  return 0;
}



//==================== FUN_040034a8 @ 040034a8 ====================

uint FUN_040034a8(byte *param_1,int param_2,undefined4 param_3,undefined4 param_4,int param_5,
                 int *param_6,int *param_7,undefined4 param_8)

{
  int iVar1;
  uint extraout_r1;
  int extraout_r1_00;
  undefined4 uVar2;
  uint uVar3;
  byte *pbVar4;
  uint uVar5;
  uint uVar6;
  int iVar7;
  int iVar8;
  code *pcVar9;
  uint *puVar10;
  uint uVar11;
  uint local_34;
  
  iVar1 = FUN_04005b98(param_1);
  if (iVar1 == 0) {
    iVar8 = param_7[1];
    iVar1 = 0x14;
    uVar6 = 1;
  }
  else {
    pbVar4 = param_1 + iVar1;
    do {
      pbVar4 = pbVar4 + -1;
      iVar1 = (uint)*pbVar4 + iVar1 * 0x10;
    } while (pbVar4 != param_1);
    iVar8 = param_7[1];
    FUN_0400c7d4(iVar1,iVar8);
    if (extraout_r1 == 0) {
      iVar1 = 0x14;
      uVar6 = 1;
    }
    else {
      iVar1 = extraout_r1 * 0x14;
      uVar6 = extraout_r1;
    }
  }
  uVar5 = *(uint *)(*param_7 + iVar1);
  puVar10 = (uint *)(*param_7 + iVar1);
  local_34 = uVar6;
  if (uVar5 == 0) {
    if (param_5 == 1) {
      if (param_7[2] != iVar8) {
LAB_04003600:
        *puVar10 = uVar6;
        uVar6 = FUN_04005c20(param_1);
        iVar7 = *param_7;
        puVar10[1] = uVar6;
        uVar2 = FUN_04005c20(param_2);
        iVar8 = *param_7;
        *(undefined4 *)(iVar7 + iVar1 + 8) = uVar2;
        iVar8 = iVar8 + iVar1;
        if ((*(int *)(iVar8 + 4) != 0) && (*(int *)(iVar8 + 8) != 0)) {
          param_7[2] = param_7[2] + 1;
          func_0x03fc3288(iVar8 + 4);
          func_0x03fc35fc(*param_7 + iVar1 + 4);
          if (((code *)param_7[3] == (code *)0x0) ||
             (iVar8 = (*(code *)param_7[3])(*param_7 + iVar1 + 4,param_2,0,param_8), iVar8 == 0)) {
            iVar8 = *param_7 + iVar1;
            if (*(code **)(iVar8 + 0xc) == (code *)0x0) {
LAB_040036d4:
              *param_6 = iVar8 + 4;
              return 1;
            }
            iVar8 = (**(code **)(iVar8 + 0xc))(param_1,param_2,0,param_8);
            if (iVar8 == 0) {
              iVar8 = *param_7 + iVar1;
              goto LAB_040036d4;
            }
            FUN_040030a8(param_7,param_7 + 2,*param_7 + iVar1 + 4,local_34);
LAB_0400391c:
            uVar2 = 0x16;
            goto LAB_040035c8;
          }
          FUN_040030a8(param_7,param_7 + 2,*param_7 + iVar1 + 4,local_34);
          goto LAB_04003944;
        }
      }
LAB_04003840:
      uVar2 = 0xc;
      goto LAB_040035c8;
    }
  }
  else {
    if (uVar5 != 0xffffffff) {
      local_34 = 0;
    }
    if (uVar6 == uVar5) {
      iVar8 = FUN_04005a60(param_1,puVar10[1]);
      if (iVar8 != 0) {
        iVar8 = param_7[1];
        goto LAB_04003538;
      }
      iVar7 = *param_7;
      iVar8 = iVar7 + iVar1;
      if (param_2 == 0 || param_5 != 1) goto LAB_040038ec;
      if ((code *)param_7[3] == (code *)0x0) {
LAB_040037dc:
        pcVar9 = *(code **)(iVar7 + iVar1 + 0xc);
        if (pcVar9 == (code *)0x0) {
LAB_0400380c:
          func_0x03fc8570(*(undefined4 *)(iVar7 + iVar1 + 8));
          iVar7 = *param_7;
          uVar2 = FUN_04005c20(param_2);
          iVar8 = *param_7;
          *(undefined4 *)(iVar7 + iVar1 + 8) = uVar2;
          iVar8 = iVar8 + iVar1;
          if (*(int *)(iVar8 + 8) != 0) {
LAB_040038ec:
            *param_6 = iVar8 + 4;
            return uVar6;
          }
          goto LAB_04003840;
        }
        iVar8 = (*pcVar9)(param_1,param_2,2,param_8);
        if (iVar8 == 0) {
          iVar7 = *param_7;
          goto LAB_0400380c;
        }
        goto LAB_0400391c;
      }
      iVar8 = (*(code *)param_7[3])(iVar8 + 4,param_2,2,param_8);
      if (iVar8 == 0) {
        iVar7 = *param_7;
        goto LAB_040037dc;
      }
LAB_04003944:
      uVar2 = 1;
      goto LAB_040035c8;
    }
LAB_04003538:
    FUN_0400c7d4(uVar6,iVar8 + -2);
    uVar11 = extraout_r1_00 + 1;
    uVar5 = uVar6;
    do {
      while( true ) {
        if (uVar11 < uVar5) {
          iVar1 = -uVar11;
        }
        else {
          iVar1 = param_7[1] - uVar11;
        }
        uVar5 = uVar5 + iVar1;
        if (uVar6 == uVar5) goto LAB_040035b4;
        iVar1 = uVar5 * 0x14;
        uVar3 = *(uint *)(*param_7 + uVar5 * 0x14);
        if (uVar6 == uVar3) break;
LAB_04003550:
        if (uVar3 == 0) goto LAB_040035b4;
      }
      iVar8 = FUN_04005a60(param_1,*(undefined4 *)(*param_7 + iVar1 + 4));
      if (iVar8 == 0) {
        puVar10 = (uint *)(*param_7 + iVar1);
        if (param_2 != 0 && param_5 == 1) {
          if ((code *)param_7[3] != (code *)0x0) {
            iVar8 = (*(code *)param_7[3])(puVar10 + 1,param_2,2,param_8);
            if (iVar8 != 0) goto LAB_04003944;
            puVar10 = (uint *)(*param_7 + iVar1);
          }
          if ((code *)puVar10[3] != (code *)0x0) {
            iVar8 = (*(code *)puVar10[3])(param_1,param_2,2,param_8);
            if (iVar8 != 0) goto LAB_0400391c;
            puVar10 = (uint *)(*param_7 + iVar1);
          }
          func_0x03fc8570(puVar10[2]);
          iVar7 = *param_7;
          uVar2 = FUN_04005c20(param_2);
          iVar8 = *param_7;
          *(undefined4 *)(iVar7 + iVar1 + 8) = uVar2;
          puVar10 = (uint *)(iVar8 + iVar1);
          if (puVar10[2] == 0) goto LAB_04003840;
        }
        *param_6 = (int)(puVar10 + 1);
        if (uVar5 != 0xffffffff) {
          return uVar5;
        }
        uVar3 = *puVar10;
        goto LAB_04003550;
      }
    } while (*(int *)(*param_7 + uVar5 * 0x14) != 0);
LAB_040035b4:
    if (param_5 == 1) {
      if (param_7[2] != param_7[1]) {
        if (local_34 == 0) {
          local_34 = uVar5;
        }
        iVar1 = local_34 * 0x14;
        puVar10 = (uint *)(*param_7 + iVar1);
        goto LAB_04003600;
      }
      goto LAB_04003840;
    }
  }
  uVar2 = 3;
LAB_040035c8:
  *DAT_04003950 = uVar2;
  *param_6 = 0;
  return 0;
}



//==================== FUN_04003954 @ 04003954 ====================

undefined4 FUN_04003954(undefined4 param_1,int *param_2,undefined4 param_3)

{
  int iVar1;
  int iVar2;
  code *pcVar3;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 uStack_2c;
  undefined4 uStack_28;
  undefined4 uStack_24;
  
  local_30 = param_1;
  iVar1 = FUN_040034a8(param_1,uStack_2c,uStack_28,uStack_24,0,&local_34,param_2,0);
  if (iVar1 == 0) {
    *DAT_04003a64 = 3;
    return 0;
  }
  if (((code *)param_2[3] != (code *)0x0) &&
     (iVar2 = (*(code *)param_2[3])(local_34,0,1,param_3), iVar2 != 0)) {
    *DAT_04003a64 = 1;
    return 0;
  }
  pcVar3 = *(code **)(*param_2 + iVar1 * 0x14 + 0xc);
  if ((pcVar3 == (code *)0x0) || (iVar2 = (*pcVar3)(param_1,0,1,param_3), iVar2 == 0)) {
    FUN_040030a8(param_2,param_2 + 2,local_34,iVar1);
    return 1;
  }
  *DAT_04003a64 = 0x16;
  return 0;
}



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



//==================== FUN_0400428c @ 0400428c ====================

undefined4 FUN_0400428c(void)

{
  FUN_04007548(DAT_040042a8,DAT_040042a4);
  return 0;
}



//==================== FUN_040042ac @ 040042ac ====================

void FUN_040042ac(uint param_1,uint param_2,undefined4 param_3)

{
  uint uVar1;
  char cVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  int iVar8;
  
  uVar7 = 0x3c;
  iVar8 = 0;
  do {
    uVar3 = 0x20 - uVar7;
    uVar4 = uVar7 - 0x20;
    uVar5 = param_1 >> (uVar7 & 0xff) | param_2 << (uVar3 & 0xff) | param_2 >> (uVar4 & 0xff);
    if (uVar5 != 0 || param_2 >> (uVar7 & 0xff) != 0) {
      cVar2 = *(char *)(DAT_040043e0 + iVar8);
      if (cVar2 != '\0') {
        param_1 = param_1 & ~(-1 << (uVar7 & 0xff));
        param_2 = param_2 & ~(-1 << (uVar7 & 0xff) | -1 << (uVar4 & 0xff) |
                             0xffffffffU >> (uVar3 & 0xff));
        if (param_1 == 0 && param_2 == 0) {
          FUN_04007548(DAT_040043e4,uVar5);
        }
        else {
          uVar1 = 1 << (uVar7 - 1 & 0xff);
          uVar6 = (1 << (uVar7 - 0x21 & 0xff) | 1U >> (0x20 - (uVar7 - 1) & 0xff)) +
                  ((param_2 << 2 | param_1 >> 0x1e) + param_2 + (uint)CARRY4(param_1 * 4,param_1)) *
                  2 + (uint)CARRY4(param_1 * 5,param_1 * 5) + (uint)CARRY4(uVar1,param_1 * 10);
          uVar7 = uVar1 + param_1 * 10 >> (uVar7 & 0xff) | uVar6 << (uVar3 & 0xff) |
                  uVar6 >> (uVar4 & 0xff);
          if (9 < uVar7) {
            uVar5 = uVar5 + 1;
            uVar7 = uVar7 - 10;
          }
          FUN_04007548(DAT_040043e4,uVar5);
          if (uVar7 != 0) {
            FUN_04007548(DAT_040043ec,uVar7);
          }
        }
        FUN_04007548(DAT_040043e8,cVar2,param_3);
        return;
      }
      break;
    }
    uVar7 = uVar7 - 10;
    iVar8 = iVar8 + 1;
  } while (uVar7 != 0);
  FUN_04007548(DAT_040043dc,param_2,param_1,param_2,param_3);
  return;
}



//==================== FUN_0400461c @ 0400461c ====================

void FUN_0400461c(undefined4 param_1,uint param_2,uint param_3)

{
  int iVar1;
  int iVar2;
  
  if ((param_2 & 3) != 0) {
    if (param_3 == 0) {
      return;
    }
    do {
      param_2 = param_2 + 1;
      param_3 = param_3 - 1;
      if (param_3 == 0) {
        return;
      }
    } while ((param_2 & 3) != 0);
  }
  iVar1 = param_2 - 4;
  if (param_3 >> 2 != 0) {
    do {
      iVar1 = iVar1 + 4;
    } while (iVar1 != param_2 + ((param_3 >> 2) + 0x3fffffff) * 4);
  }
  if ((param_3 & 3) == 0) {
    return;
  }
  iVar1 = iVar1 + 3;
  iVar2 = iVar1 + (param_3 & 3);
  do {
    iVar1 = iVar1 + 1;
  } while (iVar1 != iVar2);
  return;
}



//==================== FUN_04004700 @ 04004700 ====================

uint FUN_04004700(uint param_1)

{
  uint uVar1;
  
  uVar1 = FUN_0400461c(~param_1);
  return ~uVar1;
}



//==================== FUN_04004714 @ 04004714 ====================

void FUN_04004714(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  uint uVar1;
  uint local_c;
  
  uVar1 = FUN_0400461c(0xffffffff,param_1,param_2);
  uVar1 = ~uVar1;
  local_c = uVar1 << 0x18 | (uVar1 >> 8 & 0xff) << 0x10 | (uVar1 >> 0x10 & 0xff) << 8 |
            uVar1 >> 0x18;
  FUN_04005ea4(param_3,&local_c,4);
  return;
}



//==================== FUN_04004754 @ 04004754 ====================

uint FUN_04004754(uint *param_1,uint param_2)

{
  byte bVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  bool bVar9;
  
  uVar5 = *param_1;
  uVar6 = param_1[1];
  uVar4 = 0;
  if (uVar6 < param_2) {
    uVar7 = 0;
  }
  else {
    uVar7 = FUN_0400c66c(uVar6);
    uVar6 = uVar6 - uVar7 * param_2;
  }
  uVar8 = 0;
  bVar9 = uVar6 == 0;
  uVar2 = 1;
  if (uVar6 == 0) {
    bVar9 = uVar5 <= param_2;
  }
  uVar3 = 0;
  if (param_2 != 0 && !bVar9) {
    do {
      bVar9 = CARRY4(param_2,param_2);
      param_2 = param_2 * 2;
      uVar4 = uVar4 * 2 + (uint)bVar9;
      bVar9 = CARRY4(uVar2,uVar2);
      uVar2 = uVar2 * 2;
      uVar3 = uVar3 * 2 + (uint)bVar9;
      bVar9 = uVar6 <= uVar4;
      if (uVar4 == uVar6) {
        bVar9 = uVar5 <= param_2;
      }
    } while (!bVar9 && (int)(uint)(param_2 == 0) <= (int)uVar4);
  }
  do {
    bVar9 = uVar4 <= uVar6;
    if (uVar6 == uVar4) {
      bVar9 = param_2 <= uVar5;
    }
    if (bVar9) {
      bVar9 = uVar5 < param_2;
      uVar5 = uVar5 - param_2;
      uVar6 = uVar6 - (uVar4 + bVar9);
      bVar9 = CARRY4(uVar8,uVar2);
      uVar8 = uVar8 + uVar2;
      uVar7 = uVar7 + uVar3 + (uint)bVar9;
    }
    bVar1 = (byte)uVar3;
    uVar3 = uVar3 >> 1;
    uVar2 = (uint)(bVar1 & 1) << 0x1f | uVar2 >> 1;
    bVar1 = (byte)uVar4;
    uVar4 = uVar4 >> 1;
    param_2 = (uint)(bVar1 & 1) << 0x1f | param_2 >> 1;
  } while (uVar2 != 0 || uVar3 != 0);
  *param_1 = uVar8;
  param_1[1] = uVar7;
  return uVar5;
}



//==================== FUN_04004844 @ 04004844 ====================

void FUN_04004844(void)

{
  func_0x03fc7fbc(DAT_0400485c);
  func_0x03fbf43c(0xffffffe2);
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}



//==================== FUN_04004898 @ 04004898 ====================

byte * FUN_04004898(byte *param_1)

{
  byte bVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  byte *pbVar5;
  
  iVar2 = DAT_0400492c;
  bVar1 = *(byte *)(DAT_0400492c + (uint)*param_1);
  while ((bVar1 & 0x20) != 0) {
    param_1 = param_1 + 1;
    bVar1 = *(byte *)(DAT_0400492c + (uint)*param_1);
  }
  iVar3 = FUN_04005b98(param_1);
  if (iVar3 != 0) {
    uVar4 = iVar3 - 1;
    pbVar5 = param_1 + uVar4;
    if ((!CARRY4((uint)param_1,uVar4)) && ((*(byte *)(iVar2 + (uint)param_1[uVar4]) & 0x20) != 0)) {
      do {
        pbVar5 = pbVar5 + -1;
        if (pbVar5 == param_1 + -1) break;
      } while ((*(byte *)(iVar2 + (uint)*pbVar5) & 0x20) != 0);
    }
    pbVar5[1] = 0;
  }
  return param_1;
}



//==================== FUN_04004a9c @ 04004a9c ====================

void FUN_04004a9c(int param_1,int param_2,int param_3)

{
  int iVar1;
  
  iVar1 = *(int *)(param_1 + 0x200);
  if (iVar1 < 0x100) {
    *(int *)(param_1 + 0x200) = iVar1 + 1;
    *(undefined1 *)(param_1 + iVar1) = 0;
    iVar1 = iVar1 + 1;
  }
  else {
    *(undefined4 *)(param_1 + 0x210) = DAT_04004af0;
  }
  FUN_04005fa0(param_1 + param_3 + param_2,param_1 + param_2,iVar1 - param_2);
  *(int *)(param_1 + 0x200) = *(int *)(param_1 + 0x200) + param_3;
  return;
}



//==================== FUN_04004af4 @ 04004af4 ====================

void FUN_04004af4(int param_1,int param_2,undefined1 param_3)

{
  char cVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  if (*(char *)(param_1 + param_2) != '\x03') goto LAB_04004b10;
  iVar5 = param_2 + 2;
  if (*(byte *)(param_1 + iVar5) < 2) goto LAB_04004b10;
  *(byte *)(param_1 + iVar5) = *(byte *)(param_1 + iVar5) - 1;
  iVar4 = *(int *)(param_1 + 0x200);
  if (iVar4 < 0x100) {
    iVar3 = iVar4 + 1;
    *(int *)(param_1 + 0x200) = iVar3;
    *(undefined1 *)(param_1 + iVar4) = 3;
    if (iVar3 == 0x100) {
      iVar4 = 0x100;
    }
    cVar1 = *(char *)(param_1 + param_2 + 1);
    cVar2 = *(char *)(param_1 + iVar5);
    if (iVar3 == 0x100) goto LAB_04004bc0;
    iVar5 = iVar4 + 2;
    *(int *)(param_1 + 0x200) = iVar5;
    if (iVar5 != 0x100) {
      iVar4 = iVar4 + 3;
    }
    *(char *)(param_1 + iVar3) = cVar1 + cVar2;
    if (iVar5 == 0x100) {
      iVar4 = 0x100;
      goto LAB_04004bc0;
    }
    *(int *)(param_1 + 0x200) = iVar4;
    *(undefined1 *)(param_1 + iVar5) = 1;
  }
  else {
LAB_04004bc0:
    *(undefined4 *)(param_1 + 0x210) = DAT_04004bd0;
  }
  param_2 = iVar4 + -3;
LAB_04004b10:
  FUN_04004a9c(param_1,param_2,2);
  *(undefined1 *)(param_1 + param_2) = param_3;
  iVar5 = *(int *)(param_1 + 0x200) - param_2;
  if (iVar5 < 0x100) {
    *(char *)(param_1 + param_2 + 1) = (char)iVar5;
  }
  else {
    *(undefined4 *)(param_1 + 0x210) = DAT_04004bcc;
  }
  return;
}



//==================== FUN_040050dc @ 040050dc ====================

void FUN_040050dc(int param_1,int param_2,undefined4 param_3,undefined4 param_4,undefined4 *param_5)

{
  int iVar1;
  undefined4 uVar2;
  
  uVar2 = *param_5;
  do {
    iVar1 = FUN_04004bd4(param_1,param_2 + 2,param_3,param_4,param_5,0);
    if (iVar1 == 0) break;
    uVar2 = *param_5;
    iVar1 = FUN_04004bd4(param_1,(uint)*(byte *)(param_1 + param_2 + 1) + param_2,param_3,param_4,
                         param_5,0);
  } while (iVar1 == 0);
  *param_5 = uVar2;
  return;
}



//==================== FUN_04005170 @ 04005170 ====================

void FUN_04005170(int param_1,int param_2)

{
  int iVar1;
  
  iVar1 = *(int *)(param_1 + 0x200);
  if (iVar1 < 0x100) {
    *(int *)(param_1 + 0x200) = iVar1 + 1;
    *(undefined1 *)(param_1 + iVar1) = 0;
    iVar1 = iVar1 + 1;
  }
  else {
    *(undefined4 *)(param_1 + 0x210) = DAT_040051b0;
  }
  iVar1 = iVar1 - (param_2 + -2);
  if (iVar1 < 0x100) {
    *(char *)(param_1 + param_2) = (char)iVar1;
  }
  else {
    *(undefined4 *)(param_1 + 0x210) = DAT_040051b4;
  }
  return;
}



//==================== FUN_040051b8 @ 040051b8 ====================

void FUN_040051b8(int param_1,undefined4 *param_2)

{
  undefined1 uVar1;
  undefined4 uVar2;
  byte *pbVar3;
  uint uVar4;
  int iVar5;
  undefined4 uVar6;
  int iVar7;
  byte *pbVar8;
  int iVar9;
  byte bVar10;
  int iVar11;
  int iVar12;
  char *pcVar13;
  int iVar14;
  int iVar15;
  int iVar16;
  char cVar17;
  undefined4 uVar18;
  bool bVar19;
  ulonglong uVar20;
  
  uVar2 = DAT_040056fc;
  iVar15 = *(int *)(param_1 + 0x200);
  iVar16 = 0;
  iVar11 = *(int *)(param_1 + 0x208);
  iVar14 = iVar15;
LAB_040051e0:
  pbVar8 = (byte *)*param_2;
  pbVar3 = pbVar8 + 1;
  *param_2 = pbVar3;
  bVar10 = *pbVar8;
  if (bVar10 < 0x2c) {
    if (0x29 < bVar10) {
      if (bVar10 == 0x2a) {
        if (pbVar8[1] == 0x3f) {
          uVar18 = 0xc;
          *param_2 = pbVar8 + 2;
          goto LAB_04005328;
        }
        FUN_04004af4(param_1,iVar14,10);
      }
      else if (pbVar8[1] == 0x3f) {
        uVar18 = 0xd;
        *param_2 = pbVar8 + 2;
LAB_04005328:
        FUN_04004af4(param_1,iVar14,uVar18);
      }
      else {
        FUN_04004af4(param_1,iVar14,0xb);
      }
      goto LAB_040051e0;
    }
    if (bVar10 == 0x24) {
      iVar12 = *(int *)(param_1 + 0x200);
      if (0xff < iVar12) goto LAB_0400536c;
      *(int *)(param_1 + 0x200) = iVar12 + 1;
      *(undefined1 *)(param_1 + iVar12) = 9;
      goto LAB_040051e0;
    }
    if (bVar10 < 0x25) {
      if (bVar10 == 0) {
        *param_2 = pbVar8;
        return;
      }
    }
    else {
      if (bVar10 == 0x28) {
        iVar14 = *(int *)(param_1 + 0x200);
        iVar12 = *(int *)(param_1 + 0x208) + 1;
        *(int *)(param_1 + 0x208) = iVar12;
        uVar1 = (undefined1)iVar12;
        if (iVar14 < 0x100) {
          iVar12 = iVar14 + 1;
          *(int *)(param_1 + 0x200) = iVar12;
          *(undefined1 *)(param_1 + iVar14) = 6;
          if (iVar12 == 0x100) goto LAB_040054f0;
          *(int *)(param_1 + 0x200) = iVar14 + 2;
          *(undefined1 *)(param_1 + iVar12) = uVar1;
        }
        else {
LAB_040054f0:
          *(undefined4 *)(param_1 + 0x210) = DAT_04005708;
        }
        FUN_040051b8(param_1,param_2);
        pcVar13 = (char *)*param_2;
        *param_2 = pcVar13 + 1;
        if (*pcVar13 != ')') {
          *(undefined4 *)(param_1 + 0x210) = DAT_04005714;
          return;
        }
        iVar12 = *(int *)(param_1 + 0x200);
        if (iVar12 < 0x100) {
          iVar5 = iVar12 + 1;
          *(int *)(param_1 + 0x200) = iVar5;
          *(undefined1 *)(param_1 + iVar12) = 7;
          if (iVar5 == 0x100) goto LAB_0400536c;
          *(int *)(param_1 + 0x200) = iVar12 + 2;
          *(undefined1 *)(param_1 + iVar5) = uVar1;
          goto LAB_040051e0;
        }
        goto LAB_0400536c;
      }
      if (bVar10 == 0x29) {
        *param_2 = pbVar8;
        if (0 < iVar16) {
          FUN_04005170(param_1,iVar16);
        }
        if (iVar11 == 0) {
          *(undefined4 *)(param_1 + 0x210) = DAT_04005704;
          return;
        }
        return;
      }
    }
LAB_0400521c:
    *param_2 = pbVar8;
    iVar14 = *(int *)(param_1 + 0x200);
    uVar18 = *(undefined4 *)(param_1 + 0x204);
    while ((*pbVar8 != 0 && (iVar12 = FUN_04005b14(uVar2), iVar12 == 0))) {
      pbVar3 = (byte *)*param_2;
      iVar12 = *(int *)(param_1 + 0x204);
      *param_2 = pbVar3 + 1;
      if (iVar12 < 0x100) {
        pbVar3 = (byte *)(uint)*pbVar3;
      }
      if (iVar12 < 0x100) {
        *(int *)(param_1 + 0x204) = iVar12 + 1;
        *(char *)(param_1 + iVar12 + 0x100) = (char)pbVar3;
      }
      else {
        *(undefined4 *)(param_1 + 0x210) = DAT_04005700;
      }
      pbVar8 = (byte *)*param_2;
    }
    iVar12 = *(int *)(param_1 + 0x200);
    if (iVar12 < 0x100) {
      iVar5 = iVar12 + 1;
      *(int *)(param_1 + 0x200) = iVar5;
      *(undefined1 *)(param_1 + iVar12) = 3;
      if (iVar5 == 0x100) goto LAB_0400536c;
      iVar9 = iVar12 + 2;
      *(int *)(param_1 + 0x200) = iVar9;
      cVar17 = (char)uVar18;
      *(char *)(param_1 + iVar5) = cVar17;
      cVar17 = (char)*(undefined4 *)(param_1 + 0x204) - cVar17;
joined_r0x040054d0:
      if (iVar9 != 0x100) {
        *(int *)(param_1 + 0x200) = iVar12 + 3;
        *(char *)(param_1 + iVar9) = cVar17;
        goto LAB_040051e0;
      }
    }
  }
  else {
    if (bVar10 == 0x5b) {
      bVar10 = pbVar8[1];
      iVar14 = *(int *)(param_1 + 0x200);
      uVar18 = *(undefined4 *)(param_1 + 0x204);
      if (bVar10 == 0x5e) {
        pbVar3 = pbVar8 + 2;
        uVar6 = 5;
        *param_2 = pbVar3;
        bVar10 = pbVar8[2];
      }
      else {
        uVar6 = 4;
      }
      while (bVar10 != 0) {
        *param_2 = pbVar3 + 1;
        bVar10 = *pbVar3;
        if (bVar10 == 0x5c) {
          uVar20 = FUN_04004930(param_2);
          uVar6 = (undefined4)(uVar20 >> 0x20);
          iVar12 = *(int *)(param_1 + 0x204);
          if ((uVar20 & 0xff) == 0) {
            if (iVar12 < 0x100) {
              iVar5 = iVar12 + 1;
              bVar19 = iVar5 != 0x100;
              *(int *)(param_1 + 0x204) = iVar5;
              iVar9 = param_1 + iVar12;
              if (bVar19) {
                iVar5 = param_1 + iVar5;
                iVar12 = iVar12 + 2;
              }
              *(char *)(iVar9 + 0x100) = (char)uVar20;
              if (bVar19) {
                *(int *)(param_1 + 0x204) = iVar12;
                *(char *)(iVar5 + 0x100) = (char)(uVar20 >> 8);
                goto LAB_04005584;
              }
            }
          }
          else if (iVar12 < 0x100) {
            *(int *)(param_1 + 0x204) = iVar12 + 1;
            *(char *)(param_1 + iVar12 + 0x100) = (char)uVar20;
            goto LAB_04005584;
          }
LAB_0400557c:
          *(undefined4 *)(param_1 + 0x210) = DAT_04005700;
        }
        else {
          if (bVar10 == 0x5d) {
            iVar12 = *(int *)(param_1 + 0x200);
            if (0xff < iVar12) goto LAB_0400536c;
            iVar5 = iVar12 + 1;
            *(int *)(param_1 + 0x200) = iVar5;
            *(char *)(param_1 + iVar12) = (char)uVar6;
            if (iVar5 == 0x100) goto LAB_0400536c;
            iVar9 = iVar12 + 2;
            *(int *)(param_1 + 0x200) = iVar9;
            cVar17 = (char)uVar18;
            *(char *)(param_1 + iVar5) = cVar17;
            cVar17 = (char)*(undefined4 *)(param_1 + 0x204) - cVar17;
            goto joined_r0x040054d0;
          }
          iVar12 = *(int *)(param_1 + 0x204);
          if (0xff < iVar12) goto LAB_0400557c;
          *(int *)(param_1 + 0x204) = iVar12 + 1;
          *(byte *)(param_1 + iVar12 + 0x100) = bVar10;
        }
LAB_04005584:
        pbVar3 = (byte *)*param_2;
        bVar10 = *pbVar3;
      }
      *(undefined4 *)(param_1 + 0x210) = DAT_04005710;
      goto LAB_040051e0;
    }
    if (bVar10 < 0x5c) {
      if (bVar10 != 0x2e) {
        if (bVar10 == 0x3f) {
          uVar18 = 0xe;
          goto LAB_04005328;
        }
        goto LAB_0400521c;
      }
      iVar14 = *(int *)(param_1 + 0x200);
      if (iVar14 < 0x100) {
        *(int *)(param_1 + 0x200) = iVar14 + 1;
        *(undefined1 *)(param_1 + iVar14) = 2;
        goto LAB_040051e0;
      }
    }
    else {
      if (bVar10 != 0x5e) {
        if (bVar10 == 0x7c) {
          if (0 < iVar16) {
            FUN_04005170(param_1,iVar16);
          }
          iVar16 = iVar15 + 2;
          FUN_04004a9c(param_1,iVar15,3);
          *(undefined1 *)(param_1 + iVar15) = 1;
          iVar12 = *(int *)(param_1 + 0x200) - iVar15;
          if (iVar12 < 0x100) {
            *(char *)(param_1 + iVar15 + 1) = (char)iVar12;
          }
          else {
            *(undefined4 *)(param_1 + 0x210) = DAT_0400570c;
          }
          *(undefined1 *)(param_1 + iVar16) = 0xff;
        }
        else {
          if (bVar10 != 0x5c) goto LAB_0400521c;
          iVar14 = *(int *)(param_1 + 0x200);
          uVar4 = FUN_04004930(param_2);
          iVar12 = *(int *)(param_1 + 0x200);
          if ((uVar4 & 0xff00) == 0) {
            iVar5 = *(int *)(param_1 + 0x204);
            if (iVar12 < 0x100) {
              iVar9 = iVar12 + 1;
              *(int *)(param_1 + 0x200) = iVar9;
              *(undefined1 *)(param_1 + iVar12) = 3;
              if (iVar9 == 0x100) goto LAB_040056e4;
              iVar7 = iVar12 + 2;
              *(int *)(param_1 + 0x200) = iVar7;
              *(char *)(param_1 + iVar9) = (char)iVar5;
              if (iVar7 == 0x100) goto LAB_040056e4;
              *(int *)(param_1 + 0x200) = iVar12 + 3;
              *(undefined1 *)(param_1 + iVar7) = 1;
            }
            else {
LAB_040056e4:
              *(undefined4 *)(param_1 + 0x210) = DAT_04005708;
            }
            if (iVar5 < 0x100) {
              *(int *)(param_1 + 0x204) = iVar5 + 1;
              *(char *)(param_1 + iVar5 + 0x100) = (char)uVar4;
            }
            else {
              *(undefined4 *)(param_1 + 0x210) = DAT_04005700;
            }
          }
          else {
            if (0xff < iVar12) goto LAB_0400536c;
            *(int *)(param_1 + 0x200) = iVar12 + 1;
            *(char *)(param_1 + iVar12) = (char)(uVar4 >> 8);
          }
        }
        goto LAB_040051e0;
      }
      iVar12 = *(int *)(param_1 + 0x200);
      if (iVar12 < 0x100) {
        *(int *)(param_1 + 0x200) = iVar12 + 1;
        *(undefined1 *)(param_1 + iVar12) = 8;
        goto LAB_040051e0;
      }
    }
  }
LAB_0400536c:
  *(undefined4 *)(param_1 + 0x210) = DAT_04005708;
  goto LAB_040051e0;
}



//==================== FUN_04005718 @ 04005718 ====================

bool FUN_04005718(undefined2 *param_1,char *param_2)

{
  char cVar1;
  int iVar2;
  int iVar3;
  char *local_c;
  
  *(undefined4 *)(param_1 + 0x106) = 0;
  *(undefined4 *)(param_1 + 0x100) = 0;
  *(undefined4 *)(param_1 + 0x108) = 0;
  *(undefined4 *)(param_1 + 0x104) = 0;
  *(undefined4 *)(param_1 + 0x102) = 0;
  cVar1 = *param_2;
  *(undefined4 *)(param_1 + 0x100) = 2;
  if (cVar1 == '^') {
    *(undefined4 *)(param_1 + 0x106) = 1;
  }
  *param_1 = 6;
  local_c = param_2;
  if (*param_2 == '\0') {
    if (*(char *)(param_1 + 1) != '\x01') {
      iVar3 = 4;
      param_1[1] = 7;
      goto LAB_040057e8;
    }
LAB_0400580c:
    FUN_04005170(param_1,4);
  }
  else {
    do {
      FUN_040051b8(param_1,&local_c);
    } while (*local_c != '\0');
    if (*(char *)(param_1 + 1) == '\x01') goto LAB_0400580c;
  }
  iVar3 = *(int *)(param_1 + 0x100);
  if (iVar3 < 0x100) {
    iVar2 = iVar3 + 1;
    *(int *)(param_1 + 0x100) = iVar2;
    *(undefined1 *)((int)param_1 + iVar3) = 7;
    if (iVar2 != 0x100) {
      iVar3 = iVar3 + 2;
      *(int *)(param_1 + 0x100) = iVar3;
      *(undefined1 *)((int)param_1 + iVar2) = 0;
      if (iVar3 != 0x100) {
LAB_040057e8:
        *(int *)(param_1 + 0x100) = iVar3 + 1;
        *(undefined1 *)((int)param_1 + iVar3) = 0;
        return *(int *)(param_1 + 0x108) == 0;
      }
    }
  }
  *(undefined4 *)(param_1 + 0x108) = DAT_04005838;
  return false;
}



//==================== FUN_0400583c @ 0400583c ====================

void FUN_0400583c(int param_1,undefined4 param_2,int param_3,undefined4 param_4)

{
  int iVar1;
  int iVar2;
  int local_24 [2];
  
  local_24[0] = 0;
  if (*(int *)(param_1 + 0x20c) != 0) {
    FUN_04004bd4(param_1,0,param_2,param_3,local_24,param_4);
    return;
  }
  if (0 < param_3) {
    local_24[0] = 0;
    do {
      iVar2 = local_24[0] + 1;
      iVar1 = FUN_04004bd4(param_1,0,param_2,param_3,local_24,param_4);
      local_24[0] = iVar2;
    } while (iVar1 == 0 && iVar2 < param_3);
  }
  return;
}



//==================== FUN_04005994 @ 04005994 ====================

void FUN_04005994(int param_1,char *param_2)

{
  char cVar1;
  char *pcVar2;
  
  pcVar2 = (char *)(param_1 + -1);
  do {
    cVar1 = *param_2;
    pcVar2 = pcVar2 + 1;
    *pcVar2 = cVar1;
    param_2 = param_2 + 1;
  } while (cVar1 != '\0');
  return;
}



//==================== FUN_04005a60 @ 04005a60 ====================

int FUN_04005a60(int param_1,char *param_2)

{
  char *pcVar1;
  int iVar2;
  
  pcVar1 = (char *)(param_1 + -1);
  do {
    pcVar1 = pcVar1 + 1;
    iVar2 = (int)(char)(*pcVar1 - *param_2);
    if (iVar2 != 0) {
      return iVar2;
    }
    param_2 = param_2 + 1;
  } while (*pcVar1 != '\0');
  return 0;
}



//==================== FUN_04005a90 @ 04005a90 ====================

int FUN_04005a90(char *param_1,char *param_2,int param_3)

{
  char *pcVar1;
  int iVar2;
  
  if (param_3 == 0) {
    return 0;
  }
  iVar2 = (int)(char)(*param_1 - *param_2);
  if (iVar2 != 0) {
    return iVar2;
  }
  if (*param_1 != '\0') {
    pcVar1 = param_2 + param_3;
    while (param_2 = param_2 + 1, pcVar1 != param_2) {
      param_1 = param_1 + 1;
      iVar2 = (int)(char)(*param_1 - *param_2);
      if (iVar2 != 0) {
        return iVar2;
      }
      if (*param_1 == '\0') {
        return 0;
      }
    }
  }
  return 0;
}



//==================== FUN_04005b14 @ 04005b14 ====================

char * FUN_04005b14(char *param_1,char param_2)

{
  char cVar1;
  
  cVar1 = *param_1;
  if (cVar1 == param_2) {
    return param_1;
  }
  do {
    if (cVar1 == '\0') {
      return (char *)0x0;
    }
    param_1 = param_1 + 1;
    cVar1 = *param_1;
  } while (cVar1 != param_2);
  return param_1;
}



//==================== FUN_04005b44 @ 04005b44 ====================

char * FUN_04005b44(char *param_1,char param_2)

{
  char cVar1;
  char *pcVar2;
  
  cVar1 = *param_1;
  pcVar2 = param_1;
  while (cVar1 != '\0') {
    pcVar2 = pcVar2 + 1;
    cVar1 = *pcVar2;
  }
  cVar1 = '\0';
  while( true ) {
    if (cVar1 == param_2) {
      return pcVar2;
    }
    pcVar2 = pcVar2 + -1;
    if (pcVar2 < param_1) break;
    cVar1 = *pcVar2;
  }
  return (char *)0x0;
}



//==================== FUN_04005b98 @ 04005b98 ====================

int FUN_04005b98(char *param_1)

{
  char *pcVar1;
  
  pcVar1 = param_1;
  if (*param_1 == '\0') {
    return 0;
  }
  do {
    pcVar1 = pcVar1 + 1;
  } while (*pcVar1 != '\0');
  return (int)pcVar1 - (int)param_1;
}



//==================== FUN_04005bc4 @ 04005bc4 ====================

int FUN_04005bc4(char *param_1,int param_2)

{
  char *pcVar1;
  
  if (param_2 == 0) {
    return 0;
  }
  if (*param_1 == '\0') {
    return 0;
  }
  pcVar1 = param_1;
  do {
    pcVar1 = pcVar1 + 1;
    if (param_1 + param_2 == pcVar1) {
      return (int)(param_1 + param_2) - (int)param_1;
    }
  } while (*pcVar1 != '\0');
  return (int)pcVar1 - (int)param_1;
}



//==================== FUN_04005c20 @ 04005c20 ====================

int FUN_04005c20(char *param_1)

{
  char cVar1;
  char *pcVar2;
  int iVar3;
  
  if (param_1 != (char *)0x0) {
    if (*param_1 == '\0') {
      pcVar2 = (char *)0x1;
    }
    else {
      pcVar2 = param_1;
      do {
        pcVar2 = pcVar2 + 1;
      } while (*pcVar2 != '\0');
      pcVar2 = pcVar2 + (1 - (int)param_1);
    }
    iVar3 = func_0x03fc87b0(pcVar2);
    if (iVar3 != 0) {
      pcVar2 = (char *)(iVar3 + -1);
      do {
        cVar1 = *param_1;
        pcVar2 = pcVar2 + 1;
        *pcVar2 = cVar1;
        param_1 = param_1 + 1;
      } while (cVar1 != '\0');
      return iVar3;
    }
  }
  return 0;
}



//==================== FUN_04005e0c @ 04005e0c ====================

void FUN_04005e0c(undefined4 *param_1,undefined1 param_2,uint param_3)

{
  uint uVar1;
  undefined4 *puVar3;
  undefined4 *puVar2;
  
  if (((uint)param_1 & 3) != 0) {
    if (param_3 == 0) {
      return;
    }
    puVar3 = param_1;
    do {
      puVar2 = (undefined4 *)((int)puVar3 + 1);
      *(undefined1 *)puVar3 = param_2;
      puVar3 = puVar2;
    } while ((undefined4 *)((int)param_1 + param_3) != puVar2);
    return;
  }
  if (3 < param_3) {
    uVar1 = param_3 - 4 >> 2;
    puVar3 = param_1;
    do {
      puVar2 = puVar3 + 1;
      *puVar3 = CONCAT31(CONCAT21(CONCAT11(param_2,param_2),param_2),param_2);
      puVar3 = puVar2;
    } while (param_1 + uVar1 + 1 != puVar2);
    param_3 = param_3 & 3;
    param_1 = param_1 + uVar1 + 1;
  }
  if (param_3 == 0) {
    return;
  }
  puVar3 = param_1;
  do {
    puVar2 = (undefined4 *)((int)puVar3 + 1);
    *(undefined1 *)puVar3 = param_2;
    puVar3 = puVar2;
  } while ((undefined4 *)((int)param_1 + param_3) != puVar2);
  return;
}



//==================== FUN_04005ea4 @ 04005ea4 ====================

void FUN_04005ea4(undefined4 *param_1,undefined4 *param_2,uint param_3)

{
  uint uVar1;
  undefined4 *puVar3;
  undefined4 *puVar2;
  int iVar4;
  undefined1 *puVar5;
  undefined4 *puVar6;
  
  if (param_1 == param_2) {
    return;
  }
  if (((((uint)param_2 | (uint)param_1) & 3) != 0) || (param_3 < 4)) {
    if (param_3 == 0) {
      return;
    }
    puVar5 = (undefined1 *)((int)param_1 + -1);
    puVar3 = param_2;
    do {
      puVar2 = (undefined4 *)((int)puVar3 + 1);
      puVar5 = puVar5 + 1;
      *puVar5 = *(undefined1 *)puVar3;
      puVar3 = puVar2;
    } while (puVar2 != (undefined4 *)((int)param_2 + param_3));
    return;
  }
  uVar1 = param_3 - 4 >> 2;
  puVar3 = param_1;
  puVar2 = param_2;
  do {
    puVar6 = puVar3 + 1;
    *puVar3 = *puVar2;
    puVar3 = puVar6;
    puVar2 = puVar2 + 1;
  } while (puVar6 != param_1 + uVar1 + 1);
  iVar4 = uVar1 + 1;
  if ((param_3 & 3) == 0) {
    return;
  }
  puVar5 = (undefined1 *)((int)param_1 + iVar4 * 4 + -1);
  puVar3 = param_2 + iVar4;
  do {
    puVar2 = (undefined4 *)((int)puVar3 + 1);
    puVar5 = puVar5 + 1;
    *puVar5 = *(undefined1 *)puVar3;
    puVar3 = puVar2;
  } while (puVar2 != (undefined4 *)((int)(param_2 + iVar4) + (param_3 & 3)));
  return;
}



//==================== FUN_04005fa0 @ 04005fa0 ====================

void FUN_04005fa0(undefined1 *param_1,undefined1 *param_2,int param_3)

{
  undefined1 *puVar1;
  undefined1 *puVar2;
  
  if (param_2 == param_1) {
    return;
  }
  if (param_2 < param_1) {
    param_1 = param_1 + param_3;
    puVar2 = param_2 + param_3;
    if (param_3 == 0) {
      return;
    }
    do {
      puVar2 = puVar2 + -1;
      param_1 = param_1 + -1;
      *param_1 = *puVar2;
    } while (param_2 != puVar2);
    return;
  }
  if (param_3 == 0) {
    return;
  }
  param_1 = param_1 + -1;
  puVar2 = param_2;
  do {
    puVar1 = puVar2 + 1;
    param_1 = param_1 + 1;
    *param_1 = *puVar2;
    puVar2 = puVar1;
  } while (param_2 + param_3 != puVar1);
  return;
}



//==================== FUN_04005ff4 @ 04005ff4 ====================

int FUN_04005ff4(byte *param_1,byte *param_2,int param_3)

{
  int iVar1;
  code *UNRECOVERED_JUMPTABLE;
  byte *pbVar2;
  
  if (param_3 == 0) {
    return 0;
  }
  if ((uint)*param_1 - (uint)*param_2 != 0) {
    return (uint)*param_1 - (uint)*param_2;
  }
  pbVar2 = param_2 + param_3 + -1;
  do {
    if (param_2 == pbVar2) {
      return 0;
    }
    param_1 = param_1 + 1;
    param_2 = param_2 + 1;
  } while ((uint)*param_1 - (uint)*param_2 == 0);
                    /* WARNING: Could not recover jumptable at 0x04006038. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  iVar1 = (*UNRECOVERED_JUMPTABLE)((uint)*param_1 - (uint)*param_2);
  return iVar1;
}



//==================== FUN_04006044 @ 04006044 ====================

char * FUN_04006044(char *param_1,char *param_2)

{
  char cVar1;
  char *pcVar2;
  int iVar3;
  int iVar4;
  char cVar5;
  char *pcVar6;
  
  if (*param_2 == '\0') {
    return param_1;
  }
  pcVar2 = param_2;
  do {
    pcVar2 = pcVar2 + 1;
  } while (*pcVar2 != '\0');
  iVar4 = (int)pcVar2 - (int)param_2;
  if (iVar4 == 0) {
    return param_1;
  }
  cVar1 = *param_1;
  if (cVar1 == '\0') {
    iVar3 = 0;
  }
  else {
    pcVar2 = param_1;
    do {
      pcVar2 = pcVar2 + 1;
    } while (*pcVar2 != '\0');
    iVar3 = (int)pcVar2 - (int)param_1;
  }
  if (iVar4 <= iVar3) {
    while( true ) {
      iVar3 = iVar3 + -1;
      pcVar2 = param_1;
      pcVar6 = param_2;
      cVar5 = *param_2;
      while (cVar1 == cVar5) {
        if (param_1 + iVar4 + -1 == pcVar2) {
          return param_1;
        }
        pcVar2 = pcVar2 + 1;
        pcVar6 = pcVar6 + 1;
        cVar5 = *pcVar6;
        cVar1 = *pcVar2;
      }
      if (iVar3 < iVar4) break;
      param_1 = param_1 + 1;
      cVar1 = *param_1;
    }
  }
  return (char *)0x0;
}



//==================== FUN_04006118 @ 04006118 ====================

ulonglong FUN_04006118(uint param_1,int param_2)

{
  uint uVar1;
  uint uVar2;
  undefined4 uVar3;
  uint uVar4;
  bool bVar5;
  bool bVar6;
  uint local_18;
  uint local_14;
  
  local_18 = param_1;
  local_14 = param_2;
  uVar3 = func_0x03fb2008();
  uVar4 = local_18 >> 0x1b;
  bVar5 = local_18 * 0x20 < local_18;
  uVar1 = local_18 * 0x1f;
  bVar6 = CARRY4(local_18 * 0x7c,local_18);
  uVar2 = local_18 * 0x7d;
  local_18 = local_18 * 1000;
  local_14 = ((((local_14 << 5 | uVar4) - (local_14 + bVar5)) * 4 | uVar1 >> 0x1e) + local_14 +
             (uint)bVar6) * 8 | uVar2 >> 0x1d;
  if (local_14 == 0) {
    uVar4 = FUN_0400c66c(local_18,uVar3);
    return (ulonglong)uVar4;
  }
  FUN_04004754(&local_18,uVar3);
  return CONCAT44(local_14,local_18);
}



//==================== FUN_040061b0 @ 040061b0 ====================

undefined8 FUN_040061b0(void)

{
  uint uVar1;
  int unaff_r9;
  
  uVar1 = ~uRamf8f00604;
  if (uVar1 < *(uint *)(unaff_r9 + 0x8c)) {
    *(int *)(unaff_r9 + 0x88) = *(int *)(unaff_r9 + 0x88) + 1;
  }
  *(uint *)(unaff_r9 + 0x8c) = uVar1;
  return CONCAT44(*(undefined4 *)(unaff_r9 + 0x88),*(undefined4 *)(unaff_r9 + 0x8c));
}



//==================== FUN_040061e4 @ 040061e4 ====================

int FUN_040061e4(int param_1)

{
  int iVar1;
  
  FUN_040061b0();
  iVar1 = FUN_04006118();
  return iVar1 - param_1;
}



//==================== FUN_040061fc @ 040061fc ====================

void FUN_040061fc(void)

{
  uint uVar1;
  int iVar2;
  undefined8 uVar3;
  
  uVar3 = FUN_040061b0();
  iVar2 = (int)((ulonglong)uVar3 >> 0x20);
  uVar1 = (uint)uVar3;
  FUN_04006118(uVar1 * 1000,
               ((((iVar2 << 5 | uVar1 >> 0x1b) - (iVar2 + (uint)(uVar1 * 0x20 < uVar1))) * 4 |
                uVar1 * 0x1f >> 0x1e) + iVar2 + (uint)CARRY4(uVar1 * 0x7c,uVar1)) * 8 |
               uVar1 * 0x7d >> 0x1d);
  return;
}



//==================== FUN_04006244 @ 04006244 ====================

void FUN_04006244(uint param_1)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  bool bVar6;
  undefined8 uVar7;
  uint local_30;
  uint local_28;
  int local_24;
  
  uVar7 = FUN_040061b0();
  local_24 = 0;
  local_28 = param_1;
  uVar1 = func_0x03fb2008();
  local_24 = uVar1 * local_24 + (int)((ulonglong)uVar1 * (ulonglong)local_28 >> 0x20);
  local_30 = (uint)((ulonglong)uVar1 * (ulonglong)local_28);
  local_28 = local_30;
  if (local_24 == 0) {
    iVar2 = 0;
    uVar1 = local_30 / 1000000;
  }
  else {
    FUN_04004754(&local_28,1000000);
    iVar2 = local_24;
    uVar1 = local_28;
  }
  uVar5 = (uint)uVar7 + 1;
  uVar4 = (int)((ulonglong)uVar7 >> 0x20) + (uint)(0xfffffffe < (uint)uVar7) + iVar2 +
          (uint)CARRY4(uVar5,uVar1);
  do {
    uVar7 = FUN_040061b0();
    uVar3 = (uint)((ulonglong)uVar7 >> 0x20);
    bVar6 = uVar4 <= uVar3;
    if (uVar3 == uVar4) {
      bVar6 = uVar5 + uVar1 <= (uint)uVar7;
    }
  } while (!bVar6);
  return;
}



//==================== FUN_040062f8 @ 040062f8 ====================

void FUN_040062f8(uint param_1)

{
  uint uVar1;
  
  do {
    uVar1 = param_1;
    if (9999999 < param_1) {
      uVar1 = 10000000;
    }
    FUN_04006244(uVar1);
    param_1 = param_1 - uVar1;
  } while (param_1 != 0);
  return;
}



//==================== FUN_0400636c @ 0400636c ====================

char * FUN_0400636c(char *param_1,uint param_2)

{
  uint uVar1;
  char cVar2;
  int iVar3;
  char *pcVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  char *pcVar8;
  bool bVar9;
  
  uVar7 = (param_2 & 0xff) >> 4;
  uVar6 = (param_2 & 0xfff) >> 8;
  uVar1 = param_2 >> 0xc;
  iVar3 = (param_2 & 0xf) + (uVar7 + uVar6 + uVar1) * 6;
  uVar5 = (uint)(iVar3 * 0xcd) >> 0xb;
  pcVar4 = (char *)(uVar5 + uVar1 * 9 + uVar7 + uVar6 * 5);
  bVar9 = pcVar4 == (char *)0x0;
  if (bVar9) {
    pcVar4 = param_1 + 1;
  }
  *param_1 = (char)iVar3 + (char)uVar5 * -10 + '0';
  if (!bVar9) {
    uVar5 = (uint)((int)pcVar4 * 0xcd) >> 0xb;
    iVar3 = uVar5 + uVar6 * 2;
    param_1[1] = (char)pcVar4 + (char)uVar5 * -10 + '0';
    if (uVar1 == 0 && iVar3 == 0) {
      pcVar4 = param_1 + 2;
    }
    else {
      pcVar4 = (char *)((uint)(iVar3 * 0xd) >> 7);
      pcVar8 = pcVar4 + uVar1 * 4;
      cVar2 = (char)pcVar4;
      if (pcVar8 == (char *)0x0) {
        pcVar4 = param_1 + 3;
      }
      param_1[2] = (char)iVar3 + cVar2 * -10 + '0';
      if (pcVar8 != (char *)0x0) {
        uVar1 = (uint)((int)pcVar8 * 0xcd) >> 0xb;
        uVar5 = uVar1;
        if (uVar1 != 0) {
          uVar5 = uVar1 + 0x30;
        }
        if (uVar1 != 0) {
          pcVar4 = param_1 + 5;
          param_1[4] = (char)uVar5;
        }
        else {
          pcVar4 = param_1 + 4;
        }
        param_1[3] = (char)pcVar8 + (char)uVar1 * -10 + '0';
      }
    }
  }
  return pcVar4;
}



//==================== FUN_0400644c @ 0400644c ====================

undefined1 *
FUN_0400644c(undefined1 *param_1,undefined1 *param_2,undefined1 *param_3,int param_4,
            undefined4 param_5,uint param_6)

{
  undefined1 uVar1;
  int iVar2;
  int iVar3;
  undefined1 *puVar4;
  undefined1 *puVar5;
  undefined1 *puVar6;
  
  puVar5 = DAT_0400651c;
  if (param_3 != (undefined1 *)0x0) {
    puVar5 = param_3;
  }
  iVar2 = FUN_04005bc4(puVar5,param_5);
  iVar3 = param_4 + -1;
  if ((param_6 & 0x10) == 0) {
    if (iVar2 < param_4) {
      puVar6 = param_1 + (param_4 - iVar2);
      puVar4 = param_1;
      do {
        param_1 = puVar4 + 1;
        if (puVar4 < param_2) {
          *puVar4 = 0x20;
        }
        puVar4 = param_1;
      } while (param_1 != puVar6);
      param_4 = iVar2 + -1;
      iVar3 = iVar2 + -2;
    }
    else {
      iVar3 = param_4 + -2;
      param_4 = param_4 + -1;
    }
  }
  if (0 < iVar2) {
    puVar6 = param_1 + iVar2;
    puVar4 = param_1;
    do {
      param_1 = puVar4 + 1;
      if (puVar4 < param_2) {
        uVar1 = *puVar5;
        puVar5 = puVar5 + 1;
        *puVar4 = uVar1;
      }
      puVar4 = param_1;
    } while (puVar6 != param_1);
  }
  puVar5 = param_1;
  if (iVar2 < param_4) {
    puVar4 = param_1;
    do {
      puVar5 = puVar4 + 1;
      if (puVar4 < param_2) {
        *puVar4 = 0x20;
      }
      puVar4 = puVar5;
    } while (iVar2 < (int)(param_1 + ((iVar3 + 1) - (int)puVar5)));
  }
  return puVar5;
}



//==================== FUN_04007514 @ 04007514 ====================

void FUN_04007514(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 uStack_8;
  undefined4 uStack_4;
  
  uStack_8 = param_3;
  uStack_4 = param_4;
  FUN_040069f0(param_1,0x7fffffff,param_2,&uStack_8);
  return;
}



//==================== FUN_04007548 @ 04007548 ====================

uint FUN_04007548(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  uint uVar1;
  undefined1 auStack_830 [2072];
  undefined4 uStack_c;
  undefined4 uStack_8;
  undefined4 uStack_4;
  
  uStack_c = param_2;
  uStack_8 = param_3;
  uStack_4 = param_4;
  uVar1 = FUN_040069f0(auStack_830,0x817,param_1,&uStack_c);
  if (0x815 < uVar1) {
    uVar1 = 0x816;
  }
  func_0x03fc7fbc(auStack_830);
  return uVar1;
}



//==================== FUN_040075a0 @ 040075a0 ====================

uint FUN_040075a0(undefined4 param_1,undefined4 param_2)

{
  uint uVar1;
  undefined1 auStack_820 [2072];
  
  uVar1 = FUN_040069f0(auStack_820,0x817,param_1,param_2);
  if (0x815 < uVar1) {
    uVar1 = 0x816;
  }
  func_0x03fc7fbc(auStack_820);
  return uVar1;
}



//==================== FUN_04007678 @ 04007678 ====================

bool FUN_04007678(char *param_1,undefined4 *param_2)

{
  bool bVar1;
  undefined4 uVar2;
  char *local_14 [2];
  
  uVar2 = FUN_04007718(param_1,local_14,0x10);
  *param_2 = uVar2;
  bVar1 = false;
  if (*param_1 != '\0') {
    bVar1 = *local_14[0] == '\0';
  }
  return bVar1;
}



//==================== FUN_040076b8 @ 040076b8 ====================

void FUN_040076b8(void)

{
  func_0x03fc7f78(10);
  FUN_040062f8(100000);
  func_0x03fb1d70(0,0,0);
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}



//==================== FUN_04007718 @ 04007718 ====================

uint FUN_04007718(byte *param_1,undefined4 *param_2,uint param_3)

{
  byte bVar1;
  uint uVar2;
  byte *pbVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  
  uVar2 = (uint)*param_1;
  if (uVar2 == 0x30) {
    uVar5 = (uint)param_1[1];
    pbVar3 = param_1 + 1;
    if (uVar5 != 0x78) {
LAB_040077b4:
      bVar1 = *(byte *)(DAT_04007800 + uVar5);
      uVar2 = uVar5;
      if (param_3 == 0) {
        param_3 = 8;
      }
      goto LAB_04007744;
    }
    uVar2 = (uint)param_1[2];
    uVar4 = (uint)*(byte *)(DAT_04007800 + uVar2);
    if ((*(byte *)(DAT_04007800 + uVar2) & 0x44) == 0) goto LAB_040077b4;
    pbVar3 = param_1 + 2;
    param_3 = 0x10;
  }
  else {
    if (param_3 == 0) {
      param_3 = 10;
    }
    bVar1 = *(byte *)(DAT_04007800 + uVar2);
    pbVar3 = param_1;
LAB_04007744:
    uVar4 = (uint)bVar1;
    uVar5 = uVar4 & 0x44;
    if ((bVar1 & 0x44) == 0) goto LAB_04007794;
  }
  uVar5 = 0;
  do {
    uVar6 = uVar2 - 0x30;
    if ((uVar4 & 4) == 0) {
      uVar6 = uVar2 - 0x37;
      if ((uVar4 & 2) != 0) {
        uVar6 = (uVar2 - 0x20 & 0xff) - 0x37;
      }
    }
    if (param_3 <= uVar6) break;
    pbVar3 = pbVar3 + 1;
    uVar2 = (uint)*pbVar3;
    uVar5 = param_3 * uVar5 + uVar6;
    uVar4 = (uint)*(byte *)(DAT_04007800 + uVar2);
  } while ((*(byte *)(DAT_04007800 + uVar2) & 0x44) != 0);
LAB_04007794:
  if (param_2 != (undefined4 *)0x0) {
    *param_2 = pbVar3;
  }
  return uVar5;
}



//==================== FUN_040078a8 @ 040078a8 ====================

/* WARNING: Control flow encountered bad instruction data */

void FUN_040078a8(void)

{
  undefined4 *extraout_r1;
  
  FUN_04007718();
  switch(*(undefined1 *)*extraout_r1) {
  case 0x47:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x48:
  case 0x49:
  case 0x4a:
  case 0x4c:
  case 0x4e:
  case 0x4f:
  case 0x50:
  case 0x51:
  case 0x52:
  case 0x53:
  case 0x54:
  case 0x55:
  case 0x56:
  case 0x57:
  case 0x58:
  case 0x59:
  case 0x5a:
  case 0x5b:
  case 0x5c:
  case 0x5d:
  case 0x5e:
  case 0x5f:
  case 0x60:
  case 0x61:
  case 0x62:
  case 99:
  case 100:
  case 0x65:
  case 0x66:
  case 0x67:
  case 0x68:
  case 0x69:
  case 0x6a:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x4b:
  case 0x6b:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x4d:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  default:
    return;
  }
}



//==================== FUN_0400799c @ 0400799c ====================

undefined8 FUN_0400799c(byte *param_1,undefined4 *param_2,uint param_3)

{
  byte bVar1;
  ulonglong uVar2;
  byte *pbVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  
  uVar6 = (uint)*param_1;
  if (uVar6 == 0x30) {
    uVar5 = (uint)param_1[1];
    pbVar3 = param_1 + 1;
    if (uVar5 != 0x78) {
LAB_04007a70:
      bVar1 = *(byte *)(DAT_04007ac0 + uVar5);
      uVar6 = uVar5;
      if (param_3 == 0) {
        param_3 = 8;
      }
      goto LAB_040079cc;
    }
    uVar6 = (uint)param_1[2];
    bVar1 = *(byte *)(DAT_04007ac0 + uVar6);
    if ((bVar1 & 0x44) == 0) goto LAB_04007a70;
    pbVar3 = param_1 + 2;
    param_3 = 0x10;
  }
  else {
    if (param_3 == 0) {
      param_3 = 10;
    }
    bVar1 = *(byte *)(DAT_04007ac0 + uVar6);
    pbVar3 = param_1;
LAB_040079cc:
    if ((bVar1 & 0x44) == 0) {
      uVar5 = 0;
      iVar4 = 0;
      goto LAB_04007a44;
    }
  }
  uVar5 = 0;
  iVar4 = 0;
  do {
    uVar7 = uVar6 - 0x30;
    if ((bVar1 & 4) == 0) {
      uVar7 = uVar6 - 0x37;
      if ((bVar1 & 2) != 0) {
        uVar7 = (uVar6 - 0x20 & 0xff) - 0x37;
      }
    }
    uVar2 = (ulonglong)uVar5;
    uVar8 = (uint)(param_3 * uVar2);
    if ((int)uVar7 < 0 || param_3 <= uVar7) break;
    pbVar3 = pbVar3 + 1;
    uVar6 = (uint)*pbVar3;
    uVar5 = uVar8 + uVar7;
    bVar1 = *(byte *)(DAT_04007ac0 + uVar6);
    iVar4 = param_3 * iVar4 + (int)(param_3 * uVar2 >> 0x20) + ((int)uVar7 >> 0x1f) +
            (uint)CARRY4(uVar8,uVar7);
  } while ((bVar1 & 0x44) != 0);
LAB_04007a44:
  if (param_2 != (undefined4 *)0x0) {
    *param_2 = pbVar3;
  }
  return CONCAT44(iVar4,uVar5);
}



//==================== FUN_04007c54 @ 04007c54 ====================

undefined4 FUN_04007c54(byte *param_1)

{
  byte *pbVar1;
  byte *pbVar2;
  undefined4 uVar3;
  int iVar4;
  
  iVar4 = FUN_04005b98();
  pbVar2 = param_1 + iVar4 + -1;
  if (pbVar2 <= param_1) {
    return 0xffffffff;
  }
  if ((*(byte *)(DAT_04007c50 + (uint)param_1[iVar4 + -1]) & 4) != 0) {
    pbVar1 = param_1 + iVar4 + -2;
    do {
      pbVar2 = pbVar1;
      if (pbVar2 == param_1) {
        return 0xffffffff;
      }
      pbVar1 = pbVar2 + -1;
    } while ((*(byte *)(DAT_04007c50 + (uint)*pbVar2) & 4) != 0);
  }
  uVar3 = FUN_04007718(pbVar2 + 1,0,10);
  return uVar3;
}



//==================== FUN_04007c5c @ 04007c5c ====================

uint FUN_04007c5c(ushort *param_1,uint param_2)

{
  int iVar1;
  ushort *puVar2;
  uint uVar3;
  uint local_c;
  
  uVar3 = 0;
  for (puVar2 = param_1; 1 < (int)param_1 + (param_2 - (int)puVar2); puVar2 = puVar2 + 1) {
    uVar3 = uVar3 + *puVar2;
  }
  if ((param_2 >> 1) * -2 + param_2 == 1) {
    local_c = (uint)(byte)param_1[param_2 >> 1];
    uVar3 = uVar3 + local_c;
  }
  iVar1 = (uVar3 & 0xffff) + ((int)uVar3 >> 0x10);
  return ~(iVar1 + (iVar1 >> 0x10)) & 0xffff;
}



//==================== FUN_04007cc8 @ 04007cc8 ====================

bool FUN_04007cc8(void)

{
  uint uVar1;
  
  uVar1 = FUN_04007c5c();
  return (uVar1 & 0xfffe) == 0;
}



//==================== FUN_04007de8 @ 04007de8 ====================

void FUN_04007de8(void)

{
  int *piVar1;
  uint *puVar2;
  uint *puVar3;
  uint *puVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  int iVar9;
  int iVar10;
  uint local_28;
  uint local_24 [4];
  
  puVar4 = DAT_04007e5c;
  puVar3 = DAT_04007e54;
  puVar2 = DAT_04007e50;
  if (((*DAT_04007e50 ^ *DAT_04007e54) & *DAT_04007e58) == 0) {
    *DAT_04007e5c = *DAT_04007e54;
  }
  else {
    uVar8 = *DAT_04007e60;
    if (uVar8 == 0) {
      func_0x03fc7fbc(DAT_04007e64);
      uVar8 = *puVar3;
    }
    *puVar4 = uVar8;
  }
  uVar8 = DAT_04007e68;
  piVar1 = DAT_04007ddc;
  uVar7 = *puVar4;
  uVar6 = *puVar2;
  local_24[0] = DAT_04007e68;
  iVar10 = *DAT_04007ddc;
  local_28 = uVar6;
  iVar5 = FUN_0400a41c(iVar10,DAT_04007de0,0x806);
  iVar9 = iVar10 + iVar5;
  *(undefined2 *)(iVar10 + iVar5) = 0x100;
  *(undefined2 *)(iVar9 + 2) = 8;
  *(undefined2 *)(iVar9 + 6) = 0x100;
  *(undefined2 *)(iVar9 + 4) = 0x406;
  FUN_04005ea4(iVar9 + 8,DAT_04007de4,6);
  local_24[0] = uVar6;
  FUN_04005ea4(iVar9 + 0xe,local_24,4);
  FUN_04005ea4(iVar9 + 0x12,uVar8,6);
  local_28 = uVar7;
  FUN_04005ea4(iVar9 + 0x18,&local_28,4);
  FUN_0400929c(*piVar1,iVar5 + 0x1c);
  return;
}



//==================== FUN_04007efc @ 04007efc ====================

void FUN_04007efc(int param_1,ushort *param_2,int param_3)

{
  ushort uVar1;
  int *piVar2;
  int *piVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  code *pcVar6;
  int iVar7;
  int local_24;
  
  piVar2 = DAT_040080f4;
  if (param_3 < 0x1c) {
    FUN_04007548(DAT_040080f0,param_3,0x1c);
  }
  else if (((((ushort)(*param_2 >> 8 | *param_2 << 8) == 1) &&
            ((ushort)(param_2[1] >> 8 | param_2[1] << 8) == 0x800)) && ((char)param_2[2] == '\x06'))
          && (*(char *)((int)param_2 + 5) == '\x04')) {
    if (*DAT_040080f4 != 0) {
      FUN_04005ea4(&local_24,param_2 + 0xc,4);
      piVar3 = DAT_040080f8;
      if (*piVar2 == local_24) {
        uVar1 = param_2[3] >> 8 | param_2[3] << 8;
        if (uVar1 == 1) {
          iVar7 = FUN_0400a4b8(param_1,param_1 + 6,0x806);
          param_2[3] = 0x200;
          FUN_04005ea4(param_2 + 9,param_2 + 4,6);
          FUN_04005ea4(param_2 + 0xc,param_2 + 7,4);
          FUN_04005ea4(param_2 + 4,DAT_0400810c,6);
          FUN_04005ea4(param_2 + 7,piVar2,4);
          FUN_0400929c(param_1,iVar7 + 0x1c);
        }
        else if (uVar1 == 2) {
          if (*DAT_040080f8 != 0) {
            FUN_04005ea4(&local_24,param_2 + 7,4);
            piVar2 = DAT_04008100;
            if (local_24 == *DAT_040080fc) {
              if (*DAT_04008100 != 0) {
                FUN_04005ea4(*DAT_04008100,param_2 + 4,6);
              }
              pcVar6 = (code *)FUN_0400a02c();
              puVar5 = DAT_04008108;
              puVar4 = DAT_04008104;
              (*pcVar6)(param_2,0,local_24,0,param_3);
              FUN_04005ea4(*puVar4,param_2 + 4,6);
              FUN_0400929c(*puVar4,*puVar5);
              *piVar3 = 0;
              *puVar5 = 0;
              *piVar2 = 0;
            }
          }
        }
      }
    }
  }
  return;
}



//==================== FUN_04008c70 @ 04008c70 ====================

undefined4
FUN_04008c70(undefined4 param_1,undefined4 *param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 *local_c;
  undefined4 uStack_8;
  
  local_c = param_2;
  uStack_8 = param_3;
  func_0x03fd0cb8(0x13,&local_c,param_3,param_4,param_1);
  return *local_c;
}



//==================== FUN_04008f30 @ 04008f30 ====================

/* WARNING: Control flow encountered bad instruction data */

void FUN_04008f30(void)

{
  int *piVar1;
  
  piVar1 = (int *)FUN_04008c70();
  if (*piVar1 != 0) {
    func_0x03fd10d0();
  }
  if (*piVar1 == 0) {
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  return;
}



//==================== FUN_04008f68 @ 04008f68 ====================

int FUN_04008f68(void)

{
  int *piVar1;
  undefined4 uVar2;
  
  piVar1 = (int *)FUN_04008c70();
  if (*piVar1 == 0) {
    uVar2 = func_0x03fd1098(0x13,piVar1);
    *DAT_04008f9c = uVar2;
  }
  return *piVar1;
}



//==================== FUN_04009008 @ 04009008 ====================

int FUN_04009008(int param_1,int param_2,undefined4 param_3)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  int *piVar5;
  int iVar6;
  uint local_20;
  int local_1c;
  undefined4 uStack_18;
  
  local_20 = 0;
  local_1c = param_2;
  uStack_18 = param_3;
  uVar1 = FUN_04005b98(DAT_040090f4);
  iVar2 = FUN_04005a90(param_1,DAT_040090f4,uVar1);
  if ((iVar2 == 0) && (uVar3 = FUN_04005b98(param_1), uVar1 < uVar3)) {
    uVar1 = param_1 + uVar1;
    iVar2 = FUN_04007718(uVar1,&local_20,10);
  }
  else {
    uVar1 = 0;
    iVar2 = -1;
  }
  func_0x03fd0cb8(0x13,&local_1c);
  piVar5 = *(int **)(local_1c + 8);
  do {
    iVar6 = (int)(piVar5 + -0xc);
    if (piVar5 == (int *)(local_1c + 8)) {
      return 0;
    }
    iVar4 = func_0x03fd031c(iVar6);
    if (iVar4 == 0) {
      iVar4 = FUN_04005a60(piVar5[-0xb],param_1);
      if (iVar4 == 0) {
        return iVar6;
      }
      if ((uVar1 < local_20) && (piVar5[8] == iVar2)) {
        return iVar6;
      }
    }
    piVar5 = (int *)*piVar5;
  } while( true );
}



//==================== FUN_04009118 @ 04009118 ====================

void FUN_04009118(void)

{
  int iVar1;
  
  iVar1 = FUN_04008f68();
  if (iVar1 == 0) {
    return;
  }
  if ((*(uint *)(iVar1 + 0x48) & 1) != 0) {
    **(undefined4 **)(iVar1 + 0x28) = 1;
  }
  return;
}



//==================== FUN_04009140 @ 04009140 ====================

undefined4 FUN_04009140(void)

{
  int iVar1;
  
  iVar1 = FUN_04008f68();
  if (iVar1 == 0) {
    return 0xffffffff;
  }
  iVar1 = FUN_04008f68();
  return *(undefined4 *)(iVar1 + 0x50);
}



//==================== FUN_04009164 @ 04009164 ====================

int FUN_04009164(void)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  int *piVar5;
  
  iVar2 = func_0x03fbefb0(DAT_0400924c);
  iVar3 = func_0x03fbefb0(DAT_04009250);
  if ((iVar3 == 0) || (iVar3 = FUN_04005a60(iVar3,DAT_04009254), iVar2 == 0 || iVar3 != 0)) {
    piVar4 = (int *)FUN_04008f68();
    if (piVar4 == (int *)0x0) {
      FUN_04007548(DAT_04009258);
      return -0x13;
    }
  }
  else {
    piVar4 = (int *)FUN_04009008(iVar2);
    if (piVar4 == (int *)0x0) {
      return -0x16;
    }
  }
  piVar1 = DAT_0400925c;
  iVar2 = -0x13;
  piVar5 = piVar4;
  do {
    if (piVar5 != (int *)0x0) {
      if ((piVar5[0x12] & 1U) == 0) {
        iVar2 = *piVar1;
      }
      else {
        iVar2 = (*(code *)**(undefined4 **)(*piVar5 + 0x3c))(piVar5);
        if (-1 < iVar2) {
          *(undefined4 *)piVar5[10] = 2;
          return 0;
        }
      }
    }
    FUN_040097e4(0);
    piVar5 = (int *)FUN_04008f68();
  } while (piVar4 != piVar5);
  return iVar2;
}



//==================== FUN_0400929c @ 0400929c ====================

undefined4 FUN_0400929c(undefined4 param_1,undefined4 param_2)

{
  int *piVar1;
  undefined4 uVar2;
  
  piVar1 = (int *)FUN_04008f68();
  if (piVar1 == (int *)0x0) {
    return 0xffffffed;
  }
  if ((piVar1[0x12] & 1U) == 0) {
    return 0xffffffea;
  }
                    /* WARNING: Could not recover jumptable at 0x040092d8. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  uVar2 = (**(code **)(*(int *)(*piVar1 + 0x3c) + 4))(piVar1,param_1,param_2);
  return uVar2;
}



//==================== FUN_040092ec @ 040092ec ====================

int FUN_040092ec(undefined4 param_1,undefined4 param_2)

{
  int *piVar1;
  undefined4 uVar2;
  code *pcVar3;
  int iVar4;
  int iVar5;
  undefined4 local_14;
  
  local_14 = param_2;
  piVar1 = (int *)FUN_04008f68();
  if (piVar1 == (int *)0x0) {
    iVar4 = -0x13;
  }
  else if ((piVar1[0x12] & 1U) == 0) {
    iVar4 = -0x16;
  }
  else {
    iVar5 = 0x20;
    uVar2 = 1;
    do {
      pcVar3 = *(code **)(*(int *)(*piVar1 + 0x3c) + 8);
      iVar4 = (*pcVar3)(piVar1,uVar2,&local_14,pcVar3,param_1);
      if (iVar4 < 1) {
        if (iVar4 != 0) goto LAB_0400938c;
      }
      else {
        FUN_0400a0a8(local_14,iVar4);
      }
      pcVar3 = *(code **)(*(int *)(*piVar1 + 0x3c) + 0xc);
      if (pcVar3 != (code *)0x0) {
        (*pcVar3)(piVar1,local_14,iVar4);
      }
      if (iVar4 < 1) {
LAB_0400938c:
        if (iVar4 == -0xb) {
          return 0;
        }
        return iVar4;
      }
      iVar5 = iVar5 + -1;
      uVar2 = 0;
    } while (iVar5 != 0);
  }
  return iVar4;
}



//==================== FUN_04009550 @ 04009550 ====================

void FUN_04009550(char *param_1,char *param_2)

{
  char cVar1;
  char *pcVar2;
  char *local_14;
  
  pcVar2 = param_2 + -1;
  local_14 = param_2;
  do {
    if (param_1 == (char *)0x0) {
      cVar1 = '\0';
    }
    else {
      cVar1 = FUN_04007718(param_1,&local_14,0x10);
    }
    pcVar2 = pcVar2 + 1;
    *pcVar2 = cVar1;
    if ((param_1 != (char *)0x0) && (param_1 = local_14, *local_14 != '\0')) {
      param_1 = local_14 + 1;
    }
  } while (param_2 + 5 != pcVar2);
  return;
}



//==================== FUN_040095ac @ 040095ac ====================

undefined4 FUN_040095ac(undefined4 param_1,byte *param_2)

{
  byte bVar1;
  undefined4 uVar2;
  
  uVar2 = func_0x03fbefb0();
  uVar2 = FUN_04009550(uVar2,param_2);
  bVar1 = *param_2;
  if ((bVar1 & 1) != 0) {
    return 0;
  }
  if (((((bVar1 != 0 || param_2[1] != 0) || param_2[2] != 0) || param_2[3] != 0) || param_2[4] != 0)
      || param_2[5] != 0) {
    uVar2 = 1;
  }
  if (((((bVar1 == 0 && param_2[1] == 0) && param_2[2] == 0) && param_2[3] == 0) && param_2[4] == 0)
      && param_2[5] == 0) {
    uVar2 = 0;
  }
  return uVar2;
}



//==================== FUN_04009608 @ 04009608 ====================

void FUN_04009608(undefined4 param_1,undefined4 param_2)

{
  undefined1 auStack_1c [20];
  
  FUN_04007514(auStack_1c,DAT_04009638,param_2);
  func_0x03fbeb7c(param_1,auStack_1c);
  return;
}



//==================== FUN_0400972c @ 0400972c ====================

undefined4 FUN_0400972c(void)

{
  int iVar1;
  
  iVar1 = FUN_04008f68();
  if (iVar1 == 0) {
    return DAT_04009750;
  }
  iVar1 = FUN_04008f68();
  return *(undefined4 *)(iVar1 + 4);
}



//==================== FUN_04009754 @ 04009754 ====================

/* WARNING: Control flow encountered bad instruction data */

void FUN_04009754(void)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  
  iVar1 = func_0x03fbefb0(DAT_040097d8);
  iVar2 = func_0x03fbefb0(DAT_040097dc);
  if ((iVar2 != 0) && (iVar2 = FUN_04005a60(iVar2,DAT_040097e0), iVar2 == 0)) {
    return;
  }
  iVar2 = FUN_04008f68();
  if (iVar2 == 0) {
    if (iVar1 == 0) {
      return;
    }
  }
  else {
    if (iVar1 != 0) {
      uVar3 = FUN_0400972c();
      iVar1 = FUN_04005a60(iVar1,uVar3);
      if (iVar1 == 0) {
        return;
      }
    }
    FUN_0400972c();
  }
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



//==================== FUN_040097e4 @ 040097e4 ====================

void FUN_040097e4(int param_1)

{
  int *piVar1;
  int iVar2;
  
  iVar2 = func_0x03fbefb0(DAT_04009854);
  if ((iVar2 != 0) && (iVar2 = FUN_04005a60(iVar2,DAT_04009860), iVar2 == 0)) {
    return;
  }
  iVar2 = FUN_04008f68();
  piVar1 = DAT_04009858;
  if (iVar2 == 0) {
    return;
  }
  if (param_1 != 0) {
    iVar2 = FUN_04008f68();
    *piVar1 = iVar2;
  }
  FUN_04008f30();
  FUN_04009754();
  iVar2 = FUN_04008f68();
  if (*piVar1 == iVar2) {
    *DAT_0400985c = 1;
  }
  return;
}



//==================== FUN_0400a02c @ 0400a02c ====================

undefined4 FUN_0400a02c(void)

{
  return *DAT_0400a038;
}



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



//==================== FUN_0400a3e0 @ 0400a3e0 ====================

undefined4 FUN_0400a3e0(void)

{
  undefined4 uVar1;
  ushort uVar2;
  
  uVar2 = *DAT_0400a418 >> 8 | *DAT_0400a418 << 8;
  if (uVar2 == 0xffff) {
    uVar2 = 0xfff;
  }
  if ((uVar2 & 0xfff) == 0xfff) {
    uVar1 = 0xe;
  }
  else {
    uVar1 = 0x12;
  }
  return uVar1;
}



//==================== FUN_0400a41c @ 0400a41c ====================

undefined4 FUN_0400a41c(int param_1,undefined4 param_2,int param_3)

{
  ushort uVar1;
  uint uVar2;
  
  uVar2 = (uint)(*DAT_0400a4ac >> 8) | (*DAT_0400a4ac & 0xff) << 8;
  if (uVar2 == 0xffff) {
    uVar2 = 0xfff;
  }
  FUN_04005ea4(param_1,param_2,6);
  FUN_04005ea4(param_1 + 6,DAT_0400a4b0);
  uVar1 = (ushort)DAT_0400a4b4 & (ushort)(param_3 << 8) | (ushort)((uint)param_3 >> 8) & 0xff;
  if ((uVar2 & 0xfff) == 0xfff) {
    *(ushort *)(param_1 + 0xc) = uVar1;
    return 0xe;
  }
  *(undefined2 *)(param_1 + 0xc) = 0x81;
  *(ushort *)(param_1 + 0xe) = (ushort)((uVar2 & 0xff) << 8) | (ushort)((uVar2 & 0xfff) >> 8);
  *(ushort *)(param_1 + 0x10) = uVar1;
  return 0x12;
}



//==================== FUN_0400a4b8 @ 0400a4b8 ====================

undefined4 FUN_0400a4b8(int param_1,undefined4 param_2,ushort param_3)

{
  ushort uVar1;
  undefined4 uVar2;
  ushort uVar3;
  
  FUN_04005ea4(param_1,param_2,6);
  FUN_04005ea4(param_1 + 6,DAT_0400a530);
  uVar3 = *(ushort *)(param_1 + 0xc) >> 8 | *(ushort *)(param_1 + 0xc) << 8;
  uVar1 = (ushort)DAT_0400a534 & param_3 << 8 | param_3 >> 8;
  if (uVar3 == 0x8100) {
    *(ushort *)(param_1 + 0x10) = uVar1;
    return 0x12;
  }
  if (uVar3 < 0x5eb) {
    *(ushort *)(param_1 + 0x14) = uVar1;
    uVar2 = 0x16;
  }
  else {
    uVar2 = 0xe;
    *(ushort *)(param_1 + 0xc) = uVar1;
  }
  return uVar2;
}



//==================== FUN_0400a538 @ 0400a538 ====================

void FUN_0400a538(undefined4 *param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  undefined4 local_10;
  undefined4 local_c;
  
  *param_1 = DAT_0400a5b4;
  iVar1 = *DAT_0400a5b8;
  *DAT_0400a5b8 = iVar1 + 1;
  *(ushort *)(param_1 + 1) =
       (ushort)DAT_0400a5bc & (ushort)(iVar1 << 8) | (ushort)((uint)iVar1 >> 8) & 0xff;
  *(undefined2 *)((int)param_1 + 6) = 0x40;
  *(undefined1 *)(param_1 + 2) = 0xff;
  *(undefined2 *)((int)param_1 + 10) = 0;
  local_10 = param_3;
  local_c = param_2;
  FUN_04005ea4(param_1 + 3,&local_10,4);
  FUN_04005ea4(param_1 + 4,&local_c);
  return;
}



//==================== FUN_0400a5c0 @ 0400a5c0 ====================

void FUN_0400a5c0(int param_1,undefined4 param_2,uint param_3,uint param_4,uint param_5)

{
  undefined2 uVar1;
  uint uVar2;
  
  if ((param_5 & 1) != 0) {
    *(undefined1 *)(param_1 + param_5 + 0x1c) = 0;
  }
  FUN_0400a538(param_1,param_2,*DAT_0400a640);
  uVar2 = (param_5 & 0xffff) + 0x1c;
  *(ushort *)(param_1 + 2) = (ushort)((uVar2 & 0xff) << 8) | (ushort)(uVar2 >> 8) & 0xff;
  *(undefined1 *)(param_1 + 9) = 0x11;
  uVar2 = (param_5 & 0xffff) + 8;
  uVar1 = FUN_04007c5c(param_1,0x14);
  *(undefined2 *)(param_1 + 10) = uVar1;
  *(ushort *)(param_1 + 0x14) = (ushort)((param_4 & 0xff) << 8) | (ushort)(param_4 >> 8) & 0xff;
  *(ushort *)(param_1 + 0x16) = (ushort)((param_3 & 0xff) << 8) | (ushort)(param_3 >> 8) & 0xff;
  *(ushort *)(param_1 + 0x18) = (ushort)((uVar2 & 0xff) << 8) | (ushort)(uVar2 >> 8) & 0xff;
  *(undefined2 *)(param_1 + 0x1a) = 0;
  return;
}



//==================== FUN_0400a644 @ 0400a644 ====================

undefined4
FUN_0400a644(undefined4 param_1,int param_2,undefined4 param_3,undefined4 param_4,int param_5)

{
  int *piVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  piVar1 = DAT_0400a728;
  iVar5 = *DAT_0400a728;
  if (iVar5 == 0) {
    uVar2 = 0xffffffff;
  }
  else {
    iVar4 = param_2;
    uVar2 = param_1;
    if (0xfffffffc < param_2 - 1U && param_2 != -2) {
      iVar4 = -1;
      uVar2 = DAT_0400a72c;
    }
    iVar3 = FUN_0400a41c(iVar5,uVar2,0x800,param_4,param_1,param_2);
    FUN_0400a5c0(iVar5 + iVar3,iVar4,param_3,param_4,param_5);
    iVar5 = FUN_04005ff4(uVar2,DAT_0400a730,6);
    if (iVar5 == 0) {
      *DAT_0400a734 = iVar4;
      *DAT_0400a738 = uVar2;
      *DAT_0400a73c = iVar3 + 0x1c + param_5;
      *DAT_0400a740 = 1;
      uVar2 = FUN_040061e4();
      *DAT_0400a744 = uVar2;
      FUN_04007de8();
      uVar2 = 1;
    }
    else {
      FUN_0400929c(*piVar1);
      uVar2 = 0;
    }
  }
  return uVar2;
}



//==================== FUN_0400ac9c @ 0400ac9c ====================

void FUN_0400ac9c(undefined4 *param_1)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  undefined4 *puVar5;
  undefined1 auStack_110 [256];
  
  puVar5 = param_1 + 4;
  FUN_04005994(auStack_110,DAT_0400ad44);
  uVar2 = FUN_04005b98(auStack_110);
  uVar4 = uVar2 + 3;
  *param_1 = 0x1000000;
  uVar3 = (uVar4 & 0xfffffffc) + 0x14;
  param_1[1] = uVar3 * 0x1000000 | (uVar3 >> 8 & 0xff) << 0x10 | (uVar3 >> 0x10 & 0xff) << 8 |
               uVar3 >> 0x18;
  param_1[2] = 0;
  param_1[3] = uVar2 << 0x18 | (uVar2 >> 8 & 0xff) << 0x10 | (uVar2 >> 0x10 & 0xff) << 8 |
               uVar2 >> 0x18;
  if ((uVar2 & 3) != 0) {
    if ((int)uVar2 < 0) {
      uVar3 = uVar4;
    }
    else {
      uVar3 = uVar2;
    }
    puVar5[(int)uVar3 >> 2] = 0;
  }
  iVar1 = (int)uVar4 >> 2;
  FUN_04005ea4(puVar5,auStack_110,uVar2);
  puVar5[iVar1] = 0;
  puVar5[iVar1 + 1] = 0;
  puVar5[iVar1 + 2] = 0;
  puVar5[iVar1 + 3] = 0;
  puVar5[iVar1 + 4] = 0;
  return;
}



//==================== FUN_0400ad48 @ 0400ad48 ====================

void FUN_0400ad48(int param_1,int param_2,undefined4 param_3,int param_4)

{
  int iVar1;
  uint uVar2;
  undefined4 uVar3;
  undefined4 *puVar4;
  int iVar5;
  int iVar6;
  uint local_810 [4];
  undefined4 local_800;
  int local_7fc;
  undefined1 auStack_7f8 [2024];
  
  local_7fc = param_2 << 0x18;
  uVar2 = *DAT_0400ae28 + 1;
  *DAT_0400ae28 = uVar2;
  local_810[0] = uVar2 * 0x1000000 | (uVar2 >> 8 & 0xff) << 0x10 | (uVar2 >> 0x10 & 0xff) << 8 |
                 uVar2 >> 0x18;
  local_810[1] = 0;
  local_810[2] = 0x2000000;
  local_800 = 0x2000000;
  local_810[3] = DAT_0400ae2c | param_1 << 0x18;
  if (param_4 != 0) {
    FUN_04005ea4(auStack_7f8,param_3,param_4 * 4);
  }
  iVar5 = param_4 * 4 + 0x18;
  iVar6 = *DAT_0400ae30;
  iVar1 = FUN_0400a3e0();
  FUN_04005ea4(iVar6 + iVar1 + 0x1c,local_810,iVar5);
  if (param_1 == DAT_0400ae34) {
    uVar3 = 0x6f;
  }
  else {
    if (param_1 == DAT_0400ae34 + 5) {
      puVar4 = DAT_0400ae38;
    }
    else {
      puVar4 = DAT_0400ae3c;
    }
    uVar3 = *puVar4;
  }
  FUN_0400a644(DAT_0400ae48,*DAT_0400ae44,uVar3,*DAT_0400ae40,iVar5);
  return;
}



//==================== FUN_0400b068 @ 0400b068 ====================

void FUN_0400b068(void)

{
  int iVar1;
  undefined1 auStack_1010 [4100];
  
  iVar1 = FUN_0400ac9c(auStack_1010);
  FUN_04005ea4(iVar1,DAT_0400b0b8,0x20);
  FUN_0400ad48(DAT_0400b0bc,5,auStack_1010,(iVar1 + 0x20) - (int)auStack_1010 >> 2);
  return;
}



//==================== FUN_0400b900 @ 0400b900 ====================

void FUN_0400b900(int param_1,int param_2,int param_3)

{
  undefined2 uVar1;
  int iVar2;
  int local_24;
  int iStack_20;
  
  local_24 = param_2;
  iStack_20 = param_3;
  if (*(char *)(param_2 + 0x14) == '\0') {
    FUN_04005ea4(&local_24,param_2 + 0xc,4,0,param_1);
    if (*DAT_0400b9d4 == local_24) {
      *DAT_0400b9d8 = 2;
    }
  }
  else if (*(char *)(param_2 + 0x14) == '\b') {
    iVar2 = FUN_0400a4b8(param_1,param_1 + 6,0x800);
    *(undefined2 *)(param_2 + 10) = 0;
    *(undefined2 *)(param_2 + 6) = 0;
    FUN_04005ea4(param_2 + 0x10,param_2 + 0xc,4);
    FUN_04005ea4(param_2 + 0xc,DAT_0400b9dc,4);
    uVar1 = FUN_04007c5c(param_2,0x14);
    *(undefined2 *)(param_2 + 10) = uVar1;
    *(undefined1 *)(param_2 + 0x14) = 0;
    *(undefined2 *)(param_2 + 0x16) = 0;
    uVar1 = FUN_04007c5c(param_2 + 0x14,param_3 + -0x14);
    *(undefined2 *)(param_2 + 0x16) = uVar1;
    FUN_0400929c(param_1,iVar2 + param_3);
  }
  return;
}



//==================== FUN_0400c65c @ 0400c65c ====================

undefined4 FUN_0400c65c(void)

{
  func_0x03fd17b4();
  return 0;
}



//==================== FUN_0400c66c @ 0400c66c ====================

uint FUN_0400c66c(uint param_1,uint param_2)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  bool bVar4;
  
  if (param_2 - 1 == 0) {
    return param_1;
  }
  if (param_2 == 0) {
    thunk_FUN_04004844();
    return 0;
  }
  if (param_1 <= param_2) {
    return (uint)(param_1 == param_2);
  }
  if ((param_2 & param_2 - 1) == 0) {
    return param_1 >> (0x1fU - LZCOUNT(param_2) & 0xff);
  }
  uVar1 = param_2 << (LZCOUNT(param_2) - LZCOUNT(param_1) & 0xffU);
  uVar3 = 1 << (LZCOUNT(param_2) - LZCOUNT(param_1) & 0xffU);
  uVar2 = 0;
  while( true ) {
    if (uVar1 <= param_1) {
      param_1 = param_1 - uVar1;
      uVar2 = uVar2 | uVar3;
    }
    if (uVar1 >> 1 <= param_1) {
      param_1 = param_1 - (uVar1 >> 1);
      uVar2 = uVar2 | uVar3 >> 1;
    }
    if (uVar1 >> 2 <= param_1) {
      param_1 = param_1 - (uVar1 >> 2);
      uVar2 = uVar2 | uVar3 >> 2;
    }
    if (uVar1 >> 3 <= param_1) {
      param_1 = param_1 - (uVar1 >> 3);
      uVar2 = uVar2 | uVar3 >> 3;
    }
    bVar4 = param_1 == 0;
    if (!bVar4) {
      uVar3 = uVar3 >> 4;
      bVar4 = uVar3 == 0;
    }
    if (bVar4) break;
    uVar1 = uVar1 >> 4;
  }
  return uVar2;
}



//==================== FUN_0400c7d4 @ 0400c7d4 ====================

void FUN_0400c7d4(void)

{
  FUN_0400c66c();
  return;
}



