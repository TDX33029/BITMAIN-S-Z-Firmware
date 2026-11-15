//==================== FUN_0003af90 @ 0003af90 ====================

undefined4 FUN_0003af90(int param_1)

{
  byte bVar1;
  byte bVar2;
  undefined1 uVar3;
  char cVar4;
  int iVar5;
  int iVar6;
  uint uVar7;
  int *piVar8;
  byte *pbVar9;
  byte *pbVar10;
  int iVar11;
  uint param3;
  int iVar12;
  pthread_mutex_t *__mutex;
  undefined8 uVar13;
  undefined1 auStack_1720 [4];
  undefined4 local_171c;
  undefined1 auStack_1718 [8];
  undefined4 local_1710;
  undefined4 local_170c;
  undefined4 local_1708;
  undefined4 local_1704;
  undefined4 local_1700;
  undefined4 uStack_16fc;
  undefined4 local_16f8;
  undefined4 local_16f4;
  byte abStack_16f0 [392];
  undefined1 auStack_1568 [1344];
  char acStack_1028 [4100];
  
  iVar12 = *(int *)(param_1 + 0x24);
  __mutex = *(pthread_mutex_t **)(iVar12 + 0x14);
  memset(auStack_1568,0,0x540);
  FUN_0002a84c(auStack_1718);
  DAT_00078e18 = 0;
  pthread_mutex_lock((pthread_mutex_t *)&DAT_00078e88);
  iVar11 = DAT_0007c294;
  pthread_mutex_unlock((pthread_mutex_t *)&DAT_00078e88);
  iVar5 = pthread_mutex_lock(__mutex);
  if (iVar5 != 0) {
    piVar8 = __errno_location();
    snprintf(acStack_1028,0x1000,"WTF MUTEX ERROR ON LOCK! errno=%d in %s %s():%d",*piVar8,
             "driver-btm-zcash.c","bitmain_scanhash",0x189e);
    FUN_00038730(3,acStack_1028,1);
    FUN_00016ca8(1);
  }
  iVar5 = pthread_rwlock_rdlock((pthread_rwlock_t *)(__mutex + 1));
  if (iVar5 != 0) {
    piVar8 = __errno_location();
    snprintf(acStack_1028,0x1000,"WTF RDLOCK ERROR ON LOCK! errno=%d in %s %s():%d",*piVar8,
             "driver-btm-zcash.c","bitmain_scanhash",0x189e);
    FUN_00038730(3,acStack_1028,1);
    FUN_00016ca8(1);
  }
  iVar5 = pthread_mutex_unlock(__mutex);
  if (iVar5 != 0) {
    piVar8 = __errno_location();
    snprintf(acStack_1028,0x1000,"WTF MUTEX ERROR ON UNLOCK! errno=%d in %s %s():%d",*piVar8,
             "driver-btm-zcash.c","bitmain_scanhash",0x189e);
    FUN_00038730(3,acStack_1028,1);
    FUN_00016ca8(1);
  }
  if (iVar11 != 0) {
    iVar5 = 0;
    do {
      iVar11 = iVar11 + -1;
      pthread_mutex_lock((pthread_mutex_t *)&DAT_00078e88);
      uVar7 = DAT_0007c290;
      iVar6 = DAT_0007c290 * 0x54d;
      DAT_0007c294 = DAT_0007c294 + -1;
      bVar1 = (&DAT_0007c7da)[iVar6];
      param3 = (uint)(byte)(&DAT_0007c7e4)[iVar6];
      bVar2 = (&DAT_0007c7d9)[iVar6];
      uVar3 = (&DAT_0007c7d8)[iVar6];
      memcpy(auStack_1568,(void *)(iVar6 + 0x7c298),0x540);
      local_171c = *(undefined4 *)(&DAT_0007c7e0 + iVar6);
      if (uVar7 < 99) {
        DAT_0007c290 = uVar7 + 1;
      }
      else {
        DAT_0007c290 = 0;
      }
      pthread_mutex_unlock((pthread_mutex_t *)&DAT_00078e88);
      if (4 < param3) {
        if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
          snprintf(acStack_1028,0x1000,"Error:scanhash chainid=%u\n",param3);
          FUN_00038730(5,acStack_1028,0);
        }
        break;
      }
      pthread_mutex_lock((pthread_mutex_t *)&DAT_00078ea0);
      piVar8 = (int *)((int)&__mutex[4].__data.field5_0x14.__spins + (uint)bVar2 * 4 + 2);
      if (*piVar8 != 0) {
        iVar5 = FUN_0001a44c(*piVar8,0);
      }
      pthread_mutex_unlock((pthread_mutex_t *)&DAT_00078ea0);
      if (iVar5 == 0) {
        if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
          snprintf(acStack_1028,0x1000,"%s %d: work %02x not find error",
                   *(char **)(*(int *)(iVar12 + 4) + 8),*(int *)(iVar12 + 8),(uint)bVar2);
LAB_0003b330:
          FUN_00038730(3,acStack_1028,0);
        }
      }
      else {
        *(undefined4 *)(iVar5 + 0x168) = local_171c;
        memcpy((void *)(iVar5 + 0x16f),auStack_1568,0x540);
        FUN_0004eee0(abStack_16f0 + 0x20,200,9);
        FUN_00054f8c(abStack_16f0 + 0x20,(void *)(iVar5 + 0xe0),0x8c,0);
        local_170c = 0;
        uStack_16fc = 0;
        local_16f4 = 0;
        local_1708 = 0;
        abStack_16f0[0] = 0;
        abStack_16f0[1] = 0;
        abStack_16f0[2] = 0;
        abStack_16f0[3] = 0;
        local_1710 = 0;
        local_1704 = 0;
        abStack_16f0[4] = 0;
        abStack_16f0[5] = 0;
        abStack_16f0[6] = 0;
        abStack_16f0[7] = 0;
        abStack_16f0[8] = 0;
        abStack_16f0[9] = 0;
        abStack_16f0[10] = 0;
        abStack_16f0[0xb] = 0;
        local_1700 = 0;
        abStack_16f0[0xc] = 0;
        abStack_16f0[0xd] = 0;
        abStack_16f0[0xe] = 0;
        abStack_16f0[0xf] = 0;
        abStack_16f0[0x10] = 0;
        abStack_16f0[0x11] = 0;
        abStack_16f0[0x12] = 0;
        abStack_16f0[0x13] = 0;
        local_16f8 = 0;
        abStack_16f0[0x14] = 0;
        abStack_16f0[0x15] = 0;
        abStack_16f0[0x16] = 0;
        abStack_16f0[0x17] = 0;
        abStack_16f0[0x18] = 0;
        abStack_16f0[0x19] = 0;
        abStack_16f0[0x1a] = 0;
        abStack_16f0[0x1b] = 0;
        abStack_16f0[0x1c] = 0;
        abStack_16f0[0x1d] = 0;
        abStack_16f0[0x1e] = 0;
        abStack_16f0[0x1f] = 0;
        memset(acStack_1028,0,0x5cf);
        memcpy(acStack_1028,(void *)(iVar5 + 0xe0),0x5cf);
        FUN_00055930(acStack_1028,0x5cf,&local_1710);
        FUN_00055930(&local_1710,0x20,abStack_16f0);
        pbVar10 = abStack_16f0 + 0x20;
        pbVar9 = (byte *)(iVar5 + 0xc0);
        do {
          pbVar10 = pbVar10 + -1;
          pbVar9 = pbVar9 + -1;
          if (*pbVar10 < *pbVar9) break;
          if (*pbVar9 < *pbVar10) {
            iVar6 = FUN_0004eaf4(abStack_16f0 + 0x20,auStack_1568);
            if (iVar6 == 0) goto LAB_0003b35c;
            goto LAB_0003b250;
          }
        } while (pbVar10 != abStack_16f0);
        iVar6 = FUN_0004eaf4(abStack_16f0 + 0x20,auStack_1568);
        if (iVar6 == 0) {
          FUN_00025e28(param_1,iVar5,1);
LAB_0003b35c:
          uVar7 = (&DAT_00078e60)[param3 * 2];
          cVar4 = (&DAT_0009d4bc)[param3];
          DAT_00078e18 = CONCAT44(DAT_00078e18._4_4_ + (uint)(0xfffffffe < (uint)DAT_00078e18),
                                  (uint)DAT_00078e18 + 1);
          (&DAT_00078e60)[param3 * 2] = uVar7 + 1;
          (&DAT_00078e64)[param3 * 2] = (&DAT_00078e64)[param3 * 2] + (uint)(0xfffffffe < uVar7);
          if (cVar4 == '\0') {
            if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
              snprintf(acStack_1028,0x1000,"ChainID Cause Error! ChainID:[%d]",param3);
              goto LAB_0003b330;
            }
          }
          else {
            uVar7 = FUN_0005c648(uVar3,DAT_0009d6f3);
            uVar7 = uVar7 & 0xff;
            if (uVar7 < 3) {
              FUN_0001a730(param_1,iVar5);
              iVar6 = (param3 * 3 + uVar7 + 0x40) * 4;
              *(int *)(&DAT_0009d4b7 + iVar6) = *(int *)(&DAT_0009d4b7 + iVar6) + 1;
              goto LAB_0003b270;
            }
            if ((DAT_00078e09 != '\0') &&
               (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
              snprintf(acStack_1028,0x1000,"Which Nonce Cause Err![%d] %08x",uVar7,(uint)bVar1);
              FUN_00038730(7,acStack_1028,0);
            }
          }
        }
        else {
LAB_0003b250:
          if (iVar6 == 2) {
            uVar7 = FUN_0005c648(uVar3,DAT_0009d6f3);
            iVar6 = (param3 * 3 + (uVar7 & 0xff) + 0x4c) * 4;
            *(int *)(&DAT_0009d4b7 + iVar6) = *(int *)(&DAT_0009d4b7 + iVar6) + 1;
          }
          if (DAT_00077144 != '\0') {
            DAT_00077144 = '\0';
            memset(&DAT_0009d5a7,0,0x10);
          }
LAB_0003b270:
          FUN_0002f934(iVar5,auStack_1720,1);
          FUN_00015780(iVar5);
          iVar5 = 0;
        }
      }
    } while (iVar11 != 0);
  }
  iVar11 = pthread_rwlock_unlock((pthread_rwlock_t *)(__mutex + 1));
  if (iVar11 != 0) {
    piVar8 = __errno_location();
    snprintf(acStack_1028,0x1000,"WTF RWLOCK ERROR ON UNLOCK! errno=%d in %s %s():%d",*piVar8,
             "driver-btm-zcash.c","bitmain_scanhash",0x190b);
    FUN_00038730(3,acStack_1028,1);
    FUN_00016ca8(1);
  }
  (*DAT_00075ed8)();
  uVar13 = VectorShiftLeft(DAT_00078e18,0x10,0x40,0);
  DAT_00078e18 = VectorSub(uVar13,DAT_00078e18,8);
  return 0;
}

