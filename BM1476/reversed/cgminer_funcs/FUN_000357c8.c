//==================== FUN_000357c8 @ 000357c8 ====================

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_000357c8(int param_1,undefined4 param_2,undefined4 param_3,uint param_4)

{
  uint uVar1;
  int iVar2;
  undefined4 uVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  char *pcVar7;
  uint in_fpscr;
  double dVar8;
  double dVar9;
  undefined8 uVar10;
  double local_70;
  double local_68;
  double local_60;
  undefined4 local_58;
  undefined4 uStack_54;
  undefined4 local_50;
  undefined4 uStack_4c;
  undefined4 local_48;
  undefined4 uStack_44;
  double dStack_40;
  
  FUN_0003305c(param_1,0xb,0,0,param_4);
  pcVar7 = "SUMMARY,";
  if (param_4 != 0) {
    pcVar7 = ",\"SUMMARY\":[";
  }
  uVar1 = FUN_000307c8(param_1,pcVar7);
  iVar2 = pthread_mutex_lock((pthread_mutex_t *)&DAT_0007b384);
  if (iVar2 != 0) {
    FUN_0000b774("summary",0xae9);
  }
  dVar9 = DAT_0007b5e8 + DAT_0007b670 + DAT_0007b428;
  uVar10 = FUN_0005cc10(SUB84(dVar9,0),(int)((ulonglong)dVar9 >> 0x20));
  iVar2 = (int)((ulonglong)uVar10 >> 0x20);
  uVar4 = (uint)uVar10;
  DAT_0007b100 = uVar4;
  DAT_0007b104 = iVar2;
  local_70 = (double)FUN_0005cb64(DAT_0007b420,DAT_0007b424);
  dVar9 = DAT_000755f8;
  in_fpscr = in_fpscr & 0xfffffff;
  if (DAT_000755f8 == 0.0) {
    local_68 = _DAT_0007acc0 / DAT_000755f8;
    local_70 = local_70 * 60.0;
    dVar8 = (double)FUN_0005cb64(uVar4,iVar2);
    dVar9 = 1.0;
  }
  else {
    local_68 = _DAT_0007acc0 / DAT_000755f8;
    local_70 = (local_70 / DAT_000755f8) * 60.0;
    dVar8 = (double)FUN_0005cb64(uVar4,iVar2);
  }
  local_60 = (dVar8 / dVar9) * 60.0;
  if (DAT_00078e28 != '\0') {
    uVar4 = time((time_t *)0x0);
    srand(uVar4);
    iVar2 = DAT_00078e38;
    if (((*DAT_00076b84 == 'z') && (DAT_00076b84[1] == '9')) && (DAT_00076b84[2] == '\0')) {
      iVar5 = rand();
      iVar5 = iVar2 * 0xe + iVar5 % 2;
      iVar2 = iVar5 % 5;
    }
    else {
      iVar5 = strcmp(DAT_00076b84,"z9-mini");
      iVar2 = DAT_00078e38;
      if (iVar5 == 0) {
        iVar5 = rand();
        iVar5 = iVar2 * 3 + 1 + iVar5 % 2;
        iVar2 = iVar5 % 5;
      }
      else {
        iVar2 = 4;
        iVar5 = 0xe;
      }
    }
    uVar4 = time((time_t *)0x0);
    srand(uVar4);
    iVar6 = rand();
    dVar9 = (double)VectorSignedToFloat(iVar5,(byte)(in_fpscr >> 0x16) & 3);
    local_70 = (double)VectorSignedToFloat(iVar2,(byte)(in_fpscr >> 0x16) & 3);
    dVar8 = (double)VectorSignedToFloat(iVar6 % 99,(byte)(in_fpscr >> 0x16) & 3);
    dVar9 = dVar9 + dVar8 / 100.0;
    local_70 = dVar8 / 100.0 + local_70;
    local_68 = dVar9;
    DAT_00078e30 = dVar9;
    iVar2 = rand();
    local_60 = (double)VectorSignedToFloat(iVar2 % 4 + 100,(byte)(in_fpscr >> 0x16) & 3);
    local_60 = local_60 + 0.27;
    sprintf(&DAT_00078b84,"%f",SUB84(dVar9 + 0.79,0),(int)((ulonglong)(dVar9 + 0.79) >> 0x20));
  }
  uVar3 = FUN_00032fcc(0,"Elapsed",&DAT_000755f8,1);
  uVar3 = FUN_00032e1c(uVar3,"GHS 5s",&DAT_00078b84,0);
  uVar3 = FUN_00034eb0(uVar3,"GHS av",&local_68,0);
  uVar3 = FUN_00032ef4(uVar3,"Found Blocks",&DAT_0007b8b8,1);
  uVar3 = FUN_00032f84(uVar3,"Getworks",&DAT_0007b648,1);
  uVar3 = FUN_00032f84(uVar3,"Accepted",&DAT_0007b420,1);
  uVar3 = FUN_00032f84(uVar3,"Rejected",&DAT_0007b6f8,1);
  uVar3 = FUN_00032ed0(uVar3,"Hardware Errors",&DAT_0007b8a4,1);
  uVar3 = FUN_00035568(uVar3,"Utility",&local_70,0);
  uVar3 = FUN_00032f84(uVar3,"Discarded",&DAT_0007b7c8,1);
  uVar3 = FUN_00032f84(uVar3,"Stale",&DAT_0007bc80,1);
  uVar3 = FUN_00032ef4(uVar3,"Get Failures",&DAT_0007b43c,1);
  uVar3 = FUN_00032ef4(uVar3,"Local Work",&DAT_0007b4cc,1);
  uVar3 = FUN_00032ef4(uVar3,"Remote Failures",&DAT_0007ae68,1);
  uVar3 = FUN_00032ef4(uVar3,"Network Blocks",&DAT_0007bb1c,1);
  uVar3 = FUN_00035520(uVar3,"Total MH",&DAT_0007acc0,1);
  uVar3 = FUN_00035568(uVar3,"Work Utility",&local_60,0);
  uVar3 = FUN_000355f8(uVar3,"Difficulty Accepted",&DAT_0007b5e8,1);
  uVar3 = FUN_000355f8(uVar3,"Difficulty Rejected",&DAT_0007b670,1);
  uVar3 = FUN_000355f8(uVar3,"Difficulty Stale",&DAT_0007b428,1);
  uVar3 = FUN_00032f60(uVar3,"Best Share",&DAT_00076f18,1);
  if (DAT_0007b100 + DAT_0007b8a4 == 0 &&
      DAT_0007b104 + ((int)DAT_0007b8a4 >> 0x1f) + (uint)CARRY4(DAT_0007b100,DAT_0007b8a4) == 0) {
    dVar9 = 0.0;
  }
  else {
    dVar9 = (double)VectorSignedToFloat(DAT_0007b8a4,(byte)(in_fpscr >> 0x16) & 3);
    dVar8 = (double)FUN_0005cb64();
    dVar9 = dVar9 / dVar8;
  }
  uStack_54 = (undefined4)((ulonglong)dVar9 >> 0x20);
  local_58 = SUB84(dVar9,0);
  uVar3 = FUN_000357a4(uVar3,"Device Hardware%",&local_58,0);
  if (DAT_0007b100 == 0 && DAT_0007b104 == 0) {
    dVar9 = 0.0;
  }
  else {
    dVar9 = (double)FUN_0005cb64();
    dVar9 = DAT_0007b670 / dVar9;
  }
  uStack_4c = (undefined4)((ulonglong)dVar9 >> 0x20);
  local_50 = SUB84(dVar9,0);
  uVar3 = FUN_000357a4(uVar3,"Device Rejected%",&local_50,0);
  dVar9 = DAT_0007b670 + DAT_0007b5e8 + DAT_0007b428;
  if (dVar9 == 0.0) {
    dVar9 = 0.0;
  }
  else {
    dVar9 = DAT_0007b670 / dVar9;
  }
  uStack_44 = (undefined4)((ulonglong)dVar9 >> 0x20);
  local_48 = SUB84(dVar9,0);
  uVar3 = FUN_000357a4(uVar3,"Pool Rejected%",&local_48,0);
  dStack_40 = DAT_0007b5e8 + DAT_0007b670 + DAT_0007b428;
  if (dStack_40 == 0.0) {
    dStack_40 = 0.0;
  }
  else {
    dStack_40 = DAT_0007b428 / dStack_40;
  }
  uVar3 = FUN_000357a4(uVar3,"Pool Stale%",&dStack_40,0);
  uVar3 = FUN_00033038(uVar3,"Last getwork",&DAT_0007b948,0);
  iVar2 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_0007b384);
  if (iVar2 != 0) {
    FUN_0000b84c("summary",0xb2e);
  }
  (*DAT_00075ed8)();
  FUN_00032534(param_1,uVar3,param_4,0);
  if ((uVar1 & param_4) != 0) {
    *(undefined1 *)(param_1 + 0xd) = 1;
  }
  return;
}

