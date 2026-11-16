//==================== FUN_00044b5c @ 00044b5c ====================

void FUN_00044b5c(void)

{
  char cVar1;
  uint uVar2;
  undefined4 uVar3;
  uint uVar4;
  undefined4 uVar5;
  int iVar6;
  undefined4 uVar7;
  int iVar8;
  undefined4 uVar9;
  int iVar10;
  undefined4 *puVar11;
  int iVar12;
  char *pcVar13;
  undefined4 *puVar14;
  bool bVar15;
  char *local_1068;
  uint local_1060;
  int local_105c;
  int iStack_1050;
  int local_104c;
  int iStack_1048;
  int local_1044;
  int iStack_1040;
  int local_103c;
  int local_1038 [4];
  char acStack_1028 [4100];
  
  local_104c = 0;
  iStack_1050 = 0;
  FUN_0002a84c(&iStack_1048);
  FUN_0002a84c(&iStack_1040);
  FUN_0002a530(&iStack_1050,&iStack_1048);
  FUN_0002a530(&DAT_000790f4,&iStack_1040);
  local_1038[1] = 0;
  local_1038[2] = 0;
  local_1038[0] = 0;
  local_1038[3] = 0;
LAB_00044bf0:
  while( true ) {
    FUN_0002a84c(&iStack_1048);
    FUN_0002a84c(&iStack_1040);
    iVar6 = iStack_1048 - iStack_1050;
    if (local_1044 - local_104c < 0) {
      iVar6 = iVar6 + -1;
    }
    if (iVar6 < 12000) break;
    FUN_0002a530(&iStack_1050,&iStack_1048);
    FUN_0002a92c(3000);
  }
  if (0x78 < iVar6) {
    puVar14 = &DAT_0009d5e7;
    local_1038[0] = 0;
    local_1038[1] = 0;
    local_1038[2] = 0;
    local_1038[3] = 0;
    puVar11 = &DAT_0007c134;
    do {
      uVar9 = *puVar14;
      uVar3 = puVar14[1];
      uVar5 = puVar14[2];
      uVar7 = puVar14[3];
      puVar14 = puVar14 + 4;
      *puVar11 = uVar9;
      puVar11[1] = uVar3;
      puVar11[2] = uVar5;
      puVar11[3] = uVar7;
      puVar11 = puVar11 + 4;
    } while (puVar14 != (undefined4 *)&DAT_0009d617);
    puVar14 = (undefined4 *)&DAT_0009d5b7;
    puVar11 = &DAT_0007c164;
    do {
      uVar9 = *puVar14;
      uVar3 = puVar14[1];
      uVar5 = puVar14[2];
      uVar7 = puVar14[3];
      puVar14 = puVar14 + 4;
      *puVar11 = uVar9;
      puVar11[1] = uVar3;
      puVar11[2] = uVar5;
      puVar11[3] = uVar7;
      puVar11 = puVar11 + 4;
    } while (puVar14 != &DAT_0009d5e7);
    iVar6 = 0;
    iVar12 = 0;
    local_1068 = &DAT_0009d4bb;
    iVar10 = 0;
    pcVar13 = &DAT_0009d4bb;
    do {
      if ((pcVar13[1] != '\0') &&
         (iVar10 = iVar10 + (uint)(byte)pcVar13[0x20a], (byte)pcVar13[0x20a] != 0)) {
        uVar2 = 0;
        do {
          iVar12 = iVar12 + *(int *)(&DAT_0009d5b7 + (iVar6 * 3 + uVar2) * 4);
          if ((DAT_00078e09 != '\0') &&
             (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
            snprintf(acStack_1028,0x1000,"%s: chain %d asic %d asic_nonce_num %d",
                     "check_miner_status",iVar6,uVar2,
                     *(int *)(&DAT_0009d5b7 + (iVar6 * 3 + uVar2) * 4));
            FUN_00038730(7,acStack_1028,0);
          }
          uVar2 = uVar2 + 1;
        } while (uVar2 < (byte)pcVar13[0x20a]);
      }
      iVar6 = iVar6 + 1;
      pcVar13 = pcVar13 + 1;
    } while (iVar6 != 4);
    if (iVar10 == 0) {
      local_1060 = 1;
    }
    else {
      if ((DAT_00078e09 != '\0') &&
         (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
        snprintf(acStack_1028,0x1000,"%s: avg_num %d asic_num %d","check_miner_status",iVar12,iVar10
                );
        FUN_00038730(7,acStack_1028,0);
      }
      local_1060 = FUN_0005c648(iVar12,iVar10);
      local_1060 = local_1060 >> 3;
    }
    iVar12 = 0;
    iVar6 = 0;
    do {
      if (local_1068[1] != '\0') {
        if (local_1068[0x20a] == '\0') {
          local_105c = iVar6 << 3;
          uVar2 = 0;
        }
        else {
          local_105c = iVar6 * 8;
          puVar11 = &DAT_0007c194 + iVar6 * 3;
          iVar10 = 0;
          uVar2 = 0;
          do {
            if ((uVar2 & 7) == 0) {
              uVar4 = uVar2 + iVar10;
              if ((0x13 < uVar4) &&
                 (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)))) {
                snprintf(acStack_1028,0x1000,"asic num err![%d]",uVar4);
                FUN_00038730(3,acStack_1028,0);
              }
              iVar10 = iVar10 + 1;
              (&DAT_0009d4b4)[uVar4 + iVar6 * 0x13 + 0x163] = 0x20;
            }
            cVar1 = DAT_00078e28;
            if (local_1060 < *(uint *)(&DAT_0009d5b7 + (iVar6 * 3 + uVar2) * 4)) {
              *puVar11 = 0;
LAB_00045848:
              uVar4 = iVar10 + uVar2;
              if ((0x13 < uVar4) &&
                 (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)))) {
                snprintf(acStack_1028,0x1000,"asic num err![%d]",uVar4);
                FUN_00038730(3,acStack_1028,0);
              }
              (&DAT_0009d4b4)[uVar4 + iVar6 * 0x13 + 0x163] = 0x6f;
            }
            else {
              *puVar11 = 1;
              if (cVar1 != '\0') goto LAB_00045848;
              uVar4 = iVar10 + uVar2;
              if ((0x13 < uVar4) &&
                 (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)))) {
                snprintf(acStack_1028,0x1000,"asic num err![%d]",uVar4);
                FUN_00038730(3,acStack_1028,0);
              }
              bVar15 = DAT_00078e09 != '\0';
              iVar8 = *(int *)((int)local_1038 + iVar12);
              (&DAT_0009d4b4)[uVar4 + iVar6 * 0x13 + 0x163] = 0x78;
              *(int *)((int)local_1038 + iVar12) = iVar8 + 1;
              if ((bVar15) &&
                 (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
                snprintf(acStack_1028,0x1000,"x chain%d asic%d nonce_num=%d avg_num=%d\n",iVar6,
                         uVar2,*(int *)(&DAT_0009d5b7 + (iVar6 * 3 + uVar2) * 4),local_1060);
                FUN_00038730(7,acStack_1028,0);
              }
            }
            if ((0x13 < uVar2) &&
               (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)))) {
              snprintf(acStack_1028,0x1000,"asic num err![%d]",uVar2);
              FUN_00038730(3,acStack_1028,0);
            }
            iVar8 = iVar6 * 3 + uVar2;
            uVar2 = uVar2 + 1;
            puVar11 = puVar11 + 1;
            iVar8 = (iVar8 + 0x40) * 4;
            uVar4 = *(uint *)(iVar8 + 0x9d4b8);
            (&DAT_0009d4b7)[iVar8] = 0;
            *(uint *)(iVar8 + 0x9d4b8) = uVar4 & 0xff000000;
          } while (uVar2 < (byte)local_1068[0x20a]);
          uVar2 = iVar10 + uVar2;
          if ((0x13 < uVar2) &&
             (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)))) {
            snprintf(acStack_1028,0x1000,"asic num err![%d]",uVar2);
            FUN_00038730(3,acStack_1028,0);
          }
        }
        (&DAT_0009d617)[local_105c * 2 + iVar6 * 3 + uVar2] = 0;
      }
      iVar6 = iVar6 + 1;
      iVar12 = iVar12 + 4;
      local_1068 = local_1068 + 1;
    } while (iVar6 != 4);
    pthread_mutex_lock((pthread_mutex_t *)&DAT_00079100);
    pthread_cond_signal((pthread_cond_t *)&DAT_0007c078);
    pthread_mutex_unlock((pthread_mutex_t *)&DAT_00079100);
    memset(&DAT_0009d5e7,0,0x30);
    memset(&DAT_0009d5b7,0,0x30);
    FUN_0002a530(&iStack_1050,&iStack_1048);
  }
  if ((DAT_0009d6d6 < 0x51) && (DAT_0009d6da < 0x6f)) {
    DAT_0007911c = 0;
    DAT_0007911d = '\0';
  }
  else {
    DAT_0007911c = DAT_0007911c + 1;
    if (DAT_0007911c < 3) {
      if ((DAT_00078e3c == '\0') &&
         (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)))) {
        snprintf(acStack_1028,0x1000,
                 "Temperature pcb:%d/chip:%d is higher than %d/%d\'C for %d time, PWM is %d",
                 DAT_0009d6d6,DAT_0009d6da,0x50,0x6e,(uint)DAT_0007911c,(uint)DAT_0009d6ca);
        FUN_00038730(3,acStack_1028,0);
      }
    }
    else {
      DAT_0007911d = '\x01';
      if ((DAT_00078e3c == '\0') &&
         (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)))) {
        snprintf(acStack_1028,0x1000,
                 "%s: the temperature pcb:%d/chip:%d is too high, close PIC and need reboot!!!",
                 "check_miner_status",DAT_0009d6d6,DAT_0009d6da);
        FUN_00038730(3,acStack_1028,0);
      }
    }
  }
  iVar6 = iStack_1040 - DAT_000790f4;
  if (local_103c - DAT_000790f8 < 0) {
    iVar6 = iVar6 + -1;
  }
  if (iVar6 < 0x79) {
    DAT_0007911e = '\0';
  }
  else {
    DAT_0007911e = '\x01';
    if (((DAT_00078e3c == '\0') && (DAT_00078e09 != '\0')) &&
       ((DAT_0007ad48 != '\0' || ((DAT_00078e08 != '\0' || (6 < DAT_000766c4)))))) {
      snprintf(acStack_1028,0x1000,
               "%s: We have lost internet for %d seconds, so don\'t send work to hashboard anymore",
               "check_miner_status",iVar6);
      FUN_00038730(7,acStack_1028,0);
    }
  }
  if ((DAT_0009d4c8 & 1) != 0) {
    uVar2 = 1;
    uVar4 = 0;
LAB_00044d7c:
    iVar6 = 1;
    goto joined_r0x00044d84;
  }
  uVar4 = 1;
  do {
    if ((DAT_0009d4c8 & 1 << (uVar4 & 0xff)) != 0) {
      uVar2 = uVar4 + 1;
      if (uVar2 < 0x20) goto LAB_00044d7c;
      iVar6 = 1;
      goto LAB_00044da4;
    }
    uVar4 = uVar4 + 1;
  } while (uVar4 != 0x20);
  uVar4 = 0xffffffff;
  iVar6 = 0;
  uVar2 = 0;
  do {
    uVar2 = uVar2 + 1;
    if (0x1f < uVar2) goto LAB_00044da4;
joined_r0x00044d84:
  } while ((DAT_0009d4c8 & 1 << (uVar2 & 0xff)) == 0);
  iVar12 = 1;
  goto LAB_00044dac;
LAB_00044da4:
  uVar2 = 0xffffffff;
  iVar12 = 0;
LAB_00044dac:
  if (DAT_0009d6cc < 2) {
    iVar10 = 1;
LAB_00044fec:
    DAT_00079120 = DAT_00079120 + 1;
    if ((DAT_00078e09 != '\0') &&
       (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
      snprintf(acStack_1028,0x1000,
               "ret:%d [%d:%d]fan_error_num:%d fan_num %d fan_pwm %d fan1_speed_value %d fan2_speed_value %d"
               ,iVar10,iVar6,iVar12,DAT_00079120,(uint)DAT_0009d6cc,(uint)(byte)DAT_0009d6f8,
               *(int *)(&DAT_0009d69d + uVar4 * 4),*(int *)(&DAT_0009d69d + uVar2 * 4));
      FUN_00038730(7,acStack_1028,0);
    }
    if (0x14 < DAT_00079120) {
      uVar2 = 1;
      DAT_00079125 = '\x01';
      if ((DAT_0009d4c8 & 1) == 0) {
        do {
          if ((DAT_0009d4c8 & 1 << (uVar2 & 0xff)) != 0) {
            uVar4 = uVar2 + 1;
            if (uVar4 == 0x20) goto LAB_00045104;
            goto joined_r0x000450e8;
          }
          uVar2 = uVar2 + 1;
        } while (uVar2 != 0x20);
        uVar2 = 0xffffffff;
        uVar4 = 0;
      }
      else {
        uVar4 = 1;
        uVar2 = 0;
      }
joined_r0x000450e8:
      do {
        if ((DAT_0009d4c8 & 1 << (uVar4 & 0xff)) != 0) goto LAB_00045108;
        uVar4 = uVar4 + 1;
      } while (uVar4 < 0x20);
LAB_00045104:
      uVar4 = 0xffffffff;
LAB_00045108:
      if (iVar10 == 3) {
        if ((DAT_00078e3c != '\0') ||
           (((DAT_0007ad48 == '\0' && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 3))))
        goto LAB_00044e5c;
        uVar9 = *(undefined4 *)(&DAT_0009d69d + uVar4 * 4);
        pcVar13 = "Fan Err! Disable PIC! Fan2 speed is too low %d pwm %d ";
      }
      else {
        if (iVar10 == 4) {
          if ((DAT_00078e3c == '\0') &&
             (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)))) {
            snprintf(acStack_1028,0x1000,"Fan Err! Disable PIC! Fan1:%d Fan2:%d pwm %d",
                     *(int *)(&DAT_0009d69d + uVar2 * 4),*(int *)(&DAT_0009d69d + uVar4 * 4),
                     (uint)DAT_0009d6ca);
            FUN_00038730(3,acStack_1028,0);
          }
          goto LAB_00044e5c;
        }
        if (iVar10 != 2) {
          if ((DAT_00078e3c == '\0') &&
             (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)))) {
            snprintf(acStack_1028,0x1000,"Fan Err! Disable PIC! Fan num is %d",(uint)DAT_0009d6cc);
            FUN_00038730(3,acStack_1028,0);
          }
          goto LAB_00044e5c;
        }
        if ((DAT_00078e3c != '\0') ||
           (((DAT_0007ad48 == '\0' && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 3))))
        goto LAB_00044e5c;
        uVar9 = *(undefined4 *)(&DAT_0009d69d + uVar2 * 4);
        pcVar13 = "Fan Err! Disable PIC! Fan1 speed is too low %d pwm %d ";
      }
      snprintf(acStack_1028,0x1000,pcVar13,uVar9,(uint)DAT_0009d6ca);
      FUN_00038730(3,acStack_1028,0);
      goto LAB_00044e5c;
    }
  }
  else {
    if (iVar6 != 0) {
      if (*(uint *)(&DAT_0009d69d + uVar4 * 4) <
          (uint)((ulonglong)((longlong)(int)((uint)(byte)DAT_0009d6f8 * 6000) * 0x7e07e07f) >> 0x26)
         ) {
        iVar10 = 2;
      }
      else {
        if ((DAT_0009d6ca != 100) || (0x12bf < *(uint *)(&DAT_0009d69d + uVar4 * 4)))
        goto LAB_00044e04;
LAB_00044fe8:
        iVar10 = 4;
      }
      goto LAB_00044fec;
    }
LAB_00044e04:
    if (iVar12 != 0) {
      if (*(uint *)(&DAT_0009d69d + uVar2 * 4) <
          (uint)((ulonglong)((longlong)(int)((uint)(byte)DAT_0009d6f8 * 0x10cc) * 0x7e07e07f) >>
                0x26)) {
        iVar10 = 3;
        goto LAB_00044fec;
      }
      if ((DAT_0009d6ca == 100) && (*(uint *)(&DAT_0009d69d + uVar2 * 4) < 0xd70))
      goto LAB_00044fe8;
    }
    DAT_00079120 = 0;
  }
  DAT_00079125 = '\0';
LAB_00044e5c:
  if (DAT_00078e28 != '\0') {
    DAT_0007911e = '\0';
  }
  if (((DAT_00079124 == '\0') && (DAT_0007911e == '\0')) &&
     ((DAT_0007911d == '\0' && ((DAT_00079125 == '\0' && (DAT_00079126 == '\0')))))) {
    DAT_000790f0 = 0;
    DAT_00078e3c = '\0';
    if (DAT_00079127 != '\0') {
      DAT_000790f0 = 1;
      DAT_00078e3c = '\x01';
    }
  }
  else {
    DAT_000790f0 = 1;
    if ((DAT_00079127 == '\0') && ((DAT_0007911d != '\0' || (DAT_00079125 != '\0')))) {
      pcVar13 = &DAT_0009d4bb;
      iVar6 = 0;
      DAT_00078e3c = '\x01';
      DAT_00079127 = '\x01';
      do {
        pcVar13 = pcVar13 + 1;
        if (*pcVar13 == '\x01') {
          pthread_mutex_lock((pthread_mutex_t *)&DAT_000790d0);
          DAT_0007909c = iVar6;
          FUN_0003fc24();
          FUN_0002a92c(100);
          pthread_mutex_unlock((pthread_mutex_t *)&DAT_000790d0);
        }
        iVar6 = iVar6 + 1;
      } while (iVar6 != 4);
    }
  }
  FUN_00044ac0(DAT_000790f0);
  FUN_0002a92c(1000);
  goto LAB_00044bf0;
}

