//==================== FUN_0001bba4 @ 0001bba4 ====================

/* WARNING: Heritage AFTER dead removal. Example location: s1 : 0x0001be2c */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* WARNING: Restarted to delay deadcode elimination for space: register */

void FUN_0001bba4(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  uint uVar1;
  bool bVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  int iVar7;
  double *pdVar8;
  uint in_fpscr;
  undefined4 extraout_s0;
  double in_d0;
  undefined4 uVar9;
  undefined4 in_s3;
  undefined8 uVar10;
  double dVar11;
  double dVar12;
  double dVar13;
  undefined8 uVar14;
  char acStack_1178 [16];
  char acStack_1168 [16];
  char acStack_1158 [16];
  char acStack_1148 [256];
  char acStack_1048 [4100];
  
  uVar14 = CONCAT44(param_4,param_3);
  FUN_0002a84c(&DAT_00077148);
  FUN_0002aa24(&DAT_00077148,&DAT_000770d8);
  iVar4 = DAT_00077148;
  extraout_s0 = SUB84(in_d0,0);
  uVar9 = (undefined4)((ulonglong)in_d0 >> 0x20);
  if (DAT_00077148 - DAT_00077150 < DAT_00075dcc) {
    if (param_1 < 0) {
      return;
    }
    FUN_0002a530(&DAT_000770d8,&DAT_00077148);
    bVar2 = false;
  }
  else {
    DAT_00077150 = DAT_00077148;
    DAT_00077154 = DAT_00077155 ^ DAT_00077154;
    FUN_0002a530(&DAT_000770d8,&DAT_00077148);
    if (param_1 < 0) {
      iVar4 = pthread_mutex_lock((pthread_mutex_t *)&DAT_0007b384);
      if (iVar4 != 0) {
        FUN_0000b414("hashmeter",0x1a6b);
      }
      if (0 < DAT_0007bb6c) {
        iVar4 = 0;
        do {
          iVar3 = iVar4 + 1;
          iVar4 = FUN_0001b124(iVar4);
          iVar4 = *(int *)(iVar4 + 0x24);
          FUN_0002aa24(&DAT_00077148,iVar4 + 0x8c);
          FUN_0002a530(iVar4 + 0x8c,&DAT_00077148);
          uVar10 = VectorSignedToFloat(DAT_00075dcc,(byte)(in_fpscr >> 0x16) & 3);
          FUN_00015798(0,uVar9,extraout_s0,in_s3,(int)uVar10,iVar4 + 0x30);
          FUN_00015798(0,uVar9,extraout_s0,in_s3,0,iVar4 + 0x38);
          FUN_00015798(0,uVar9,extraout_s0,in_s3,0,iVar4 + 0x40);
          FUN_00015798(0,uVar9,extraout_s0,in_s3,0,iVar4 + 0x48);
          iVar4 = iVar3;
        } while (iVar3 < DAT_0007bb6c);
      }
      iVar4 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_0007b384);
      if (iVar4 != 0) {
        FUN_0000b5c4("hashmeter",0x1a78);
      }
      bVar2 = true;
      (*DAT_00075ed8)();
      goto LAB_0001be98;
    }
    bVar2 = true;
  }
  iVar3 = FUN_0001b124(param_1);
  iVar7 = *(int *)(iVar3 + 0x24);
  FUN_0002a530(iVar3 + 0x2c,&DAT_00077148);
  *(int *)(iVar7 + 0xf0) = iVar4;
  FUN_0002aa24(&DAT_00077148,iVar7 + 0x8c);
  FUN_0002a530(iVar7 + 0x8c,&DAT_00077148);
  iVar4 = pthread_mutex_lock((pthread_mutex_t *)&DAT_0007b384);
  if (iVar4 != 0) {
    FUN_0000b414("hashmeter",0x1a51);
  }
  dVar13 = (double)FUN_0005cb50(param_3,param_4);
  iVar4 = DAT_00075dcc;
  uVar5 = SUB84(dVar13,0);
  *(double *)(iVar7 + 0x50) = *(double *)(iVar7 + 0x50) + dVar13;
  uVar10 = VectorSignedToFloat(iVar4,(byte)(in_fpscr >> 0x16) & 3);
  FUN_00015798(uVar5,uVar9,extraout_s0,in_s3,(int)uVar10,iVar7 + 0x30);
  FUN_00015798(uVar5,uVar9,extraout_s0,in_s3,0,iVar7 + 0x38);
  FUN_00015798(uVar5,uVar9,extraout_s0,in_s3,0,iVar7 + 0x40);
  FUN_00015798(uVar5,uVar9,extraout_s0,in_s3,0,iVar7 + 0x48);
  iVar4 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_0007b384);
  if (iVar4 != 0) {
    FUN_0000b5c4("hashmeter",0x1a57);
  }
  (*DAT_00075ed8)();
  FUN_00017744(acStack_1148,0x100,iVar7);
  if (DAT_00077018 == '\0') {
    printf("%s          \r",acStack_1148);
    fflush(stdout);
  }
  else if ((DAT_00078e09 != '\0') &&
          (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
    snprintf(acStack_1048,0x1000,"%s",acStack_1148);
    FUN_00038730(7,acStack_1048,0);
  }
LAB_0001be98:
  iVar4 = pthread_mutex_lock((pthread_mutex_t *)&DAT_0007b384);
  if (iVar4 != 0) {
    FUN_0000b414("hashmeter",0x1a7b);
  }
  dVar13 = (double)FUN_0005cb50(param_3,param_4);
  uVar5 = SUB84(dVar13,0);
  _DAT_0007acc0 = dVar13 + _DAT_0007acc0;
  if (bVar2) {
    DAT_00077140 = DAT_00077140 + 1;
    uVar10 = 0;
    iVar4 = 0;
    pdVar8 = (double *)&DAT_000770e0;
    if (0xb < DAT_00077140) {
      DAT_00077140 = 0;
    }
    do {
      dVar11 = *pdVar8;
      pdVar8 = pdVar8 + 1;
      uVar1 = in_fpscr & 0xfffffff | (uint)(dVar11 < 0.0) << 0x1f;
      in_fpscr = uVar1 | (uint)NAN(dVar11) << 0x1c;
      if ((byte)(uVar1 >> 0x1f) == ((byte)(in_fpscr >> 0x1c) & 1)) {
        dVar12 = (double)FUN_0005cb50((int)uVar10,(int)((ulonglong)uVar10 >> 0x20));
        iVar4 = iVar4 + 1;
        uVar10 = FUN_0005cc48(SUB84(dVar11 + dVar12,0),(int)((ulonglong)(dVar11 + dVar12) >> 0x20));
      }
    } while (pdVar8 != (double *)&DAT_00077140);
    if (iVar4 != 0) {
      uVar14 = FUN_0005cbc4((int)uVar10,(int)((ulonglong)uVar10 >> 0x20),iVar4,iVar4 >> 0x1f);
    }
    uVar10 = VectorSignedToFloat(DAT_00075dcc,(byte)(in_fpscr >> 0x16) & 3);
    uVar6 = FUN_0005cb50((int)uVar14,(int)((ulonglong)uVar14 >> 0x20));
    FUN_00015798(uVar6,uVar9,(int)uVar10,in_s3,(int)uVar10,&DAT_0007bb10);
    FUN_00015798(uVar5,uVar9,extraout_s0,in_s3,0,&DAT_0007bc98);
    FUN_00015798(uVar5,uVar9,extraout_s0,in_s3,0,&DAT_0007bc88);
    FUN_00015798(uVar5,uVar9,extraout_s0,in_s3,0,&DAT_0007b2f8);
    in_d0 = _DAT_0007bb10;
    _DAT_0007bad0 = llround(_DAT_0007bb10);
    iVar4 = DAT_00077140;
    dVar11 = 0.0;
    *(undefined8 *)(&DAT_000770e0 + DAT_00077140 * 2) = 0;
    _DAT_0007bad0 = _DAT_0007bad0 * 1000000;
  }
  else {
    dVar11 = *(double *)(&DAT_000770e0 + DAT_00077140 * 2);
    iVar4 = DAT_00077140;
  }
  *(double *)(&DAT_000770e0 + iVar4 * 2) = dVar11 + dVar13;
  FUN_0002aa24(&DAT_00077148,&DAT_000770d0);
  DAT_000755f8 = in_d0;
  if (in_d0 - DAT_00075de8 <= 86400.0) {
    DAT_00075de8 = in_d0;
  }
  else {
    if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
      snprintf(acStack_1048,0x1000,"cgminer time error total_secs = %f last_total_secs = %f",in_d0,
               DAT_00075de8);
      FUN_00038730(3,acStack_1048,0);
    }
    iVar4 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_0007b384);
    if (iVar4 != 0) {
      FUN_0000b5c4("hashmeter",0x1aa3);
    }
    (*DAT_00075ed8)();
    FUN_0001b8b4();
    iVar4 = pthread_mutex_lock((pthread_mutex_t *)&DAT_0007b384);
    if (iVar4 != 0) {
      FUN_0000b414("hashmeter",0x1aa5);
    }
  }
  if (bVar2) {
    dVar13 = (_DAT_0007acc0 / DAT_000755f8) * 1000000.0;
    uVar14 = FUN_0005cc48(SUB84(dVar13,0),(int)((ulonglong)dVar13 >> 0x20));
    FUN_00011580((int)uVar14,(int)((ulonglong)uVar14 >> 0x20),acStack_1178,0x10);
    DAT_00077158 = _DAT_0007bb10 / 1000.0;
    uVar14 = FUN_0005cc48(SUB84(_DAT_0007bb10 * 1000000.0,0),
                          (int)((ulonglong)(_DAT_0007bb10 * 1000000.0) >> 0x20));
    FUN_00011580((int)uVar14,(int)((ulonglong)uVar14 >> 0x20),acStack_1168,0x10);
    uVar14 = FUN_0005cc48(SUB84(_DAT_0007bc98 * 1000000.0,0),
                          (int)((ulonglong)(_DAT_0007bc98 * 1000000.0) >> 0x20));
    FUN_00011580((int)uVar14,(int)((ulonglong)uVar14 >> 0x20),acStack_1158,0x10);
    uVar14 = FUN_0005cc48(SUB84(_DAT_0007bc88 * 1000000.0,0),
                          (int)((ulonglong)(_DAT_0007bc88 * 1000000.0) >> 0x20));
    FUN_00011580((int)uVar14,(int)((ulonglong)uVar14 >> 0x20),acStack_1148,0x10);
    uVar14 = FUN_0005cc48(SUB84(_DAT_0007b2f8 * 1000000.0,0),
                          (int)((ulonglong)(_DAT_0007b2f8 * 1000000.0) >> 0x20));
    FUN_00011580((int)uVar14,(int)((ulonglong)uVar14 >> 0x20),acStack_1048,0x10);
    snprintf(&DAT_00077160,0x100,"(%ds):%s (1m):%s (5m):%s (15m):%s (avg):%sh/s",DAT_00075dcc,
             acStack_1168,acStack_1158,acStack_1148,acStack_1048,acStack_1178);
  }
  iVar4 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_0007b384);
  if (iVar4 != 0) {
    FUN_0000b5c4("hashmeter",0x1ac2);
  }
  (*DAT_00075ed8)();
  if (bVar2) {
    if (DAT_00077018 == '\0') {
      printf("%s          \r",&DAT_00077160);
      fflush(stdout);
    }
    else if ((DAT_00078e09 != '\0') &&
            (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
      snprintf(acStack_1048,0x1000,"%s",&DAT_00077160);
      FUN_00038730(7,acStack_1048,0);
    }
  }
  return;
}

