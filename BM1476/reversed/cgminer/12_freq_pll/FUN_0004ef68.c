//==================== FUN_0004ef68 @ 0004ef68 ====================

void FUN_0004ef68(uint *param_1,byte *param_2)

{
  byte *pbVar1;
  byte *pbVar2;
  byte bVar3;
  int iVar4;
  uint *puVar5;
  uint *puVar6;
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
  byte *pbVar30;
  uint uVar31;
  uint uVar32;
  uint uVar33;
  uint *puVar34;
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
  undefined4 *puVar46;
  uint uVar47;
  uint uVar48;
  uint uVar49;
  uint uVar50;
  uint uVar51;
  uint uVar52;
  uint uVar53;
  uint uVar54;
  uint uVar55;
  ulonglong uVar56;
  ulonglong uVar57;
  ulonglong uVar58;
  ulonglong uVar59;
  ulonglong uVar60;
  undefined8 uVar61;
  ulonglong uVar62;
  ulonglong uVar63;
  undefined4 local_128 [2];
  uint local_120 [5];
  int iStack_10c;
  uint local_108;
  int iStack_104;
  uint local_100;
  int iStack_fc;
  uint local_f8;
  int iStack_f4;
  uint local_f0;
  int iStack_ec;
  uint local_e8;
  int iStack_e4;
  uint local_e0;
  int iStack_dc;
  uint local_d8;
  int iStack_d4;
  uint local_d0;
  int iStack_cc;
  uint local_c8;
  int iStack_c4;
  uint local_c0;
  int iStack_bc;
  uint local_b8;
  int iStack_b4;
  uint local_b0;
  int iStack_ac;
  uint local_a8;
  int iStack_a4;
  uint local_a0 [32];
  
  pbVar30 = param_2 + 0x80;
  puVar46 = local_128;
  do {
    uVar59 = VectorShiftLeft((ulonglong)param_2[1],8,0x40,0);
    bVar3 = *param_2;
    uVar56 = VectorShiftLeft((ulonglong)param_2[2],0x10,0x40,0);
    uVar62 = VectorShiftLeft((ulonglong)param_2[3],0x18,0x40,0);
    pbVar1 = param_2 + 6;
    uVar57 = VectorShiftLeft((ulonglong)param_2[4],0x20,0x40,0);
    pbVar2 = param_2 + 7;
    uVar63 = VectorShiftLeft((ulonglong)param_2[5],0x28,0x40,0);
    param_2 = param_2 + 8;
    uVar60 = VectorShiftLeft((ulonglong)*pbVar1,0x30,0x40,0);
    uVar58 = VectorShiftLeft((ulonglong)*pbVar2,0x38,0x40,0);
    uVar58 = CONCAT44((int)((uVar59 | uVar56) >> 0x20),(uint)bVar3 | (uint)(uVar59 | uVar56)) |
             uVar62 | uVar57 | uVar63 | uVar60 | uVar58;
    puVar46[2] = (int)uVar58;
    puVar46[3] = (int)(uVar58 >> 0x20);
    puVar46 = puVar46 + 2;
  } while (param_2 != pbVar30);
  puVar6 = &local_a8;
  puVar34 = param_1 + -2;
  do {
    uVar11 = puVar34[3];
    puVar5 = puVar6 + 2;
    *puVar5 = puVar34[2];
    puVar6[3] = uVar11;
    puVar6 = puVar5;
    puVar34 = puVar34 + 2;
  } while (puVar5 != local_a0 + 0xe);
  uVar7 = local_a0[2] + local_a0[10] + local_120[4];
  uVar12 = local_a0[3] + local_a0[0xb] + (uint)CARRY4(local_a0[2],local_a0[10]) + iStack_10c +
           (uint)CARRY4(local_a0[2] + local_a0[10],local_120[4]);
  uVar20 = param_1[0x12] ^ 0x2b3e6c1f ^ uVar7;
  uVar25 = param_1[0x13] ^ 0x9b05688c ^ uVar12;
  uVar61 = VectorAdd(CONCAT44(uVar20,uVar25),0xbb67ae8584caa73b,8);
  uVar31 = local_a0[6] + local_a0[0xe] + local_f0;
  uVar35 = local_a0[7] + local_a0[0xf] + (uint)CARRY4(local_a0[6],local_a0[0xe]) + iStack_ec +
           (uint)CARRY4(local_a0[6] + local_a0[0xe],local_f0);
  uVar44 = (uint)((ulonglong)uVar61 >> 0x20);
  uVar37 = (uint)uVar61;
  uVar11 = uVar44 ^ local_a0[0xb];
  uVar47 = (uVar37 ^ local_a0[10]) >> 0x18 | uVar11 << 8;
  uVar11 = uVar11 >> 0x18 | (uVar37 ^ local_a0[10]) << 8;
  uVar21 = local_a0[0] + local_a0[8] + local_120[0];
  uVar26 = local_a0[1] + local_a0[9] + (uint)CARRY4(local_a0[0],local_a0[8]) + local_120[1] +
           (uint)CARRY4(local_a0[0] + local_a0[8],local_120[0]);
  uVar22 = local_a0[0xc] + local_a0[4] + local_100;
  uVar27 = local_a0[0xd] + local_a0[5] + (uint)CARRY4(local_a0[0xc],local_a0[4]) + iStack_fc +
           (uint)CARRY4(local_a0[0xc] + local_a0[4],local_100);
  uVar23 = uVar7 + local_108 + uVar47;
  uVar28 = uVar12 + iStack_104 + (uint)CARRY4(uVar7,local_108) + uVar11 +
           (uint)CARRY4(uVar7 + local_108,uVar47);
  uVar32 = param_1[0x16] ^ 0x137e2179 ^ uVar31;
  uVar36 = param_1[0x17] ^ 0x5be0cd19 ^ uVar35;
  uVar24 = param_1[0x10] ^ 0xade682d1 ^ uVar21;
  uVar29 = param_1[0x11] ^ 0x510e527f ^ uVar26;
  uVar8 = param_1[0x14] ^ 0xfb41bd6b ^ uVar22;
  uVar13 = param_1[0x15] ^ 0x1f83d9ab ^ uVar27;
  uVar25 = uVar25 ^ uVar23;
  uVar20 = uVar20 ^ uVar28;
  uVar9 = uVar36 + 0x5f1d36f1;
  uVar14 = uVar32 + 0xa54ff53a + (uint)(0xa0e2c90e < uVar36);
  uVar15 = uVar25 >> 0x10 | uVar20 << 0x10;
  uVar43 = uVar29 + 0xf3bcc908;
  uVar45 = uVar24 + 0x6a09e667 + (uint)(0xc4336f7 < uVar29);
  uVar10 = uVar13 + 0xfe94f82b;
  uVar16 = uVar8 + 0x3c6ef372 + (uint)(0x16b07d4 < uVar13);
  uVar17 = uVar20 >> 0x10 | uVar25 << 0x10;
  uVar12 = uVar14 ^ local_a0[0xf];
  uVar25 = uVar45 ^ local_a0[9];
  uVar7 = (uVar9 ^ local_a0[0xe]) >> 0x18 | uVar12 << 8;
  uVar20 = uVar16 ^ local_a0[0xd];
  uVar18 = uVar12 >> 0x18 | (uVar9 ^ local_a0[0xe]) << 8;
  uVar19 = (uVar10 ^ local_a0[0xc]) >> 0x18 | uVar20 << 8;
  uVar12 = (uVar43 ^ local_a0[8]) >> 0x18 | uVar25 << 8;
  uVar48 = uVar25 >> 0x18 | (uVar43 ^ local_a0[8]) << 8;
  uVar20 = uVar20 >> 0x18 | (uVar10 ^ local_a0[0xc]) << 8;
  uVar33 = uVar15 + uVar37;
  uVar37 = uVar17 + uVar44 + (uint)CARRY4(uVar15,uVar37);
  uVar11 = uVar37 ^ uVar11;
  uVar25 = (uVar33 ^ uVar47) << 1 | uVar11 >> 0x1f;
  uVar44 = uVar11 << 1 | (uVar33 ^ uVar47) >> 0x1f;
  uVar38 = uVar31 + local_e8 + uVar7;
  uVar42 = uVar35 + iStack_e4 + (uint)CARRY4(uVar31,local_e8) + uVar18 +
           (uint)CARRY4(uVar31 + local_e8,uVar7);
  uVar39 = uVar21 + local_120[2] + uVar12;
  uVar26 = uVar26 + local_120[3] + (uint)CARRY4(uVar21,local_120[2]) + uVar48 +
           (uint)CARRY4(uVar21 + local_120[2],uVar12);
  uVar35 = uVar22 + local_f8 + uVar19;
  uVar47 = uVar27 + iStack_f4 + (uint)CARRY4(uVar22,local_f8) + uVar20 +
           (uint)CARRY4(uVar22 + local_f8,uVar19);
  uVar32 = uVar42 ^ uVar32;
  uVar11 = (uVar38 ^ uVar36) >> 0x10 | uVar32 << 0x10;
  uVar40 = uVar39 + local_e0 + uVar25;
  uVar49 = uVar32 >> 0x10 | (uVar38 ^ uVar36) << 0x10;
  uVar8 = uVar47 ^ uVar8;
  uVar27 = uVar26 + iStack_dc + (uint)CARRY4(uVar39,local_e0) + uVar44 +
           (uint)CARRY4(uVar39 + local_e0,uVar25);
  uVar36 = uVar27 ^ uVar49;
  uVar21 = (uVar35 ^ uVar13) >> 0x10 | uVar8 << 0x10;
  uVar22 = uVar8 >> 0x10 | (uVar35 ^ uVar13) << 0x10;
  uVar8 = uVar21 + uVar10;
  uVar26 = uVar26 ^ uVar24;
  uVar10 = uVar22 + uVar16 + (uint)CARRY4(uVar21,uVar10);
  uVar16 = uVar36 + uVar8;
  uVar31 = (uVar40 ^ uVar11) + uVar10 + (uint)CARRY4(uVar36,uVar8);
  uVar10 = uVar10 ^ uVar20;
  uVar44 = uVar31 ^ uVar44;
  uVar41 = (uVar39 ^ uVar29) >> 0x10 | uVar26 << 0x10;
  uVar39 = uVar26 >> 0x10 | (uVar39 ^ uVar29) << 0x10;
  uVar20 = (uVar16 ^ uVar25) >> 0x18 | uVar44 << 8;
  uVar44 = uVar44 >> 0x18 | (uVar16 ^ uVar25) << 8;
  uVar25 = (uVar8 ^ uVar19) << 1 | uVar10 >> 0x1f;
  uVar26 = uVar10 << 1 | (uVar8 ^ uVar19) >> 0x1f;
  uVar19 = uVar40 + local_d8 + uVar20;
  uVar24 = uVar27 + iStack_d4 + (uint)CARRY4(uVar40,local_d8) + uVar44 +
           (uint)CARRY4(uVar40 + local_d8,uVar20);
  uVar27 = uVar41 + uVar43;
  uVar32 = uVar39 + uVar45 + (uint)CARRY4(uVar41,uVar43);
  uVar29 = uVar23 + local_d0 + uVar25;
  uVar23 = uVar28 + iStack_cc + (uint)CARRY4(uVar23,local_d0) + uVar26 +
           (uint)CARRY4(uVar23 + local_d0,uVar25);
  uVar10 = uVar24 ^ uVar40 ^ uVar11;
  uVar48 = uVar32 ^ uVar48;
  uVar8 = (uVar19 ^ uVar36) >> 0x10 | uVar10 << 0x10;
  uVar40 = uVar11 + uVar9;
  uVar28 = uVar49 + uVar14 + (uint)CARRY4(uVar11,uVar9);
  uVar39 = uVar39 ^ uVar23;
  uVar11 = uVar39 + uVar40;
  uVar9 = (uVar41 ^ uVar29) + uVar28 + (uint)CARRY4(uVar39,uVar40);
  uVar10 = uVar10 >> 0x10 | (uVar19 ^ uVar36) << 0x10;
  uVar28 = uVar28 ^ uVar18;
  uVar13 = (uVar27 ^ uVar12) << 1 | uVar48 >> 0x1f;
  uVar43 = uVar8 + uVar16;
  uVar45 = uVar10 + uVar31 + (uint)CARRY4(uVar8,uVar16);
  uVar14 = (uVar40 ^ uVar7) << 1 | uVar28 >> 0x1f;
  uVar16 = uVar28 << 1 | (uVar40 ^ uVar7) >> 0x1f;
  uVar7 = uVar48 << 1 | (uVar27 ^ uVar12) >> 0x1f;
  uVar26 = uVar9 ^ uVar26;
  uVar44 = uVar45 ^ uVar44;
  uVar36 = uVar38 + local_b0 + uVar13;
  uVar12 = uVar42 + iStack_ac + (uint)CARRY4(uVar38,local_b0) + uVar7 +
           (uint)CARRY4(uVar38 + local_b0,uVar13);
  uVar28 = uVar35 + local_c0 + uVar14;
  uVar50 = (uVar43 ^ uVar20) << 1 | uVar44 >> 0x1f;
  uVar35 = uVar47 + iStack_bc + (uint)CARRY4(uVar35,local_c0) + uVar16 +
           (uint)CARRY4(uVar35 + local_c0,uVar14);
  uVar22 = uVar12 ^ uVar22;
  uVar44 = uVar44 << 1 | (uVar43 ^ uVar20) >> 0x1f;
  uVar31 = uVar22 + uVar33;
  uVar33 = (uVar36 ^ uVar21) + uVar37 + (uint)CARRY4(uVar22,uVar33);
  uVar40 = (uVar11 ^ uVar25) >> 0x18 | uVar26 << 8;
  uVar20 = uVar26 >> 0x18 | (uVar11 ^ uVar25) << 8;
  uVar17 = uVar35 ^ uVar17;
  uVar7 = uVar33 ^ uVar7;
  uVar48 = (uVar31 ^ uVar13) >> 0x18 | uVar7 << 8;
  uVar13 = uVar7 >> 0x18 | (uVar31 ^ uVar13) << 8;
  uVar25 = uVar29 + local_c8 + uVar40;
  uVar47 = uVar23 + iStack_c4 + (uint)CARRY4(uVar29,local_c8) + uVar20 +
           (uint)CARRY4(uVar29 + local_c8,uVar40);
  uVar18 = uVar36 + local_a8 + uVar48;
  uVar26 = uVar12 + iStack_a4 + (uint)CARRY4(uVar36,local_a8) + uVar13 +
           (uint)CARRY4(uVar36 + local_a8,uVar48);
  uVar12 = uVar47 ^ uVar41 ^ uVar29;
  uVar7 = uVar26 ^ uVar36 ^ uVar21;
  uVar51 = (uVar25 ^ uVar39) >> 0x10 | uVar12 << 0x10;
  uVar37 = uVar17 + uVar27;
  uVar38 = (uVar28 ^ uVar15) + uVar32 + (uint)CARRY4(uVar17,uVar27);
  uVar52 = (uVar18 ^ uVar22) >> 0x10 | uVar7 << 0x10;
  uVar23 = uVar12 >> 0x10 | (uVar25 ^ uVar39) << 0x10;
  uVar29 = uVar25 + local_100 + uVar50;
  uVar32 = uVar47 + iStack_fc + (uint)CARRY4(uVar25,local_100) + uVar44 +
           (uint)CARRY4(uVar25 + local_100,uVar50);
  uVar25 = uVar7 >> 0x10 | (uVar18 ^ uVar22) << 0x10;
  uVar16 = uVar38 ^ uVar16;
  uVar7 = uVar51 + uVar11;
  uVar21 = uVar23 + uVar9 + (uint)CARRY4(uVar51,uVar11);
  uVar9 = uVar52 + uVar31;
  uVar31 = uVar25 + uVar33 + (uint)CARRY4(uVar52,uVar31);
  uVar47 = (uVar37 ^ uVar14) >> 0x18 | uVar16 << 8;
  uVar14 = uVar16 >> 0x18 | (uVar37 ^ uVar14) << 8;
  uVar20 = uVar21 ^ uVar20;
  uVar13 = uVar31 ^ uVar13;
  uVar53 = (uVar7 ^ uVar40) << 1 | uVar20 >> 0x1f;
  uVar11 = uVar28 + local_b8 + uVar47;
  uVar12 = uVar35 + iStack_b4 + (uint)CARRY4(uVar28,local_b8) + uVar14 +
           (uint)CARRY4(uVar28 + local_b8,uVar47);
  uVar27 = uVar20 << 1 | (uVar7 ^ uVar40) >> 0x1f;
  uVar54 = (uVar9 ^ uVar48) << 1 | uVar13 >> 0x1f;
  uVar20 = uVar12 ^ uVar28 ^ uVar15;
  uVar36 = uVar13 << 1 | (uVar9 ^ uVar48) >> 0x1f;
  uVar22 = uVar11 + local_d8 + uVar53;
  uVar48 = uVar12 + iStack_d4 + (uint)CARRY4(uVar11,local_d8) + uVar27 +
           (uint)CARRY4(uVar11 + local_d8,uVar53);
  uVar16 = (uVar11 ^ uVar17) >> 0x10 | uVar20 << 0x10;
  uVar41 = uVar19 + local_b0 + uVar54;
  uVar49 = uVar24 + iStack_ac + (uint)CARRY4(uVar19,local_b0) + uVar36 +
           (uint)CARRY4(uVar19 + local_b0,uVar54);
  uVar11 = uVar20 >> 0x10 | (uVar11 ^ uVar17) << 0x10;
  uVar25 = uVar48 ^ uVar25;
  uVar23 = uVar49 ^ uVar23;
  uVar33 = uVar32 ^ uVar11;
  uVar20 = uVar16 + uVar37;
  uVar12 = uVar11 + uVar38 + (uint)CARRY4(uVar16,uVar37);
  uVar11 = uVar33 + uVar9;
  uVar9 = (uVar29 ^ uVar16) + uVar31 + (uint)CARRY4(uVar33,uVar9);
  uVar17 = uVar25 + uVar43;
  uVar28 = (uVar22 ^ uVar52) + uVar45 + (uint)CARRY4(uVar25,uVar43);
  uVar44 = uVar9 ^ uVar44;
  uVar14 = uVar12 ^ uVar14;
  uVar37 = uVar23 + uVar20;
  uVar40 = (uVar41 ^ uVar51) + uVar12 + (uint)CARRY4(uVar23,uVar20);
  uVar27 = uVar28 ^ uVar27;
  uVar36 = uVar40 ^ uVar36;
  uVar42 = (uVar11 ^ uVar50) >> 0x18 | uVar44 << 8;
  uVar31 = uVar44 >> 0x18 | (uVar11 ^ uVar50) << 8;
  uVar38 = (uVar20 ^ uVar47) << 1 | uVar14 >> 0x1f;
  uVar39 = (uVar37 ^ uVar54) >> 0x18 | uVar36 << 8;
  uVar12 = uVar29 + local_e0 + uVar42;
  uVar35 = uVar14 << 1 | (uVar20 ^ uVar47) >> 0x1f;
  uVar13 = uVar32 + iStack_dc + (uint)CARRY4(uVar29,local_e0) + uVar31 +
           (uint)CARRY4(uVar29 + local_e0,uVar42);
  uVar14 = (uVar17 ^ uVar53) >> 0x18 | uVar27 << 8;
  uVar15 = uVar27 >> 0x18 | (uVar17 ^ uVar53) << 8;
  uVar24 = uVar36 >> 0x18 | (uVar37 ^ uVar54) << 8;
  uVar19 = uVar13 ^ uVar29 ^ uVar16;
  uVar32 = uVar41 + local_d0 + uVar39;
  uVar36 = uVar49 + iStack_cc + (uint)CARRY4(uVar41,local_d0) + uVar24 +
           (uint)CARRY4(uVar41 + local_d0,uVar39);
  uVar20 = uVar22 + local_a8 + uVar14;
  uVar16 = uVar48 + iStack_a4 + (uint)CARRY4(uVar22,local_a8) + uVar15 +
           (uint)CARRY4(uVar22 + local_a8,uVar14);
  uVar44 = (uVar12 ^ uVar33) >> 0x10 | uVar19 << 0x10;
  uVar48 = uVar19 >> 0x10 | (uVar12 ^ uVar33) << 0x10;
  uVar19 = uVar36 ^ uVar41 ^ uVar51;
  uVar22 = uVar16 ^ uVar22 ^ uVar52;
  uVar45 = (uVar32 ^ uVar23) >> 0x10 | uVar19 << 0x10;
  uVar27 = uVar44 + uVar11;
  uVar29 = uVar48 + uVar9 + (uint)CARRY4(uVar44,uVar11);
  uVar41 = (uVar20 ^ uVar25) >> 0x10 | uVar22 << 0x10;
  uVar9 = uVar19 >> 0x10 | (uVar32 ^ uVar23) << 0x10;
  uVar22 = uVar22 >> 0x10 | (uVar20 ^ uVar25) << 0x10;
  uVar31 = uVar29 ^ uVar31;
  uVar43 = (uVar27 ^ uVar42) << 1 | uVar31 >> 0x1f;
  uVar33 = uVar45 + uVar37;
  uVar40 = uVar9 + uVar40 + (uint)CARRY4(uVar45,uVar37);
  uVar19 = uVar41 + uVar17;
  uVar17 = uVar22 + uVar28 + (uint)CARRY4(uVar41,uVar17);
  uVar23 = uVar18 + local_b8 + uVar38;
  uVar18 = uVar26 + iStack_b4 + (uint)CARRY4(uVar18,local_b8) + uVar35 +
           (uint)CARRY4(uVar18 + local_b8,uVar38);
  uVar24 = uVar40 ^ uVar24;
  uVar28 = uVar31 << 1 | (uVar27 ^ uVar42) >> 0x1f;
  uVar15 = uVar17 ^ uVar15;
  uVar42 = (uVar33 ^ uVar39) << 1 | uVar24 >> 0x1f;
  uVar24 = uVar24 << 1 | (uVar33 ^ uVar39) >> 0x1f;
  uVar10 = uVar18 ^ uVar10;
  uVar37 = uVar10 + uVar7;
  uVar39 = (uVar23 ^ uVar8) + uVar21 + (uint)CARRY4(uVar10,uVar7);
  uVar47 = (uVar19 ^ uVar14) << 1 | uVar15 >> 0x1f;
  uVar35 = uVar39 ^ uVar35;
  uVar14 = uVar15 << 1 | (uVar19 ^ uVar14) >> 0x1f;
  uVar49 = (uVar37 ^ uVar38) >> 0x18 | uVar35 << 8;
  uVar11 = uVar32 + local_120[2] + uVar43;
  uVar26 = uVar35 >> 0x18 | (uVar37 ^ uVar38) << 8;
  uVar25 = uVar36 + local_120[3] + (uint)CARRY4(uVar32,local_120[2]) + uVar28 +
           (uint)CARRY4(uVar32 + local_120[2],uVar43);
  uVar7 = uVar23 + local_f0 + uVar49;
  uVar21 = uVar18 + iStack_ec + (uint)CARRY4(uVar23,local_f0) + uVar26 +
           (uint)CARRY4(uVar23 + local_f0,uVar49);
  uVar31 = uVar12 + local_120[0] + uVar47;
  uVar32 = uVar13 + local_120[1] + (uint)CARRY4(uVar12,local_120[0]) + uVar14 +
           (uint)CARRY4(uVar12 + local_120[0],uVar47);
  uVar12 = uVar21 ^ uVar23 ^ uVar8;
  uVar8 = uVar7 + local_f8 + uVar42;
  uVar9 = uVar32 ^ uVar9;
  uVar21 = uVar21 + iStack_f4 + (uint)CARRY4(uVar7,local_f8) + uVar24 +
           (uint)CARRY4(uVar7 + local_f8,uVar42);
  uVar15 = (uVar7 ^ uVar10) >> 0x10 | uVar12 << 0x10;
  uVar7 = uVar12 >> 0x10 | (uVar7 ^ uVar10) << 0x10;
  uVar12 = uVar25 ^ uVar7;
  uVar22 = uVar21 ^ uVar22;
  uVar35 = uVar15 + uVar37;
  uVar36 = uVar7 + uVar39 + (uint)CARRY4(uVar15,uVar37);
  uVar10 = uVar22 + uVar27;
  uVar13 = (uVar8 ^ uVar41) + uVar29 + (uint)CARRY4(uVar22,uVar27);
  uVar26 = uVar36 ^ uVar26;
  uVar18 = (uVar35 ^ uVar49) << 1 | uVar26 >> 0x1f;
  uVar29 = uVar26 << 1 | (uVar35 ^ uVar49) >> 0x1f;
  uVar24 = uVar13 ^ uVar24;
  uVar39 = uVar20 + local_c8 + uVar18;
  uVar38 = (uVar10 ^ uVar42) >> 0x18 | uVar24 << 8;
  uVar37 = uVar16 + iStack_c4 + (uint)CARRY4(uVar20,local_c8) + uVar29 +
           (uint)CARRY4(uVar20 + local_c8,uVar18);
  uVar26 = uVar24 >> 0x18 | (uVar10 ^ uVar42) << 8;
  uVar16 = uVar8 + local_108 + uVar38;
  uVar24 = uVar21 + iStack_104 + (uint)CARRY4(uVar8,local_108) + uVar26 +
           (uint)CARRY4(uVar8 + local_108,uVar38);
  uVar48 = uVar37 ^ uVar48;
  uVar27 = uVar12 + uVar19;
  uVar19 = (uVar11 ^ uVar15) + uVar17 + (uint)CARRY4(uVar12,uVar19);
  uVar21 = uVar24 ^ uVar8 ^ uVar41;
  uVar23 = uVar21 >> 0x10 | (uVar16 ^ uVar22) << 0x10;
  uVar7 = uVar9 + uVar35;
  uVar20 = (uVar31 ^ uVar45) + uVar36 + (uint)CARRY4(uVar9,uVar35);
  uVar28 = uVar19 ^ uVar28;
  uVar21 = (uVar16 ^ uVar22) >> 0x10 | uVar21 << 0x10;
  uVar14 = uVar20 ^ uVar14;
  uVar49 = (uVar7 ^ uVar47) >> 0x18 | uVar14 << 8;
  uVar8 = uVar14 >> 0x18 | (uVar7 ^ uVar47) << 8;
  uVar14 = uVar48 + uVar33;
  uVar22 = (uVar39 ^ uVar44) + uVar40 + (uint)CARRY4(uVar48,uVar33);
  uVar36 = uVar21 + uVar10;
  uVar47 = uVar23 + uVar13 + (uint)CARRY4(uVar21,uVar10);
  uVar17 = (uVar27 ^ uVar43) >> 0x18 | uVar28 << 8;
  uVar29 = uVar22 ^ uVar29;
  uVar26 = uVar47 ^ uVar26;
  uVar13 = uVar28 >> 0x18 | (uVar27 ^ uVar43) << 8;
  uVar10 = (uVar36 ^ uVar38) << 1 | uVar26 >> 0x1f;
  uVar28 = uVar11 + local_c0 + uVar17;
  uVar33 = uVar25 + iStack_bc + (uint)CARRY4(uVar11,local_c0) + uVar13 +
           (uint)CARRY4(uVar11 + local_c0,uVar17);
  uVar40 = uVar31 + local_120[4] + uVar49;
  uVar26 = uVar26 << 1 | (uVar36 ^ uVar38) >> 0x1f;
  uVar42 = uVar32 + iStack_10c + (uint)CARRY4(uVar31,local_120[4]) + uVar8 +
           (uint)CARRY4(uVar31 + local_120[4],uVar49);
  uVar11 = uVar33 ^ uVar11 ^ uVar15;
  uVar41 = (uVar14 ^ uVar18) >> 0x18 | uVar29 << 8;
  uVar43 = uVar29 >> 0x18 | (uVar14 ^ uVar18) << 8;
  uVar25 = uVar42 ^ uVar31 ^ uVar45;
  uVar45 = (uVar28 ^ uVar12) >> 0x10 | uVar11 << 0x10;
  uVar12 = uVar11 >> 0x10 | (uVar28 ^ uVar12) << 0x10;
  uVar31 = uVar39 + local_e8 + uVar41;
  uVar35 = uVar37 + iStack_e4 + (uint)CARRY4(uVar39,local_e8) + uVar43 +
           (uint)CARRY4(uVar39 + local_e8,uVar41);
  uVar29 = uVar28 + local_c8 + uVar10;
  uVar32 = (uVar40 ^ uVar9) >> 0x10 | uVar25 << 0x10;
  uVar28 = uVar33 + iStack_c4 + (uint)CARRY4(uVar28,local_c8) + uVar26 +
           (uint)CARRY4(uVar28 + local_c8,uVar10);
  uVar33 = uVar25 >> 0x10 | (uVar40 ^ uVar9) << 0x10;
  uVar37 = uVar45 + uVar27;
  uVar38 = uVar12 + uVar19 + (uint)CARRY4(uVar45,uVar27);
  uVar11 = uVar35 ^ uVar39 ^ uVar44;
  uVar13 = uVar38 ^ uVar13;
  uVar44 = uVar32 + uVar7;
  uVar9 = uVar11 >> 0x10 | (uVar31 ^ uVar48) << 0x10;
  uVar15 = uVar33 + uVar20 + (uint)CARRY4(uVar32,uVar7);
  uVar33 = uVar33 ^ uVar28;
  uVar19 = (uVar31 ^ uVar48) >> 0x10 | uVar11 << 0x10;
  uVar18 = (uVar37 ^ uVar17) << 1 | uVar13 >> 0x1f;
  uVar13 = uVar13 << 1 | (uVar37 ^ uVar17) >> 0x1f;
  uVar8 = uVar15 ^ uVar8;
  uVar20 = (uVar44 ^ uVar49) << 1 | uVar8 >> 0x1f;
  uVar48 = uVar19 + uVar14;
  uVar7 = uVar9 + uVar22 + (uint)CARRY4(uVar19,uVar14);
  uVar43 = uVar43 ^ uVar7;
  uVar11 = uVar33 + uVar48;
  uVar17 = uVar8 << 1 | (uVar44 ^ uVar49) >> 0x1f;
  uVar25 = (uVar32 ^ uVar29) + uVar7 + (uint)CARRY4(uVar33,uVar48);
  uVar27 = uVar40 + local_c0 + uVar18;
  uVar39 = uVar42 + iStack_bc + (uint)CARRY4(uVar40,local_c0) + uVar13 +
           (uint)CARRY4(uVar40 + local_c0,uVar18);
  uVar26 = uVar25 ^ uVar26;
  uVar14 = uVar31 + local_f8 + uVar20;
  uVar22 = uVar35 + iStack_f4 + (uint)CARRY4(uVar31,local_f8) + uVar17 +
           (uint)CARRY4(uVar31 + local_f8,uVar20);
  uVar49 = (uVar11 ^ uVar10) >> 0x18 | uVar26 << 8;
  uVar9 = uVar39 ^ uVar9;
  uVar23 = uVar22 ^ uVar23;
  uVar7 = uVar9 + uVar36;
  uVar8 = (uVar27 ^ uVar19) + uVar47 + (uint)CARRY4(uVar9,uVar36);
  uVar31 = uVar23 + uVar37;
  uVar36 = (uVar14 ^ uVar21) + uVar38 + (uint)CARRY4(uVar23,uVar37);
  uVar10 = uVar26 >> 0x18 | (uVar11 ^ uVar10) << 8;
  uVar26 = (uVar41 ^ uVar48) << 1 | uVar43 >> 0x1f;
  uVar47 = uVar43 << 1 | (uVar41 ^ uVar48) >> 0x1f;
  uVar13 = uVar8 ^ uVar13;
  uVar40 = uVar29 + local_e0 + uVar49;
  uVar17 = uVar36 ^ uVar17;
  uVar42 = uVar28 + iStack_dc + (uint)CARRY4(uVar29,local_e0) + uVar10 +
           (uint)CARRY4(uVar29 + local_e0,uVar49);
  uVar35 = uVar16 + local_a8 + uVar26;
  uVar37 = uVar24 + iStack_a4 + (uint)CARRY4(uVar16,local_a8) + uVar47 +
           (uint)CARRY4(uVar16 + local_a8,uVar26);
  uVar41 = (uVar7 ^ uVar18) >> 0x18 | uVar13 << 8;
  uVar43 = (uVar31 ^ uVar20) >> 0x18 | uVar17 << 8;
  uVar48 = uVar42 ^ uVar32 ^ uVar29;
  uVar24 = uVar13 >> 0x18 | (uVar7 ^ uVar18) << 8;
  uVar12 = uVar37 ^ uVar12;
  uVar16 = uVar17 >> 0x18 | (uVar31 ^ uVar20) << 8;
  uVar13 = uVar27 + local_120[0] + uVar41;
  uVar18 = uVar39 + local_120[1] + (uint)CARRY4(uVar27,local_120[0]) + uVar24 +
           (uint)CARRY4(uVar27 + local_120[0],uVar41);
  uVar29 = uVar14 + local_120[4] + uVar43;
  uVar32 = uVar22 + iStack_10c + (uint)CARRY4(uVar14,local_120[4]) + uVar16 +
           (uint)CARRY4(uVar14 + local_120[4],uVar43);
  uVar20 = uVar18 ^ uVar27 ^ uVar19;
  uVar50 = (uVar40 ^ uVar33) >> 0x10 | uVar48 << 0x10;
  uVar21 = uVar32 ^ uVar14 ^ uVar21;
  uVar48 = uVar48 >> 0x10 | (uVar40 ^ uVar33) << 0x10;
  uVar51 = (uVar13 ^ uVar9) >> 0x10 | uVar20 << 0x10;
  uVar20 = uVar20 >> 0x10 | (uVar13 ^ uVar9) << 0x10;
  uVar19 = uVar12 + uVar44;
  uVar52 = (uVar29 ^ uVar23) >> 0x10 | uVar21 << 0x10;
  uVar33 = (uVar35 ^ uVar45) + uVar15 + (uint)CARRY4(uVar12,uVar44);
  uVar47 = uVar33 ^ uVar47;
  uVar27 = uVar50 + uVar11;
  uVar44 = uVar21 >> 0x10 | (uVar29 ^ uVar23) << 0x10;
  uVar28 = uVar48 + uVar25 + (uint)CARRY4(uVar50,uVar11);
  uVar11 = uVar51 + uVar7;
  uVar25 = uVar20 + uVar8 + (uint)CARRY4(uVar51,uVar7);
  uVar38 = uVar52 + uVar31;
  uVar39 = uVar44 + uVar36 + (uint)CARRY4(uVar52,uVar31);
  uVar10 = uVar28 ^ uVar10;
  uVar14 = (uVar19 ^ uVar26) >> 0x18 | uVar47 << 8;
  uVar53 = (uVar27 ^ uVar49) << 1 | uVar10 >> 0x1f;
  uVar8 = uVar47 >> 0x18 | (uVar19 ^ uVar26) << 8;
  uVar24 = uVar25 ^ uVar24;
  uVar16 = uVar39 ^ uVar16;
  uVar47 = (uVar11 ^ uVar41) << 1 | uVar24 >> 0x1f;
  uVar23 = uVar35 + local_b8 + uVar14;
  uVar36 = uVar37 + iStack_b4 + (uint)CARRY4(uVar35,local_b8) + uVar8 +
           (uint)CARRY4(uVar35 + local_b8,uVar14);
  uVar15 = uVar24 << 1 | (uVar11 ^ uVar41) >> 0x1f;
  uVar21 = uVar10 << 1 | (uVar27 ^ uVar49) >> 0x1f;
  uVar7 = uVar36 ^ uVar35 ^ uVar45;
  uVar26 = (uVar38 ^ uVar43) << 1 | uVar16 >> 0x1f;
  uVar17 = uVar40 + local_d0 + uVar47;
  uVar22 = uVar16 << 1 | (uVar38 ^ uVar43) >> 0x1f;
  uVar24 = uVar42 + iStack_cc + (uint)CARRY4(uVar40,local_d0) + uVar15 +
           (uint)CARRY4(uVar40 + local_d0,uVar47);
  uVar37 = uVar13 + local_108 + uVar26;
  uVar9 = (uVar23 ^ uVar12) >> 0x10 | uVar7 << 0x10;
  uVar18 = uVar18 + iStack_104 + (uint)CARRY4(uVar13,local_108) + uVar22 +
           (uint)CARRY4(uVar13 + local_108,uVar26);
  uVar16 = uVar7 >> 0x10 | (uVar23 ^ uVar12) << 0x10;
  uVar7 = uVar9 + uVar19;
  uVar12 = uVar16 + uVar33 + (uint)CARRY4(uVar9,uVar19);
  uVar16 = uVar24 ^ uVar16;
  uVar48 = uVar18 ^ uVar48;
  uVar8 = uVar12 ^ uVar8;
  uVar31 = uVar16 + uVar38;
  uVar35 = (uVar17 ^ uVar9) + uVar39 + (uint)CARRY4(uVar16,uVar38);
  uVar39 = (uVar7 ^ uVar14) << 1 | uVar8 >> 0x1f;
  uVar10 = uVar48 + uVar7;
  uVar19 = (uVar37 ^ uVar50) + uVar12 + (uint)CARRY4(uVar48,uVar7);
  uVar15 = uVar35 ^ uVar15;
  uVar12 = uVar8 << 1 | (uVar7 ^ uVar14) >> 0x1f;
  uVar22 = uVar19 ^ uVar22;
  uVar7 = (uVar31 ^ uVar47) >> 0x18 | uVar15 << 8;
  uVar8 = uVar15 >> 0x18 | (uVar31 ^ uVar47) << 8;
  uVar33 = (uVar10 ^ uVar26) >> 0x18 | uVar22 << 8;
  uVar26 = uVar22 >> 0x18 | (uVar10 ^ uVar26) << 8;
  uVar13 = uVar17 + local_b0 + uVar7;
  uVar15 = uVar24 + iStack_ac + (uint)CARRY4(uVar17,local_b0) + uVar8 +
           (uint)CARRY4(uVar17 + local_b0,uVar7);
  uVar14 = uVar37 + local_f0 + uVar33;
  uVar18 = uVar18 + iStack_ec + (uint)CARRY4(uVar37,local_f0) + uVar26 +
           (uint)CARRY4(uVar37 + local_f0,uVar33);
  uVar9 = uVar15 ^ uVar17 ^ uVar9;
  uVar24 = uVar18 ^ uVar37 ^ uVar50;
  uVar49 = (uVar13 ^ uVar16) >> 0x10 | uVar9 << 0x10;
  uVar22 = uVar9 >> 0x10 | (uVar13 ^ uVar16) << 0x10;
  uVar17 = (uVar14 ^ uVar48) >> 0x10 | uVar24 << 0x10;
  uVar24 = uVar24 >> 0x10 | (uVar14 ^ uVar48) << 0x10;
  uVar9 = uVar49 + uVar31;
  uVar48 = uVar22 + uVar35 + (uint)CARRY4(uVar49,uVar31);
  uVar16 = uVar17 + uVar10;
  uVar19 = uVar24 + uVar19 + (uint)CARRY4(uVar17,uVar10);
  uVar8 = uVar8 ^ uVar48;
  uVar31 = uVar29 + local_e8 + uVar39;
  uVar32 = uVar32 + iStack_e4 + (uint)CARRY4(uVar29,local_e8) + uVar12 +
           (uint)CARRY4(uVar29 + local_e8,uVar39);
  uVar50 = (uVar7 ^ uVar9) << 1 | uVar8 >> 0x1f;
  uVar26 = uVar19 ^ uVar26;
  uVar29 = uVar23 + local_d8 + uVar53;
  uVar10 = uVar36 + iStack_d4 + (uint)CARRY4(uVar23,local_d8) + uVar21 +
           (uint)CARRY4(uVar23 + local_d8,uVar53);
  uVar8 = uVar8 << 1 | (uVar7 ^ uVar9) >> 0x1f;
  uVar20 = uVar32 ^ uVar20;
  uVar54 = (uVar16 ^ uVar33) << 1 | uVar26 >> 0x1f;
  uVar23 = uVar26 << 1 | (uVar16 ^ uVar33) >> 0x1f;
  uVar44 = uVar10 ^ uVar44;
  uVar7 = uVar44 + uVar11;
  uVar25 = (uVar29 ^ uVar52) + uVar25 + (uint)CARRY4(uVar44,uVar11);
  uVar21 = uVar25 ^ uVar21;
  uVar37 = uVar14 + local_108 + uVar50;
  uVar43 = uVar18 + iStack_104 + (uint)CARRY4(uVar14,local_108) + uVar8 +
           (uint)CARRY4(uVar14 + local_108,uVar50);
  uVar26 = (uVar7 ^ uVar53) >> 0x18 | uVar21 << 8;
  uVar14 = uVar21 >> 0x18 | (uVar7 ^ uVar53) << 8;
  uVar41 = uVar29 + local_100 + uVar26;
  uVar45 = uVar10 + iStack_fc + (uint)CARRY4(uVar29,local_100) + uVar14 +
           (uint)CARRY4(uVar29 + local_100,uVar26);
  uVar10 = uVar20 + uVar27;
  uVar18 = (uVar31 ^ uVar51) + uVar28 + (uint)CARRY4(uVar20,uVar27);
  uVar11 = uVar45 ^ uVar29 ^ uVar52;
  uVar12 = uVar18 ^ uVar12;
  uVar27 = (uVar41 ^ uVar44) >> 0x10 | uVar11 << 0x10;
  uVar21 = uVar11 >> 0x10 | (uVar41 ^ uVar44) << 0x10;
  uVar11 = uVar27 + uVar7;
  uVar7 = uVar21 + uVar25 + (uint)CARRY4(uVar27,uVar7);
  uVar38 = (uVar10 ^ uVar39) >> 0x18 | uVar12 << 8;
  uVar36 = uVar12 >> 0x18 | (uVar10 ^ uVar39) << 8;
  uVar14 = uVar7 ^ uVar14;
  uVar29 = uVar31 + local_120[2] + uVar38;
  uVar12 = (uVar11 ^ uVar26) << 1 | uVar14 >> 0x1f;
  uVar32 = uVar32 + local_120[3] + (uint)CARRY4(uVar31,local_120[2]) + uVar36 +
           (uint)CARRY4(uVar31 + local_120[2],uVar38);
  uVar26 = uVar14 << 1 | (uVar11 ^ uVar26) >> 0x1f;
  uVar25 = uVar32 ^ uVar31 ^ uVar51;
  uVar14 = uVar13 + local_e8 + uVar12;
  uVar44 = uVar15 + iStack_e4 + (uint)CARRY4(uVar13,local_e8) + uVar26 +
           (uint)CARRY4(uVar13 + local_e8,uVar12);
  uVar13 = (uVar29 ^ uVar20) >> 0x10 | uVar25 << 0x10;
  uVar24 = uVar44 ^ uVar24;
  uVar15 = uVar25 >> 0x10 | (uVar29 ^ uVar20) << 0x10;
  uVar28 = uVar29 + local_b8 + uVar54;
  uVar35 = uVar32 + iStack_b4 + (uint)CARRY4(uVar29,local_b8) + uVar23 +
           (uint)CARRY4(uVar29 + local_b8,uVar54);
  uVar21 = uVar35 ^ uVar21;
  uVar20 = uVar13 + uVar10;
  uVar25 = uVar15 + uVar18 + (uint)CARRY4(uVar13,uVar10);
  uVar15 = uVar15 ^ uVar43;
  uVar29 = uVar24 + uVar20;
  uVar33 = (uVar14 ^ uVar17) + uVar25 + (uint)CARRY4(uVar24,uVar20);
  uVar25 = uVar25 ^ uVar36;
  uVar47 = uVar15 + uVar11;
  uVar39 = (uVar13 ^ uVar37) + uVar7 + (uint)CARRY4(uVar15,uVar11);
  uVar26 = uVar33 ^ uVar26;
  uVar11 = (uVar29 ^ uVar12) >> 0x18 | uVar26 << 8;
  uVar31 = uVar21 + uVar9;
  uVar36 = (uVar28 ^ uVar27) + uVar48 + (uint)CARRY4(uVar21,uVar9);
  uVar9 = uVar26 >> 0x18 | (uVar29 ^ uVar12) << 8;
  uVar42 = (uVar20 ^ uVar38) << 1 | uVar25 >> 0x1f;
  uVar8 = uVar39 ^ uVar8;
  uVar48 = uVar25 << 1 | (uVar20 ^ uVar38) >> 0x1f;
  uVar23 = uVar36 ^ uVar23;
  uVar51 = (uVar47 ^ uVar50) >> 0x18 | uVar8 << 8;
  uVar38 = uVar14 + local_d8 + uVar11;
  uVar40 = uVar44 + iStack_d4 + (uint)CARRY4(uVar14,local_d8) + uVar9 +
           (uint)CARRY4(uVar14 + local_d8,uVar11);
  uVar44 = uVar41 + local_c8 + uVar42;
  uVar26 = uVar45 + iStack_c4 + (uint)CARRY4(uVar41,local_c8) + uVar48 +
           (uint)CARRY4(uVar41 + local_c8,uVar42);
  uVar25 = uVar40 ^ uVar14 ^ uVar17;
  uVar45 = (uVar31 ^ uVar54) >> 0x18 | uVar23 << 8;
  uVar12 = uVar8 >> 0x18 | (uVar47 ^ uVar50) << 8;
  uVar10 = uVar23 >> 0x18 | (uVar31 ^ uVar54) << 8;
  uVar22 = uVar26 ^ uVar22;
  uVar50 = (uVar38 ^ uVar24) >> 0x10 | uVar25 << 0x10;
  uVar7 = uVar37 + local_120[2] + uVar51;
  uVar20 = uVar43 + local_120[3] + (uint)CARRY4(uVar37,local_120[2]) + uVar12 +
           (uint)CARRY4(uVar37 + local_120[2],uVar51);
  uVar32 = uVar28 + local_c0 + uVar45;
  uVar35 = uVar35 + iStack_bc + (uint)CARRY4(uVar28,local_c0) + uVar10 +
           (uint)CARRY4(uVar28 + local_c0,uVar45);
  uVar18 = uVar25 >> 0x10 | (uVar38 ^ uVar24) << 0x10;
  uVar14 = uVar20 ^ uVar13 ^ uVar37;
  uVar23 = uVar22 + uVar16;
  uVar37 = (uVar44 ^ uVar49) + uVar19 + (uint)CARRY4(uVar22,uVar16);
  uVar25 = uVar35 ^ uVar28 ^ uVar27;
  uVar16 = uVar50 + uVar29;
  uVar43 = (uVar7 ^ uVar15) >> 0x10 | uVar14 << 0x10;
  uVar17 = uVar18 + uVar33 + (uint)CARRY4(uVar50,uVar29);
  uVar9 = uVar17 ^ uVar9;
  uVar41 = (uVar32 ^ uVar21) >> 0x10 | uVar25 << 0x10;
  uVar48 = uVar37 ^ uVar48;
  uVar52 = (uVar16 ^ uVar11) << 1 | uVar9 >> 0x1f;
  uVar13 = uVar9 << 1 | (uVar16 ^ uVar11) >> 0x1f;
  uVar8 = uVar25 >> 0x10 | (uVar32 ^ uVar21) << 0x10;
  uVar29 = uVar14 >> 0x10 | (uVar7 ^ uVar15) << 0x10;
  uVar25 = (uVar23 ^ uVar42) >> 0x18 | uVar48 << 8;
  uVar9 = uVar48 >> 0x18 | (uVar23 ^ uVar42) << 8;
  uVar11 = uVar43 + uVar47;
  uVar21 = uVar29 + uVar39 + (uint)CARRY4(uVar43,uVar47);
  uVar28 = uVar41 + uVar31;
  uVar33 = uVar8 + uVar36 + (uint)CARRY4(uVar41,uVar31);
  uVar19 = uVar44 + local_b0 + uVar25;
  uVar14 = uVar26 + iStack_ac + (uint)CARRY4(uVar44,local_b0) + uVar9 +
           (uint)CARRY4(uVar44 + local_b0,uVar25);
  uVar12 = uVar21 ^ uVar12;
  uVar10 = uVar33 ^ uVar10;
  uVar39 = (uVar28 ^ uVar45) << 1 | uVar10 >> 0x1f;
  uVar44 = uVar14 ^ uVar44 ^ uVar49;
  uVar48 = uVar19 + local_a8 + uVar52;
  uVar42 = (uVar19 ^ uVar22) >> 0x10 | uVar44 << 0x10;
  uVar47 = uVar14 + iStack_a4 + (uint)CARRY4(uVar19,local_a8) + uVar13 +
           (uint)CARRY4(uVar19 + local_a8,uVar52);
  uVar14 = uVar10 << 1 | (uVar28 ^ uVar45) >> 0x1f;
  uVar24 = uVar44 >> 0x10 | (uVar19 ^ uVar22) << 0x10;
  uVar31 = (uVar11 ^ uVar51) << 1 | uVar12 >> 0x1f;
  uVar15 = uVar12 << 1 | (uVar11 ^ uVar51) >> 0x1f;
  uVar10 = uVar42 + uVar23;
  uVar8 = uVar47 ^ uVar8;
  uVar19 = uVar24 + uVar37 + (uint)CARRY4(uVar42,uVar23);
  uVar26 = uVar8 + uVar11;
  uVar27 = (uVar48 ^ uVar41) + uVar21 + (uint)CARRY4(uVar8,uVar11);
  uVar9 = uVar19 ^ uVar9;
  uVar44 = uVar7 + local_f8 + uVar39;
  uVar22 = uVar20 + iStack_f4 + (uint)CARRY4(uVar7,local_f8) + uVar14 +
           (uint)CARRY4(uVar7 + local_f8,uVar39);
  uVar21 = (uVar10 ^ uVar25) << 1 | uVar9 >> 0x1f;
  uVar23 = uVar9 << 1 | (uVar10 ^ uVar25) >> 0x1f;
  uVar13 = uVar27 ^ uVar13;
  uVar11 = uVar38 + local_120[4] + uVar31;
  uVar25 = uVar40 + iStack_10c + (uint)CARRY4(uVar38,local_120[4]) + uVar15 +
           (uint)CARRY4(uVar38 + local_120[4],uVar31);
  uVar36 = uVar32 + local_100 + uVar21;
  uVar37 = uVar35 + iStack_fc + (uint)CARRY4(uVar32,local_100) + uVar23 +
           (uint)CARRY4(uVar32 + local_100,uVar21);
  uVar32 = (uVar26 ^ uVar52) >> 0x18 | uVar13 << 8;
  uVar18 = uVar22 ^ uVar18;
  uVar35 = uVar13 >> 0x18 | (uVar26 ^ uVar52) << 8;
  uVar7 = uVar48 + local_e0 + uVar32;
  uVar9 = uVar47 + iStack_dc + (uint)CARRY4(uVar48,local_e0) + uVar35 +
          (uint)CARRY4(uVar48 + local_e0,uVar32);
  uVar24 = uVar25 ^ uVar24;
  uVar29 = uVar37 ^ uVar29;
  uVar20 = uVar9 ^ uVar48 ^ uVar41;
  uVar12 = uVar18 + uVar10;
  uVar10 = (uVar44 ^ uVar50) + uVar19 + (uint)CARRY4(uVar18,uVar10);
  uVar13 = (uVar7 ^ uVar8) >> 0x10 | uVar20 << 0x10;
  uVar48 = uVar20 >> 0x10 | (uVar7 ^ uVar8) << 0x10;
  uVar20 = uVar24 + uVar28;
  uVar8 = (uVar11 ^ uVar42) + uVar33 + (uint)CARRY4(uVar24,uVar28);
  uVar14 = uVar10 ^ uVar14;
  uVar19 = uVar13 + uVar26;
  uVar26 = uVar48 + uVar27 + (uint)CARRY4(uVar13,uVar26);
  uVar15 = uVar8 ^ uVar15;
  uVar38 = uVar29 + uVar16;
  uVar40 = (uVar36 ^ uVar43) + uVar17 + (uint)CARRY4(uVar29,uVar16);
  uVar35 = uVar35 ^ uVar26;
  uVar45 = (uVar32 ^ uVar19) << 1 | uVar35 >> 0x1f;
  uVar23 = uVar40 ^ uVar23;
  uVar33 = uVar35 << 1 | (uVar32 ^ uVar19) >> 0x1f;
  uVar41 = (uVar12 ^ uVar39) >> 0x18 | uVar14 << 8;
  uVar49 = (uVar20 ^ uVar31) >> 0x18 | uVar15 << 8;
  uVar35 = uVar15 >> 0x18 | (uVar20 ^ uVar31) << 8;
  uVar14 = uVar14 >> 0x18 | (uVar12 ^ uVar39) << 8;
  uVar16 = (uVar38 ^ uVar21) >> 0x18 | uVar23 << 8;
  uVar17 = uVar23 >> 0x18 | (uVar38 ^ uVar21) << 8;
  uVar32 = uVar11 + local_f0 + uVar49;
  uVar21 = uVar25 + iStack_ec + (uint)CARRY4(uVar11,local_f0) + uVar35 +
           (uint)CARRY4(uVar11 + local_f0,uVar49);
  uVar23 = uVar44 + local_d0 + uVar41;
  uVar31 = uVar22 + iStack_cc + (uint)CARRY4(uVar44,local_d0) + uVar14 +
           (uint)CARRY4(uVar44 + local_d0,uVar41);
  uVar28 = uVar21 ^ uVar11 ^ uVar42;
  uVar11 = uVar36 + local_120[0] + uVar16;
  uVar25 = uVar37 + local_120[1] + (uint)CARRY4(uVar36,local_120[0]) + uVar17 +
           (uint)CARRY4(uVar36 + local_120[0],uVar16);
  uVar37 = uVar32 + local_d8 + uVar45;
  uVar27 = uVar31 ^ uVar44 ^ uVar50;
  uVar47 = uVar21 + iStack_d4 + (uint)CARRY4(uVar32,local_d8) + uVar33 +
           (uint)CARRY4(uVar32 + local_d8,uVar45);
  uVar22 = (uVar32 ^ uVar24) >> 0x10 | uVar28 << 0x10;
  uVar21 = uVar25 ^ uVar36 ^ uVar43;
  uVar15 = (uVar23 ^ uVar18) >> 0x10 | uVar27 << 0x10;
  uVar44 = uVar28 >> 0x10 | (uVar32 ^ uVar24) << 0x10;
  uVar18 = uVar27 >> 0x10 | (uVar23 ^ uVar18) << 0x10;
  uVar43 = (uVar11 ^ uVar29) >> 0x10 | uVar21 << 0x10;
  uVar21 = uVar21 >> 0x10 | (uVar11 ^ uVar29) << 0x10;
  uVar27 = uVar22 + uVar20;
  uVar32 = uVar44 + uVar8 + (uint)CARRY4(uVar22,uVar20);
  uVar39 = uVar15 + uVar12;
  uVar42 = uVar18 + uVar10 + (uint)CARRY4(uVar15,uVar12);
  uVar18 = uVar18 ^ uVar47;
  uVar12 = uVar43 + uVar38;
  uVar20 = uVar21 + uVar40 + (uint)CARRY4(uVar43,uVar38);
  uVar35 = uVar32 ^ uVar35;
  uVar28 = uVar18 + uVar12;
  uVar38 = (uVar15 ^ uVar37) + uVar20 + (uint)CARRY4(uVar18,uVar12);
  uVar14 = uVar42 ^ uVar14;
  uVar50 = (uVar27 ^ uVar49) << 1 | uVar35 >> 0x1f;
  uVar33 = uVar38 ^ uVar33;
  uVar36 = uVar35 << 1 | (uVar27 ^ uVar49) >> 0x1f;
  uVar40 = (uVar39 ^ uVar41) << 1 | uVar14 >> 0x1f;
  uVar24 = uVar33 >> 0x18 | (uVar28 ^ uVar45) << 8;
  uVar20 = uVar20 ^ uVar17;
  uVar10 = uVar14 << 1 | (uVar39 ^ uVar41) >> 0x1f;
  uVar8 = (uVar28 ^ uVar45) >> 0x18 | uVar33 << 8;
  uVar17 = uVar37 + local_120[0] + uVar8;
  uVar35 = uVar47 + local_120[1] + (uint)CARRY4(uVar37,local_120[0]) + uVar24 +
           (uint)CARRY4(uVar37 + local_120[0],uVar8);
  uVar15 = uVar35 ^ uVar15 ^ uVar37;
  uVar45 = (uVar12 ^ uVar16) << 1 | uVar20 >> 0x1f;
  uVar14 = uVar20 << 1 | (uVar12 ^ uVar16) >> 0x1f;
  uVar37 = (uVar17 ^ uVar18) >> 0x10 | uVar15 << 0x10;
  uVar18 = uVar15 >> 0x10 | (uVar17 ^ uVar18) << 0x10;
  uVar29 = uVar37 + uVar28;
  uVar33 = uVar18 + uVar38 + (uint)CARRY4(uVar37,uVar28);
  uVar28 = uVar23 + local_f8 + uVar50;
  uVar23 = uVar31 + iStack_f4 + (uint)CARRY4(uVar23,local_f8) + uVar36 +
           (uint)CARRY4(uVar23 + local_f8,uVar50);
  uVar12 = uVar11 + local_120[4] + uVar40;
  uVar20 = uVar25 + iStack_10c + (uint)CARRY4(uVar11,local_120[4]) + uVar10 +
           (uint)CARRY4(uVar11 + local_120[4],uVar40);
  uVar24 = uVar24 ^ uVar33;
  uVar21 = uVar23 ^ uVar21;
  uVar49 = (uVar8 ^ uVar29) << 1 | uVar24 >> 0x1f;
  uVar15 = uVar24 << 1 | (uVar8 ^ uVar29) >> 0x1f;
  uVar48 = uVar20 ^ uVar48;
  uVar11 = uVar7 + local_d0 + uVar45;
  uVar25 = uVar9 + iStack_cc + (uint)CARRY4(uVar7,local_d0) + uVar14 +
           (uint)CARRY4(uVar7 + local_d0,uVar45);
  uVar7 = uVar21 + uVar19;
  uVar8 = (uVar28 ^ uVar43) + uVar26 + (uint)CARRY4(uVar21,uVar19);
  uVar19 = uVar48 + uVar27;
  uVar31 = (uVar12 ^ uVar13) + uVar32 + (uint)CARRY4(uVar48,uVar27);
  uVar44 = uVar25 ^ uVar44;
  uVar36 = uVar8 ^ uVar36;
  uVar10 = uVar31 ^ uVar10;
  uVar32 = (uVar7 ^ uVar50) >> 0x18 | uVar36 << 8;
  uVar16 = uVar36 >> 0x18 | (uVar7 ^ uVar50) << 8;
  uVar27 = (uVar19 ^ uVar40) >> 0x18 | uVar10 << 8;
  uVar9 = uVar10 >> 0x18 | (uVar19 ^ uVar40) << 8;
  uVar36 = uVar28 + local_e8 + uVar32;
  uVar40 = uVar23 + iStack_e4 + (uint)CARRY4(uVar28,local_e8) + uVar16 +
           (uint)CARRY4(uVar28 + local_e8,uVar32);
  uVar24 = uVar12 + local_100 + uVar27;
  uVar26 = uVar20 + iStack_fc + (uint)CARRY4(uVar12,local_100) + uVar9 +
           (uint)CARRY4(uVar12 + local_100,uVar27);
  uVar20 = uVar40 ^ uVar28 ^ uVar43;
  uVar10 = uVar26 ^ uVar12 ^ uVar13;
  uVar50 = (uVar36 ^ uVar21) >> 0x10 | uVar20 << 0x10;
  uVar12 = uVar20 >> 0x10 | (uVar36 ^ uVar21) << 0x10;
  uVar21 = uVar44 + uVar39;
  uVar23 = (uVar11 ^ uVar22) + uVar42 + (uint)CARRY4(uVar44,uVar39);
  uVar14 = uVar23 ^ uVar14;
  uVar51 = (uVar24 ^ uVar48) >> 0x10 | uVar10 << 0x10;
  uVar47 = uVar50 + uVar7;
  uVar10 = uVar10 >> 0x10 | (uVar24 ^ uVar48) << 0x10;
  uVar41 = uVar12 + uVar8 + (uint)CARRY4(uVar50,uVar7);
  uVar39 = (uVar21 ^ uVar45) >> 0x18 | uVar14 << 8;
  uVar16 = uVar41 ^ uVar16;
  uVar20 = uVar14 >> 0x18 | (uVar21 ^ uVar45) << 8;
  uVar43 = (uVar47 ^ uVar32) << 1 | uVar16 >> 0x1f;
  uVar13 = uVar16 << 1 | (uVar47 ^ uVar32) >> 0x1f;
  uVar48 = uVar51 + uVar19;
  uVar32 = uVar10 + uVar31 + (uint)CARRY4(uVar51,uVar19);
  uVar9 = uVar32 ^ uVar9;
  uVar38 = uVar11 + local_a8 + uVar39;
  uVar19 = (uVar48 ^ uVar27) << 1 | uVar9 >> 0x1f;
  uVar42 = uVar25 + iStack_a4 + (uint)CARRY4(uVar11,local_a8) + uVar20 +
           (uint)CARRY4(uVar11 + local_a8,uVar39);
  uVar7 = uVar42 ^ uVar11 ^ uVar22;
  uVar16 = uVar9 << 1 | (uVar48 ^ uVar27) >> 0x1f;
  uVar28 = uVar17 + local_b0 + uVar43;
  uVar22 = uVar35 + iStack_ac + (uint)CARRY4(uVar17,local_b0) + uVar13 +
           (uint)CARRY4(uVar17 + local_b0,uVar43);
  uVar31 = (uVar38 ^ uVar44) >> 0x10 | uVar7 << 0x10;
  uVar11 = uVar36 + local_c8 + uVar19;
  uVar14 = uVar7 >> 0x10 | (uVar38 ^ uVar44) << 0x10;
  uVar25 = uVar40 + iStack_c4 + (uint)CARRY4(uVar36,local_c8) + uVar16 +
           (uint)CARRY4(uVar36 + local_c8,uVar19);
  uVar8 = uVar31 + uVar21;
  uVar9 = uVar14 + uVar23 + (uint)CARRY4(uVar31,uVar21);
  uVar14 = uVar22 ^ uVar14;
  uVar18 = uVar25 ^ uVar18;
  uVar7 = uVar14 + uVar48;
  uVar21 = (uVar28 ^ uVar31) + uVar32 + (uint)CARRY4(uVar14,uVar48);
  uVar20 = uVar9 ^ uVar20;
  uVar40 = (uVar8 ^ uVar39) << 1 | uVar20 >> 0x1f;
  uVar13 = uVar21 ^ uVar13;
  uVar32 = uVar18 + uVar8;
  uVar44 = uVar20 << 1 | (uVar8 ^ uVar39) >> 0x1f;
  uVar36 = (uVar11 ^ uVar37) + uVar9 + (uint)CARRY4(uVar18,uVar8);
  uVar35 = (uVar7 ^ uVar43) >> 0x18 | uVar13 << 8;
  uVar16 = uVar36 ^ uVar16;
  uVar8 = uVar13 >> 0x18 | (uVar7 ^ uVar43) << 8;
  uVar27 = (uVar32 ^ uVar19) >> 0x18 | uVar16 << 8;
  uVar23 = uVar16 >> 0x18 | (uVar32 ^ uVar19) << 8;
  uVar19 = uVar28 + local_120[2] + uVar35;
  uVar22 = uVar22 + local_120[3] + (uint)CARRY4(uVar28,local_120[2]) + uVar8 +
           (uint)CARRY4(uVar28 + local_120[2],uVar35);
  uVar17 = uVar24 + local_f0 + uVar40;
  uVar48 = uVar26 + iStack_ec + (uint)CARRY4(uVar24,local_f0) + uVar44 +
           (uint)CARRY4(uVar24 + local_f0,uVar40);
  uVar9 = uVar11 + local_c0 + uVar27;
  uVar13 = uVar25 + iStack_bc + (uint)CARRY4(uVar11,local_c0) + uVar23 +
           (uint)CARRY4(uVar11 + local_c0,uVar27);
  uVar20 = uVar22 ^ uVar28 ^ uVar31;
  uVar45 = (uVar19 ^ uVar14) >> 0x10 | uVar20 << 0x10;
  uVar12 = uVar48 ^ uVar12;
  uVar16 = uVar20 >> 0x10 | (uVar19 ^ uVar14) << 0x10;
  uVar14 = uVar13 ^ uVar11 ^ uVar37;
  uVar11 = uVar45 + uVar7;
  uVar20 = uVar16 + uVar21 + (uint)CARRY4(uVar45,uVar7);
  uVar25 = (uVar9 ^ uVar18) >> 0x10 | uVar14 << 0x10;
  uVar24 = uVar14 >> 0x10 | (uVar9 ^ uVar18) << 0x10;
  uVar7 = uVar12 + uVar29;
  uVar21 = (uVar17 ^ uVar50) + uVar33 + (uint)CARRY4(uVar12,uVar29);
  uVar8 = uVar20 ^ uVar8;
  uVar39 = uVar25 + uVar32;
  uVar43 = uVar24 + uVar36 + (uint)CARRY4(uVar25,uVar32);
  uVar44 = uVar21 ^ uVar44;
  uVar52 = (uVar11 ^ uVar35) << 1 | uVar8 >> 0x1f;
  uVar8 = uVar8 << 1 | (uVar11 ^ uVar35) >> 0x1f;
  uVar23 = uVar43 ^ uVar23;
  uVar18 = uVar38 + local_108 + uVar49;
  uVar35 = uVar42 + iStack_104 + (uint)CARRY4(uVar38,local_108) + uVar15 +
           (uint)CARRY4(uVar38 + local_108,uVar49);
  uVar42 = (uVar39 ^ uVar27) << 1 | uVar23 >> 0x1f;
  uVar10 = uVar35 ^ uVar10;
  uVar32 = uVar23 << 1 | (uVar39 ^ uVar27) >> 0x1f;
  uVar29 = uVar10 + uVar47;
  uVar38 = (uVar7 ^ uVar40) >> 0x18 | uVar44 << 8;
  uVar33 = (uVar18 ^ uVar51) + uVar41 + (uint)CARRY4(uVar10,uVar47);
  uVar27 = uVar9 + local_f0 + uVar52;
  uVar28 = uVar13 + iStack_ec + (uint)CARRY4(uVar9,local_f0) + uVar8 +
           (uint)CARRY4(uVar9 + local_f0,uVar52);
  uVar44 = uVar44 >> 0x18 | (uVar7 ^ uVar40) << 8;
  uVar15 = uVar33 ^ uVar15;
  uVar47 = (uVar29 ^ uVar49) >> 0x18 | uVar15 << 8;
  uVar14 = uVar15 >> 0x18 | (uVar29 ^ uVar49) << 8;
  uVar31 = uVar17 + local_e0 + uVar38;
  uVar15 = uVar48 + iStack_dc + (uint)CARRY4(uVar17,local_e0) + uVar44 +
           (uint)CARRY4(uVar17 + local_e0,uVar38);
  uVar26 = uVar18 + local_b8 + uVar47;
  uVar23 = uVar35 + iStack_b4 + (uint)CARRY4(uVar18,local_b8) + uVar14 +
           (uint)CARRY4(uVar18 + local_b8,uVar47);
  uVar9 = uVar15 ^ uVar17 ^ uVar50;
  uVar13 = uVar23 ^ uVar18 ^ uVar51;
  uVar35 = uVar31 + local_120[0] + uVar42;
  uVar37 = (uVar31 ^ uVar12) >> 0x10 | uVar9 << 0x10;
  uVar36 = uVar15 + local_120[1] + (uint)CARRY4(uVar31,local_120[0]) + uVar32 +
           (uint)CARRY4(uVar31 + local_120[0],uVar42);
  uVar15 = (uVar26 ^ uVar10) >> 0x10 | uVar13 << 0x10;
  uVar10 = uVar13 >> 0x10 | (uVar26 ^ uVar10) << 0x10;
  uVar13 = uVar9 >> 0x10 | (uVar31 ^ uVar12) << 0x10;
  uVar12 = uVar15 + uVar29;
  uVar9 = uVar10 + uVar33 + (uint)CARRY4(uVar15,uVar29);
  uVar18 = uVar37 + uVar7;
  uVar10 = uVar36 ^ uVar10;
  uVar17 = uVar13 + uVar21 + (uint)CARRY4(uVar37,uVar7);
  uVar13 = uVar13 ^ uVar28;
  uVar14 = uVar9 ^ uVar14;
  uVar44 = uVar17 ^ uVar44;
  uVar33 = (uVar12 ^ uVar47) << 1 | uVar14 >> 0x1f;
  uVar48 = uVar14 << 1 | (uVar12 ^ uVar47) >> 0x1f;
  uVar21 = (uVar18 ^ uVar38) << 1 | uVar44 >> 0x1f;
  uVar29 = uVar19 + local_120[4] + uVar33;
  uVar44 = uVar44 << 1 | (uVar18 ^ uVar38) >> 0x1f;
  uVar31 = uVar22 + iStack_10c + (uint)CARRY4(uVar19,local_120[4]) + uVar48 +
           (uint)CARRY4(uVar19 + local_120[4],uVar33);
  uVar24 = uVar31 ^ uVar24;
  uVar7 = uVar26 + local_e0 + uVar21;
  uVar14 = uVar23 + iStack_dc + (uint)CARRY4(uVar26,local_e0) + uVar44 +
           (uint)CARRY4(uVar26 + local_e0,uVar21);
  uVar19 = uVar24 + uVar18;
  uVar17 = (uVar29 ^ uVar25) + uVar17 + (uint)CARRY4(uVar24,uVar18);
  uVar16 = uVar14 ^ uVar16;
  uVar48 = uVar17 ^ uVar48;
  uVar49 = (uVar19 ^ uVar33) >> 0x18 | uVar48 << 8;
  uVar23 = uVar48 >> 0x18 | (uVar19 ^ uVar33) << 8;
  uVar18 = uVar13 + uVar12;
  uVar48 = (uVar37 ^ uVar27) + uVar9 + (uint)CARRY4(uVar13,uVar12);
  uVar33 = uVar10 + uVar11;
  uVar38 = (uVar35 ^ uVar15) + uVar20 + (uint)CARRY4(uVar10,uVar11);
  uVar22 = uVar29 + local_c0 + uVar49;
  uVar26 = uVar31 + iStack_bc + (uint)CARRY4(uVar29,local_c0) + uVar23 +
           (uint)CARRY4(uVar29 + local_c0,uVar49);
  uVar40 = uVar16 + uVar39;
  uVar41 = (uVar7 ^ uVar45) + uVar43 + (uint)CARRY4(uVar16,uVar39);
  uVar11 = uVar29 ^ uVar25 ^ uVar26;
  uVar8 = uVar48 ^ uVar8;
  uVar43 = (uVar24 ^ uVar22) >> 0x10 | uVar11 << 0x10;
  uVar32 = uVar38 ^ uVar32;
  uVar24 = uVar11 >> 0x10 | (uVar24 ^ uVar22) << 0x10;
  uVar44 = uVar41 ^ uVar44;
  uVar50 = (uVar18 ^ uVar52) >> 0x18 | uVar8 << 8;
  uVar8 = uVar8 >> 0x18 | (uVar18 ^ uVar52) << 8;
  uVar29 = uVar43 + uVar19;
  uVar31 = uVar24 + uVar17 + (uint)CARRY4(uVar43,uVar19);
  uVar23 = uVar31 ^ uVar23;
  uVar39 = (uVar33 ^ uVar42) >> 0x18 | uVar32 << 8;
  uVar47 = uVar27 + local_d0 + uVar50;
  uVar51 = (uVar40 ^ uVar21) >> 0x18 | uVar44 << 8;
  uVar28 = uVar28 + iStack_cc + (uint)CARRY4(uVar27,local_d0) + uVar8 +
           (uint)CARRY4(uVar27 + local_d0,uVar50);
  uVar17 = uVar32 >> 0x18 | (uVar33 ^ uVar42) << 8;
  uVar32 = uVar35 + local_c8 + uVar39;
  uVar12 = uVar44 >> 0x18 | (uVar40 ^ uVar21) << 8;
  uVar36 = uVar36 + iStack_c4 + (uint)CARRY4(uVar35,local_c8) + uVar17 +
           (uint)CARRY4(uVar35 + local_c8,uVar39);
  uVar25 = uVar28 ^ uVar37 ^ uVar27;
  uVar20 = (uVar29 ^ uVar49) << 1 | uVar23 >> 0x1f;
  uVar44 = uVar7 + local_108 + uVar51;
  uVar27 = uVar14 + iStack_104 + (uint)CARRY4(uVar7,local_108) + uVar12 +
           (uint)CARRY4(uVar7 + local_108,uVar51);
  uVar21 = uVar23 << 1 | (uVar29 ^ uVar49) >> 0x1f;
  uVar11 = uVar36 ^ uVar35 ^ uVar15;
  uVar49 = (uVar47 ^ uVar13) >> 0x10 | uVar25 << 0x10;
  uVar9 = uVar25 >> 0x10 | (uVar47 ^ uVar13) << 0x10;
  uVar25 = uVar27 ^ uVar7 ^ uVar45;
  uVar35 = (uVar32 ^ uVar10) >> 0x10 | uVar11 << 0x10;
  uVar15 = uVar11 >> 0x10 | (uVar32 ^ uVar10) << 0x10;
  uVar7 = (uVar44 ^ uVar16) >> 0x10 | uVar25 << 0x10;
  uVar10 = uVar25 >> 0x10 | (uVar44 ^ uVar16) << 0x10;
  uVar13 = uVar49 + uVar18;
  uVar14 = uVar9 + uVar48 + (uint)CARRY4(uVar49,uVar18);
  uVar11 = uVar35 + uVar33;
  uVar25 = uVar15 + uVar38 + (uint)CARRY4(uVar35,uVar33);
  uVar8 = uVar14 ^ uVar8;
  uVar16 = uVar7 + uVar40;
  uVar19 = uVar10 + uVar41 + (uint)CARRY4(uVar7,uVar40);
  uVar17 = uVar25 ^ uVar17;
  uVar40 = (uVar11 ^ uVar39) << 1 | uVar17 >> 0x1f;
  uVar12 = uVar19 ^ uVar12;
  uVar18 = uVar44 + local_120[2] + uVar20;
  uVar48 = uVar17 << 1 | (uVar11 ^ uVar39) >> 0x1f;
  uVar17 = uVar27 + local_120[3] + (uint)CARRY4(uVar44,local_120[2]) + uVar21 +
           (uint)CARRY4(uVar44 + local_120[2],uVar20);
  uVar15 = uVar17 ^ uVar15;
  uVar42 = (uVar13 ^ uVar50) << 1 | uVar8 >> 0x1f;
  uVar33 = uVar47 + local_e8 + uVar40;
  uVar47 = uVar28 + iStack_e4 + (uint)CARRY4(uVar47,local_e8) + uVar48 +
           (uint)CARRY4(uVar47 + local_e8,uVar40);
  uVar8 = uVar8 << 1 | (uVar13 ^ uVar50) >> 0x1f;
  uVar44 = uVar15 + uVar13;
  uVar13 = (uVar18 ^ uVar35) + uVar14 + (uint)CARRY4(uVar15,uVar13);
  uVar45 = (uVar16 ^ uVar51) << 1 | uVar12 >> 0x1f;
  uVar23 = uVar22 + local_100 + uVar42;
  uVar14 = uVar12 << 1 | (uVar16 ^ uVar51) >> 0x1f;
  uVar28 = uVar26 + iStack_fc + (uint)CARRY4(uVar22,local_100) + uVar8 +
           (uint)CARRY4(uVar22 + local_100,uVar42);
  uVar21 = uVar13 ^ uVar21;
  uVar22 = uVar32 + local_a8 + uVar45;
  uVar32 = uVar36 + iStack_a4 + (uint)CARRY4(uVar32,local_a8) + uVar14 +
           (uint)CARRY4(uVar32 + local_a8,uVar45);
  uVar24 = uVar47 ^ uVar24;
  uVar27 = (uVar44 ^ uVar20) >> 0x18 | uVar21 << 8;
  uVar10 = uVar28 ^ uVar10;
  uVar20 = uVar21 >> 0x18 | (uVar44 ^ uVar20) << 8;
  uVar36 = uVar18 + local_d8 + uVar27;
  uVar9 = uVar32 ^ uVar9;
  uVar39 = uVar17 + iStack_d4 + (uint)CARRY4(uVar18,local_d8) + uVar20 +
           (uint)CARRY4(uVar18 + local_d8,uVar27);
  uVar12 = uVar39 ^ uVar18 ^ uVar35;
  uVar50 = (uVar36 ^ uVar15) >> 0x10 | uVar12 << 0x10;
  uVar38 = uVar24 + uVar16;
  uVar41 = (uVar33 ^ uVar43) + uVar19 + (uint)CARRY4(uVar24,uVar16);
  uVar15 = uVar12 >> 0x10 | (uVar36 ^ uVar15) << 0x10;
  uVar17 = uVar10 + uVar11;
  uVar26 = (uVar23 ^ uVar7) + uVar25 + (uint)CARRY4(uVar10,uVar11);
  uVar48 = uVar41 ^ uVar48;
  uVar37 = uVar9 + uVar29;
  uVar29 = (uVar22 ^ uVar49) + uVar31 + (uint)CARRY4(uVar9,uVar29);
  uVar8 = uVar26 ^ uVar8;
  uVar51 = (uVar38 ^ uVar40) >> 0x18 | uVar48 << 8;
  uVar16 = uVar50 + uVar44;
  uVar18 = uVar15 + uVar13 + (uint)CARRY4(uVar50,uVar44);
  uVar14 = uVar29 ^ uVar14;
  uVar20 = uVar20 ^ uVar18;
  uVar52 = (uVar27 ^ uVar16) << 1 | uVar20 >> 0x1f;
  uVar13 = uVar20 << 1 | (uVar27 ^ uVar16) >> 0x1f;
  uVar53 = (uVar17 ^ uVar42) >> 0x18 | uVar8 << 8;
  uVar12 = uVar8 >> 0x18 | (uVar17 ^ uVar42) << 8;
  uVar20 = uVar48 >> 0x18 | (uVar38 ^ uVar40) << 8;
  uVar25 = (uVar37 ^ uVar45) >> 0x18 | uVar14 << 8;
  uVar19 = uVar14 >> 0x18 | (uVar37 ^ uVar45) << 8;
  uVar8 = uVar23 + local_b8 + uVar53;
  uVar14 = uVar28 + iStack_b4 + (uint)CARRY4(uVar23,local_b8) + uVar12 +
           (uint)CARRY4(uVar23 + local_b8,uVar53);
  uVar40 = uVar33 + local_f8 + uVar51;
  uVar42 = uVar47 + iStack_f4 + (uint)CARRY4(uVar33,local_f8) + uVar20 +
           (uint)CARRY4(uVar33 + local_f8,uVar51);
  uVar7 = uVar14 ^ uVar23 ^ uVar7;
  uVar11 = uVar22 + local_b0 + uVar25;
  uVar21 = uVar32 + iStack_ac + (uint)CARRY4(uVar22,local_b0) + uVar19 +
           (uint)CARRY4(uVar22 + local_b0,uVar25);
  uVar35 = uVar7 >> 0x10 | (uVar8 ^ uVar10) << 0x10;
  uVar48 = uVar8 + local_c0 + uVar52;
  uVar27 = uVar14 + iStack_bc + (uint)CARRY4(uVar8,local_c0) + uVar13 +
           (uint)CARRY4(uVar8 + local_c0,uVar52);
  uVar14 = uVar42 ^ uVar33 ^ uVar43;
  uVar8 = (uVar8 ^ uVar10) >> 0x10 | uVar7 << 0x10;
  uVar44 = uVar21 ^ uVar22 ^ uVar49;
  uVar7 = (uVar40 ^ uVar24) >> 0x10 | uVar14 << 0x10;
  uVar10 = uVar14 >> 0x10 | (uVar40 ^ uVar24) << 0x10;
  uVar22 = (uVar11 ^ uVar9) >> 0x10 | uVar44 << 0x10;
  uVar14 = uVar44 >> 0x10 | (uVar11 ^ uVar9) << 0x10;
  uVar44 = uVar8 + uVar17;
  uVar17 = uVar35 + uVar26 + (uint)CARRY4(uVar8,uVar17);
  uVar24 = uVar27 ^ uVar10;
  uVar32 = uVar22 + uVar37;
  uVar43 = uVar14 + uVar29 + (uint)CARRY4(uVar22,uVar37);
  uVar12 = uVar17 ^ uVar12;
  uVar29 = uVar7 + uVar38;
  uVar37 = uVar10 + uVar41 + (uint)CARRY4(uVar7,uVar38);
  uVar9 = (uVar44 ^ uVar53) << 1 | uVar12 >> 0x1f;
  uVar28 = uVar24 + uVar32;
  uVar31 = (uVar48 ^ uVar7) + uVar43 + (uint)CARRY4(uVar24,uVar32);
  uVar26 = uVar12 << 1 | (uVar44 ^ uVar53) >> 0x1f;
  uVar20 = uVar37 ^ uVar20;
  uVar13 = uVar31 ^ uVar13;
  uVar47 = uVar40 + local_120[2] + uVar9;
  uVar40 = uVar42 + local_120[3] + (uint)CARRY4(uVar40,local_120[2]) + uVar26 +
           (uint)CARRY4(uVar40 + local_120[2],uVar9);
  uVar43 = uVar43 ^ uVar19;
  uVar23 = (uVar28 ^ uVar52) >> 0x18 | uVar13 << 8;
  uVar10 = uVar13 >> 0x18 | (uVar28 ^ uVar52) << 8;
  uVar14 = uVar40 ^ uVar14;
  uVar38 = (uVar29 ^ uVar51) << 1 | uVar20 >> 0x1f;
  uVar13 = uVar20 << 1 | (uVar29 ^ uVar51) >> 0x1f;
  uVar12 = uVar48 + local_f8 + uVar23;
  uVar45 = (uVar32 ^ uVar25) << 1 | uVar43 >> 0x1f;
  uVar20 = uVar27 + iStack_f4 + (uint)CARRY4(uVar48,local_f8) + uVar10 +
           (uint)CARRY4(uVar48 + local_f8,uVar23);
  uVar7 = uVar20 ^ uVar48 ^ uVar7;
  uVar33 = (uVar12 ^ uVar24) >> 0x10 | uVar7 << 0x10;
  uVar24 = uVar7 >> 0x10 | (uVar12 ^ uVar24) << 0x10;
  uVar19 = uVar33 + uVar28;
  uVar48 = uVar24 + uVar31 + (uint)CARRY4(uVar33,uVar28);
  uVar7 = uVar43 << 1 | (uVar32 ^ uVar25) >> 0x1f;
  uVar41 = uVar14 + uVar16;
  uVar42 = (uVar47 ^ uVar22) + uVar18 + (uint)CARRY4(uVar14,uVar16);
  uVar10 = uVar48 ^ uVar10;
  uVar18 = uVar11 + local_b0 + uVar38;
  uVar21 = uVar21 + iStack_ac + (uint)CARRY4(uVar11,local_b0) + uVar13 +
           (uint)CARRY4(uVar11 + local_b0,uVar38);
  uVar26 = uVar42 ^ uVar26;
  uVar43 = (uVar19 ^ uVar23) << 1 | uVar10 >> 0x1f;
  uVar32 = uVar36 + local_100 + uVar45;
  uVar25 = uVar10 << 1 | (uVar19 ^ uVar23) >> 0x1f;
  uVar31 = uVar39 + iStack_fc + (uint)CARRY4(uVar36,local_100) + uVar7 +
           (uint)CARRY4(uVar36 + local_100,uVar45);
  uVar15 = uVar21 ^ uVar15;
  uVar35 = uVar31 ^ uVar35;
  uVar23 = uVar35 + uVar29;
  uVar29 = (uVar32 ^ uVar8) + uVar37 + (uint)CARRY4(uVar35,uVar29);
  uVar27 = uVar15 + uVar44;
  uVar28 = (uVar18 ^ uVar50) + uVar17 + (uint)CARRY4(uVar15,uVar44);
  uVar7 = uVar29 ^ uVar7;
  uVar36 = (uVar41 ^ uVar9) >> 0x18 | uVar26 << 8;
  uVar13 = uVar28 ^ uVar13;
  uVar37 = uVar26 >> 0x18 | (uVar41 ^ uVar9) << 8;
  uVar10 = (uVar23 ^ uVar45) >> 0x18 | uVar7 << 8;
  uVar16 = (uVar27 ^ uVar38) >> 0x18 | uVar13 << 8;
  uVar39 = uVar47 + local_a8 + uVar36;
  uVar44 = uVar7 >> 0x18 | (uVar23 ^ uVar45) << 8;
  uVar40 = uVar40 + iStack_a4 + (uint)CARRY4(uVar47,local_a8) + uVar37 +
           (uint)CARRY4(uVar47 + local_a8,uVar36);
  uVar7 = uVar13 >> 0x18 | (uVar27 ^ uVar38) << 8;
  uVar11 = uVar40 ^ uVar47 ^ uVar22;
  uVar22 = uVar32 + local_d0 + uVar10;
  uVar31 = uVar31 + iStack_cc + (uint)CARRY4(uVar32,local_d0) + uVar44 +
           (uint)CARRY4(uVar32 + local_d0,uVar10);
  uVar13 = uVar18 + local_b8 + uVar16;
  uVar26 = uVar21 + iStack_b4 + (uint)CARRY4(uVar18,local_b8) + uVar7 +
           (uint)CARRY4(uVar18 + local_b8,uVar16);
  uVar21 = (uVar39 ^ uVar14) >> 0x10 | uVar11 << 0x10;
  uVar9 = uVar11 >> 0x10 | (uVar39 ^ uVar14) << 0x10;
  uVar11 = uVar31 ^ uVar32 ^ uVar8;
  uVar8 = uVar26 ^ uVar18 ^ uVar50;
  uVar17 = uVar21 + uVar41;
  uVar47 = (uVar22 ^ uVar35) >> 0x10 | uVar11 << 0x10;
  uVar45 = (uVar13 ^ uVar15) >> 0x10 | uVar8 << 0x10;
  uVar14 = uVar8 >> 0x10 | (uVar13 ^ uVar15) << 0x10;
  uVar32 = uVar9 + uVar42 + (uint)CARRY4(uVar21,uVar41);
  uVar35 = uVar11 >> 0x10 | (uVar22 ^ uVar35) << 0x10;
  uVar11 = uVar47 + uVar23;
  uVar37 = uVar37 ^ uVar32;
  uVar8 = uVar35 + uVar29 + (uint)CARRY4(uVar47,uVar23);
  uVar41 = (uVar36 ^ uVar17) << 1 | uVar37 >> 0x1f;
  uVar15 = uVar45 + uVar27;
  uVar23 = uVar37 << 1 | (uVar36 ^ uVar17) >> 0x1f;
  uVar18 = uVar14 + uVar28 + (uint)CARRY4(uVar45,uVar27);
  uVar44 = uVar8 ^ uVar44;
  uVar42 = (uVar11 ^ uVar10) << 1 | uVar44 >> 0x1f;
  uVar7 = uVar18 ^ uVar7;
  uVar44 = uVar44 << 1 | (uVar11 ^ uVar10) >> 0x1f;
  uVar27 = uVar12 + local_120[0] + uVar41;
  uVar10 = (uVar15 ^ uVar16) << 1 | uVar7 >> 0x1f;
  uVar36 = uVar20 + local_120[1] + (uint)CARRY4(uVar12,local_120[0]) + uVar23 +
           (uint)CARRY4(uVar12 + local_120[0],uVar41);
  uVar29 = uVar13 + local_d8 + uVar42;
  uVar26 = uVar26 + iStack_d4 + (uint)CARRY4(uVar13,local_d8) + uVar44 +
           (uint)CARRY4(uVar13 + local_d8,uVar42);
  uVar35 = uVar36 ^ uVar35;
  uVar20 = uVar7 << 1 | (uVar15 ^ uVar16) >> 0x1f;
  uVar13 = uVar39 + local_f0 + uVar10;
  uVar16 = uVar40 + iStack_ec + (uint)CARRY4(uVar39,local_f0) + uVar20 +
           (uint)CARRY4(uVar39 + local_f0,uVar10);
  uVar9 = uVar26 ^ uVar9;
  uVar24 = uVar16 ^ uVar24;
  uVar38 = uVar35 + uVar15;
  uVar39 = (uVar27 ^ uVar47) + uVar18 + (uint)CARRY4(uVar35,uVar15);
  uVar7 = uVar24 + uVar11;
  uVar12 = (uVar13 ^ uVar33) + uVar8 + (uint)CARRY4(uVar24,uVar11);
  uVar28 = uVar9 + uVar19;
  uVar37 = (uVar29 ^ uVar21) + uVar48 + (uint)CARRY4(uVar9,uVar19);
  uVar23 = uVar39 ^ uVar23;
  uVar49 = (uVar38 ^ uVar41) >> 0x18 | uVar23 << 8;
  uVar20 = uVar12 ^ uVar20;
  uVar18 = uVar23 >> 0x18 | (uVar38 ^ uVar41) << 8;
  uVar40 = (uVar7 ^ uVar10) >> 0x18 | uVar20 << 8;
  uVar44 = uVar37 ^ uVar44;
  uVar10 = uVar20 >> 0x18 | (uVar7 ^ uVar10) << 8;
  uVar48 = uVar27 + local_e8 + uVar49;
  uVar41 = (uVar28 ^ uVar42) >> 0x18 | uVar44 << 8;
  uVar23 = uVar36 + iStack_e4 + (uint)CARRY4(uVar27,local_e8) + uVar18 +
           (uint)CARRY4(uVar27 + local_e8,uVar49);
  uVar44 = uVar44 >> 0x18 | (uVar28 ^ uVar42) << 8;
  uVar15 = uVar13 + local_108 + uVar40;
  uVar11 = uVar23 ^ uVar27 ^ uVar47;
  uVar27 = uVar16 + iStack_104 + (uint)CARRY4(uVar13,local_108) + uVar10 +
           (uint)CARRY4(uVar13 + local_108,uVar40);
  uVar36 = uVar29 + local_120[4] + uVar41;
  uVar20 = (uVar48 ^ uVar35) >> 0x10 | uVar11 << 0x10;
  uVar47 = uVar26 + iStack_10c + (uint)CARRY4(uVar29,local_120[4]) + uVar44 +
           (uint)CARRY4(uVar29 + local_120[4],uVar41);
  uVar19 = uVar11 >> 0x10 | (uVar48 ^ uVar35) << 0x10;
  uVar11 = uVar27 ^ uVar13 ^ uVar33;
  uVar8 = uVar47 ^ uVar29 ^ uVar21;
  uVar42 = (uVar15 ^ uVar24) >> 0x10 | uVar11 << 0x10;
  uVar26 = uVar20 + uVar38;
  uVar21 = uVar11 >> 0x10 | (uVar15 ^ uVar24) << 0x10;
  uVar29 = uVar19 + uVar39 + (uint)CARRY4(uVar20,uVar38);
  uVar35 = (uVar36 ^ uVar9) >> 0x10 | uVar8 << 0x10;
  uVar24 = uVar8 >> 0x10 | (uVar36 ^ uVar9) << 0x10;
  uVar11 = uVar42 + uVar7;
  uVar8 = uVar21 + uVar12 + (uint)CARRY4(uVar42,uVar7);
  uVar18 = uVar29 ^ uVar18;
  uVar38 = uVar35 + uVar28;
  uVar28 = uVar24 + uVar37 + (uint)CARRY4(uVar35,uVar28);
  uVar33 = (uVar26 ^ uVar49) << 1 | uVar18 >> 0x1f;
  uVar10 = uVar8 ^ uVar10;
  uVar12 = uVar22 + local_e0 + uVar43;
  uVar16 = uVar31 + iStack_dc + (uint)CARRY4(uVar22,local_e0) + uVar25 +
           (uint)CARRY4(uVar22 + local_e0,uVar43);
  uVar18 = uVar18 << 1 | (uVar26 ^ uVar49) >> 0x1f;
  uVar44 = uVar28 ^ uVar44;
  uVar50 = (uVar11 ^ uVar40) << 1 | uVar10 >> 0x1f;
  uVar14 = uVar16 ^ uVar14;
  uVar9 = uVar10 << 1 | (uVar11 ^ uVar40) >> 0x1f;
  uVar22 = uVar15 + local_e8 + uVar33;
  uVar27 = uVar27 + iStack_e4 + (uint)CARRY4(uVar15,local_e8) + uVar18 +
           (uint)CARRY4(uVar15 + local_e8,uVar33);
  uVar31 = uVar14 + uVar17;
  uVar17 = (uVar12 ^ uVar45) + uVar32 + (uint)CARRY4(uVar14,uVar17);
  uVar24 = uVar27 ^ uVar24;
  uVar25 = uVar17 ^ uVar25;
  uVar40 = (uVar31 ^ uVar43) >> 0x18 | uVar25 << 8;
  uVar15 = uVar25 >> 0x18 | (uVar31 ^ uVar43) << 8;
  uVar43 = (uVar38 ^ uVar41) << 1 | uVar44 >> 0x1f;
  uVar7 = uVar12 + local_c8 + uVar40;
  uVar13 = uVar44 << 1 | (uVar38 ^ uVar41) >> 0x1f;
  uVar25 = uVar16 + iStack_c4 + (uint)CARRY4(uVar12,local_c8) + uVar15 +
           (uint)CARRY4(uVar12 + local_c8,uVar40);
  uVar12 = uVar25 ^ uVar12 ^ uVar45;
  uVar37 = (uVar7 ^ uVar14) >> 0x10 | uVar12 << 0x10;
  uVar10 = uVar12 >> 0x10 | (uVar7 ^ uVar14) << 0x10;
  uVar14 = uVar37 + uVar31;
  uVar16 = uVar10 + uVar17 + (uint)CARRY4(uVar37,uVar31);
  uVar12 = uVar7 + local_108 + uVar43;
  uVar7 = uVar25 + iStack_104 + (uint)CARRY4(uVar7,local_108) + uVar13 +
          (uint)CARRY4(uVar7 + local_108,uVar43);
  uVar39 = uVar36 + local_c0 + uVar50;
  uVar32 = uVar47 + iStack_bc + (uint)CARRY4(uVar36,local_c0) + uVar9 +
           (uint)CARRY4(uVar36 + local_c0,uVar50);
  uVar31 = uVar24 + uVar14;
  uVar15 = uVar16 ^ uVar15;
  uVar36 = (uVar22 ^ uVar35) + uVar16 + (uint)CARRY4(uVar24,uVar14);
  uVar17 = (uVar14 ^ uVar40) << 1 | uVar15 >> 0x1f;
  uVar19 = uVar7 ^ uVar19;
  uVar25 = uVar15 << 1 | (uVar14 ^ uVar40) >> 0x1f;
  uVar10 = uVar32 ^ uVar10;
  uVar18 = uVar36 ^ uVar18;
  uVar44 = uVar48 + local_b8 + uVar17;
  uVar47 = (uVar31 ^ uVar33) >> 0x18 | uVar18 << 8;
  uVar48 = uVar23 + iStack_b4 + (uint)CARRY4(uVar48,local_b8) + uVar25 +
           (uint)CARRY4(uVar48 + local_b8,uVar17);
  uVar14 = uVar18 >> 0x18 | (uVar31 ^ uVar33) << 8;
  uVar21 = uVar48 ^ uVar21;
  uVar45 = uVar22 + local_b0 + uVar47;
  uVar49 = uVar27 + iStack_ac + (uint)CARRY4(uVar22,local_b0) + uVar14 +
           (uint)CARRY4(uVar22 + local_b0,uVar47);
  uVar16 = uVar22 ^ uVar35 ^ uVar49;
  uVar22 = uVar21 + uVar38;
  uVar33 = uVar16 >> 0x10 | (uVar24 ^ uVar45) << 0x10;
  uVar27 = (uVar44 ^ uVar42) + uVar28 + (uint)CARRY4(uVar21,uVar38);
  uVar15 = (uVar24 ^ uVar45) >> 0x10 | uVar16 << 0x10;
  uVar25 = uVar27 ^ uVar25;
  uVar38 = (uVar22 ^ uVar17) >> 0x18 | uVar25 << 8;
  uVar18 = uVar25 >> 0x18 | (uVar22 ^ uVar17) << 8;
  uVar17 = uVar19 + uVar11;
  uVar28 = (uVar12 ^ uVar20) + uVar8 + (uint)CARRY4(uVar19,uVar11);
  uVar24 = uVar15 + uVar31;
  uVar31 = uVar33 + uVar36 + (uint)CARRY4(uVar15,uVar31);
  uVar11 = uVar10 + uVar26;
  uVar25 = (uVar39 ^ uVar37) + uVar29 + (uint)CARRY4(uVar10,uVar26);
  uVar13 = uVar28 ^ uVar13;
  uVar14 = uVar31 ^ uVar14;
  uVar51 = (uVar24 ^ uVar47) << 1 | uVar14 >> 0x1f;
  uVar9 = uVar25 ^ uVar9;
  uVar16 = uVar14 << 1 | (uVar24 ^ uVar47) >> 0x1f;
  uVar23 = uVar44 + local_c8 + uVar38;
  uVar35 = uVar48 + iStack_c4 + (uint)CARRY4(uVar44,local_c8) + uVar18 +
           (uint)CARRY4(uVar44 + local_c8,uVar38);
  uVar47 = (uVar17 ^ uVar43) >> 0x18 | uVar13 << 8;
  uVar44 = uVar44 ^ uVar42 ^ uVar35;
  uVar8 = uVar13 >> 0x18 | (uVar17 ^ uVar43) << 8;
  uVar13 = (uVar11 ^ uVar50) >> 0x18 | uVar9 << 8;
  uVar14 = (uVar21 ^ uVar23) >> 0x10 | uVar44 << 0x10;
  uVar9 = uVar9 >> 0x18 | (uVar11 ^ uVar50) << 8;
  uVar48 = uVar12 + local_d8 + uVar47;
  uVar44 = uVar44 >> 0x10 | (uVar21 ^ uVar23) << 0x10;
  uVar29 = uVar7 + iStack_d4 + (uint)CARRY4(uVar12,local_d8) + uVar8 +
           (uint)CARRY4(uVar12 + local_d8,uVar47);
  uVar21 = uVar29 ^ uVar12 ^ uVar20;
  uVar26 = uVar39 + local_120[2] + uVar13;
  uVar32 = uVar32 + local_120[3] + (uint)CARRY4(uVar39,local_120[2]) + uVar9 +
           (uint)CARRY4(uVar39 + local_120[2],uVar13);
  uVar7 = uVar23 + local_f8 + uVar51;
  uVar20 = uVar35 + iStack_f4 + (uint)CARRY4(uVar23,local_f8) + uVar16 +
           (uint)CARRY4(uVar23 + local_f8,uVar51);
  uVar23 = uVar14 + uVar22;
  uVar22 = uVar44 + uVar27 + (uint)CARRY4(uVar14,uVar22);
  uVar18 = uVar22 ^ uVar18;
  uVar12 = (uVar48 ^ uVar19) >> 0x10 | uVar21 << 0x10;
  uVar27 = uVar32 ^ uVar39 ^ uVar37;
  uVar21 = uVar21 >> 0x10 | (uVar48 ^ uVar19) << 0x10;
  uVar50 = (uVar23 ^ uVar38) << 1 | uVar18 >> 0x1f;
  uVar36 = (uVar26 ^ uVar10) >> 0x10 | uVar27 << 0x10;
  uVar35 = uVar27 >> 0x10 | (uVar26 ^ uVar10) << 0x10;
  uVar10 = uVar18 << 1 | (uVar23 ^ uVar38) >> 0x1f;
  uVar19 = uVar36 + uVar11;
  uVar18 = uVar35 + uVar25 + (uint)CARRY4(uVar36,uVar11);
  uVar27 = uVar20 ^ uVar21;
  uVar37 = uVar12 + uVar17;
  uVar40 = uVar21 + uVar28 + (uint)CARRY4(uVar12,uVar17);
  uVar11 = uVar27 + uVar19;
  uVar25 = (uVar7 ^ uVar12) + uVar18 + (uint)CARRY4(uVar27,uVar19);
  uVar18 = uVar18 ^ uVar9;
  uVar8 = uVar40 ^ uVar8;
  uVar16 = uVar25 ^ uVar16;
  uVar52 = (uVar37 ^ uVar47) << 1 | uVar8 >> 0x1f;
  uVar38 = uVar48 + local_120[4] + uVar50;
  uVar8 = uVar8 << 1 | (uVar37 ^ uVar47) >> 0x1f;
  uVar41 = uVar29 + iStack_10c + (uint)CARRY4(uVar48,local_120[4]) + uVar10 +
           (uint)CARRY4(uVar48 + local_120[4],uVar50);
  uVar35 = uVar41 ^ uVar35;
  uVar53 = (uVar11 ^ uVar51) >> 0x18 | uVar16 << 8;
  uVar48 = uVar26 + local_e0 + uVar52;
  uVar28 = uVar32 + iStack_dc + (uint)CARRY4(uVar26,local_e0) + uVar8 +
           (uint)CARRY4(uVar26 + local_e0,uVar52);
  uVar21 = uVar16 >> 0x18 | (uVar11 ^ uVar51) << 8;
  uVar51 = (uVar19 ^ uVar13) << 1 | uVar18 >> 0x1f;
  uVar9 = uVar18 << 1 | (uVar19 ^ uVar13) >> 0x1f;
  uVar26 = uVar35 + uVar24;
  uVar42 = (uVar38 ^ uVar36) + uVar31 + (uint)CARRY4(uVar35,uVar24);
  uVar33 = uVar28 ^ uVar33;
  uVar10 = uVar42 ^ uVar10;
  uVar47 = uVar7 + local_120[0] + uVar53;
  uVar43 = uVar20 + local_120[1] + (uint)CARRY4(uVar7,local_120[0]) + uVar21 +
           (uint)CARRY4(uVar7 + local_120[0],uVar53);
  uVar7 = uVar43 ^ uVar7 ^ uVar12;
  uVar24 = uVar45 + local_a8 + uVar51;
  uVar39 = (uVar26 ^ uVar50) >> 0x18 | uVar10 << 8;
  uVar16 = uVar49 + iStack_a4 + (uint)CARRY4(uVar45,local_a8) + uVar9 +
           (uint)CARRY4(uVar45 + local_a8,uVar51);
  uVar17 = uVar10 >> 0x18 | (uVar26 ^ uVar50) << 8;
  uVar44 = uVar16 ^ uVar44;
  uVar54 = (uVar47 ^ uVar27) >> 0x10 | uVar7 << 0x10;
  uVar29 = uVar38 + local_d0 + uVar39;
  uVar18 = uVar7 >> 0x10 | (uVar47 ^ uVar27) << 0x10;
  uVar32 = uVar41 + iStack_cc + (uint)CARRY4(uVar38,local_d0) + uVar17 +
           (uint)CARRY4(uVar38 + local_d0,uVar39);
  uVar7 = uVar32 ^ uVar38 ^ uVar36;
  uVar41 = uVar54 + uVar11;
  uVar49 = uVar18 + uVar25 + (uint)CARRY4(uVar54,uVar11);
  uVar45 = uVar44 + uVar37;
  uVar37 = (uVar24 ^ uVar14) + uVar40 + (uint)CARRY4(uVar44,uVar37);
  uVar19 = uVar33 + uVar23;
  uVar22 = (uVar48 ^ uVar15) + uVar22 + (uint)CARRY4(uVar33,uVar23);
  uVar21 = uVar49 ^ uVar21;
  uVar9 = uVar37 ^ uVar9;
  uVar55 = (uVar41 ^ uVar53) << 1 | uVar21 >> 0x1f;
  uVar8 = uVar22 ^ uVar8;
  uVar31 = uVar21 << 1 | (uVar41 ^ uVar53) >> 0x1f;
  uVar50 = (uVar29 ^ uVar35) >> 0x10 | uVar7 << 0x10;
  uVar13 = uVar7 >> 0x10 | (uVar29 ^ uVar35) << 0x10;
  uVar12 = uVar50 + uVar26;
  uVar25 = uVar13 + uVar42 + (uint)CARRY4(uVar50,uVar26);
  uVar20 = (uVar45 ^ uVar51) >> 0x18 | uVar9 << 8;
  uVar21 = uVar9 >> 0x18 | (uVar45 ^ uVar51) << 8;
  uVar9 = (uVar19 ^ uVar52) >> 0x18 | uVar8 << 8;
  uVar17 = uVar25 ^ uVar17;
  uVar7 = uVar8 >> 0x18 | (uVar19 ^ uVar52) << 8;
  uVar26 = uVar24 + local_100 + uVar20;
  uVar8 = uVar16 + iStack_fc + (uint)CARRY4(uVar24,local_100) + uVar21 +
          (uint)CARRY4(uVar24 + local_100,uVar20);
  uVar51 = (uVar12 ^ uVar39) << 1 | uVar17 >> 0x1f;
  uVar10 = uVar48 + local_f0 + uVar9;
  uVar16 = uVar28 + iStack_ec + (uint)CARRY4(uVar48,local_f0) + uVar7 +
           (uint)CARRY4(uVar48 + local_f0,uVar9);
  uVar11 = uVar8 ^ uVar24 ^ uVar14;
  uVar14 = uVar17 << 1 | (uVar12 ^ uVar39) >> 0x1f;
  uVar24 = uVar26 + local_b0 + uVar55;
  uVar27 = uVar8 + iStack_ac + (uint)CARRY4(uVar26,local_b0) + uVar31 +
           (uint)CARRY4(uVar26 + local_b0,uVar55);
  uVar36 = (uVar26 ^ uVar44) >> 0x10 | uVar11 << 0x10;
  uVar8 = uVar16 ^ uVar48 ^ uVar15;
  uVar38 = uVar11 >> 0x10 | (uVar26 ^ uVar44) << 0x10;
  uVar40 = uVar47 + local_f0 + uVar51;
  uVar44 = (uVar10 ^ uVar33) >> 0x10 | uVar8 << 0x10;
  uVar42 = uVar43 + iStack_ec + (uint)CARRY4(uVar47,local_f0) + uVar14 +
           (uint)CARRY4(uVar47 + local_f0,uVar51);
  uVar17 = uVar8 >> 0x10 | (uVar10 ^ uVar33) << 0x10;
  uVar11 = uVar44 + uVar19;
  uVar8 = uVar17 + uVar22 + (uint)CARRY4(uVar44,uVar19);
  uVar17 = uVar17 ^ uVar27;
  uVar28 = uVar36 + uVar45;
  uVar35 = uVar38 + uVar37 + (uint)CARRY4(uVar36,uVar45);
  uVar38 = uVar38 ^ uVar42;
  uVar48 = uVar17 + uVar12;
  uVar23 = (uVar44 ^ uVar24) + uVar25 + (uint)CARRY4(uVar17,uVar12);
  uVar7 = uVar8 ^ uVar7;
  uVar21 = uVar35 ^ uVar21;
  uVar43 = (uVar28 ^ uVar20) << 1 | uVar21 >> 0x1f;
  uVar37 = uVar38 + uVar11;
  uVar31 = uVar23 ^ uVar31;
  uVar39 = (uVar36 ^ uVar40) + uVar8 + (uint)CARRY4(uVar38,uVar11);
  uVar25 = uVar21 << 1 | (uVar28 ^ uVar20) >> 0x1f;
  uVar45 = (uVar11 ^ uVar9) << 1 | uVar7 >> 0x1f;
  uVar14 = uVar39 ^ uVar14;
  uVar47 = (uVar48 ^ uVar55) >> 0x18 | uVar31 << 8;
  uVar26 = (uVar37 ^ uVar51) >> 0x18 | uVar14 << 8;
  uVar20 = uVar14 >> 0x18 | (uVar37 ^ uVar51) << 8;
  uVar12 = uVar31 >> 0x18 | (uVar48 ^ uVar55) << 8;
  uVar19 = uVar24 + local_d8 + uVar47;
  uVar22 = uVar27 + iStack_d4 + (uint)CARRY4(uVar24,local_d8) + uVar12 +
           (uint)CARRY4(uVar24 + local_d8,uVar47);
  uVar15 = uVar7 << 1 | (uVar11 ^ uVar9) >> 0x1f;
  uVar31 = uVar40 + local_a8 + uVar26;
  uVar33 = uVar42 + iStack_a4 + (uint)CARRY4(uVar40,local_a8) + uVar20 +
           (uint)CARRY4(uVar40 + local_a8,uVar26);
  uVar7 = uVar36 ^ uVar40 ^ uVar33;
  uVar11 = uVar22 ^ uVar44 ^ uVar24;
  uVar21 = (uVar19 ^ uVar17) >> 0x10 | uVar11 << 0x10;
  uVar44 = uVar11 >> 0x10 | (uVar19 ^ uVar17) << 0x10;
  uVar17 = (uVar38 ^ uVar31) >> 0x10 | uVar7 << 0x10;
  uVar24 = uVar7 >> 0x10 | (uVar38 ^ uVar31) << 0x10;
  uVar11 = uVar21 + uVar48;
  uVar8 = uVar44 + uVar23 + (uint)CARRY4(uVar21,uVar48);
  uVar7 = uVar17 + uVar37;
  uVar9 = uVar24 + uVar39 + (uint)CARRY4(uVar17,uVar37);
  uVar12 = uVar8 ^ uVar12;
  uVar51 = (uVar11 ^ uVar47) << 1 | uVar12 >> 0x1f;
  uVar27 = uVar12 << 1 | (uVar11 ^ uVar47) >> 0x1f;
  uVar12 = uVar10 + local_c8 + uVar43;
  uVar10 = uVar16 + iStack_c4 + (uint)CARRY4(uVar10,local_c8) + uVar25 +
           (uint)CARRY4(uVar10 + local_c8,uVar43);
  uVar48 = uVar29 + local_120[0] + uVar45;
  uVar16 = uVar32 + local_120[1] + (uint)CARRY4(uVar29,local_120[0]) + uVar15 +
           (uint)CARRY4(uVar29 + local_120[0],uVar45);
  uVar13 = uVar10 ^ uVar13;
  uVar18 = uVar16 ^ uVar18;
  uVar20 = uVar9 ^ uVar20;
  uVar52 = (uVar7 ^ uVar26) << 1 | uVar20 >> 0x1f;
  uVar29 = uVar20 << 1 | (uVar7 ^ uVar26) >> 0x1f;
  uVar20 = uVar18 + uVar28;
  uVar14 = (uVar48 ^ uVar54) + uVar35 + (uint)CARRY4(uVar18,uVar28);
  uVar37 = uVar13 + uVar41;
  uVar38 = (uVar12 ^ uVar50) + uVar49 + (uint)CARRY4(uVar13,uVar41);
  uVar28 = uVar31 + local_c0 + uVar51;
  uVar31 = uVar33 + iStack_bc + (uint)CARRY4(uVar31,local_c0) + uVar27 +
           (uint)CARRY4(uVar31 + local_c0,uVar51);
  uVar15 = uVar14 ^ uVar15;
  uVar25 = uVar38 ^ uVar25;
  uVar47 = (uVar20 ^ uVar45) >> 0x18 | uVar15 << 8;
  uVar33 = (uVar37 ^ uVar43) >> 0x18 | uVar25 << 8;
  uVar35 = uVar15 >> 0x18 | (uVar20 ^ uVar45) << 8;
  uVar23 = uVar25 >> 0x18 | (uVar37 ^ uVar43) << 8;
  uVar25 = uVar48 + local_e0 + uVar47;
  uVar16 = uVar16 + iStack_dc + (uint)CARRY4(uVar48,local_e0) + uVar35 +
           (uint)CARRY4(uVar48 + local_e0,uVar47);
  uVar32 = uVar16 ^ uVar48 ^ uVar54;
  uVar15 = uVar12 + local_108 + uVar33;
  uVar48 = uVar10 + iStack_104 + (uint)CARRY4(uVar12,local_108) + uVar23 +
           (uint)CARRY4(uVar12 + local_108,uVar33);
  uVar26 = uVar48 ^ uVar12 ^ uVar50;
  uVar12 = (uVar25 ^ uVar18) >> 0x10 | uVar32 << 0x10;
  uVar10 = uVar32 >> 0x10 | (uVar25 ^ uVar18) << 0x10;
  uVar42 = (uVar15 ^ uVar13) >> 0x10 | uVar26 << 0x10;
  uVar36 = uVar26 >> 0x10 | (uVar15 ^ uVar13) << 0x10;
  uVar18 = uVar12 + uVar20;
  uVar14 = uVar10 + uVar14 + (uint)CARRY4(uVar12,uVar20);
  uVar20 = uVar42 + uVar37;
  uVar26 = uVar36 + uVar38 + (uint)CARRY4(uVar42,uVar37);
  uVar10 = uVar10 ^ uVar31;
  uVar35 = uVar14 ^ uVar35;
  uVar39 = uVar25 + local_d0 + uVar52;
  uVar37 = (uVar18 ^ uVar47) << 1 | uVar35 >> 0x1f;
  uVar41 = uVar16 + iStack_cc + (uint)CARRY4(uVar25,local_d0) + uVar29 +
           (uint)CARRY4(uVar25 + local_d0,uVar52);
  uVar13 = uVar35 << 1 | (uVar18 ^ uVar47) >> 0x1f;
  uVar23 = uVar26 ^ uVar23;
  uVar36 = uVar41 ^ uVar36;
  uVar35 = (uVar20 ^ uVar33) << 1 | uVar23 >> 0x1f;
  uVar16 = uVar23 << 1 | (uVar20 ^ uVar33) >> 0x1f;
  uVar23 = uVar15 + local_120[2] + uVar37;
  uVar32 = uVar48 + local_120[3] + (uint)CARRY4(uVar15,local_120[2]) + uVar13 +
           (uint)CARRY4(uVar15 + local_120[2],uVar37);
  uVar44 = uVar32 ^ uVar44;
  uVar38 = uVar10 + uVar20;
  uVar40 = (uVar12 ^ uVar28) + uVar26 + (uint)CARRY4(uVar10,uVar20);
  uVar26 = uVar19 + local_b8 + uVar35;
  uVar22 = uVar22 + iStack_b4 + (uint)CARRY4(uVar19,local_b8) + uVar16 +
           (uint)CARRY4(uVar19 + local_b8,uVar35);
  uVar27 = uVar40 ^ uVar27;
  uVar24 = uVar22 ^ uVar24;
  uVar43 = (uVar38 ^ uVar51) >> 0x18 | uVar27 << 8;
  uVar33 = uVar27 >> 0x18 | (uVar38 ^ uVar51) << 8;
  uVar20 = uVar24 + uVar18;
  uVar25 = (uVar26 ^ uVar17) + uVar14 + (uint)CARRY4(uVar24,uVar18);
  uVar18 = uVar44 + uVar7;
  uVar27 = (uVar23 ^ uVar21) + uVar9 + (uint)CARRY4(uVar44,uVar7);
  uVar16 = uVar25 ^ uVar16;
  uVar13 = uVar27 ^ uVar13;
  uVar14 = uVar28 + local_120[4] + uVar43;
  uVar45 = (uVar18 ^ uVar37) >> 0x18 | uVar13 << 8;
  uVar19 = uVar31 + iStack_10c + (uint)CARRY4(uVar28,local_120[4]) + uVar33 +
           (uint)CARRY4(uVar28 + local_120[4],uVar43);
  uVar15 = (uVar20 ^ uVar35) >> 0x18 | uVar16 << 8;
  uVar9 = uVar13 >> 0x18 | (uVar18 ^ uVar37) << 8;
  uVar7 = uVar19 ^ uVar12 ^ uVar28;
  uVar49 = (uVar14 ^ uVar10) >> 0x10 | uVar7 << 0x10;
  uVar13 = uVar16 >> 0x18 | (uVar20 ^ uVar35) << 8;
  uVar48 = uVar7 >> 0x10 | (uVar14 ^ uVar10) << 0x10;
  uVar7 = uVar26 + local_e8 + uVar15;
  uVar12 = uVar22 + iStack_e4 + (uint)CARRY4(uVar26,local_e8) + uVar13 +
           (uint)CARRY4(uVar26 + local_e8,uVar15);
  uVar35 = uVar23 + local_100 + uVar45;
  uVar37 = uVar32 + iStack_fc + (uint)CARRY4(uVar23,local_100) + uVar9 +
           (uint)CARRY4(uVar23 + local_100,uVar45);
  uVar10 = uVar12 ^ uVar26 ^ uVar17;
  uVar50 = (uVar7 ^ uVar24) >> 0x10 | uVar10 << 0x10;
  uVar26 = uVar36 + uVar11;
  uVar47 = (uVar39 ^ uVar42) + uVar8 + (uint)CARRY4(uVar36,uVar11);
  uVar11 = uVar37 ^ uVar23 ^ uVar21;
  uVar16 = uVar49 + uVar38;
  uVar22 = uVar48 + uVar40 + (uint)CARRY4(uVar49,uVar38);
  uVar33 = uVar22 ^ uVar33;
  uVar40 = (uVar35 ^ uVar44) >> 0x10 | uVar11 << 0x10;
  uVar51 = (uVar16 ^ uVar43) << 1 | uVar33 >> 0x1f;
  uVar8 = uVar33 << 1 | (uVar16 ^ uVar43) >> 0x1f;
  uVar29 = uVar47 ^ uVar29;
  uVar31 = uVar10 >> 0x10 | (uVar7 ^ uVar24) << 0x10;
  uVar23 = uVar11 >> 0x10 | (uVar35 ^ uVar44) << 0x10;
  uVar11 = uVar50 + uVar20;
  uVar20 = uVar31 + uVar25 + (uint)CARRY4(uVar50,uVar20);
  uVar33 = uVar40 + uVar18;
  uVar38 = uVar23 + uVar27 + (uint)CARRY4(uVar40,uVar18);
  uVar25 = (uVar26 ^ uVar52) >> 0x18 | uVar29 << 8;
  uVar18 = uVar29 >> 0x18 | (uVar26 ^ uVar52) << 8;
  uVar9 = uVar38 ^ uVar9;
  uVar27 = uVar39 + local_f8 + uVar25;
  uVar29 = uVar41 + iStack_f4 + (uint)CARRY4(uVar39,local_f8) + uVar18 +
           (uint)CARRY4(uVar39 + local_f8,uVar25);
  uVar13 = uVar20 ^ uVar13;
  uVar32 = uVar9 << 1 | (uVar33 ^ uVar45) >> 0x1f;
  uVar44 = uVar7 + local_e0 + uVar51;
  uVar52 = (uVar11 ^ uVar15) << 1 | uVar13 >> 0x1f;
  uVar17 = uVar12 + iStack_dc + (uint)CARRY4(uVar7,local_e0) + uVar8 +
           (uint)CARRY4(uVar7 + local_e0,uVar51);
  uVar24 = (uVar33 ^ uVar45) << 1 | uVar9 >> 0x1f;
  uVar7 = uVar29 ^ uVar39 ^ uVar42;
  uVar28 = (uVar27 ^ uVar36) >> 0x10 | uVar7 << 0x10;
  uVar23 = uVar17 ^ uVar23;
  uVar9 = uVar7 >> 0x10 | (uVar27 ^ uVar36) << 0x10;
  uVar12 = uVar13 << 1 | (uVar11 ^ uVar15) >> 0x1f;
  uVar13 = uVar28 + uVar26;
  uVar21 = uVar9 + uVar47 + (uint)CARRY4(uVar28,uVar26);
  uVar18 = uVar21 ^ uVar18;
  uVar7 = uVar23 + uVar13;
  uVar10 = uVar18 << 1 | (uVar13 ^ uVar25) >> 0x1f;
  uVar21 = (uVar44 ^ uVar40) + uVar21 + (uint)CARRY4(uVar23,uVar13);
  uVar15 = uVar27 + local_120[2] + uVar24;
  uVar25 = (uVar13 ^ uVar25) << 1 | uVar18 >> 0x1f;
  uVar27 = uVar29 + local_120[3] + (uint)CARRY4(uVar27,local_120[2]) + uVar32 +
           (uint)CARRY4(uVar27 + local_120[2],uVar24);
  uVar13 = uVar35 + local_e8 + uVar52;
  uVar8 = uVar21 ^ uVar8;
  uVar29 = uVar37 + iStack_e4 + (uint)CARRY4(uVar35,local_e8) + uVar12 +
           (uint)CARRY4(uVar35 + local_e8,uVar52);
  uVar36 = uVar14 + local_d0 + uVar25;
  uVar47 = uVar19 + iStack_cc + (uint)CARRY4(uVar14,local_d0) + uVar10 +
           (uint)CARRY4(uVar14 + local_d0,uVar25);
  uVar48 = uVar27 ^ uVar48;
  uVar19 = (uVar7 ^ uVar51) >> 0x18 | uVar8 << 8;
  uVar35 = uVar8 >> 0x18 | (uVar7 ^ uVar51) << 8;
  uVar18 = uVar44 + local_100 + uVar19;
  uVar31 = uVar47 ^ uVar31;
  uVar26 = uVar17 + iStack_fc + (uint)CARRY4(uVar44,local_100) + uVar35 +
           (uint)CARRY4(uVar44 + local_100,uVar19);
  uVar9 = uVar29 ^ uVar9;
  uVar8 = uVar26 ^ uVar44 ^ uVar40;
  uVar51 = (uVar18 ^ uVar23) >> 0x10 | uVar8 << 0x10;
  uVar23 = uVar8 >> 0x10 | (uVar18 ^ uVar23) << 0x10;
  uVar17 = uVar48 + uVar11;
  uVar39 = (uVar15 ^ uVar49) + uVar20 + (uint)CARRY4(uVar48,uVar11);
  uVar20 = uVar31 + uVar33;
  uVar14 = (uVar36 ^ uVar50) + uVar38 + (uint)CARRY4(uVar31,uVar33);
  uVar41 = uVar51 + uVar7;
  uVar45 = uVar23 + uVar21 + (uint)CARRY4(uVar51,uVar7);
  uVar32 = uVar39 ^ uVar32;
  uVar37 = uVar9 + uVar16;
  uVar38 = (uVar13 ^ uVar28) + uVar22 + (uint)CARRY4(uVar9,uVar16);
  uVar10 = uVar14 ^ uVar10;
  uVar42 = (uVar17 ^ uVar24) >> 0x18 | uVar32 << 8;
  uVar35 = uVar35 ^ uVar45;
  uVar53 = (uVar19 ^ uVar41) << 1 | uVar35 >> 0x1f;
  uVar33 = uVar35 << 1 | (uVar19 ^ uVar41) >> 0x1f;
  uVar12 = uVar38 ^ uVar12;
  uVar43 = (uVar20 ^ uVar25) >> 0x18 | uVar10 << 8;
  uVar35 = uVar10 >> 0x18 | (uVar20 ^ uVar25) << 8;
  uVar25 = uVar32 >> 0x18 | (uVar17 ^ uVar24) << 8;
  uVar21 = (uVar37 ^ uVar52) >> 0x18 | uVar12 << 8;
  uVar16 = uVar36 + local_120[4] + uVar43;
  uVar19 = uVar47 + iStack_10c + (uint)CARRY4(uVar36,local_120[4]) + uVar35 +
           (uint)CARRY4(uVar36 + local_120[4],uVar43);
  uVar11 = uVar15 + local_f8 + uVar42;
  uVar44 = uVar12 >> 0x18 | (uVar37 ^ uVar52) << 8;
  uVar8 = uVar27 + iStack_f4 + (uint)CARRY4(uVar15,local_f8) + uVar25 +
          (uint)CARRY4(uVar15 + local_f8,uVar42);
  uVar22 = uVar19 ^ uVar36 ^ uVar50;
  uVar7 = uVar13 + local_f0 + uVar21;
  uVar10 = uVar29 + iStack_ec + (uint)CARRY4(uVar13,local_f0) + uVar44 +
           (uint)CARRY4(uVar13 + local_f0,uVar21);
  uVar27 = uVar22 >> 0x10 | (uVar16 ^ uVar31) << 0x10;
  uVar12 = uVar8 ^ uVar15 ^ uVar49;
  uVar29 = uVar16 + local_a8 + uVar53;
  uVar15 = uVar19 + iStack_a4 + (uint)CARRY4(uVar16,local_a8) + uVar33 +
           (uint)CARRY4(uVar16 + local_a8,uVar53);
  uVar24 = (uVar16 ^ uVar31) >> 0x10 | uVar22 << 0x10;
  uVar19 = uVar10 ^ uVar13 ^ uVar28;
  uVar22 = (uVar11 ^ uVar48) >> 0x10 | uVar12 << 0x10;
  uVar16 = uVar12 >> 0x10 | (uVar11 ^ uVar48) << 0x10;
  uVar13 = (uVar7 ^ uVar9) >> 0x10 | uVar19 << 0x10;
  uVar9 = uVar19 >> 0x10 | (uVar7 ^ uVar9) << 0x10;
  uVar47 = uVar24 + uVar20;
  uVar40 = uVar27 + uVar14 + (uint)CARRY4(uVar24,uVar20);
  uVar12 = uVar22 + uVar17;
  uVar14 = uVar16 + uVar39 + (uint)CARRY4(uVar22,uVar17);
  uVar35 = uVar40 ^ uVar35;
  uVar16 = uVar15 ^ uVar16;
  uVar49 = (uVar47 ^ uVar43) << 1 | uVar35 >> 0x1f;
  uVar17 = uVar13 + uVar37;
  uVar48 = uVar9 + uVar38 + (uint)CARRY4(uVar13,uVar37);
  uVar37 = uVar16 + uVar17;
  uVar38 = (uVar29 ^ uVar22) + uVar48 + (uint)CARRY4(uVar16,uVar17);
  uVar19 = uVar35 << 1 | (uVar47 ^ uVar43) >> 0x1f;
  uVar33 = uVar38 ^ uVar33;
  uVar25 = uVar14 ^ uVar25;
  uVar20 = (uVar37 ^ uVar53) >> 0x18 | uVar33 << 8;
  uVar48 = uVar48 ^ uVar44;
  uVar44 = uVar33 >> 0x18 | (uVar37 ^ uVar53) << 8;
  uVar50 = (uVar12 ^ uVar42) << 1 | uVar25 >> 0x1f;
  uVar31 = uVar29 + local_c8 + uVar20;
  uVar36 = uVar15 + iStack_c4 + (uint)CARRY4(uVar29,local_c8) + uVar44 +
           (uint)CARRY4(uVar29 + local_c8,uVar20);
  uVar15 = uVar25 << 1 | (uVar12 ^ uVar42) >> 0x1f;
  uVar25 = uVar36 ^ uVar29 ^ uVar22;
  uVar32 = (uVar31 ^ uVar16) >> 0x10 | uVar25 << 0x10;
  uVar28 = uVar25 >> 0x10 | (uVar31 ^ uVar16) << 0x10;
  uVar22 = uVar32 + uVar37;
  uVar29 = uVar28 + uVar38 + (uint)CARRY4(uVar32,uVar37);
  uVar44 = uVar44 ^ uVar29;
  uVar16 = uVar11 + local_b8 + uVar49;
  uVar11 = uVar8 + iStack_b4 + (uint)CARRY4(uVar11,local_b8) + uVar19 +
           (uint)CARRY4(uVar11 + local_b8,uVar49);
  uVar35 = uVar7 + local_108 + uVar50;
  uVar37 = uVar10 + iStack_104 + (uint)CARRY4(uVar7,local_108) + uVar15 +
           (uint)CARRY4(uVar7 + local_108,uVar50);
  uVar43 = (uVar20 ^ uVar22) << 1 | uVar44 >> 0x1f;
  uVar20 = uVar44 << 1 | (uVar20 ^ uVar22) >> 0x1f;
  uVar9 = uVar11 ^ uVar9;
  uVar23 = uVar37 ^ uVar23;
  uVar39 = uVar9 + uVar41;
  uVar42 = (uVar16 ^ uVar13) + uVar45 + (uint)CARRY4(uVar9,uVar41);
  uVar19 = uVar42 ^ uVar19;
  uVar10 = (uVar17 ^ uVar21) << 1 | uVar48 >> 0x1f;
  uVar38 = (uVar39 ^ uVar49) >> 0x18 | uVar19 << 8;
  uVar33 = uVar48 << 1 | (uVar17 ^ uVar21) >> 0x1f;
  uVar41 = uVar18 + local_d8 + uVar10;
  uVar44 = uVar19 >> 0x18 | (uVar39 ^ uVar49) << 8;
  uVar19 = uVar26 + iStack_d4 + (uint)CARRY4(uVar18,local_d8) + uVar33 +
           (uint)CARRY4(uVar18 + local_d8,uVar10);
  uVar27 = uVar19 ^ uVar27;
  uVar17 = uVar16 + local_120[0] + uVar38;
  uVar48 = uVar11 + local_120[1] + (uint)CARRY4(uVar16,local_120[0]) + uVar44 +
           (uint)CARRY4(uVar16 + local_120[0],uVar38);
  uVar11 = uVar27 + uVar12;
  uVar25 = (uVar41 ^ uVar24) + uVar14 + (uint)CARRY4(uVar27,uVar12);
  uVar7 = uVar48 ^ uVar16 ^ uVar13;
  uVar52 = (uVar17 ^ uVar9) >> 0x10 | uVar7 << 0x10;
  uVar33 = uVar33 ^ uVar25;
  uVar18 = uVar7 >> 0x10 | (uVar17 ^ uVar9) << 0x10;
  uVar7 = uVar23 + uVar47;
  uVar21 = (uVar35 ^ uVar51) + uVar40 + (uint)CARRY4(uVar23,uVar47);
  uVar8 = (uVar10 ^ uVar11) >> 0x18 | uVar33 << 8;
  uVar10 = uVar33 >> 0x18 | (uVar10 ^ uVar11) << 8;
  uVar15 = uVar21 ^ uVar15;
  uVar16 = uVar41 + local_b0 + uVar8;
  uVar26 = uVar19 + iStack_ac + (uint)CARRY4(uVar41,local_b0) + uVar10 +
           (uint)CARRY4(uVar41 + local_b0,uVar8);
  uVar33 = uVar52 + uVar39;
  uVar47 = uVar18 + uVar42 + (uint)CARRY4(uVar52,uVar39);
  uVar13 = (uVar7 ^ uVar50) >> 0x18 | uVar15 << 8;
  uVar12 = uVar26 ^ uVar41 ^ uVar24;
  uVar14 = uVar15 >> 0x18 | (uVar7 ^ uVar50) << 8;
  uVar44 = uVar47 ^ uVar44;
  uVar40 = uVar35 + local_c0 + uVar13;
  uVar45 = uVar37 + iStack_bc + (uint)CARRY4(uVar35,local_c0) + uVar14 +
           (uint)CARRY4(uVar35 + local_c0,uVar13);
  uVar41 = (uVar33 ^ uVar38) << 1 | uVar44 >> 0x1f;
  uVar9 = uVar45 ^ uVar35 ^ uVar51;
  uVar19 = uVar44 << 1 | (uVar33 ^ uVar38) >> 0x1f;
  uVar35 = uVar16 + local_120[4] + uVar43;
  uVar38 = (uVar16 ^ uVar27) >> 0x10 | uVar12 << 0x10;
  uVar37 = uVar26 + iStack_10c + (uint)CARRY4(uVar16,local_120[4]) + uVar20 +
           (uint)CARRY4(uVar16 + local_120[4],uVar43);
  uVar39 = uVar12 >> 0x10 | (uVar16 ^ uVar27) << 0x10;
  uVar42 = uVar31 + local_120[0] + uVar41;
  uVar15 = (uVar40 ^ uVar23) >> 0x10 | uVar9 << 0x10;
  uVar49 = uVar36 + local_120[1] + (uint)CARRY4(uVar31,local_120[0]) + uVar19 +
           (uint)CARRY4(uVar31 + local_120[0],uVar41);
  uVar26 = uVar9 >> 0x10 | (uVar40 ^ uVar23) << 0x10;
  uVar12 = uVar38 + uVar11;
  uVar9 = uVar39 + uVar25 + (uint)CARRY4(uVar38,uVar11);
  uVar39 = uVar39 ^ uVar49;
  uVar25 = uVar15 + uVar7;
  uVar16 = uVar26 + uVar21 + (uint)CARRY4(uVar15,uVar7);
  uVar26 = uVar26 ^ uVar37;
  uVar10 = uVar9 ^ uVar10;
  uVar24 = uVar26 + uVar33;
  uVar50 = (uVar12 ^ uVar8) << 1 | uVar10 >> 0x1f;
  uVar23 = (uVar15 ^ uVar35) + uVar47 + (uint)CARRY4(uVar26,uVar33);
  uVar21 = uVar10 << 1 | (uVar12 ^ uVar8) >> 0x1f;
  uVar27 = uVar39 + uVar25;
  uVar36 = (uVar38 ^ uVar42) + uVar16 + (uint)CARRY4(uVar39,uVar25);
  uVar20 = uVar23 ^ uVar20;
  uVar16 = uVar16 ^ uVar14;
  uVar19 = uVar36 ^ uVar19;
  uVar31 = uVar40 + local_100 + uVar50;
  uVar47 = uVar45 + iStack_fc + (uint)CARRY4(uVar40,local_100) + uVar21 +
           (uint)CARRY4(uVar40 + local_100,uVar50);
  uVar11 = (uVar27 ^ uVar41) >> 0x18 | uVar19 << 8;
  uVar33 = (uVar24 ^ uVar43) >> 0x18 | uVar20 << 8;
  uVar44 = uVar20 >> 0x18 | (uVar24 ^ uVar43) << 8;
  uVar40 = uVar35 + local_108 + uVar33;
  uVar7 = uVar19 >> 0x18 | (uVar27 ^ uVar41) << 8;
  uVar43 = uVar37 + iStack_104 + (uint)CARRY4(uVar35,local_108) + uVar44 +
           (uint)CARRY4(uVar35 + local_108,uVar33);
  uVar8 = (uVar25 ^ uVar13) << 1 | uVar16 >> 0x1f;
  uVar19 = uVar16 << 1 | (uVar25 ^ uVar13) >> 0x1f;
  uVar41 = uVar42 + local_120[2] + uVar11;
  uVar45 = uVar49 + local_120[3] + (uint)CARRY4(uVar42,local_120[2]) + uVar7 +
           (uint)CARRY4(uVar42 + local_120[2],uVar11);
  uVar20 = uVar43 ^ uVar15 ^ uVar35;
  uVar18 = uVar47 ^ uVar18;
  uVar49 = (uVar40 ^ uVar26) >> 0x10 | uVar20 << 0x10;
  uVar10 = uVar20 >> 0x10 | (uVar40 ^ uVar26) << 0x10;
  uVar25 = uVar45 ^ uVar38 ^ uVar42;
  uVar20 = uVar49 + uVar24;
  uVar13 = uVar10 + uVar23 + (uint)CARRY4(uVar49,uVar24);
  uVar14 = (uVar41 ^ uVar39) >> 0x10 | uVar25 << 0x10;
  uVar15 = uVar25 >> 0x10 | (uVar41 ^ uVar39) << 0x10;
  uVar25 = uVar18 + uVar22;
  uVar16 = (uVar31 ^ uVar52) + uVar29 + (uint)CARRY4(uVar18,uVar22);
  uVar44 = uVar13 ^ uVar44;
  uVar35 = uVar14 + uVar27;
  uVar42 = (uVar20 ^ uVar33) << 1 | uVar44 >> 0x1f;
  uVar37 = uVar15 + uVar36 + (uint)CARRY4(uVar14,uVar27);
  uVar21 = uVar16 ^ uVar21;
  uVar44 = uVar44 << 1 | (uVar20 ^ uVar33) >> 0x1f;
  uVar24 = uVar17 + local_f0 + uVar8;
  uVar26 = uVar48 + iStack_ec + (uint)CARRY4(uVar17,local_f0) + uVar19 +
           (uint)CARRY4(uVar17 + local_f0,uVar8);
  uVar7 = uVar37 ^ uVar7;
  uVar28 = uVar26 ^ uVar28;
  uVar27 = uVar41 + local_e0 + uVar42;
  uVar33 = uVar45 + iStack_dc + (uint)CARRY4(uVar41,local_e0) + uVar44 +
           (uint)CARRY4(uVar41 + local_e0,uVar42);
  uVar23 = uVar28 + uVar12;
  uVar29 = (uVar24 ^ uVar32) + uVar9 + (uint)CARRY4(uVar28,uVar12);
  uVar41 = (uVar35 ^ uVar11) << 1 | uVar7 >> 0x1f;
  uVar48 = uVar7 << 1 | (uVar35 ^ uVar11) >> 0x1f;
  uVar39 = (uVar25 ^ uVar50) >> 0x18 | uVar21 << 8;
  uVar19 = uVar29 ^ uVar19;
  uVar12 = uVar21 >> 0x18 | (uVar25 ^ uVar50) << 8;
  uVar22 = (uVar23 ^ uVar8) >> 0x18 | uVar19 << 8;
  uVar8 = uVar19 >> 0x18 | (uVar23 ^ uVar8) << 8;
  uVar9 = uVar31 + local_f8 + uVar39;
  uVar17 = uVar47 + iStack_f4 + (uint)CARRY4(uVar31,local_f8) + uVar12 +
           (uint)CARRY4(uVar31 + local_f8,uVar39);
  uVar7 = uVar24 + local_e8 + uVar22;
  uVar26 = uVar26 + iStack_e4 + (uint)CARRY4(uVar24,local_e8) + uVar8 +
           (uint)CARRY4(uVar24 + local_e8,uVar22);
  uVar21 = uVar17 ^ uVar31 ^ uVar52;
  uVar11 = uVar26 ^ uVar24 ^ uVar32;
  uVar38 = (uVar9 ^ uVar18) >> 0x10 | uVar21 << 0x10;
  uVar21 = uVar21 >> 0x10 | (uVar9 ^ uVar18) << 0x10;
  uVar32 = uVar7 + local_b0 + uVar41;
  uVar19 = (uVar7 ^ uVar28) >> 0x10 | uVar11 << 0x10;
  uVar36 = uVar26 + iStack_ac + (uint)CARRY4(uVar7,local_b0) + uVar48 +
           (uint)CARRY4(uVar7 + local_b0,uVar41);
  uVar24 = uVar11 >> 0x10 | (uVar7 ^ uVar28) << 0x10;
  uVar11 = uVar38 + uVar25;
  uVar7 = uVar21 + uVar16 + (uint)CARRY4(uVar38,uVar25);
  uVar21 = uVar36 ^ uVar21;
  uVar25 = uVar19 + uVar23;
  uVar18 = uVar24 + uVar29 + (uint)CARRY4(uVar19,uVar23);
  uVar24 = uVar24 ^ uVar33;
  uVar12 = uVar7 ^ uVar12;
  uVar45 = (uVar11 ^ uVar39) << 1 | uVar12 >> 0x1f;
  uVar12 = uVar12 << 1 | (uVar11 ^ uVar39) >> 0x1f;
  uVar8 = uVar18 ^ uVar8;
  uVar31 = (uVar25 ^ uVar22) << 1 | uVar8 >> 0x1f;
  uVar8 = uVar8 << 1 | (uVar25 ^ uVar22) >> 0x1f;
  uVar39 = uVar40 + local_d0 + uVar45;
  uVar47 = uVar43 + iStack_cc + (uint)CARRY4(uVar40,local_d0) + uVar12 +
           (uint)CARRY4(uVar40 + local_d0,uVar45);
  uVar26 = uVar9 + local_c0 + uVar31;
  uVar29 = uVar17 + iStack_bc + (uint)CARRY4(uVar9,local_c0) + uVar8 +
           (uint)CARRY4(uVar9 + local_c0,uVar31);
  uVar15 = uVar47 ^ uVar15;
  uVar9 = uVar24 + uVar11;
  uVar7 = (uVar19 ^ uVar27) + uVar7 + (uint)CARRY4(uVar24,uVar11);
  uVar10 = uVar29 ^ uVar10;
  uVar44 = uVar7 ^ uVar44;
  uVar43 = (uVar9 ^ uVar42) >> 0x18 | uVar44 << 8;
  uVar16 = uVar44 >> 0x18 | (uVar9 ^ uVar42) << 8;
  uVar11 = uVar15 + uVar25;
  uVar25 = (uVar39 ^ uVar14) + uVar18 + (uint)CARRY4(uVar15,uVar25);
  uVar23 = uVar10 + uVar35;
  uVar28 = (uVar26 ^ uVar49) + uVar37 + (uint)CARRY4(uVar10,uVar35);
  uVar44 = uVar27 + local_d8 + uVar43;
  uVar22 = uVar33 + iStack_d4 + (uint)CARRY4(uVar27,local_d8) + uVar16 +
           (uint)CARRY4(uVar27 + local_d8,uVar43);
  uVar12 = uVar25 ^ uVar12;
  uVar18 = uVar21 + uVar20;
  uVar17 = (uVar32 ^ uVar38) + uVar13 + (uint)CARRY4(uVar21,uVar20);
  uVar8 = uVar28 ^ uVar8;
  uVar48 = uVar17 ^ uVar48;
  uVar20 = uVar22 ^ uVar19 ^ uVar27;
  uVar50 = (uVar44 ^ uVar24) >> 0x10 | uVar20 << 0x10;
  uVar27 = uVar20 >> 0x10 | (uVar44 ^ uVar24) << 0x10;
  uVar40 = (uVar11 ^ uVar45) >> 0x18 | uVar12 << 8;
  uVar35 = uVar50 + uVar9;
  uVar42 = (uVar23 ^ uVar31) >> 0x18 | uVar8 << 8;
  uVar37 = uVar27 + uVar7 + (uint)CARRY4(uVar50,uVar9);
  uVar51 = (uVar18 ^ uVar41) >> 0x18 | uVar48 << 8;
  uVar9 = uVar12 >> 0x18 | (uVar11 ^ uVar45) << 8;
  uVar16 = uVar37 ^ uVar16;
  uVar31 = uVar8 >> 0x18 | (uVar23 ^ uVar31) << 8;
  uVar24 = uVar48 >> 0x18 | (uVar18 ^ uVar41) << 8;
  uVar33 = uVar39 + local_c8 + uVar40;
  uVar47 = uVar47 + iStack_c4 + (uint)CARRY4(uVar39,local_c8) + uVar9 +
           (uint)CARRY4(uVar39 + local_c8,uVar40);
  uVar7 = uVar26 + local_b8 + uVar42;
  uVar20 = uVar29 + iStack_b4 + (uint)CARRY4(uVar26,local_b8) + uVar31 +
           (uint)CARRY4(uVar26 + local_b8,uVar42);
  uVar12 = uVar47 ^ uVar39 ^ uVar14;
  uVar39 = (uVar35 ^ uVar43) << 1 | uVar16 >> 0x1f;
  uVar29 = uVar32 + local_a8 + uVar51;
  uVar13 = uVar16 << 1 | (uVar35 ^ uVar43) >> 0x1f;
  uVar36 = uVar36 + iStack_a4 + (uint)CARRY4(uVar32,local_a8) + uVar24 +
           (uint)CARRY4(uVar32 + local_a8,uVar51);
  uVar16 = uVar20 ^ uVar26 ^ uVar49;
  uVar41 = (uVar33 ^ uVar15) >> 0x10 | uVar12 << 0x10;
  uVar14 = uVar12 >> 0x10 | (uVar33 ^ uVar15) << 0x10;
  uVar48 = uVar36 ^ uVar32 ^ uVar38;
  uVar8 = (uVar7 ^ uVar10) >> 0x10 | uVar16 << 0x10;
  uVar19 = uVar16 >> 0x10 | (uVar7 ^ uVar10) << 0x10;
  uVar12 = uVar41 + uVar11;
  uVar15 = (uVar29 ^ uVar21) >> 0x10 | uVar48 << 0x10;
  uVar25 = uVar14 + uVar25 + (uint)CARRY4(uVar41,uVar11);
  uVar48 = uVar48 >> 0x10 | (uVar29 ^ uVar21) << 0x10;
  uVar11 = uVar8 + uVar23;
  uVar21 = uVar19 + uVar28 + (uint)CARRY4(uVar8,uVar23);
  uVar16 = uVar15 + uVar18;
  uVar18 = uVar48 + uVar17 + (uint)CARRY4(uVar15,uVar18);
  uVar9 = uVar25 ^ uVar9;
  uVar31 = uVar21 ^ uVar31;
  uVar32 = uVar33 + local_100 + uVar39;
  uVar33 = uVar47 + iStack_fc + (uint)CARRY4(uVar33,local_100) + uVar13 +
           (uint)CARRY4(uVar33 + local_100,uVar39);
  uVar24 = uVar18 ^ uVar24;
  uVar38 = (uVar11 ^ uVar42) << 1 | uVar31 >> 0x1f;
  uVar10 = uVar31 << 1 | (uVar11 ^ uVar42) >> 0x1f;
  uVar19 = uVar33 ^ uVar19;
  uVar42 = (uVar12 ^ uVar40) << 1 | uVar9 >> 0x1f;
  uVar9 = uVar9 << 1 | (uVar12 ^ uVar40) >> 0x1f;
  uVar26 = uVar29 + local_b8 + uVar38;
  uVar29 = uVar36 + iStack_b4 + (uint)CARRY4(uVar29,local_b8) + uVar10 +
           (uint)CARRY4(uVar29 + local_b8,uVar38);
  uVar17 = uVar19 + uVar16;
  uVar18 = (uVar32 ^ uVar8) + uVar18 + (uint)CARRY4(uVar19,uVar16);
  uVar40 = (uVar16 ^ uVar51) << 1 | uVar24 >> 0x1f;
  uVar23 = uVar7 + local_d8 + uVar42;
  uVar16 = uVar24 << 1 | (uVar16 ^ uVar51) >> 0x1f;
  uVar28 = uVar20 + iStack_d4 + (uint)CARRY4(uVar7,local_d8) + uVar9 +
           (uint)CARRY4(uVar7 + local_d8,uVar42);
  uVar13 = uVar18 ^ uVar13;
  uVar24 = uVar44 + local_b0 + uVar40;
  uVar44 = uVar22 + iStack_ac + (uint)CARRY4(uVar44,local_b0) + uVar16 +
           (uint)CARRY4(uVar44 + local_b0,uVar40);
  uVar27 = uVar29 ^ uVar27;
  uVar20 = (uVar17 ^ uVar39) >> 0x18 | uVar13 << 8;
  uVar48 = uVar28 ^ uVar48;
  uVar31 = uVar13 >> 0x18 | (uVar17 ^ uVar39) << 8;
  uVar14 = uVar44 ^ uVar14;
  uVar19 = uVar32 + local_e0 + uVar20 ^ uVar19;
  uVar7 = uVar33 + iStack_dc + (uint)CARRY4(uVar32,local_e0) + uVar31 +
          (uint)CARRY4(uVar32 + local_e0,uVar20) ^ uVar32 ^ uVar8;
  uVar36 = uVar27 + uVar12;
  uVar22 = uVar7 >> 0x10 | uVar19 << 0x10;
  uVar47 = (uVar26 ^ uVar50) + uVar25 + (uint)CARRY4(uVar27,uVar12);
  uVar7 = uVar19 >> 0x10 | uVar7 << 0x10;
  uVar32 = uVar14 + uVar11;
  uVar33 = (uVar24 ^ uVar41) + uVar21 + (uint)CARRY4(uVar14,uVar11);
  uVar10 = uVar47 ^ uVar10;
  uVar8 = uVar48 + uVar35;
  uVar13 = (uVar23 ^ uVar15) + uVar37 + (uint)CARRY4(uVar48,uVar35);
  uVar16 = uVar33 ^ uVar16;
  uVar12 = (uVar36 ^ uVar38) >> 0x18 | uVar10 << 8;
  uVar21 = uVar10 >> 0x18 | (uVar36 ^ uVar38) << 8;
  uVar9 = uVar13 ^ uVar9;
  uVar20 = uVar20 ^ uVar7 + uVar17;
  uVar31 = uVar31 ^ uVar22 + uVar18 + (uint)CARRY4(uVar7,uVar17);
  uVar18 = uVar20 << 1 | uVar31 >> 0x1f;
  uVar17 = uVar31 << 1 | uVar20 >> 0x1f;
  uVar11 = (uVar32 ^ uVar40) >> 0x18 | uVar16 << 8;
  uVar20 = (uVar8 ^ uVar42) >> 0x18 | uVar9 << 8;
  uVar31 = uVar24 + local_d0 + uVar11;
  uVar25 = uVar44 + iStack_cc + (uint)CARRY4(uVar24,local_d0) +
           (uVar16 >> 0x18 | (uVar32 ^ uVar40) << 8) + (uint)CARRY4(uVar24 + local_d0,uVar11);
  uVar35 = uVar25 ^ uVar24 ^ uVar41;
  uVar11 = uVar23 + local_a8 + uVar20;
  uVar20 = uVar28 + iStack_a4 + (uint)CARRY4(uVar23,local_a8) +
           (uVar9 >> 0x18 | (uVar8 ^ uVar42) << 8) + (uint)CARRY4(uVar23 + local_a8,uVar20);
  uVar27 = uVar26 + local_f0 + uVar12 ^ uVar27;
  uVar10 = uVar29 + iStack_ec + (uint)CARRY4(uVar26,local_f0) + uVar21 +
           (uint)CARRY4(uVar26 + local_f0,uVar12) ^ uVar26 ^ uVar50;
  uVar24 = uVar31 + local_120[2] + uVar18;
  uVar26 = uVar25 + local_120[3] + (uint)CARRY4(uVar31,local_120[2]) + uVar17 +
           (uint)CARRY4(uVar31 + local_120[2],uVar18);
  uVar16 = (uVar31 ^ uVar14) >> 0x10 | uVar35 << 0x10;
  uVar25 = uVar20 ^ uVar23 ^ uVar15;
  uVar9 = uVar27 >> 0x10 | uVar10 << 0x10;
  uVar44 = uVar10 >> 0x10 | uVar27 << 0x10;
  uVar23 = (uVar11 ^ uVar48) >> 0x10 | uVar25 << 0x10;
  uVar19 = uVar16 + uVar32;
  uVar15 = uVar26 ^ uVar44;
  uVar10 = uVar23 + uVar8;
  uVar12 = uVar9 + uVar36 ^ uVar12;
  uVar21 = uVar44 + uVar47 + (uint)CARRY4(uVar9,uVar36) ^ uVar21;
  uVar18 = uVar15 + uVar10 ^ uVar18;
  uVar17 = (uVar24 ^ uVar9) +
           (uVar25 >> 0x10 | (uVar11 ^ uVar48) << 0x10) + uVar13 + (uint)CARRY4(uVar23,uVar8) +
           (uint)CARRY4(uVar15,uVar10) ^ uVar17;
  uVar10 = uVar12 << 1 | uVar21 >> 0x1f;
  uVar8 = uVar18 >> 0x18 | uVar17 << 8;
  uVar12 = uVar21 << 1 | uVar12 >> 0x1f;
  uVar25 = uVar24 + local_c0 + uVar8;
  uVar21 = uVar26 + iStack_bc + (uint)CARRY4(uVar24,local_c0) + (uVar17 >> 0x18 | uVar18 << 8) +
           (uint)CARRY4(uVar24 + local_c0,uVar8);
  iVar4 = 8;
  uVar9 = uVar11 + local_c8 + uVar10;
  uVar11 = uVar20 + iStack_c4 + (uint)CARRY4(uVar11,local_c8) + uVar12 +
           (uint)CARRY4(uVar11 + local_c8,uVar10);
  uVar22 = uVar11 ^ uVar22;
  uVar20 = uVar22 + uVar19;
  uVar8 = (uVar9 ^ uVar7) +
          (uVar35 >> 0x10 | (uVar31 ^ uVar14) << 0x10) + uVar33 + (uint)CARRY4(uVar16,uVar32) +
          (uint)CARRY4(uVar22,uVar19);
  uVar12 = uVar8 ^ uVar12;
  uVar13 = (uVar20 ^ uVar10) >> 0x18 | uVar12 << 8;
  uVar22 = uVar9 + local_e8 + uVar13 ^ uVar22;
  uVar7 = uVar11 + iStack_e4 + (uint)CARRY4(uVar9,local_e8) +
          (uVar12 >> 0x18 | (uVar20 ^ uVar10) << 8) + (uint)CARRY4(uVar9 + local_e8,uVar13) ^
          uVar9 ^ uVar7;
  uVar11 = uVar22 >> 0x10 | uVar7 << 0x10;
  uVar12 = uVar11 + uVar20;
  uVar11 = (uVar7 >> 0x10 | uVar22 << 0x10) + uVar8 + (uint)CARRY4(uVar11,uVar20);
  puVar6 = local_a0 + 0x10;
  puVar34 = local_a0;
  while( true ) {
    iVar4 = iVar4 + -1;
    *param_1 = *param_1 ^ uVar25 ^ uVar12;
    param_1[1] = param_1[1] ^ uVar21 ^ uVar11;
    if (iVar4 == 0) break;
    uVar25 = puVar34[2];
    uVar21 = puVar34[3];
    uVar12 = puVar6[2];
    uVar11 = puVar6[3];
    puVar6 = puVar6 + 2;
    puVar34 = puVar34 + 2;
    param_1 = param_1 + 2;
  }
  return;
}

