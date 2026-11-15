//==================== FUN_00047354 @ 00047354 ====================

void FUN_00047354(void)

{
  int *piVar1;
  undefined4 *puVar2;
  int iVar3;
  int iVar4;
  char *pcVar5;
  char *param3;
  int iVar6;
  int *piVar7;
  uint param4;
  uint param5;
  int iVar8;
  int iVar9;
  bool bVar10;
  int local_102c;
  char acStack_1028 [4100];
  
  FUN_00043e4c(800);
  local_102c = 0x32f;
  do {
    puVar2 = &DAT_0007c130;
    pcVar5 = &DAT_0009d4bb;
    do {
      pcVar5 = pcVar5 + 1;
      if (*pcVar5 == '\x01') {
        iVar6 = 0;
        do {
          iVar8 = iVar6 + 4;
          *(undefined4 *)((int)puVar2 + iVar6 + 4) = 0;
          *(undefined4 *)((int)puVar2 + iVar6 + 100) = 0;
          *(undefined4 *)((int)puVar2 + iVar6 + 0xf4) = 800;
          *(int *)((int)puVar2 + iVar6 + 0xc4) = local_102c;
          *(undefined4 *)((int)puVar2 + iVar6 + 0x94) = 0x307;
          *(undefined4 *)((int)puVar2 + iVar6 + 0x124) = 0;
          iVar6 = iVar8;
        } while (iVar8 != 0xc);
      }
      puVar2 = puVar2 + 3;
    } while (pcVar5 != &DAT_0009d4bf);
LAB_000473a8:
    do {
      do {
        sleep(1);
        pthread_mutex_lock((pthread_mutex_t *)&DAT_00079100);
        pthread_cond_wait((pthread_cond_t *)&DAT_0007c078,(pthread_mutex_t *)&DAT_00079100);
        pthread_mutex_unlock((pthread_mutex_t *)&DAT_00079100);
      } while (DAT_00078e3c != '\0');
      pcVar5 = &DAT_0009d4bb;
      param4 = 0;
      do {
        pcVar5 = pcVar5 + 1;
        if (*pcVar5 == '\x01') {
          param5 = 0;
          piVar7 = &DAT_0007c194 + param4 * 3;
          iVar6 = *piVar7;
          if (iVar6 != 0) goto LAB_00047680;
LAB_0004751c:
          if (0xf < (uint)piVar7[-0x18]) goto LAB_00047680;
          iVar9 = piVar7[0x24];
          iVar8 = piVar7[0x18];
          if (iVar8 == iVar9) goto LAB_00047824;
          piVar7[0xc] = iVar9;
          iVar3 = iVar6;
          if (iVar9 != 100) {
            puVar2 = &DAT_00069204;
            iVar3 = 1;
            do {
              piVar1 = puVar2 + 4;
              puVar2 = puVar2 + 4;
              if (iVar9 == *piVar1) goto joined_r0x00047570;
              iVar3 = iVar3 + 1;
            } while (iVar3 != 0x73);
            iVar3 = -1;
          }
joined_r0x00047570:
          if (iVar8 == 100) {
            iVar9 = 0;
          }
          else {
            puVar2 = &DAT_00069204;
            iVar9 = 1;
            do {
              piVar1 = puVar2 + 4;
              puVar2 = puVar2 + 4;
              if (iVar8 == *piVar1) goto LAB_0004759c;
              iVar9 = iVar9 + 1;
            } while (iVar9 != 0x73);
            iVar9 = -1;
          }
LAB_0004759c:
          if (iVar9 - iVar3 == 1) {
            iVar3 = iVar3 + 1;
            iVar8 = 1;
          }
          else {
            iVar8 = 1;
            iVar3 = iVar3 + (iVar9 - iVar3) / 2;
          }
          do {
            iVar9 = (&DAT_00069204)[iVar3 * 4];
            bVar10 = DAT_00078e09 != '\0';
            piVar7[0x24] = iVar9;
            if ((bVar10) &&
               (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
              if (iVar8 == 1) {
                param3 = "up";
              }
              else {
                param3 = "down";
              }
              snprintf(acStack_1028,0x1000,
                       "%s freq[%d-%d]: cur freq %d, [%d, %d], invalid nonce %d, ox status %d\n",
                       param3,param4,param5,iVar9,piVar7[0xc],piVar7[0x18],piVar7[-0x18],iVar6);
              FUN_00038730(7,acStack_1028,0);
              iVar9 = piVar7[0x24];
            }
            FUN_00043734(param4 & 0xff,param5 & 0xff,iVar9);
            while( true ) {
              param5 = param5 + 1;
              if (param5 == 3) goto LAB_000473f8;
              piVar7 = piVar7 + 1;
              iVar6 = *piVar7;
              if (iVar6 == 0) goto LAB_0004751c;
LAB_00047680:
              iVar8 = piVar7[0xc];
              iVar9 = piVar7[0x24];
              if (iVar8 != iVar9) break;
LAB_00047824:
              piVar7[0x30] = 1;
            }
            if (iVar9 == 100) {
              iVar4 = -1;
LAB_000476c4:
              if (iVar8 == 100) goto LAB_00047814;
LAB_000476cc:
              puVar2 = &DAT_00069204;
              iVar3 = 1;
              do {
                piVar1 = puVar2 + 4;
                puVar2 = puVar2 + 4;
                if (iVar8 == *piVar1) goto LAB_000476f4;
                iVar3 = iVar3 + 1;
              } while (iVar3 != 0x73);
              iVar3 = -1;
            }
            else {
              puVar2 = &DAT_00069204;
              iVar4 = 1;
              do {
                piVar1 = puVar2 + 4;
                puVar2 = puVar2 + 4;
                if (iVar9 == *piVar1) {
                  iVar4 = iVar4 + -1;
                  goto LAB_000476c4;
                }
                iVar4 = iVar4 + 1;
              } while (iVar4 != 0x73);
              iVar4 = -2;
              if (iVar8 != 100) goto LAB_000476cc;
LAB_00047814:
              iVar3 = 0;
            }
LAB_000476f4:
            iVar8 = -1;
            if (iVar4 - iVar3 == 1) {
              iVar3 = iVar3 + 1;
            }
            else {
              iVar3 = iVar3 + (iVar4 - iVar3) / 2;
            }
            piVar7[0x18] = (&DAT_00069204)[iVar4 * 4];
          } while( true );
        }
LAB_000473f8:
        param4 = param4 + 1;
      } while (param4 != 4);
      puVar2 = &DAT_0007c130;
      iVar9 = 0;
      pcVar5 = &DAT_0009d4bb;
      iVar8 = 0;
      iVar6 = 0;
      do {
        pcVar5 = pcVar5 + 1;
        if (*pcVar5 == '\x01') {
          iVar9 = iVar9 + (&DAT_0007c224)[iVar6 * 3] + puVar2[0x3e] + puVar2[0x3f];
          iVar8 = iVar8 + (&DAT_0007c254)[iVar6 * 3] + puVar2[0x4a] + puVar2[0x4b];
          if ((DAT_00078e09 != '\0') &&
             (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
            snprintf(acStack_1028,0x1000,"chain%d %d %d %d\n",iVar6,puVar2[0x3d],puVar2[0x3e],
                     puVar2[0x3f]);
            FUN_00038730(7,acStack_1028,0);
          }
        }
        iVar6 = iVar6 + 1;
        puVar2 = puVar2 + 3;
      } while (iVar6 != 4);
    } while (iVar8 != (uint)DAT_0009d6cb * 3);
    iVar6 = FUN_0005c710(iVar9,iVar8);
    if (800 < iVar6 || local_102c == 0x339) {
      FUN_000470a8();
      if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
        snprintf(acStack_1028,0x1000,"scan freq done, avg freq %d max %d, reboot cgminer\n",iVar6,
                 local_102c);
        FUN_00038730(5,acStack_1028,0);
      }
      system("/etc/init.d/cgminer.sh restart > /dev/null 2>&1 &");
      goto LAB_000473a8;
    }
    if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
      snprintf(acStack_1028,0x1000,"scan freq again, avg freq %d max %d\n",iVar6,0x339);
      FUN_00038730(5,acStack_1028,0);
    }
    local_102c = 0x339;
  } while( true );
}

