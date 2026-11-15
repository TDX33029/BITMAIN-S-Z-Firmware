//==================== FUN_0000fc90 @ 0000fc90 ====================

void FUN_0000fc90(uint *param_1,uint *param_2,int param_3)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  int iVar7;
  int iVar8;
  uint *puVar9;
  uint uVar10;
  uint uVar11;
  uint uVar12;
  uint *puVar13;
  uint uVar14;
  uint auStack_13c [71];
  
  if (param_3 == 0) {
    puVar13 = param_2 + 0x10;
    puVar9 = auStack_13c + 7;
    do {
      uVar3 = *param_2;
      uVar1 = param_2[1];
      uVar4 = param_2[2];
      uVar10 = param_2[3];
      param_2 = param_2 + 4;
      *puVar9 = uVar3;
      puVar9[1] = uVar1;
      puVar9[2] = uVar4;
      puVar9[3] = uVar10;
      puVar9 = puVar9 + 4;
    } while (param_2 != puVar13);
  }
  else {
    iVar8 = 0;
    do {
      uVar3 = *(uint *)((int)param_2 + iVar8);
      *(uint *)((int)auStack_13c + iVar8 + 0x1c) =
           uVar3 << 0x18 | (uVar3 >> 8 & 0xff) << 0x10 | (uVar3 >> 0x10 & 0xff) << 8 | uVar3 >> 0x18
      ;
      iVar8 = iVar8 + 4;
    } while (iVar8 != 0x40);
  }
  puVar9 = auStack_13c + 0x15;
  do {
    uVar10 = *puVar9;
    uVar3 = puVar9[1];
    uVar4 = puVar9[-0xd];
    uVar1 = puVar9[-0xc];
    puVar9[2] = puVar9[-0xe] + puVar9[-5] +
                ((uVar10 >> 0x11 | uVar10 << 0xf) ^ (uVar10 >> 0x13 | uVar10 << 0xd) ^ uVar10 >> 10)
                + ((uVar4 >> 7 | uVar4 << 0x19) ^ (uVar4 >> 0x12 | uVar4 << 0xe) ^ uVar4 >> 3);
    puVar9[3] = uVar4 + puVar9[-4] +
                ((uVar3 >> 0x11 | uVar3 << 0xf) ^ (uVar3 >> 0x13 | uVar3 << 0xd) ^ uVar3 >> 10) +
                ((uVar1 >> 7 | uVar1 << 0x19) ^ (uVar1 >> 0x12 | uVar1 << 0xe) ^ uVar1 >> 3);
    puVar9 = puVar9 + 2;
  } while (puVar9 != auStack_13c + 0x45);
  uVar3 = *param_1;
  uVar10 = param_1[1];
  uVar5 = param_1[2];
  uVar1 = param_1[4];
  uVar2 = param_1[5];
  uVar6 = param_1[6];
  iVar8 = param_1[7] + auStack_13c[7] + 0x428a2f98 +
          ((uVar1 >> 6 | uVar1 << 0x1a) ^ (uVar1 >> 0xb | uVar1 << 0x15) ^
          (uVar1 >> 0x19 | uVar1 << 7)) + ((uVar6 ^ uVar2) & uVar1 ^ uVar6);
  uVar4 = iVar8 + param_1[3];
  uVar14 = ((uVar5 | uVar10) & uVar3 | uVar5 & uVar10) +
           ((uVar3 >> 2 | uVar3 << 0x1e) ^ (uVar3 >> 0xd | uVar3 << 0x13) ^
           (uVar3 >> 0x16 | uVar3 << 10)) + iVar8;
  iVar8 = uVar6 + auStack_13c[8] + 0x71374491 + (uVar4 & (uVar2 ^ uVar1) ^ uVar2) +
          ((uVar4 >> 6 | uVar4 * 0x4000000) ^ (uVar4 >> 0xb | uVar4 * 0x200000) ^
          (uVar4 >> 0x19 | uVar4 * 0x80));
  uVar5 = iVar8 + uVar5;
  uVar12 = ((uVar14 >> 2 | uVar14 * 0x40000000) ^ (uVar14 >> 0xd | uVar14 * 0x80000) ^
           (uVar14 >> 0x16 | uVar14 * 0x400)) + (uVar14 & (uVar10 | uVar3) | uVar10 & uVar3) + iVar8
  ;
  iVar8 = uVar2 + auStack_13c[9] + -0x4a3f0431 + (uVar5 & (uVar4 ^ uVar1) ^ uVar1) +
          ((uVar5 >> 6 | uVar5 * 0x4000000) ^ (uVar5 >> 0xb | uVar5 * 0x200000) ^
          (uVar5 >> 0x19 | uVar5 * 0x80));
  uVar2 = ((uVar12 >> 2 | uVar12 * 0x40000000) ^ (uVar12 >> 0xd | uVar12 * 0x80000) ^
          (uVar12 >> 0x16 | uVar12 * 0x400)) + (uVar12 & (uVar14 | uVar3) | uVar14 & uVar3) + iVar8;
  uVar10 = iVar8 + uVar10;
  iVar8 = uVar1 + auStack_13c[10] + -0x164a245b + (uVar10 & (uVar5 ^ uVar4) ^ uVar4) +
          ((uVar10 >> 6 | uVar10 * 0x4000000) ^ (uVar10 >> 0xb | uVar10 * 0x200000) ^
          (uVar10 >> 0x19 | uVar10 * 0x80));
  uVar6 = ((uVar2 >> 2 | uVar2 * 0x40000000) ^ (uVar2 >> 0xd | uVar2 * 0x80000) ^
          (uVar2 >> 0x16 | uVar2 * 0x400)) + (uVar2 & (uVar12 | uVar14) | uVar12 & uVar14) + iVar8;
  uVar3 = iVar8 + uVar3;
  iVar8 = auStack_13c[0xb] + 0x3956c25b + uVar4 + (uVar3 & (uVar10 ^ uVar5) ^ uVar5) +
          ((uVar3 >> 6 | uVar3 * 0x4000000) ^ (uVar3 >> 0xb | uVar3 * 0x200000) ^
          (uVar3 >> 0x19 | uVar3 * 0x80));
  uVar11 = ((uVar6 >> 2 | uVar6 * 0x40000000) ^ (uVar6 >> 0xd | uVar6 * 0x80000) ^
           (uVar6 >> 0x16 | uVar6 * 0x400)) + (uVar6 & (uVar2 | uVar12) | uVar2 & uVar12) + iVar8;
  uVar14 = iVar8 + uVar14;
  iVar8 = auStack_13c[0xc] + 0x59f111f1 + uVar5 + (uVar14 & (uVar3 ^ uVar10) ^ uVar10) +
          ((uVar14 >> 6 | uVar14 * 0x4000000) ^ (uVar14 >> 0xb | uVar14 * 0x200000) ^
          (uVar14 >> 0x19 | uVar14 * 0x80));
  uVar1 = ((uVar11 >> 2 | uVar11 * 0x40000000) ^ (uVar11 >> 0xd | uVar11 * 0x80000) ^
          (uVar11 >> 0x16 | uVar11 * 0x400)) + (uVar11 & (uVar6 | uVar2) | uVar6 & uVar2) + iVar8;
  uVar12 = iVar8 + uVar12;
  iVar8 = auStack_13c[0xd] + -0x6dc07d5c + uVar10 + (uVar12 & (uVar14 ^ uVar3) ^ uVar3) +
          ((uVar12 >> 6 | uVar12 * 0x4000000) ^ (uVar12 >> 0xb | uVar12 * 0x200000) ^
          (uVar12 >> 0x19 | uVar12 * 0x80));
  uVar2 = iVar8 + uVar2;
  uVar10 = ((uVar1 >> 2 | uVar1 * 0x40000000) ^ (uVar1 >> 0xd | uVar1 * 0x80000) ^
           (uVar1 >> 0x16 | uVar1 * 0x400)) + (uVar1 & (uVar11 | uVar6) | uVar11 & uVar6) + iVar8;
  iVar8 = auStack_13c[0xe] + -0x54e3a12b + uVar3 + (uVar2 & (uVar12 ^ uVar14) ^ uVar14) +
          ((uVar2 >> 6 | uVar2 * 0x4000000) ^ (uVar2 >> 0xb | uVar2 * 0x200000) ^
          (uVar2 >> 0x19 | uVar2 * 0x80));
  uVar6 = iVar8 + uVar6;
  uVar5 = ((uVar10 >> 2 | uVar10 * 0x40000000) ^ (uVar10 >> 0xd | uVar10 * 0x80000) ^
          (uVar10 >> 0x16 | uVar10 * 0x400)) + (uVar10 & (uVar1 | uVar11) | uVar1 & uVar11) + iVar8;
  iVar8 = auStack_13c[0xf] + -0x27f85568 + uVar14 + (uVar6 & (uVar2 ^ uVar12) ^ uVar12) +
          ((uVar6 >> 6 | uVar6 * 0x4000000) ^ (uVar6 >> 0xb | uVar6 * 0x200000) ^
          (uVar6 >> 0x19 | uVar6 * 0x80));
  uVar11 = iVar8 + uVar11;
  uVar14 = ((uVar5 >> 2 | uVar5 * 0x40000000) ^ (uVar5 >> 0xd | uVar5 * 0x80000) ^
           (uVar5 >> 0x16 | uVar5 * 0x400)) + (uVar5 & (uVar10 | uVar1) | uVar10 & uVar1) + iVar8;
  iVar8 = auStack_13c[0x10] + 0x12835b01 + uVar12 + (uVar11 & (uVar6 ^ uVar2) ^ uVar2) +
          ((uVar11 >> 6 | uVar11 * 0x4000000) ^ (uVar11 >> 0xb | uVar11 * 0x200000) ^
          (uVar11 >> 0x19 | uVar11 * 0x80));
  uVar4 = ((uVar14 >> 2 | uVar14 * 0x40000000) ^ (uVar14 >> 0xd | uVar14 * 0x80000) ^
          (uVar14 >> 0x16 | uVar14 * 0x400)) + (uVar14 & (uVar5 | uVar10) | uVar5 & uVar10) + iVar8;
  uVar1 = iVar8 + uVar1;
  iVar8 = auStack_13c[0x11] + 0x243185be + uVar2 + (uVar1 & (uVar11 ^ uVar6) ^ uVar6) +
          ((uVar1 >> 6 | uVar1 * 0x4000000) ^ (uVar1 >> 0xb | uVar1 * 0x200000) ^
          (uVar1 >> 0x19 | uVar1 * 0x80));
  uVar3 = ((uVar4 >> 2 | uVar4 * 0x40000000) ^ (uVar4 >> 0xd | uVar4 * 0x80000) ^
          (uVar4 >> 0x16 | uVar4 * 0x400)) + (uVar4 & (uVar14 | uVar5) | uVar14 & uVar5) + iVar8;
  uVar10 = iVar8 + uVar10;
  iVar8 = auStack_13c[0x12] + 0x550c7dc3 + uVar6 + (uVar10 & (uVar1 ^ uVar11) ^ uVar11) +
          ((uVar10 >> 6 | uVar10 * 0x4000000) ^ (uVar10 >> 0xb | uVar10 * 0x200000) ^
          (uVar10 >> 0x19 | uVar10 * 0x80));
  uVar5 = iVar8 + uVar5;
  uVar12 = ((uVar3 >> 2 | uVar3 * 0x40000000) ^ (uVar3 >> 0xd | uVar3 * 0x80000) ^
           (uVar3 >> 0x16 | uVar3 * 0x400)) + (uVar3 & (uVar4 | uVar14) | uVar4 & uVar14) + iVar8;
  iVar8 = auStack_13c[0x13] + 0x72be5d74 + uVar11 + (uVar5 & (uVar10 ^ uVar1) ^ uVar1) +
          ((uVar5 >> 6 | uVar5 * 0x4000000) ^ (uVar5 >> 0xb | uVar5 * 0x200000) ^
          (uVar5 >> 0x19 | uVar5 * 0x80));
  uVar2 = ((uVar12 >> 2 | uVar12 * 0x40000000) ^ (uVar12 >> 0xd | uVar12 * 0x80000) ^
          (uVar12 >> 0x16 | uVar12 * 0x400)) + (uVar12 & (uVar3 | uVar4) | uVar3 & uVar4) + iVar8;
  uVar14 = iVar8 + uVar14;
  iVar8 = auStack_13c[0x14] + -0x7f214e02 + uVar1 + (uVar14 & (uVar5 ^ uVar10) ^ uVar10) +
          ((uVar14 >> 6 | uVar14 * 0x4000000) ^ (uVar14 >> 0xb | uVar14 * 0x200000) ^
          (uVar14 >> 0x19 | uVar14 * 0x80));
  uVar4 = iVar8 + uVar4;
  uVar6 = ((uVar2 >> 2 | uVar2 * 0x40000000) ^ (uVar2 >> 0xd | uVar2 * 0x80000) ^
          (uVar2 >> 0x16 | uVar2 * 0x400)) + (uVar2 & (uVar12 | uVar3) | uVar12 & uVar3) + iVar8;
  iVar8 = auStack_13c[0x15] + -0x6423f959 + uVar10 + (uVar4 & (uVar14 ^ uVar5) ^ uVar5) +
          ((uVar4 >> 6 | uVar4 * 0x4000000) ^ (uVar4 >> 0xb | uVar4 * 0x200000) ^
          (uVar4 >> 0x19 | uVar4 * 0x80));
  uVar10 = ((uVar6 >> 2 | uVar6 * 0x40000000) ^ (uVar6 >> 0xd | uVar6 * 0x80000) ^
           (uVar6 >> 0x16 | uVar6 * 0x400)) + (uVar6 & (uVar2 | uVar12) | uVar2 & uVar12) + iVar8;
  uVar3 = iVar8 + uVar3;
  iVar8 = auStack_13c[0x16] + -0x3e640e8c + uVar5 + (uVar3 & (uVar4 ^ uVar14) ^ uVar14) +
          ((uVar3 >> 6 | uVar3 * 0x4000000) ^ (uVar3 >> 0xb | uVar3 * 0x200000) ^
          (uVar3 >> 0x19 | uVar3 * 0x80));
  uVar1 = ((uVar10 >> 2 | uVar10 * 0x40000000) ^ (uVar10 >> 0xd | uVar10 * 0x80000) ^
          (uVar10 >> 0x16 | uVar10 * 0x400)) + (uVar10 & (uVar6 | uVar2) | uVar6 & uVar2) + iVar8;
  uVar12 = iVar8 + uVar12;
  iVar8 = auStack_13c[0x17] + -0x1b64963f + uVar14 + (uVar12 & (uVar3 ^ uVar4) ^ uVar4) +
          ((uVar12 >> 6 | uVar12 * 0x4000000) ^ (uVar12 >> 0xb | uVar12 * 0x200000) ^
          (uVar12 >> 0x19 | uVar12 * 0x80));
  uVar11 = ((uVar1 >> 2 | uVar1 * 0x40000000) ^ (uVar1 >> 0xd | uVar1 * 0x80000) ^
           (uVar1 >> 0x16 | uVar1 * 0x400)) + (uVar1 & (uVar10 | uVar6) | uVar10 & uVar6) + iVar8;
  uVar2 = iVar8 + uVar2;
  iVar8 = auStack_13c[0x18] + -0x1041b87a + uVar4 + (uVar2 & (uVar12 ^ uVar3) ^ uVar3) +
          ((uVar2 >> 6 | uVar2 * 0x4000000) ^ (uVar2 >> 0xb | uVar2 * 0x200000) ^
          (uVar2 >> 0x19 | uVar2 * 0x80));
  uVar6 = iVar8 + uVar6;
  uVar5 = ((uVar11 >> 2 | uVar11 * 0x40000000) ^ (uVar11 >> 0xd | uVar11 * 0x80000) ^
          (uVar11 >> 0x16 | uVar11 * 0x400)) + (uVar11 & (uVar1 | uVar10) | uVar1 & uVar10) + iVar8;
  iVar8 = auStack_13c[0x19] + 0xfc19dc6 + uVar3 + (uVar6 & (uVar2 ^ uVar12) ^ uVar12) +
          ((uVar6 >> 6 | uVar6 * 0x4000000) ^ (uVar6 >> 0xb | uVar6 * 0x200000) ^
          (uVar6 >> 0x19 | uVar6 * 0x80));
  uVar14 = ((uVar5 >> 2 | uVar5 * 0x40000000) ^ (uVar5 >> 0xd | uVar5 * 0x80000) ^
           (uVar5 >> 0x16 | uVar5 * 0x400)) + (uVar5 & (uVar11 | uVar1) | uVar11 & uVar1) + iVar8;
  uVar10 = iVar8 + uVar10;
  iVar8 = auStack_13c[0x1a] + 0x240ca1cc + uVar12 + (uVar10 & (uVar6 ^ uVar2) ^ uVar2) +
          ((uVar10 >> 6 | uVar10 * 0x4000000) ^ (uVar10 >> 0xb | uVar10 * 0x200000) ^
          (uVar10 >> 0x19 | uVar10 * 0x80));
  uVar1 = iVar8 + uVar1;
  uVar4 = ((uVar14 >> 2 | uVar14 * 0x40000000) ^ (uVar14 >> 0xd | uVar14 * 0x80000) ^
          (uVar14 >> 0x16 | uVar14 * 0x400)) + (uVar14 & (uVar5 | uVar11) | uVar5 & uVar11) + iVar8;
  iVar8 = auStack_13c[0x1b] + 0x2de92c6f + uVar2 + (uVar1 & (uVar10 ^ uVar6) ^ uVar6) +
          ((uVar1 >> 6 | uVar1 * 0x4000000) ^ (uVar1 >> 0xb | uVar1 * 0x200000) ^
          (uVar1 >> 0x19 | uVar1 * 0x80));
  uVar3 = ((uVar4 >> 2 | uVar4 * 0x40000000) ^ (uVar4 >> 0xd | uVar4 * 0x80000) ^
          (uVar4 >> 0x16 | uVar4 * 0x400)) + (uVar4 & (uVar14 | uVar5) | uVar14 & uVar5) + iVar8;
  uVar11 = iVar8 + uVar11;
  iVar8 = auStack_13c[0x1c] + 0x4a7484aa + uVar6 + (uVar11 & (uVar1 ^ uVar10) ^ uVar10) +
          ((uVar11 >> 6 | uVar11 * 0x4000000) ^ (uVar11 >> 0xb | uVar11 * 0x200000) ^
          (uVar11 >> 0x19 | uVar11 * 0x80));
  uVar12 = ((uVar3 >> 2 | uVar3 * 0x40000000) ^ (uVar3 >> 0xd | uVar3 * 0x80000) ^
           (uVar3 >> 0x16 | uVar3 * 0x400)) + (uVar3 & (uVar4 | uVar14) | uVar4 & uVar14) + iVar8;
  uVar5 = iVar8 + uVar5;
  iVar8 = auStack_13c[0x1d] + 0x5cb0a9dc + uVar10 + (uVar5 & (uVar11 ^ uVar1) ^ uVar1) +
          ((uVar5 >> 6 | uVar5 * 0x4000000) ^ (uVar5 >> 0xb | uVar5 * 0x200000) ^
          (uVar5 >> 0x19 | uVar5 * 0x80));
  uVar2 = ((uVar12 >> 2 | uVar12 * 0x40000000) ^ (uVar12 >> 0xd | uVar12 * 0x80000) ^
          (uVar12 >> 0x16 | uVar12 * 0x400)) + (uVar12 & (uVar3 | uVar4) | uVar3 & uVar4) + iVar8;
  uVar14 = iVar8 + uVar14;
  iVar8 = auStack_13c[0x1e] + 0x76f988da + uVar1 + (uVar14 & (uVar5 ^ uVar11) ^ uVar11) +
          ((uVar14 >> 6 | uVar14 * 0x4000000) ^ (uVar14 >> 0xb | uVar14 * 0x200000) ^
          (uVar14 >> 0x19 | uVar14 * 0x80));
  uVar4 = iVar8 + uVar4;
  uVar6 = ((uVar2 >> 2 | uVar2 * 0x40000000) ^ (uVar2 >> 0xd | uVar2 * 0x80000) ^
          (uVar2 >> 0x16 | uVar2 * 0x400)) + (uVar2 & (uVar12 | uVar3) | uVar12 & uVar3) + iVar8;
  iVar8 = auStack_13c[0x1f] + -0x67c1aeae + uVar11 + (uVar4 & (uVar14 ^ uVar5) ^ uVar5) +
          ((uVar4 >> 6 | uVar4 * 0x4000000) ^ (uVar4 >> 0xb | uVar4 * 0x200000) ^
          (uVar4 >> 0x19 | uVar4 * 0x80));
  uVar10 = ((uVar6 >> 2 | uVar6 * 0x40000000) ^ (uVar6 >> 0xd | uVar6 * 0x80000) ^
           (uVar6 >> 0x16 | uVar6 * 0x400)) + (uVar6 & (uVar2 | uVar12) | uVar2 & uVar12) + iVar8;
  uVar3 = iVar8 + uVar3;
  iVar8 = auStack_13c[0x20] + -0x57ce3993 + uVar5 + (uVar3 & (uVar4 ^ uVar14) ^ uVar14) +
          ((uVar3 >> 6 | uVar3 * 0x4000000) ^ (uVar3 >> 0xb | uVar3 * 0x200000) ^
          (uVar3 >> 0x19 | uVar3 * 0x80));
  uVar12 = iVar8 + uVar12;
  uVar1 = ((uVar10 >> 2 | uVar10 * 0x40000000) ^ (uVar10 >> 0xd | uVar10 * 0x80000) ^
          (uVar10 >> 0x16 | uVar10 * 0x400)) + (uVar10 & (uVar6 | uVar2) | uVar6 & uVar2) + iVar8;
  iVar8 = auStack_13c[0x21] + -0x4ffcd838 + uVar14 + (uVar12 & (uVar3 ^ uVar4) ^ uVar4) +
          ((uVar12 >> 6 | uVar12 * 0x4000000) ^ (uVar12 >> 0xb | uVar12 * 0x200000) ^
          (uVar12 >> 0x19 | uVar12 * 0x80));
  uVar11 = ((uVar1 >> 2 | uVar1 * 0x40000000) ^ (uVar1 >> 0xd | uVar1 * 0x80000) ^
           (uVar1 >> 0x16 | uVar1 * 0x400)) + (uVar1 & (uVar10 | uVar6) | uVar10 & uVar6) + iVar8;
  uVar2 = iVar8 + uVar2;
  iVar8 = auStack_13c[0x22] + -0x40a68039 + uVar4 + (uVar2 & (uVar12 ^ uVar3) ^ uVar3) +
          ((uVar2 >> 6 | uVar2 * 0x4000000) ^ (uVar2 >> 0xb | uVar2 * 0x200000) ^
          (uVar2 >> 0x19 | uVar2 * 0x80));
  uVar5 = ((uVar11 >> 2 | uVar11 * 0x40000000) ^ (uVar11 >> 0xd | uVar11 * 0x80000) ^
          (uVar11 >> 0x16 | uVar11 * 0x400)) + (uVar11 & (uVar1 | uVar10) | uVar1 & uVar10) + iVar8;
  uVar6 = iVar8 + uVar6;
  iVar8 = auStack_13c[0x23] + -0x391ff40d + uVar3 + (uVar6 & (uVar2 ^ uVar12) ^ uVar12) +
          ((uVar6 >> 6 | uVar6 * 0x4000000) ^ (uVar6 >> 0xb | uVar6 * 0x200000) ^
          (uVar6 >> 0x19 | uVar6 * 0x80));
  uVar14 = ((uVar5 >> 2 | uVar5 * 0x40000000) ^ (uVar5 >> 0xd | uVar5 * 0x80000) ^
           (uVar5 >> 0x16 | uVar5 * 0x400)) + (uVar5 & (uVar11 | uVar1) | uVar11 & uVar1) + iVar8;
  uVar10 = iVar8 + uVar10;
  iVar8 = auStack_13c[0x24] + -0x2a586eb9 + uVar12 + (uVar10 & (uVar6 ^ uVar2) ^ uVar2) +
          ((uVar10 >> 6 | uVar10 * 0x4000000) ^ (uVar10 >> 0xb | uVar10 * 0x200000) ^
          (uVar10 >> 0x19 | uVar10 * 0x80));
  uVar1 = iVar8 + uVar1;
  uVar4 = ((uVar14 >> 2 | uVar14 * 0x40000000) ^ (uVar14 >> 0xd | uVar14 * 0x80000) ^
          (uVar14 >> 0x16 | uVar14 * 0x400)) + (uVar14 & (uVar5 | uVar11) | uVar5 & uVar11) + iVar8;
  iVar8 = auStack_13c[0x25] + 0x6ca6351 + uVar2 + (uVar1 & (uVar10 ^ uVar6) ^ uVar6) +
          ((uVar1 >> 6 | uVar1 * 0x4000000) ^ (uVar1 >> 0xb | uVar1 * 0x200000) ^
          (uVar1 >> 0x19 | uVar1 * 0x80));
  uVar3 = ((uVar4 >> 2 | uVar4 * 0x40000000) ^ (uVar4 >> 0xd | uVar4 * 0x80000) ^
          (uVar4 >> 0x16 | uVar4 * 0x400)) + (uVar4 & (uVar14 | uVar5) | uVar14 & uVar5) + iVar8;
  uVar11 = iVar8 + uVar11;
  iVar8 = auStack_13c[0x26] + 0x14292967 + uVar6 + (uVar11 & (uVar1 ^ uVar10) ^ uVar10) +
          ((uVar11 >> 6 | uVar11 * 0x4000000) ^ (uVar11 >> 0xb | uVar11 * 0x200000) ^
          (uVar11 >> 0x19 | uVar11 * 0x80));
  uVar5 = iVar8 + uVar5;
  uVar12 = ((uVar3 >> 2 | uVar3 * 0x40000000) ^ (uVar3 >> 0xd | uVar3 * 0x80000) ^
           (uVar3 >> 0x16 | uVar3 * 0x400)) + (uVar3 & (uVar4 | uVar14) | uVar4 & uVar14) + iVar8;
  iVar8 = auStack_13c[0x27] + 0x27b70a85 + uVar10 + (uVar5 & (uVar11 ^ uVar1) ^ uVar1) +
          ((uVar5 >> 6 | uVar5 * 0x4000000) ^ (uVar5 >> 0xb | uVar5 * 0x200000) ^
          (uVar5 >> 0x19 | uVar5 * 0x80));
  uVar2 = ((uVar12 >> 2 | uVar12 * 0x40000000) ^ (uVar12 >> 0xd | uVar12 * 0x80000) ^
          (uVar12 >> 0x16 | uVar12 * 0x400)) + (uVar12 & (uVar3 | uVar4) | uVar3 & uVar4) + iVar8;
  uVar14 = iVar8 + uVar14;
  iVar8 = auStack_13c[0x28] + 0x2e1b2138 + uVar1 + (uVar14 & (uVar5 ^ uVar11) ^ uVar11) +
          ((uVar14 >> 6 | uVar14 * 0x4000000) ^ (uVar14 >> 0xb | uVar14 * 0x200000) ^
          (uVar14 >> 0x19 | uVar14 * 0x80));
  uVar6 = ((uVar2 >> 2 | uVar2 * 0x40000000) ^ (uVar2 >> 0xd | uVar2 * 0x80000) ^
          (uVar2 >> 0x16 | uVar2 * 0x400)) + (uVar2 & (uVar12 | uVar3) | uVar12 & uVar3) + iVar8;
  uVar4 = iVar8 + uVar4;
  iVar8 = auStack_13c[0x29] + 0x4d2c6dfc + uVar11 + (uVar4 & (uVar14 ^ uVar5) ^ uVar5) +
          ((uVar4 >> 6 | uVar4 * 0x4000000) ^ (uVar4 >> 0xb | uVar4 * 0x200000) ^
          (uVar4 >> 0x19 | uVar4 * 0x80));
  uVar10 = ((uVar6 >> 2 | uVar6 * 0x40000000) ^ (uVar6 >> 0xd | uVar6 * 0x80000) ^
           (uVar6 >> 0x16 | uVar6 * 0x400)) + (uVar6 & (uVar2 | uVar12) | uVar2 & uVar12) + iVar8;
  uVar3 = iVar8 + uVar3;
  iVar8 = auStack_13c[0x2a] + 0x53380d13 + uVar5 + (uVar3 & (uVar4 ^ uVar14) ^ uVar14) +
          ((uVar3 >> 6 | uVar3 * 0x4000000) ^ (uVar3 >> 0xb | uVar3 * 0x200000) ^
          (uVar3 >> 0x19 | uVar3 * 0x80));
  uVar12 = iVar8 + uVar12;
  uVar1 = ((uVar10 >> 2 | uVar10 * 0x40000000) ^ (uVar10 >> 0xd | uVar10 * 0x80000) ^
          (uVar10 >> 0x16 | uVar10 * 0x400)) + (uVar10 & (uVar6 | uVar2) | uVar6 & uVar2) + iVar8;
  iVar8 = auStack_13c[0x2b] + 0x650a7354 + uVar14 + (uVar12 & (uVar3 ^ uVar4) ^ uVar4) +
          ((uVar12 >> 6 | uVar12 * 0x4000000) ^ (uVar12 >> 0xb | uVar12 * 0x200000) ^
          (uVar12 >> 0x19 | uVar12 * 0x80));
  uVar11 = ((uVar1 >> 2 | uVar1 * 0x40000000) ^ (uVar1 >> 0xd | uVar1 * 0x80000) ^
           (uVar1 >> 0x16 | uVar1 * 0x400)) + (uVar1 & (uVar10 | uVar6) | uVar10 & uVar6) + iVar8;
  uVar2 = iVar8 + uVar2;
  iVar8 = auStack_13c[0x2c] + 0x766a0abb + uVar4 + (uVar2 & (uVar12 ^ uVar3) ^ uVar3) +
          ((uVar2 >> 6 | uVar2 * 0x4000000) ^ (uVar2 >> 0xb | uVar2 * 0x200000) ^
          (uVar2 >> 0x19 | uVar2 * 0x80));
  uVar6 = iVar8 + uVar6;
  uVar5 = ((uVar11 >> 2 | uVar11 * 0x40000000) ^ (uVar11 >> 0xd | uVar11 * 0x80000) ^
          (uVar11 >> 0x16 | uVar11 * 0x400)) + (uVar11 & (uVar1 | uVar10) | uVar1 & uVar10) + iVar8;
  iVar8 = auStack_13c[0x2d] + -0x7e3d36d2 + uVar3 + (uVar6 & (uVar2 ^ uVar12) ^ uVar12) +
          ((uVar6 >> 6 | uVar6 * 0x4000000) ^ (uVar6 >> 0xb | uVar6 * 0x200000) ^
          (uVar6 >> 0x19 | uVar6 * 0x80));
  uVar14 = ((uVar5 >> 2 | uVar5 * 0x40000000) ^ (uVar5 >> 0xd | uVar5 * 0x80000) ^
           (uVar5 >> 0x16 | uVar5 * 0x400)) + (uVar5 & (uVar11 | uVar1) | uVar11 & uVar1) + iVar8;
  uVar10 = iVar8 + uVar10;
  iVar8 = auStack_13c[0x2e] + -0x6d8dd37b + uVar12 + (uVar10 & (uVar6 ^ uVar2) ^ uVar2) +
          ((uVar10 >> 6 | uVar10 * 0x4000000) ^ (uVar10 >> 0xb | uVar10 * 0x200000) ^
          (uVar10 >> 0x19 | uVar10 * 0x80));
  uVar4 = ((uVar14 >> 2 | uVar14 * 0x40000000) ^ (uVar14 >> 0xd | uVar14 * 0x80000) ^
          (uVar14 >> 0x16 | uVar14 * 0x400)) + (uVar14 & (uVar5 | uVar11) | uVar5 & uVar11) + iVar8;
  uVar1 = iVar8 + uVar1;
  iVar8 = auStack_13c[0x2f] + -0x5d40175f + uVar2 + (uVar1 & (uVar10 ^ uVar6) ^ uVar6) +
          ((uVar1 >> 6 | uVar1 * 0x4000000) ^ (uVar1 >> 0xb | uVar1 * 0x200000) ^
          (uVar1 >> 0x19 | uVar1 * 0x80));
  uVar3 = ((uVar4 >> 2 | uVar4 * 0x40000000) ^ (uVar4 >> 0xd | uVar4 * 0x80000) ^
          (uVar4 >> 0x16 | uVar4 * 0x400)) + (uVar4 & (uVar14 | uVar5) | uVar14 & uVar5) + iVar8;
  uVar11 = iVar8 + uVar11;
  iVar8 = auStack_13c[0x30] + -0x57e599b5 + uVar6 + (uVar11 & (uVar1 ^ uVar10) ^ uVar10) +
          ((uVar11 >> 6 | uVar11 * 0x4000000) ^ (uVar11 >> 0xb | uVar11 * 0x200000) ^
          (uVar11 >> 0x19 | uVar11 * 0x80));
  uVar5 = iVar8 + uVar5;
  uVar12 = ((uVar3 >> 2 | uVar3 * 0x40000000) ^ (uVar3 >> 0xd | uVar3 * 0x80000) ^
           (uVar3 >> 0x16 | uVar3 * 0x400)) + (uVar3 & (uVar4 | uVar14) | uVar4 & uVar14) + iVar8;
  iVar8 = auStack_13c[0x31] + -0x3db47490 + uVar10 + (uVar5 & (uVar11 ^ uVar1) ^ uVar1) +
          ((uVar5 >> 6 | uVar5 * 0x4000000) ^ (uVar5 >> 0xb | uVar5 * 0x200000) ^
          (uVar5 >> 0x19 | uVar5 * 0x80));
  uVar2 = ((uVar12 >> 2 | uVar12 * 0x40000000) ^ (uVar12 >> 0xd | uVar12 * 0x80000) ^
          (uVar12 >> 0x16 | uVar12 * 0x400)) + (uVar12 & (uVar3 | uVar4) | uVar3 & uVar4) + iVar8;
  uVar14 = iVar8 + uVar14;
  iVar8 = auStack_13c[0x32] + -0x3893ae5d + uVar1 + (uVar14 & (uVar5 ^ uVar11) ^ uVar11) +
          ((uVar14 >> 6 | uVar14 * 0x4000000) ^ (uVar14 >> 0xb | uVar14 * 0x200000) ^
          (uVar14 >> 0x19 | uVar14 * 0x80));
  uVar4 = iVar8 + uVar4;
  uVar6 = ((uVar2 >> 2 | uVar2 * 0x40000000) ^ (uVar2 >> 0xd | uVar2 * 0x80000) ^
          (uVar2 >> 0x16 | uVar2 * 0x400)) + (uVar2 & (uVar12 | uVar3) | uVar12 & uVar3) + iVar8;
  iVar8 = auStack_13c[0x33] + -0x2e6d17e7 + uVar11 + (uVar4 & (uVar14 ^ uVar5) ^ uVar5) +
          ((uVar4 >> 6 | uVar4 * 0x4000000) ^ (uVar4 >> 0xb | uVar4 * 0x200000) ^
          (uVar4 >> 0x19 | uVar4 * 0x80));
  uVar10 = ((uVar6 >> 2 | uVar6 * 0x40000000) ^ (uVar6 >> 0xd | uVar6 * 0x80000) ^
           (uVar6 >> 0x16 | uVar6 * 0x400)) + (uVar6 & (uVar2 | uVar12) | uVar2 & uVar12) + iVar8;
  uVar3 = iVar8 + uVar3;
  iVar8 = auStack_13c[0x34] + -0x2966f9dc + uVar5 + (uVar3 & (uVar4 ^ uVar14) ^ uVar14) +
          ((uVar3 >> 6 | uVar3 * 0x4000000) ^ (uVar3 >> 0xb | uVar3 * 0x200000) ^
          (uVar3 >> 0x19 | uVar3 * 0x80));
  uVar1 = ((uVar10 >> 2 | uVar10 * 0x40000000) ^ (uVar10 >> 0xd | uVar10 * 0x80000) ^
          (uVar10 >> 0x16 | uVar10 * 0x400)) + (uVar10 & (uVar6 | uVar2) | uVar6 & uVar2) + iVar8;
  uVar12 = iVar8 + uVar12;
  iVar8 = auStack_13c[0x35] + -0xbf1ca7b + uVar14 + (uVar12 & (uVar3 ^ uVar4) ^ uVar4) +
          ((uVar12 >> 6 | uVar12 * 0x4000000) ^ (uVar12 >> 0xb | uVar12 * 0x200000) ^
          (uVar12 >> 0x19 | uVar12 * 0x80));
  uVar11 = ((uVar1 >> 2 | uVar1 * 0x40000000) ^ (uVar1 >> 0xd | uVar1 * 0x80000) ^
           (uVar1 >> 0x16 | uVar1 * 0x400)) + (uVar1 & (uVar10 | uVar6) | uVar10 & uVar6) + iVar8;
  uVar2 = iVar8 + uVar2;
  iVar8 = auStack_13c[0x36] + 0x106aa070 + uVar4 + (uVar2 & (uVar12 ^ uVar3) ^ uVar3) +
          ((uVar2 >> 6 | uVar2 * 0x4000000) ^ (uVar2 >> 0xb | uVar2 * 0x200000) ^
          (uVar2 >> 0x19 | uVar2 * 0x80));
  uVar6 = iVar8 + uVar6;
  uVar5 = ((uVar11 >> 2 | uVar11 * 0x40000000) ^ (uVar11 >> 0xd | uVar11 * 0x80000) ^
          (uVar11 >> 0x16 | uVar11 * 0x400)) + (uVar11 & (uVar1 | uVar10) | uVar1 & uVar10) + iVar8;
  iVar8 = auStack_13c[0x37] + 0x19a4c116 + uVar3 + (uVar6 & (uVar2 ^ uVar12) ^ uVar12) +
          ((uVar6 >> 6 | uVar6 * 0x4000000) ^ (uVar6 >> 0xb | uVar6 * 0x200000) ^
          (uVar6 >> 0x19 | uVar6 * 0x80));
  uVar14 = ((uVar5 >> 2 | uVar5 * 0x40000000) ^ (uVar5 >> 0xd | uVar5 * 0x80000) ^
           (uVar5 >> 0x16 | uVar5 * 0x400)) + (uVar5 & (uVar11 | uVar1) | uVar11 & uVar1) + iVar8;
  uVar10 = iVar8 + uVar10;
  iVar8 = auStack_13c[0x38] + 0x1e376c08 + uVar12 + (uVar10 & (uVar6 ^ uVar2) ^ uVar2) +
          ((uVar10 >> 6 | uVar10 * 0x4000000) ^ (uVar10 >> 0xb | uVar10 * 0x200000) ^
          (uVar10 >> 0x19 | uVar10 * 0x80));
  uVar1 = iVar8 + uVar1;
  uVar4 = ((uVar14 >> 2 | uVar14 * 0x40000000) ^ (uVar14 >> 0xd | uVar14 * 0x80000) ^
          (uVar14 >> 0x16 | uVar14 * 0x400)) + (uVar14 & (uVar5 | uVar11) | uVar5 & uVar11) + iVar8;
  iVar8 = auStack_13c[0x39] + 0x2748774c + uVar2 + (uVar1 & (uVar10 ^ uVar6) ^ uVar6) +
          ((uVar1 >> 6 | uVar1 * 0x4000000) ^ (uVar1 >> 0xb | uVar1 * 0x200000) ^
          (uVar1 >> 0x19 | uVar1 * 0x80));
  uVar3 = ((uVar4 >> 2 | uVar4 * 0x40000000) ^ (uVar4 >> 0xd | uVar4 * 0x80000) ^
          (uVar4 >> 0x16 | uVar4 * 0x400)) + (uVar4 & (uVar14 | uVar5) | uVar14 & uVar5) + iVar8;
  uVar11 = iVar8 + uVar11;
  iVar8 = auStack_13c[0x3a] + 0x34b0bcb5 + uVar6 + (uVar11 & (uVar1 ^ uVar10) ^ uVar10) +
          ((uVar11 >> 6 | uVar11 * 0x4000000) ^ (uVar11 >> 0xb | uVar11 * 0x200000) ^
          (uVar11 >> 0x19 | uVar11 * 0x80));
  uVar12 = ((uVar3 >> 2 | uVar3 * 0x40000000) ^ (uVar3 >> 0xd | uVar3 * 0x80000) ^
           (uVar3 >> 0x16 | uVar3 * 0x400)) + (uVar3 & (uVar4 | uVar14) | uVar4 & uVar14) + iVar8;
  uVar5 = iVar8 + uVar5;
  iVar8 = auStack_13c[0x3b] + 0x391c0cb3 + uVar10 + (uVar5 & (uVar11 ^ uVar1) ^ uVar1) +
          ((uVar5 >> 6 | uVar5 * 0x4000000) ^ (uVar5 >> 0xb | uVar5 * 0x200000) ^
          (uVar5 >> 0x19 | uVar5 * 0x80));
  uVar2 = ((uVar12 >> 2 | uVar12 * 0x40000000) ^ (uVar12 >> 0xd | uVar12 * 0x80000) ^
          (uVar12 >> 0x16 | uVar12 * 0x400)) + (uVar12 & (uVar3 | uVar4) | uVar3 & uVar4) + iVar8;
  uVar14 = iVar8 + uVar14;
  iVar8 = auStack_13c[0x3c] + 0x4ed8aa4a + uVar1 + (uVar14 & (uVar5 ^ uVar11) ^ uVar11) +
          ((uVar14 >> 6 | uVar14 * 0x4000000) ^ (uVar14 >> 0xb | uVar14 * 0x200000) ^
          (uVar14 >> 0x19 | uVar14 * 0x80));
  uVar4 = iVar8 + uVar4;
  uVar6 = ((uVar2 >> 2 | uVar2 * 0x40000000) ^ (uVar2 >> 0xd | uVar2 * 0x80000) ^
          (uVar2 >> 0x16 | uVar2 * 0x400)) + (uVar2 & (uVar12 | uVar3) | uVar12 & uVar3) + iVar8;
  iVar8 = auStack_13c[0x3d] + 0x5b9cca4f + uVar11 + (uVar4 & (uVar14 ^ uVar5) ^ uVar5) +
          ((uVar4 >> 6 | uVar4 * 0x4000000) ^ (uVar4 >> 0xb | uVar4 * 0x200000) ^
          (uVar4 >> 0x19 | uVar4 * 0x80));
  uVar10 = ((uVar6 >> 2 | uVar6 * 0x40000000) ^ (uVar6 >> 0xd | uVar6 * 0x80000) ^
           (uVar6 >> 0x16 | uVar6 * 0x400)) + (uVar6 & (uVar2 | uVar12) | uVar2 & uVar12) + iVar8;
  uVar3 = iVar8 + uVar3;
  iVar8 = auStack_13c[0x3e] + 0x682e6ff3 + uVar5 + (uVar3 & (uVar4 ^ uVar14) ^ uVar14) +
          ((uVar3 >> 6 | uVar3 * 0x4000000) ^ (uVar3 >> 0xb | uVar3 * 0x200000) ^
          (uVar3 >> 0x19 | uVar3 * 0x80));
  uVar12 = iVar8 + uVar12;
  uVar1 = ((uVar10 >> 2 | uVar10 * 0x40000000) ^ (uVar10 >> 0xd | uVar10 * 0x80000) ^
          (uVar10 >> 0x16 | uVar10 * 0x400)) + (uVar10 & (uVar6 | uVar2) | uVar6 & uVar2) + iVar8;
  iVar8 = auStack_13c[0x3f] + 0x748f82ee + uVar14 + (uVar12 & (uVar3 ^ uVar4) ^ uVar4) +
          ((uVar12 >> 6 | uVar12 * 0x4000000) ^ (uVar12 >> 0xb | uVar12 * 0x200000) ^
          (uVar12 >> 0x19 | uVar12 * 0x80));
  auStack_13c[6] =
       ((uVar1 >> 2 | uVar1 * 0x40000000) ^ (uVar1 >> 0xd | uVar1 * 0x80000) ^
       (uVar1 >> 0x16 | uVar1 * 0x400)) + (uVar1 & (uVar10 | uVar6) | uVar10 & uVar6) + iVar8;
  uVar2 = iVar8 + uVar2;
  iVar8 = auStack_13c[0x40] + 0x78a5636f + uVar4 + (uVar2 & (uVar12 ^ uVar3) ^ uVar3) +
          ((uVar2 >> 6 | uVar2 * 0x4000000) ^ (uVar2 >> 0xb | uVar2 * 0x200000) ^
          (uVar2 >> 0x19 | uVar2 * 0x80));
  auStack_13c[5] =
       ((auStack_13c[6] >> 2 | auStack_13c[6] * 0x40000000) ^
        (auStack_13c[6] >> 0xd | auStack_13c[6] * 0x80000) ^
       (auStack_13c[6] >> 0x16 | auStack_13c[6] * 0x400)) +
       (auStack_13c[6] & (uVar1 | uVar10) | uVar1 & uVar10) + iVar8;
  uVar6 = iVar8 + uVar6;
  iVar8 = auStack_13c[0x41] + -0x7b3787ec + uVar3 + (uVar6 & (uVar2 ^ uVar12) ^ uVar12) +
          ((uVar6 >> 6 | uVar6 * 0x4000000) ^ (uVar6 >> 0xb | uVar6 * 0x200000) ^
          (uVar6 >> 0x19 | uVar6 * 0x80));
  uVar10 = iVar8 + uVar10;
  auStack_13c[4] =
       ((auStack_13c[5] >> 2 | auStack_13c[5] * 0x40000000) ^
        (auStack_13c[5] >> 0xd | auStack_13c[5] * 0x80000) ^
       (auStack_13c[5] >> 0x16 | auStack_13c[5] * 0x400)) +
       (auStack_13c[5] & (auStack_13c[6] | uVar1) | auStack_13c[6] & uVar1) + iVar8;
  iVar8 = auStack_13c[0x42] + -0x7338fdf8 + uVar12 + (uVar10 & (uVar6 ^ uVar2) ^ uVar2) +
          ((uVar10 >> 6 | uVar10 * 0x4000000) ^ (uVar10 >> 0xb | uVar10 * 0x200000) ^
          (uVar10 >> 0x19 | uVar10 * 0x80));
  uVar1 = iVar8 + uVar1;
  uVar3 = ((auStack_13c[4] >> 2 | auStack_13c[4] * 0x40000000) ^
           (auStack_13c[4] >> 0xd | auStack_13c[4] * 0x80000) ^
          (auStack_13c[4] >> 0x16 | auStack_13c[4] * 0x400)) +
          (auStack_13c[4] & (auStack_13c[5] | auStack_13c[6]) | auStack_13c[5] & auStack_13c[6]) +
          iVar8;
  iVar8 = auStack_13c[0x43] + -0x6f410006 + uVar2 + (uVar1 & (uVar10 ^ uVar6) ^ uVar6) +
          ((uVar1 >> 6 | uVar1 * 0x4000000) ^ (uVar1 >> 0xb | uVar1 * 0x200000) ^
          (uVar1 >> 0x19 | uVar1 * 0x80));
  auStack_13c[6] = iVar8 + auStack_13c[6];
  auStack_13c[2] =
       ((uVar3 >> 2 | uVar3 * 0x40000000) ^ (uVar3 >> 0xd | uVar3 * 0x80000) ^
       (uVar3 >> 0x16 | uVar3 * 0x400)) +
       (uVar3 & (auStack_13c[4] | auStack_13c[5]) | auStack_13c[4] & auStack_13c[5]) + iVar8;
  iVar8 = auStack_13c[0x44] + -0x5baf9315 + uVar6 + (auStack_13c[6] & (uVar1 ^ uVar10) ^ uVar10) +
          ((auStack_13c[6] >> 6 | auStack_13c[6] * 0x4000000) ^
           (auStack_13c[6] >> 0xb | auStack_13c[6] * 0x200000) ^
          (auStack_13c[6] >> 0x19 | auStack_13c[6] * 0x80));
  auStack_13c[1] =
       ((auStack_13c[2] >> 2 | auStack_13c[2] * 0x40000000) ^
        (auStack_13c[2] >> 0xd | auStack_13c[2] * 0x80000) ^
       (auStack_13c[2] >> 0x16 | auStack_13c[2] * 0x400)) +
       (auStack_13c[2] & (uVar3 | auStack_13c[4]) | uVar3 & auStack_13c[4]) + iVar8;
  auStack_13c[5] = iVar8 + auStack_13c[5];
  iVar8 = auStack_13c[0x45] + -0x41065c09 + uVar10 +
          (auStack_13c[5] & (auStack_13c[6] ^ uVar1) ^ uVar1) +
          ((auStack_13c[5] >> 6 | auStack_13c[5] * 0x4000000) ^
           (auStack_13c[5] >> 0xb | auStack_13c[5] * 0x200000) ^
          (auStack_13c[5] >> 0x19 | auStack_13c[5] * 0x80));
  auStack_13c[4] = iVar8 + auStack_13c[4];
  auStack_13c[0] =
       ((auStack_13c[1] >> 2 | auStack_13c[1] * 0x40000000) ^
        (auStack_13c[1] >> 0xd | auStack_13c[1] * 0x80000) ^
       (auStack_13c[1] >> 0x16 | auStack_13c[1] * 0x400)) +
       (auStack_13c[1] & (auStack_13c[2] | uVar3) | auStack_13c[2] & uVar3) + iVar8;
  iVar8 = auStack_13c[0x46] + -0x398e870e + uVar1 +
          (auStack_13c[4] & (auStack_13c[5] ^ auStack_13c[6]) ^ auStack_13c[6]) +
          ((auStack_13c[4] >> 6 | auStack_13c[4] * 0x4000000) ^
           (auStack_13c[4] >> 0xb | auStack_13c[4] * 0x200000) ^
          (auStack_13c[4] >> 0x19 | auStack_13c[4] * 0x80));
  iVar7 = ((auStack_13c[0] >> 2 | auStack_13c[0] * 0x40000000) ^
           (auStack_13c[0] >> 0xd | auStack_13c[0] * 0x80000) ^
          (auStack_13c[0] >> 0x16 | auStack_13c[0] * 0x400)) +
          (auStack_13c[0] & (auStack_13c[1] | auStack_13c[2]) | auStack_13c[1] & auStack_13c[2]) +
          iVar8;
  auStack_13c[3] = iVar8 + uVar3;
  iVar8 = 0;
  while( true ) {
    *(int *)((int)param_1 + iVar8) = *(int *)((int)param_1 + iVar8) + iVar7;
    if (iVar8 + 4 == 0x20) break;
    iVar7 = *(int *)((int)auStack_13c + iVar8);
    iVar8 = iVar8 + 4;
  }
  return;
}

