//==================== FUN_00049cc8 @ 00049cc8 ====================

undefined4 FUN_00049cc8(int param_1)

{
  byte bVar1;
  uint uVar2;
  uint uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  uint uVar6;
  undefined4 uVar7;
  char *pcVar8;
  byte *pbVar9;
  int iVar10;
  undefined4 uVar11;
  uint uVar12;
  int *piVar14;
  int *piVar13;
  int iVar15;
  bool bVar16;
  undefined4 local_1048;
  undefined4 local_1044;
  uint local_1040;
  undefined4 uStack_103c;
  uint local_1038;
  undefined4 uStack_1034;
  undefined4 uStack_1030;
  undefined4 uStack_102c;
  char local_1028 [12];
  uint local_101c;
  char local_1018 [20];
  undefined2 local_1004;
  undefined2 uStack_1002;
  char acStack_1000 [4];
  undefined2 uStack_ffc;
  undefined1 local_ffa;
  
  uVar2 = *(uint *)(param_1 + 0x492);
  uVar4 = *(undefined4 *)(param_1 + 0x496);
  uVar6 = *(uint *)(param_1 + 0x49a);
  uVar11 = *(undefined4 *)(param_1 + 0x49e);
  uVar3 = *(uint *)(param_1 + 0x4a2);
  uVar5 = *(undefined4 *)(param_1 + 0x4a6);
  uVar7 = *(undefined4 *)(param_1 + 0x4aa);
  uVar12 = *(uint *)(param_1 + 0x4ae);
  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
    local_1028[0] = 'z';
    local_1028[1] = '1';
    local_1028[2] = '5';
    local_1028[3] = ' ';
    local_1028[4] = 'i';
    local_1028[5] = 'n';
    local_1028[6] = 'i';
    local_1028[7] = 't';
    local_1028[8] = '.';
    local_1028[9] = '.';
    local_1028[10] = '.';
    local_1028[0xb] = '\n';
    local_101c = local_101c & 0xffffff00;
    FUN_00038730(5,local_1028,0);
  }
  FUN_00056a90();
  do {
    if (0 < DAT_0007b198) {
      iVar15 = 0;
      piVar13 = DAT_0007b438;
      do {
        piVar14 = piVar13 + 1;
        pcVar8 = *(char **)(*piVar13 + 0xc4);
        if (((pcVar8 != (char *)0x0) && (*pcVar8 != '\0')) &&
           ((pcVar8 = *(char **)(*piVar13 + 0xcc), pcVar8 != (char *)0x0 && (*pcVar8 != '\0')))) {
          iVar15 = iVar15 + 1;
        }
        piVar13 = piVar14;
      } while (piVar14 != DAT_0007b438 + DAT_0007b198);
      if (iVar15 != 0) break;
    }
    if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
      local_1028[0] = 'N';
      local_1028[1] = 'o';
      local_1028[2] = ' ';
      local_1028[3] = 'v';
      local_1028[4] = 'a';
      local_1028[5] = 'l';
      local_1028[6] = 'i';
      local_1028[7] = 'd';
      local_1028[8] = ' ';
      local_1028[9] = 'p';
      local_1028[10] = 'o';
      local_1028[0xb] = 'o';
      local_101c._0_1_ = 'l';
      local_101c._1_1_ = 's';
      local_101c._2_1_ = ',';
      local_101c._3_1_ = ' ';
      local_1018[0] = 'p';
      local_1018[1] = 'l';
      local_1018[2] = 'e';
      local_1018[3] = 'a';
      local_1018[4] = 's';
      local_1018[5] = 'e';
      local_1018[6] = ' ';
      local_1018[7] = 'c';
      local_1018[8] = 'o';
      local_1018[9] = 'n';
      local_1018[10] = 'f';
      local_1018[0xb] = 'i';
      local_1018[0xc] = 'g';
      local_1018[0xd] = 'u';
      local_1018[0xe] = 'r';
      local_1018[0xf] = 'e';
      local_1018[0x10] = ' ';
      local_1018[0x11] = 't';
      local_1018[0x12] = 'h';
      local_1018[0x13] = 'e';
      local_1004._0_1_ = 'm';
      local_1004._1_1_ = ' ';
      register0x00000026 = 'f';
      register0x00000027 = 'i';
      acStack_1000[0] = 'r';
      acStack_1000[1] = 's';
      acStack_1000[2] = 't';
      acStack_1000[3] = '!';
      uStack_ffc = 0x2121;
      local_ffa = 0;
      FUN_00038730(4,local_1028,0);
    }
    sleep(5);
  } while( true );
  FUN_0003b704();
  FUN_0003ba70();
  FUN_0003bb44(0x1a);
  DAT_0009d6f3 = 1;
  DAT_00078b98 = 0x352e30;
  DAT_00078b94 = 0x2e302e39;
  local_1048 = uVar2;
  local_1044 = uVar4;
  local_1040 = uVar6;
  uStack_103c = uVar11;
  local_1038 = uVar3;
  uStack_1034 = uVar5;
  uStack_1030 = uVar7;
  uStack_102c = uVar12;
  FUN_00045b30();
  FUN_00045d88(DAT_00076b78);
  FUN_00045b50();
  uVar2 = local_1048 & 0xff;
  if (uVar2 != 0x51) {
    if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 3)) {
      return 0xffffffff;
    }
    pcVar8 = "%s: config_parameter.token_type != 0x%x, it is 0x%x";
    uVar4 = 0xffffffff;
    uVar3 = 0x51;
LAB_00049eec:
    snprintf(local_1028,0x1000,pcVar8,"bitmain_ZCASH_init",uVar3,uVar2);
    FUN_00038730(3,local_1028,0);
    return uVar4;
  }
  pbVar9 = (byte *)&local_1048;
  uVar2 = 0xff;
  uVar3 = 0x51;
  while (uVar2 = ((uint)*(ushort *)(&DAT_00068aec + (uVar3 ^ uVar2 >> 8) * 2) ^ uVar2 << 8) & 0xffff
        , pbVar9 != (byte *)((int)&uStack_102c + 1)) {
    pbVar9 = pbVar9 + 1;
    uVar3 = (uint)*pbVar9;
  }
  uVar3 = uStack_102c >> 0x10;
  if (uVar3 != uVar2) {
    if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 3)) {
      return 0xfffffffe;
    }
    pcVar8 = "%s: config_parameter.crc = 0x%x, but we calculate it as 0x%x";
    uVar4 = 0xfffffffe;
    goto LAB_00049eec;
  }
  iVar15 = 0;
  do {
    iVar10 = iVar15 + 0x1c;
    iVar15 = iVar15 + 1;
    iVar10 = param_1 + iVar10 * 4;
    *(undefined2 *)(iVar10 + 6) = 0;
    *(undefined2 *)(iVar10 + 8) = 0;
  } while (iVar15 != 0x80);
  FUN_00044978();
  if (DAT_00078e28 != '\0') {
    DAT_00078e38 = (uint)DAT_0009d6cb;
    uStack_103c._0_3_ = CONCAT12(0x32,(undefined2)uStack_103c);
    local_1040 = local_1040 | 2;
  }
  FUN_00041308();
  FUN_000413f0();
  sleep(1);
  iVar15 = FUN_000416ec();
  if (iVar15 == -3) {
    return 0xfffffffd;
  }
  iVar15 = access("/tmp/delete_freq",0);
  uVar2 = local_1040;
  if (iVar15 == 0) {
    if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
      local_1028[0] = 'd';
      local_1028[1] = 'e';
      local_1028[2] = 'l';
      local_1028[3] = 'e';
      local_1028[4] = 't';
      local_1028[5] = 'e';
      local_1028[6] = ' ';
      local_1028[7] = 'f';
      local_1028[8] = 'r';
      local_1028[9] = 'e';
      local_1028[10] = 'q';
      local_1028[0xb] = ' ';
      local_101c._0_1_ = 'd';
      local_101c._1_1_ = 'a';
      local_101c._2_1_ = 't';
      local_101c._3_1_ = 'a';
      local_1018[0] = ' ';
      local_1018[1] = 'a';
      local_1018[2] = 'n';
      local_1018[3] = 'd';
      local_1018[4] = ' ';
      local_1018[5] = 's';
      local_1018[6] = 'c';
      local_1018[7] = 'a';
      local_1018[8] = 'n';
      local_1018[9] = ' ';
      local_1018[10] = 'f';
      local_1018[0xb] = 'r';
      local_1018[0xc] = 'e';
      local_1018[0xd] = 'q';
      local_1018[0xe] = ' ';
      local_1018[0xf] = 'a';
      local_1018[0x10] = 'g';
      local_1018[0x11] = 'a';
      local_1018[0x12] = 'i';
      local_1018[0x13] = 'n';
      local_1004._0_1_ = '\n';
      local_1004._1_1_ = '\0';
      FUN_00038730(5,local_1028,0);
    }
    local_1028[4] = '\0';
    local_1028[5] = '\0';
    local_1028[6] = '\0';
    local_1028[7] = '\0';
    local_1028[0] = '\0';
    local_1028[1] = '\0';
    local_1028[2] = '\0';
    local_1028[3] = '\0';
    local_1028[8] = '\0';
    local_1028[9] = '\0';
    local_1028[10] = '\0';
    local_1028[0xb] = '\0';
    local_101c = 0;
    local_1018[0] = '\0';
    local_1018[1] = '\0';
    local_1018[2] = '\0';
    local_1018[3] = '\0';
    local_1018[4] = '\0';
    local_1018[5] = '\0';
    local_1018[6] = '\0';
    local_1018[7] = '\0';
    local_1018[8] = '\0';
    local_1018[9] = '\0';
    local_1018[10] = '\0';
    local_1018[0xb] = '\0';
    local_1018[0xc] = '\0';
    local_1018[0xd] = '\0';
    local_1018[0xe] = '\0';
    local_1018[0xf] = '\0';
    local_1018[0x10] = '\0';
    local_1018[0x11] = '\0';
    local_1018[0x12] = '\0';
    local_1018[0x13] = '\0';
    FUN_00046b94(local_1028);
    system("rm /tmp/delete_freq");
    sleep(3);
                    /* WARNING: Subroutine does not return */
    exit(1);
  }
  if (((local_1040 & 8) == 0) || ((local_1038 & 0xffff) == 800 || (local_1038 & 0xffff) == 0)) {
    DAT_0009d6f8 = DAT_0009d6f8 & 0xff0000ff | 0x32000;
  }
  else {
    DAT_0009d6f8._0_3_ = CONCAT21((undefined2)local_1038,(undefined1)DAT_0009d6f8);
  }
  FUN_0003a354();
  FUN_0002a92c(100);
  FUN_000414d8();
  FUN_0002a92c(200);
  FUN_0002a92c(200);
  FUN_00041614();
  FUN_0002a92c(100);
  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
    snprintf(local_1028,0x1000,"%s %d","reset_all_hash_board_high",0x1128);
    FUN_00038730(5,local_1028,0);
  }
  uVar3 = 0;
  *(undefined4 *)(DAT_00078e10 + 0x34) = 0;
  FUN_0002a92c(100);
  FUN_000448bc(param_1);
  sleep(3);
  FUN_00043f04(local_1044);
  FUN_0002a92c(100);
  FUN_000467b0();
  DAT_0009d6f2 = 1;
  FUN_000481b4();
  FUN_0002a92c(100);
  sleep(2);
  FUN_00049578(1);
  FUN_0002a92c(300);
  pbVar9 = &DAT_0009d4bc;
  do {
    if (*pbVar9 == 1) {
      iVar15 = 3;
      *(undefined1 *)(uVar3 + 0x79bf4) = 1;
      do {
        bVar1 = pbVar9[0x209];
        if (bVar1 == 3) {
          *(undefined1 *)(uVar3 + 0x79bf4) = 0;
          break;
        }
        bVar16 = DAT_0007ad48 != '\0';
        *(undefined1 *)(uVar3 + 0x79bf4) = 1;
        if (((bVar16) || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
          snprintf(local_1028,0x1000,"recheck: chainid %d exist %d asicnum %d realnum %d",uVar3,
                   (uint)*pbVar9,(uint)bVar1,3);
          FUN_00038730(5,local_1028,0);
          if (*(char *)(uVar3 + 0x79bf4) == '\0') break;
        }
        FUN_00049700(param_1,uVar3 & 0xff);
        iVar15 = iVar15 + -1;
      } while (iVar15 != 0);
    }
    uVar3 = uVar3 + 1;
    pbVar9 = pbVar9 + 1;
  } while (uVar3 != 4);
  FUN_00046cbc();
  FUN_00044104(DAT_00076b60);
  FUN_0002a92c(100);
  FUN_00043060();
  FUN_0002a92c(100);
  FUN_000432c0(DAT_00076b74);
  FUN_0002a92c(100);
  FUN_0004330c(0);
  FUN_0002a92c(100);
  FUN_0004614c();
  FUN_000463c8();
  FUN_00043594(0);
  if ((uVar2 & 4) == 0) {
LAB_0004a37c:
    uVar6 = (uint)uStack_103c._2_1_;
    uVar3 = (local_1040 & 3) >> 1;
    DAT_0009d6f7 = (undefined1)uVar3;
    DAT_0009d6f8 = CONCAT31(DAT_0009d6f8._1_3_,uStack_103c._2_1_);
    if (DAT_0007ad48 == '\0') goto LAB_0004a1fc;
  }
  else {
    DAT_0009d4c4 = 100000000;
    if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
      snprintf(local_1028,0x1000,"dev.timeout = %d us",100000000);
      FUN_00038730(5,local_1028,0);
      goto LAB_0004a37c;
    }
    uVar6 = (uint)uStack_103c._2_1_;
    uVar3 = (local_1040 & 3) >> 1;
    DAT_0009d6f7 = (undefined1)uVar3;
    DAT_0009d6f8 = CONCAT31(DAT_0009d6f8._1_3_,uStack_103c._2_1_);
LAB_0004a1fc:
    if ((DAT_00078e08 == '\0') && (DAT_000766c4 < 5)) goto LAB_0004a244;
  }
  snprintf(local_1028,0x1000,"%s: fan_eft : %d  fan_pwm : %d","bitmain_ZCASH_init",uVar3,uVar6);
  FUN_00038730(5,local_1028,0);
LAB_0004a244:
  if (((uVar2 & 2) == 0) || (100 < uVar6)) {
    FUN_00045ec0();
  }
  else {
    FUN_00045d88(uVar6);
  }
  iVar15 = FUN_000498f4();
  if (iVar15 == -8) {
    return 0xfffffff8;
  }
  sleep(1);
  FUN_00049be8();
  if (iVar15 == -7) {
    return 0xfffffff9;
  }
  iVar15 = FUN_000499d4(param_1);
  if (iVar15 == -5) {
    return 0xfffffffb;
  }
  iVar15 = FUN_00049ac0();
  if (iVar15 == -6) {
    return 0xfffffffa;
  }
  FUN_000442b0();
  DAT_000790ec = 1;
  DAT_000790ed = 1;
  DAT_000790ee = 1;
  DAT_000790ef = 1;
  if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 5)) {
    DAT_000790ec = 1;
    DAT_000790ed = 1;
    DAT_000790ee = 1;
    DAT_000790ef = 1;
    return 0;
  }
  snprintf(local_1028,0x1000,"%s done","bitmain_ZCASH_init");
  FUN_00038730(5,local_1028,0);
  return 0;
}

