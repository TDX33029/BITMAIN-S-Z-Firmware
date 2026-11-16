//==================== FUN_000481ec @ 000481ec ====================

void FUN_000481ec(uint param_1,uint param_2,uint param_3,int param_4)

{
  char cVar1;
  char cVar2;
  int iVar3;
  byte bVar4;
  int iVar5;
  undefined4 uVar6;
  undefined *puVar7;
  char *__format;
  uint uVar8;
  uint uVar9;
  int iVar10;
  short *psVar11;
  undefined4 uStack_1030;
  undefined2 local_102c;
  undefined1 local_102a;
  char acStack_1028 [4100];
  
  local_102c = 0;
  uStack_1030 = 0;
  local_102a = 0;
  FUN_000481b4();
  if ((DAT_00078e09 != '\0') &&
     (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
    snprintf(acStack_1028,0x1000,"%s: check chain J%d mode %d chipaddr %02x regaddr %02x",
             "check_asic_reg",param_1,param_4,param_2,param_3);
    FUN_00038730(7,acStack_1028,0);
  }
  uVar6 = FUN_0003cef0(&uStack_1030,param_4,param_2,param_3);
  FUN_0003c150(param_1,&uStack_1030,uVar6);
  if ((param_3 == 0) && ((&DAT_00079b90)[param_1] != '\0')) {
    (&DAT_0009d6c5)[param_1] = 0;
  }
  uVar9 = 0;
  do {
    while( true ) {
      usleep(300000);
      pthread_mutex_lock((pthread_mutex_t *)&DAT_00079b78);
      iVar5 = DAT_0007bd9c;
      pthread_mutex_unlock((pthread_mutex_t *)&DAT_00079b78);
      if (iVar5 != 0) break;
      uVar9 = uVar9 + 1;
      usleep(100000);
      if (2 < uVar9) {
        if (param_3 == 0) {
          if (DAT_0009d6f4 < (byte)(&DAT_0009d6c5)[param_1]) {
            DAT_0009d6f4 = (&DAT_0009d6c5)[param_1];
          }
          if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
            snprintf(acStack_1028,0x1000,"%s: chain J%d has %d ASIC","check_asic_reg",param_1,
                     (uint)(byte)(&DAT_0009d6c5)[param_1]);
            FUN_00038730(5,acStack_1028,0);
          }
        }
        FUN_000481b4();
        return;
      }
    }
    iVar10 = 0;
    do {
      pthread_mutex_lock((pthread_mutex_t *)&DAT_00079b78);
      iVar3 = DAT_0007bd98 * 7;
      if ((byte)(&DAT_0007bda6)[iVar3] == param_1) {
        cVar1 = (&DAT_0007bda5)[iVar3];
        uVar8 = *(uint *)(&DAT_0007bda1 + iVar3);
        uVar9 = uVar8 >> 0x10 & 0xff;
        if (param_3 == 0) {
          if ((&DAT_00079b90)[param_1] == '\0') {
            if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
              __format = "%s: the asic address is 0x%08x";
              goto LAB_000484f0;
            }
          }
          else {
            (&DAT_0009d6c5)[param_1] = (&DAT_0009d6c5)[param_1] + '\x01';
          }
        }
        else if (param_3 == 0xc) {
          if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
            __format = "%s: the asic freq is 0x%08x";
            goto LAB_000484f0;
          }
        }
        else if (param_3 == 0x14) {
          if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
            __format = "%s: the asic tm is 0x%08x";
            goto LAB_000484f0;
          }
        }
        else if (param_3 == 0x1c) {
          if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
            __format = "%s: the asic misc is 0x%08x";
            goto LAB_000484f0;
          }
        }
        else if (param_3 == 0x20) {
          if ((uVar8 << 0x18 & 0xc0000000) == 0) {
            bVar4 = (byte)(uVar8 >> 0x18);
            puVar7 = &DAT_00079bb4 + param_1 * 0xc;
            psVar11 = (short *)(&DAT_0009d534 + param_1 * 0x18);
            iVar3 = 0;
            cVar2 = DAT_00079130;
            while( true ) {
              if (cVar2 == cVar1) {
                if (uVar9 == 0) {
                  *psVar11 = bVar4 - 0x40;
                  *puVar7 = 1;
                }
                else if (uVar9 == 1) {
                  psVar11[1] = bVar4 - 0x40;
                  puVar7[1] = 1;
                }
                else if (uVar9 == 0x11) {
                  *(byte *)(param_1 * 4 + 0x79b94 + iVar3) = bVar4;
                  puVar7[2] = 1;
                }
                else if (uVar9 == 0xfe) {
                  (&DAT_00079ba4)[iVar3 + param_1 * 4] = bVar4;
                  puVar7[3] = 1;
                }
              }
              psVar11 = psVar11 + 4;
              puVar7 = puVar7 + 4;
              if (iVar3 + 1 == 3) break;
              cVar2 = (&DAT_00079131)[iVar3];
              iVar3 = iVar3 + 1;
            }
          }
        }
        else if (param_3 == 0x54) {
          if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
            __format = "%s: the start nonce offset is 0x%08x";
LAB_000484f0:
            snprintf(acStack_1028,0x1000,__format,"check_asic_reg",
                     uVar8 << 0x18 | (uVar8 >> 8 & 0xff) << 0x10 | uVar9 << 8 | uVar8 >> 0x18);
            FUN_00038730(5,acStack_1028,0);
          }
        }
        else if ((param_3 == 0x30) &&
                (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)))) {
          __format = "%s: the asic IO strength is 0x%08x";
          goto LAB_000484f0;
        }
        DAT_0007bd98 = DAT_0007bd98 + 1;
        DAT_0007bd9c = DAT_0007bd9c + -1;
        if (DAT_0007bd98 == 100) {
          DAT_0007bd98 = 0;
        }
      }
      else {
        DAT_0007bd98 = DAT_0007bd98 + 1;
        DAT_0007bd9c = DAT_0007bd9c + -1;
        if (DAT_0007bd98 == 100) {
          DAT_0007bd98 = 0;
        }
        if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
          snprintf(acStack_1028,0x1000,
                   "%s: the return data is from chain%d, but it should be from chain%d chipaddr=0x%02x reg=0x%02x\n"
                   ,"check_asic_reg",(uint)(byte)(&DAT_0007bda6)[iVar3],param_1,param_2,param_3);
          FUN_00038730(5,acStack_1028,0);
        }
      }
      iVar10 = iVar10 + 1;
      pthread_mutex_unlock((pthread_mutex_t *)&DAT_00079b78);
    } while (iVar10 != iVar5);
    uVar9 = 0;
  } while( true );
}

