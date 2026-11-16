//==================== FUN_000099a0 @ 000099a0 ====================

void FUN_000099a0(int param_1,uint *param_2)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  uint local_74;
  uint local_70;
  uint local_6c;
  uint local_68;
  uint local_64;
  uint local_60;
  uint local_5c;
  uint local_58;
  uint local_54;
  uint local_50;
  uint local_4c;
  uint local_48;
  uint local_44;
  uint local_40;
  uint local_3c;
  uint local_38;
  uint local_34 [8];
  uint local_14;
  
  FUN_00002200(local_34,param_1,0x20);
  for (local_14 = 0; local_14 < 0x40; local_14 = local_14 + 0x10) {
    if (local_14 == 0) {
      local_74 = *param_2;
    }
    else {
      local_74 = local_74 +
                 ((local_3c >> 0x11 | local_3c << 0xf) ^ (local_3c >> 0x13 | local_3c << 0xd) ^
                 local_3c >> 10) + local_50 +
                 ((local_70 >> 7 | local_70 << 0x19) ^ (local_70 >> 0x12 | local_70 << 0xe) ^
                 local_70 >> 3);
    }
    iVar1 = local_34[7] +
            ((local_34[4] >> 6 | local_34[4] << 0x1a) ^ (local_34[4] >> 0xb | local_34[4] << 0x15) ^
            (local_34[4] >> 0x19 | local_34[4] << 7)) +
            (local_34[6] ^ local_34[4] & (local_34[5] ^ local_34[6])) +
            *(int *)(s_0_00015bb7 + local_14 * 4 + 1) + local_74;
    local_34[3] = local_34[3] + iVar1;
    uVar2 = iVar1 + ((local_34[0] >> 2 | local_34[0] << 0x1e) ^
                     (local_34[0] >> 0xd | local_34[0] << 0x13) ^
                    (local_34[0] >> 0x16 | local_34[0] << 10)) +
                    (local_34[1] ^ (local_34[0] ^ local_34[1]) & (local_34[1] ^ local_34[2]));
    if (local_14 == 0) {
      local_70 = param_2[1];
    }
    else {
      local_70 = local_70 +
                 ((local_38 >> 0x11 | local_38 << 0xf) ^ (local_38 >> 0x13 | local_38 << 0xd) ^
                 local_38 >> 10) + local_4c +
                 ((local_6c >> 7 | local_6c << 0x19) ^ (local_6c >> 0x12 | local_6c << 0xe) ^
                 local_6c >> 3);
    }
    iVar1 = local_34[6] +
            ((local_34[3] >> 6 | local_34[3] * 0x4000000) ^
             (local_34[3] >> 0xb | local_34[3] * 0x200000) ^
            (local_34[3] >> 0x19 | local_34[3] * 0x80)) +
            (local_34[5] ^ local_34[3] & (local_34[4] ^ local_34[5])) +
            *(int *)(s_0_00015bb7 + (local_14 + 1) * 4 + 1) + local_70;
    local_34[2] = local_34[2] + iVar1;
    uVar3 = iVar1 + ((uVar2 >> 2 | uVar2 * 0x40000000) ^ (uVar2 >> 0xd | uVar2 * 0x80000) ^
                    (uVar2 >> 0x16 | uVar2 * 0x400)) +
                    (local_34[0] ^ (uVar2 ^ local_34[0]) & (local_34[0] ^ local_34[1]));
    if (local_14 == 0) {
      local_6c = param_2[2];
    }
    else {
      local_6c = local_6c +
                 ((local_74 >> 0x11 | local_74 << 0xf) ^ (local_74 >> 0x13 | local_74 << 0xd) ^
                 local_74 >> 10) + local_48 +
                 ((local_68 >> 7 | local_68 << 0x19) ^ (local_68 >> 0x12 | local_68 << 0xe) ^
                 local_68 >> 3);
    }
    iVar1 = local_34[5] +
            ((local_34[2] >> 6 | local_34[2] * 0x4000000) ^
             (local_34[2] >> 0xb | local_34[2] * 0x200000) ^
            (local_34[2] >> 0x19 | local_34[2] * 0x80)) +
            (local_34[4] ^ local_34[2] & (local_34[3] ^ local_34[4])) +
            *(int *)(s_0_00015bb7 + (local_14 + 2) * 4 + 1) + local_6c;
    local_34[1] = local_34[1] + iVar1;
    uVar4 = iVar1 + ((uVar3 >> 2 | uVar3 * 0x40000000) ^ (uVar3 >> 0xd | uVar3 * 0x80000) ^
                    (uVar3 >> 0x16 | uVar3 * 0x400)) +
                    (uVar2 ^ (uVar3 ^ uVar2) & (uVar2 ^ local_34[0]));
    if (local_14 == 0) {
      local_68 = param_2[3];
    }
    else {
      local_68 = local_68 +
                 ((local_70 >> 0x11 | local_70 << 0xf) ^ (local_70 >> 0x13 | local_70 << 0xd) ^
                 local_70 >> 10) + local_44 +
                 ((local_64 >> 7 | local_64 << 0x19) ^ (local_64 >> 0x12 | local_64 << 0xe) ^
                 local_64 >> 3);
    }
    iVar1 = local_34[4] +
            ((local_34[1] >> 6 | local_34[1] * 0x4000000) ^
             (local_34[1] >> 0xb | local_34[1] * 0x200000) ^
            (local_34[1] >> 0x19 | local_34[1] * 0x80)) +
            (local_34[3] ^ local_34[1] & (local_34[2] ^ local_34[3])) +
            *(int *)(s_0_00015bb7 + (local_14 + 3) * 4 + 1) + local_68;
    local_34[0] = local_34[0] + iVar1;
    uVar5 = iVar1 + ((uVar4 >> 2 | uVar4 * 0x40000000) ^ (uVar4 >> 0xd | uVar4 * 0x80000) ^
                    (uVar4 >> 0x16 | uVar4 * 0x400)) + (uVar3 ^ (uVar4 ^ uVar3) & (uVar3 ^ uVar2));
    if (local_14 == 0) {
      local_64 = param_2[4];
    }
    else {
      local_64 = local_64 +
                 ((local_6c >> 0x11 | local_6c << 0xf) ^ (local_6c >> 0x13 | local_6c << 0xd) ^
                 local_6c >> 10) + local_40 +
                 ((local_60 >> 7 | local_60 << 0x19) ^ (local_60 >> 0x12 | local_60 << 0xe) ^
                 local_60 >> 3);
    }
    iVar1 = local_34[3] +
            ((local_34[0] >> 6 | local_34[0] * 0x4000000) ^
             (local_34[0] >> 0xb | local_34[0] * 0x200000) ^
            (local_34[0] >> 0x19 | local_34[0] * 0x80)) +
            (local_34[2] ^ local_34[0] & (local_34[1] ^ local_34[2])) +
            *(int *)(s_0_00015bb7 + (local_14 + 4) * 4 + 1) + local_64;
    uVar2 = uVar2 + iVar1;
    uVar6 = iVar1 + ((uVar5 >> 2 | uVar5 * 0x40000000) ^ (uVar5 >> 0xd | uVar5 * 0x80000) ^
                    (uVar5 >> 0x16 | uVar5 * 0x400)) + (uVar4 ^ (uVar5 ^ uVar4) & (uVar4 ^ uVar3));
    if (local_14 == 0) {
      local_60 = param_2[5];
    }
    else {
      local_60 = local_60 +
                 ((local_68 >> 0x11 | local_68 << 0xf) ^ (local_68 >> 0x13 | local_68 << 0xd) ^
                 local_68 >> 10) + local_3c +
                 ((local_5c >> 7 | local_5c << 0x19) ^ (local_5c >> 0x12 | local_5c << 0xe) ^
                 local_5c >> 3);
    }
    iVar1 = local_34[2] +
            ((uVar2 >> 6 | uVar2 * 0x4000000) ^ (uVar2 >> 0xb | uVar2 * 0x200000) ^
            (uVar2 >> 0x19 | uVar2 * 0x80)) + (local_34[1] ^ uVar2 & (local_34[0] ^ local_34[1])) +
            *(int *)(s_0_00015bb7 + (local_14 + 5) * 4 + 1) + local_60;
    uVar3 = uVar3 + iVar1;
    uVar7 = iVar1 + ((uVar6 >> 2 | uVar6 * 0x40000000) ^ (uVar6 >> 0xd | uVar6 * 0x80000) ^
                    (uVar6 >> 0x16 | uVar6 * 0x400)) + (uVar5 ^ (uVar6 ^ uVar5) & (uVar5 ^ uVar4));
    if (local_14 == 0) {
      local_5c = param_2[6];
    }
    else {
      local_5c = local_5c +
                 ((local_64 >> 0x11 | local_64 << 0xf) ^ (local_64 >> 0x13 | local_64 << 0xd) ^
                 local_64 >> 10) + local_38 +
                 ((local_58 >> 7 | local_58 << 0x19) ^ (local_58 >> 0x12 | local_58 << 0xe) ^
                 local_58 >> 3);
    }
    iVar1 = local_34[1] +
            ((uVar3 >> 6 | uVar3 * 0x4000000) ^ (uVar3 >> 0xb | uVar3 * 0x200000) ^
            (uVar3 >> 0x19 | uVar3 * 0x80)) + (local_34[0] ^ uVar3 & (uVar2 ^ local_34[0])) +
            *(int *)(s_0_00015bb7 + (local_14 + 6) * 4 + 1) + local_5c;
    uVar4 = uVar4 + iVar1;
    uVar8 = iVar1 + ((uVar7 >> 2 | uVar7 * 0x40000000) ^ (uVar7 >> 0xd | uVar7 * 0x80000) ^
                    (uVar7 >> 0x16 | uVar7 * 0x400)) + (uVar6 ^ (uVar7 ^ uVar6) & (uVar6 ^ uVar5));
    if (local_14 == 0) {
      local_58 = param_2[7];
    }
    else {
      local_58 = local_58 +
                 ((local_60 >> 0x11 | local_60 << 0xf) ^ (local_60 >> 0x13 | local_60 << 0xd) ^
                 local_60 >> 10) + local_74 +
                 ((local_54 >> 7 | local_54 << 0x19) ^ (local_54 >> 0x12 | local_54 << 0xe) ^
                 local_54 >> 3);
    }
    iVar1 = local_34[0] +
            ((uVar4 >> 6 | uVar4 * 0x4000000) ^ (uVar4 >> 0xb | uVar4 * 0x200000) ^
            (uVar4 >> 0x19 | uVar4 * 0x80)) + (uVar2 ^ uVar4 & (uVar3 ^ uVar2)) +
            *(int *)(s_0_00015bb7 + (local_14 + 7) * 4 + 1) + local_58;
    uVar5 = uVar5 + iVar1;
    uVar9 = iVar1 + ((uVar8 >> 2 | uVar8 * 0x40000000) ^ (uVar8 >> 0xd | uVar8 * 0x80000) ^
                    (uVar8 >> 0x16 | uVar8 * 0x400)) + (uVar7 ^ (uVar8 ^ uVar7) & (uVar7 ^ uVar6));
    if (local_14 == 0) {
      local_54 = param_2[8];
    }
    else {
      local_54 = local_54 +
                 ((local_5c >> 0x11 | local_5c << 0xf) ^ (local_5c >> 0x13 | local_5c << 0xd) ^
                 local_5c >> 10) + local_70 +
                 ((local_50 >> 7 | local_50 << 0x19) ^ (local_50 >> 0x12 | local_50 << 0xe) ^
                 local_50 >> 3);
    }
    iVar1 = uVar2 + ((uVar5 >> 6 | uVar5 * 0x4000000) ^ (uVar5 >> 0xb | uVar5 * 0x200000) ^
                    (uVar5 >> 0x19 | uVar5 * 0x80)) + (uVar3 ^ uVar5 & (uVar4 ^ uVar3)) +
                    *(int *)(s_0_00015bb7 + (local_14 + 8) * 4 + 1) + local_54;
    uVar6 = uVar6 + iVar1;
    local_34[7] = iVar1 + ((uVar9 >> 2 | uVar9 * 0x40000000) ^ (uVar9 >> 0xd | uVar9 * 0x80000) ^
                          (uVar9 >> 0x16 | uVar9 * 0x400)) +
                          (uVar8 ^ (uVar9 ^ uVar8) & (uVar8 ^ uVar7));
    if (local_14 == 0) {
      local_50 = param_2[9];
    }
    else {
      local_50 = local_50 +
                 ((local_58 >> 0x11 | local_58 << 0xf) ^ (local_58 >> 0x13 | local_58 << 0xd) ^
                 local_58 >> 10) + local_6c +
                 ((local_4c >> 7 | local_4c << 0x19) ^ (local_4c >> 0x12 | local_4c << 0xe) ^
                 local_4c >> 3);
    }
    iVar1 = uVar3 + ((uVar6 >> 6 | uVar6 * 0x4000000) ^ (uVar6 >> 0xb | uVar6 * 0x200000) ^
                    (uVar6 >> 0x19 | uVar6 * 0x80)) + (uVar4 ^ uVar6 & (uVar5 ^ uVar4)) +
                    *(int *)(s_0_00015bb7 + (local_14 + 9) * 4 + 1) + local_50;
    uVar7 = uVar7 + iVar1;
    local_34[6] = iVar1 + ((local_34[7] >> 2 | local_34[7] * 0x40000000) ^
                           (local_34[7] >> 0xd | local_34[7] * 0x80000) ^
                          (local_34[7] >> 0x16 | local_34[7] * 0x400)) +
                          (uVar9 ^ (local_34[7] ^ uVar9) & (uVar9 ^ uVar8));
    if (local_14 == 0) {
      local_4c = param_2[10];
    }
    else {
      local_4c = local_4c +
                 ((local_54 >> 0x11 | local_54 << 0xf) ^ (local_54 >> 0x13 | local_54 << 0xd) ^
                 local_54 >> 10) + local_68 +
                 ((local_48 >> 7 | local_48 << 0x19) ^ (local_48 >> 0x12 | local_48 << 0xe) ^
                 local_48 >> 3);
    }
    iVar1 = uVar4 + ((uVar7 >> 6 | uVar7 * 0x4000000) ^ (uVar7 >> 0xb | uVar7 * 0x200000) ^
                    (uVar7 >> 0x19 | uVar7 * 0x80)) + (uVar5 ^ uVar7 & (uVar6 ^ uVar5)) +
                    *(int *)(s_0_00015bb7 + (local_14 + 10) * 4 + 1) + local_4c;
    uVar8 = uVar8 + iVar1;
    local_34[5] = iVar1 + ((local_34[6] >> 2 | local_34[6] * 0x40000000) ^
                           (local_34[6] >> 0xd | local_34[6] * 0x80000) ^
                          (local_34[6] >> 0x16 | local_34[6] * 0x400)) +
                          (local_34[7] ^ (local_34[6] ^ local_34[7]) & (local_34[7] ^ uVar9));
    if (local_14 == 0) {
      local_48 = param_2[0xb];
    }
    else {
      local_48 = local_48 +
                 ((local_50 >> 0x11 | local_50 << 0xf) ^ (local_50 >> 0x13 | local_50 << 0xd) ^
                 local_50 >> 10) + local_64 +
                 ((local_44 >> 7 | local_44 << 0x19) ^ (local_44 >> 0x12 | local_44 << 0xe) ^
                 local_44 >> 3);
    }
    iVar1 = uVar5 + ((uVar8 >> 6 | uVar8 * 0x4000000) ^ (uVar8 >> 0xb | uVar8 * 0x200000) ^
                    (uVar8 >> 0x19 | uVar8 * 0x80)) + (uVar6 ^ uVar8 & (uVar7 ^ uVar6)) +
                    *(int *)(s_0_00015bb7 + (local_14 + 0xb) * 4 + 1) + local_48;
    uVar9 = uVar9 + iVar1;
    uVar2 = iVar1 + ((local_34[5] >> 2 | local_34[5] * 0x40000000) ^
                     (local_34[5] >> 0xd | local_34[5] * 0x80000) ^
                    (local_34[5] >> 0x16 | local_34[5] * 0x400)) +
                    (local_34[6] ^ (local_34[5] ^ local_34[6]) & (local_34[6] ^ local_34[7]));
    if (local_14 == 0) {
      local_44 = param_2[0xc];
    }
    else {
      local_44 = local_44 +
                 ((local_4c >> 0x11 | local_4c << 0xf) ^ (local_4c >> 0x13 | local_4c << 0xd) ^
                 local_4c >> 10) + local_60 +
                 ((local_40 >> 7 | local_40 << 0x19) ^ (local_40 >> 0x12 | local_40 << 0xe) ^
                 local_40 >> 3);
    }
    iVar1 = uVar6 + ((uVar9 >> 6 | uVar9 * 0x4000000) ^ (uVar9 >> 0xb | uVar9 * 0x200000) ^
                    (uVar9 >> 0x19 | uVar9 * 0x80)) + (uVar7 ^ uVar9 & (uVar8 ^ uVar7)) +
                    *(int *)(s_0_00015bb7 + (local_14 + 0xc) * 4 + 1) + local_44;
    local_34[7] = local_34[7] + iVar1;
    local_34[3] = iVar1 + ((uVar2 >> 2 | uVar2 * 0x40000000) ^ (uVar2 >> 0xd | uVar2 * 0x80000) ^
                          (uVar2 >> 0x16 | uVar2 * 0x400)) +
                          (local_34[5] ^ (uVar2 ^ local_34[5]) & (local_34[5] ^ local_34[6]));
    if (local_14 == 0) {
      local_40 = param_2[0xd];
    }
    else {
      local_40 = local_40 +
                 ((local_48 >> 0x11 | local_48 << 0xf) ^ (local_48 >> 0x13 | local_48 << 0xd) ^
                 local_48 >> 10) + local_5c +
                 ((local_3c >> 7 | local_3c << 0x19) ^ (local_3c >> 0x12 | local_3c << 0xe) ^
                 local_3c >> 3);
    }
    iVar1 = uVar7 + ((local_34[7] >> 6 | local_34[7] * 0x4000000) ^
                     (local_34[7] >> 0xb | local_34[7] * 0x200000) ^
                    (local_34[7] >> 0x19 | local_34[7] * 0x80)) +
                    (uVar8 ^ local_34[7] & (uVar9 ^ uVar8)) +
                    *(int *)(s_0_00015bb7 + (local_14 + 0xd) * 4 + 1) + local_40;
    local_34[6] = local_34[6] + iVar1;
    local_34[2] = iVar1 + ((local_34[3] >> 2 | local_34[3] * 0x40000000) ^
                           (local_34[3] >> 0xd | local_34[3] * 0x80000) ^
                          (local_34[3] >> 0x16 | local_34[3] * 0x400)) +
                          (uVar2 ^ (local_34[3] ^ uVar2) & (uVar2 ^ local_34[5]));
    if (local_14 == 0) {
      local_3c = param_2[0xe];
    }
    else {
      local_3c = local_3c +
                 ((local_44 >> 0x11 | local_44 << 0xf) ^ (local_44 >> 0x13 | local_44 << 0xd) ^
                 local_44 >> 10) + local_58 +
                 ((local_38 >> 7 | local_38 << 0x19) ^ (local_38 >> 0x12 | local_38 << 0xe) ^
                 local_38 >> 3);
    }
    iVar1 = uVar8 + ((local_34[6] >> 6 | local_34[6] * 0x4000000) ^
                     (local_34[6] >> 0xb | local_34[6] * 0x200000) ^
                    (local_34[6] >> 0x19 | local_34[6] * 0x80)) +
                    (uVar9 ^ local_34[6] & (local_34[7] ^ uVar9)) +
                    *(int *)(s_0_00015bb7 + (local_14 + 0xe) * 4 + 1) + local_3c;
    local_34[5] = local_34[5] + iVar1;
    local_34[1] = iVar1 + ((local_34[2] >> 2 | local_34[2] * 0x40000000) ^
                           (local_34[2] >> 0xd | local_34[2] * 0x80000) ^
                          (local_34[2] >> 0x16 | local_34[2] * 0x400)) +
                          (local_34[3] ^ (local_34[2] ^ local_34[3]) & (local_34[3] ^ uVar2));
    if (local_14 == 0) {
      local_38 = param_2[0xf];
    }
    else {
      local_38 = local_38 +
                 ((local_40 >> 0x11 | local_40 << 0xf) ^ (local_40 >> 0x13 | local_40 << 0xd) ^
                 local_40 >> 10) + local_54 +
                 ((local_74 >> 7 | local_74 << 0x19) ^ (local_74 >> 0x12 | local_74 << 0xe) ^
                 local_74 >> 3);
    }
    iVar1 = uVar9 + ((local_34[5] >> 6 | local_34[5] * 0x4000000) ^
                     (local_34[5] >> 0xb | local_34[5] * 0x200000) ^
                    (local_34[5] >> 0x19 | local_34[5] * 0x80)) +
                    (local_34[7] ^ local_34[5] & (local_34[6] ^ local_34[7])) +
                    *(int *)(s_0_00015bb7 + (local_14 + 0xf) * 4 + 1) + local_38;
    local_34[4] = uVar2 + iVar1;
    local_34[0] = iVar1 + ((local_34[1] >> 2 | local_34[1] * 0x40000000) ^
                           (local_34[1] >> 0xd | local_34[1] * 0x80000) ^
                          (local_34[1] >> 0x16 | local_34[1] * 0x400)) +
                          (local_34[2] ^ (local_34[1] ^ local_34[2]) & (local_34[2] ^ local_34[3]));
  }
  for (local_14 = 0; local_14 < 8; local_14 = local_14 + 1) {
    *(uint *)(param_1 + local_14 * 4) = *(int *)(param_1 + local_14 * 4) + local_34[local_14];
  }
  return;
}

