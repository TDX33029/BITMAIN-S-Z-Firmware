//==================== FUN_00024db0 @ 00024db0 ====================

void FUN_00024db0(int *param_1)

{
  longlong lVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  int iVar6;
  int iVar7;
  uint uVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  int iVar13;
  int param3;
  double *pdVar14;
  int iVar15;
  uint uVar16;
  int iVar17;
  int iVar18;
  int extraout_r12;
  bool bVar19;
  double param4;
  double dVar20;
  double dVar21;
  longlong lVar22;
  int local_108c;
  int local_1088;
  int local_1084;
  int local_107c;
  uint local_1074;
  uint local_1068;
  int iStack_1060;
  int local_105c;
  int iStack_1058;
  int local_1054;
  int iStack_1050;
  int local_104c;
  int iStack_1048;
  int local_1044;
  timespec atStack_1040 [512];
  
  iVar18 = param_1[9];
  param3 = *param_1;
  iVar13 = *(int *)(iVar18 + 4);
  iVar9 = DAT_00075dcc / 5;
  if (iVar9 == 0) {
    iVar9 = 1;
  }
  if (param_1[1] == 0) {
    local_1068 = 1;
  }
  else {
    local_1068 = (uint)*(byte *)(param_1 + 2);
  }
  local_1074 = (**(code **)(iVar13 + 0x30))(param_1);
  FUN_0002a84c(&iStack_1060);
  FUN_0002a84c(&iStack_1048);
  if (*(char *)(iVar18 + 0x16c) == '\0') {
    iVar10 = iVar9 * 1000000;
    lVar1 = 0;
    iVar12 = iVar10 + 0x3ff;
    if (-1 < iVar10) {
      iVar12 = iVar10;
    }
    iVar7 = 0;
    iVar17 = 0;
    do {
      iVar3 = FUN_000247d4(param_1,param3);
      *(undefined1 *)((int)param_1 + 0x3e) = 0;
      *(undefined1 *)(iVar18 + 0xac) = 1;
      FUN_0002a84c(&iStack_1050);
      *(undefined4 *)(iVar3 + 0x6c8) = 0;
      *(undefined8 *)(iVar18 + 0xa0) = 0;
      iVar4 = (**(code **)(iVar13 + 0x38))(param_1,iVar3);
      cVar2 = DAT_0007b859;
      if (iVar4 == 0) {
        if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
          snprintf((char *)atStack_1040,0x1000,"work prepare failed, exiting mining thread %d",
                   param3);
          FUN_00038730(3,atStack_1040,0);
        }
        break;
      }
      dVar20 = *(double *)(iVar3 + 0x748);
      pdVar14 = (double *)(iVar3 + 0x6b0);
      param4 = *(double *)(iVar13 + 0x68);
      if (dVar20 < *(double *)(iVar13 + 0x68)) {
        param4 = dVar20;
      }
      *pdVar14 = param4;
      if (param4 < *(double *)(iVar13 + 0x78)) {
        param4 = *(double *)(iVar13 + 0x78);
      }
      *pdVar14 = param4;
      if (cVar2 != '\0') {
        dVar21 = (double)FUN_0005cb64(DAT_0007b100,DAT_0007b104);
        iVar4 = iVar13;
        if ((dVar21 / DAT_000755f8) * 60.0 <= 30.0) {
          dVar21 = *(double *)(iVar13 + 0x70);
          iVar4 = extraout_r12;
LAB_0002543c:
          if (dVar20 < dVar21) {
            iVar4 = iVar13;
          }
          if (dVar20 < dVar21) {
            *(double *)(iVar4 + 0x70) = dVar20;
            param4 = *pdVar14;
          }
        }
        else {
          dVar21 = *(double *)(iVar13 + 0x70);
          if ((*(double *)(iVar13 + 0x68) <= dVar21) || (dVar20 <= dVar21)) goto LAB_0002543c;
          bVar19 = DAT_00078e09 != '\0';
          param4 = dVar21 + 1.0;
          *(double *)(iVar13 + 0x70) = param4;
          if ((bVar19) && (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))
             ) {
            snprintf((char *)atStack_1040,0x1000,"Driver %s working diff changed to %.0f",
                     *(char **)(iVar13 + 4),param4);
            FUN_00038730(7,atStack_1040,0);
            param4 = *(double *)(iVar13 + 0x70);
          }
          dVar20 = *(double *)(iVar3 + 0x748);
          if (dVar20 < param4) {
            param4 = dVar20;
          }
          *pdVar14 = param4;
        }
        FUN_00016244(SUB84(param4,0),iVar3 + 0x789);
      }
      local_107c = iVar18 + 0x128;
      local_1084 = iVar3 + 0x778;
      local_1088 = iVar18 + 0x138;
      local_108c = iVar18 + 0x130;
      while( true ) {
        FUN_0002a84c(&iStack_1058);
        FUN_0002a448(&iStack_1058,&iStack_1060);
        FUN_0002a47c(&iStack_1060,local_107c);
        iVar4 = FUN_0002a4c8(&iStack_1060,local_108c);
        if (iVar4 != 0) {
          FUN_0002a530(local_108c,&iStack_1060);
        }
        iVar4 = FUN_0002a4fc(&iStack_1060,local_1088);
        if (iVar4 != 0) {
          FUN_0002a530(local_1088,&iStack_1060);
        }
        *(int *)(iVar18 + 0x124) = *(int *)(iVar18 + 0x124) + 1;
        iVar15 = *(int *)(iVar3 + 0x6d4);
        FUN_0002a47c(&iStack_1060,iVar15 + 0x19c);
        iVar4 = FUN_0002a4c8(&iStack_1060,iVar15 + 0x1a4);
        if (iVar4 != 0) {
          FUN_0002a530(iVar15 + 0x1a4,&iStack_1060);
        }
        iVar4 = FUN_0002a4fc(&iStack_1060,iVar15 + 0x1ac);
        if (iVar4 != 0) {
          FUN_0002a530(iVar15 + 0x1ac,&iStack_1060);
        }
        *(int *)(iVar15 + 0x198) = *(int *)(iVar15 + 0x198) + 1;
        FUN_0002a84c(local_1084);
        pthread_setcancelstate(1,(int *)0x0);
        FUN_0000fc24(param_1);
        lVar22 = (**(code **)(iVar13 + 0x40))(param_1,iVar3,local_1074 + *(int *)(iVar3 + 0x6c8),0);
        iVar11 = (int)((ulonglong)lVar22 >> 0x20);
        FUN_0000fc58(param_1);
        pthread_setcancelstate(0,(int *)0x0);
        pthread_testcancel();
        FUN_0002a84c(&iStack_1060);
        iVar15 = local_105c;
        iVar4 = iStack_1060;
        if (lVar22 == -1) break;
        iVar6 = *(int *)(iVar18 + 0xa4);
        lVar1 = lVar22 + lVar1;
        uVar16 = (uint)((ulonglong)lVar1 >> 0x20);
        bVar19 = *(uint *)(iVar18 + 0xa0) < (uint)lVar22;
        if ((int)(iVar6 - (iVar11 + (uint)bVar19)) < 0 !=
            (SBORROW4(iVar6,iVar11) != SBORROW4(iVar6 - iVar11,(uint)bVar19))) {
          *(longlong *)(iVar18 + 0xa0) = lVar22;
        }
        iVar11 = local_105c - local_1054;
        bVar19 = iVar11 < 0;
        if (bVar19) {
          iVar11 = iVar11 + 1000000;
        }
        iVar11 = iVar7 + iVar11;
        iVar7 = iStack_1060 - iStack_1058;
        if (bVar19) {
          iVar7 = iVar7 + -1;
        }
        iVar17 = iVar17 + iVar7;
        iVar7 = iVar11;
        if (1000000 < iVar11) {
          iVar7 = iVar11 + -0xf4000;
        }
        if (1000000 < iVar11) {
          iVar17 = iVar17 + 1;
          iVar7 = iVar7 + -0x240;
        }
        iVar11 = iStack_1060 - iStack_1050;
        if (local_105c - local_104c < 0) {
          iVar11 = iVar11 + -1;
        }
        if (iVar17 < iVar9) {
          if (local_1074 != 0xffffffff) {
            iVar6 = iVar7 + 0x400;
            if (iVar7 + 0x400 < 0) {
              iVar6 = iVar7 + 0x7ff;
            }
            iVar6 = FUN_0005c710(1000000,iVar6 >> 10);
            iVar6 = (iVar6 + 0x10) * iVar9;
            uVar8 = FUN_0005c648(0xfffffc00,iVar6);
            if (uVar8 < local_1074) {
              local_1074 = 0xffffffff;
            }
            else {
              local_1074 = local_1074 * iVar6 >> 10;
            }
            goto LAB_00025244;
          }
        }
        else {
          if (iVar9 < iVar17) {
            local_1074 = FUN_0005c648(iVar9 * local_1074,iVar17);
          }
          else if (100000 < iVar7) {
            uVar5 = FUN_0005c710(iVar10 + iVar7,iVar12 >> 10);
            local_1074 = FUN_0005c648(local_1074 << 10,uVar5);
          }
LAB_00025244:
          iVar15 = iVar15 - local_1044;
          bVar19 = iVar15 < 0;
          if (bVar19) {
            iVar15 = iVar15 + 0xf4000;
          }
          iVar4 = iVar4 - iStack_1048;
          if (bVar19) {
            iVar4 = iVar4 + -1;
            iVar15 = iVar15 + 0x240;
          }
          uVar8 = (uint)lVar1 | uVar16;
          if (((uVar8 != 0) && ((0 < iVar4 || (200000 < iVar15)))) || (DAT_00075dcc <= iVar4)) {
            FUN_0001bba4(param3,uVar8,(uint)lVar1,uVar16);
            FUN_0002a530(&iStack_1048,&iStack_1060);
            lVar1 = 0;
          }
          if (*(char *)((int)param_1 + 0x3e) != '\0') {
            if (local_1068 == 0) {
              atStack_1040[0].tv_sec = local_1068;
              atStack_1040[0].tv_nsec = param_1[1] * 250000000;
              nanosleep(atStack_1040,(timespec *)0x0);
            }
            goto LAB_00024f3c;
          }
          if (((char)param_1[0xf] == '\0') && (*(int *)(iVar18 + 0x20) == 0)) {
            iVar7 = 0;
            iVar17 = 0;
          }
          else {
            iVar17 = 0;
            iVar7 = 0;
            FUN_00011c8c(param_1,param3,iVar13);
          }
        }
        if (DAT_00075df0 < iVar11) goto LAB_00024f3c;
        bVar19 = *(int *)(iVar18 + 0xa4) == 0;
        if (*(int *)(iVar18 + 0xa4) == 0) {
          bVar19 = *(uint *)(iVar18 + 0xa0) < 0xfffffffe;
        }
        if ((!bVar19) || (iVar4 = FUN_00022b44(iVar3,0), iVar4 != 0)) goto LAB_00024f3c;
      }
      if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
        snprintf((char *)atStack_1040,0x1000,"%s %d failure, disabling!",*(char **)(iVar13 + 8),
                 *(int *)(iVar18 + 8));
        FUN_00038730(3,atStack_1040,0);
      }
      *(undefined4 *)(iVar18 + 0x20) = 1;
      FUN_0002ed88(iVar18,1);
      *(undefined1 *)(iVar18 + 0x16c) = 1;
LAB_00024f3c:
      FUN_00015780(iVar3);
    } while (*(char *)(iVar18 + 0x16c) == '\0');
  }
  *(undefined4 *)(iVar18 + 0x20) = 1;
  return;
}

