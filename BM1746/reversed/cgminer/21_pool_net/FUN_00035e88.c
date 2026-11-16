//==================== FUN_00035e88 @ 00035e88 ====================

void FUN_00035e88(int param_1,undefined4 param_2,undefined4 param_3,int param_4)

{
  time_t tVar1;
  uint uVar2;
  undefined4 uVar3;
  undefined *puVar4;
  int iVar5;
  int iVar6;
  char *pcVar7;
  undefined4 uVar8;
  double dVar9;
  int local_144;
  int local_13c;
  undefined4 local_138;
  undefined4 local_134;
  undefined4 local_130;
  undefined4 local_12c;
  char local_128 [260];
  
  memset(local_128,0,0x100);
  if (DAT_0007b198 == 0) {
    FUN_0003305c(param_1,8,0,0,param_4);
  }
  else {
    FUN_0003305c(param_1,7,0,0,param_4);
    if (param_4 == 0) {
      local_144 = 0;
    }
    else {
      local_144 = FUN_000307c8(param_1,",\"POOLS\":[");
    }
    local_13c = 0;
    if (0 < DAT_0007b198) {
      uVar8 = 0;
      do {
        iVar6 = *(int *)(DAT_0007b438 + local_13c * 4);
        if (*(char *)(iVar6 + 0x89) == '\0') {
          iVar5 = *(int *)(iVar6 + 0x84);
          if (iVar5 == 1) {
            pcVar7 = "Alive";
            if (*(char *)(iVar6 + 0x81) != '\0') {
              pcVar7 = "Dead";
            }
          }
          else if (iVar5 == 0) {
            pcVar7 = "Disabled";
          }
          else {
            pcVar7 = "Rejecting";
            if (iVar5 != 2) {
              pcVar7 = "Unknown";
            }
          }
          puVar4 = &DAT_000670ac;
          if (*(int *)(iVar6 + 0x8c) == 0) {
            puVar4 = &UNK_000670b0;
          }
          if (*(int *)(iVar6 + 0x180) < 1) {
            local_128[0] = '0';
            local_128[1] = '\0';
          }
          else {
            tVar1 = time((time_t *)0x0);
            uVar2 = tVar1 - *(int *)(iVar6 + 0x180);
            uVar2 = uVar2 & ~((int)uVar2 >> 0x1f);
            iVar5 = (int)uVar2 % 0xe10;
            sprintf(local_128,"%d:%02d:%02d",(int)uVar2 / 0xe10,iVar5 / 0x3c,iVar5 % 0x3c);
          }
          uVar8 = FUN_00032ed0(uVar8,&DAT_00067488,&local_13c,0);
          uVar8 = FUN_00032df8(uVar8,"URL",*(undefined4 *)(iVar6 + 0xc4),0);
          uVar8 = FUN_00032e1c(uVar8,"Status",pcVar7,0);
          uVar8 = FUN_00032ed0(uVar8,"Priority",iVar6 + 4,0);
          uVar8 = FUN_00032ed0(uVar8,"Quota",iVar6 + 0x38,0);
          uVar8 = FUN_00032e1c(uVar8,"Long Poll",puVar4,0);
          uVar8 = FUN_00032ef4(uVar8,"Getworks",iVar6 + 0x94,0);
          uVar8 = FUN_00032f84(uVar8,"Accepted",iVar6 + 8,0);
          uVar8 = FUN_00032f84(uVar8,"Rejected",iVar6 + 0x10,0);
          uVar8 = FUN_00032ef4(uVar8,"Discarded",iVar6 + 0x9c,0);
          uVar8 = FUN_00032ef4(uVar8,"Stale",iVar6 + 0x98,0);
          uVar8 = FUN_00032ef4(uVar8,"Get Failures",iVar6 + 0xa0,0);
          uVar8 = FUN_00032ef4(uVar8,"Remote Failures",iVar6 + 0xa4,0);
          uVar8 = FUN_00032df8(uVar8,&DAT_000674b8,*(undefined4 *)(iVar6 + 0xcc),0);
          uVar8 = FUN_00032e1c(uVar8,"Last Share Time",local_128,0);
          uVar8 = FUN_00032e1c(uVar8,&DAT_000674d0,iVar6 + 0x30,0);
          uVar8 = FUN_00032f84(uVar8,"Diff1 Shares",iVar6 + 0x28,0);
          if (*(int *)(iVar6 + 0xd8) == 0) {
            uVar8 = FUN_00032e40(uVar8,"Proxy Type","",0);
            uVar8 = FUN_00032e40(uVar8,"Proxy","",0);
          }
          else {
            uVar3 = FUN_00029690(*(undefined4 *)(iVar6 + 0xd4));
            uVar8 = FUN_00032e40(uVar8,"Proxy Type",uVar3,0);
            uVar8 = FUN_00032df8(uVar8,"Proxy",*(undefined4 *)(iVar6 + 0xd8),0);
          }
          uVar8 = FUN_000355f8(uVar8,"Difficulty Accepted",iVar6 + 0x68,0);
          uVar8 = FUN_000355f8(uVar8,"Difficulty Rejected",iVar6 + 0x70,0);
          uVar8 = FUN_000355f8(uVar8,"Difficulty Stale",iVar6 + 0x78,0);
          uVar8 = FUN_000355f8(uVar8,"Last Share Difficulty",iVar6 + 0x188,0);
          uVar8 = FUN_00032ff0(uVar8,"Has Stratum",iVar6 + 0x298,0);
          uVar8 = FUN_00032ff0(uVar8,"Stratum Active",iVar6 + 0x299,0);
          if (*(char *)(iVar6 + 0x299) == '\0') {
            uVar8 = FUN_00032e40(uVar8,"Stratum URL","");
          }
          else {
            uVar8 = FUN_00032df8(uVar8,"Stratum URL",*(undefined4 *)(iVar6 + 600),0);
          }
          uVar8 = FUN_00032ff0(uVar8,&DAT_0006753c,iVar6 + 0x2f8,0);
          uVar8 = FUN_00032f60(uVar8,"Best Share",iVar6 + 400,1);
          dVar9 = *(double *)(iVar6 + 0x70) + *(double *)(iVar6 + 0x68) + *(double *)(iVar6 + 0x78);
          if (dVar9 == 0.0) {
            dVar9 = 0.0;
          }
          else {
            dVar9 = *(double *)(iVar6 + 0x70) / dVar9;
          }
          local_134 = (undefined4)((ulonglong)dVar9 >> 0x20);
          local_138 = SUB84(dVar9,0);
          uVar8 = FUN_000357a4(uVar8,"Pool Rejected%",&local_138,0);
          dVar9 = *(double *)(iVar6 + 0x68) + *(double *)(iVar6 + 0x70) + *(double *)(iVar6 + 0x78);
          if (dVar9 == 0.0) {
            dVar9 = 0.0;
          }
          else {
            dVar9 = *(double *)(iVar6 + 0x78) / dVar9;
          }
          local_12c = (undefined4)((ulonglong)dVar9 >> 0x20);
          local_130 = SUB84(dVar9,0);
          uVar8 = FUN_000357a4(uVar8,"Pool Stale%",&local_130,0);
          uVar8 = FUN_00032534(param_1,uVar8,param_4);
        }
        local_13c = local_13c + 1;
      } while (local_13c < DAT_0007b198);
    }
    if (local_144 != 0) {
      *(undefined1 *)(param_1 + 0xd) = 1;
    }
  }
  return;
}

