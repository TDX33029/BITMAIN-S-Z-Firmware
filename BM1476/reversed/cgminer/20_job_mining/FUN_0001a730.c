//==================== FUN_0001a730 @ 0001a730 ====================

void FUN_0001a730(int param_1,int param_2)

{
  char cVar1;
  int iVar2;
  time_t tVar3;
  bool bVar4;
  double dVar5;
  double in_d16;
  double dVar6;
  undefined8 uVar7;
  char acStack_1020 [4096];
  
  dVar5 = DAT_00075de0;
  uVar7 = FUN_0001a4b4(param_2);
  bVar4 = DAT_0007b859 != '\0';
  if (bVar4) {
    in_d16 = 65536.0;
  }
  *(undefined8 *)(param_2 + 0x6b8) = uVar7;
  if (bVar4) {
    dVar5 = dVar5 * in_d16;
  }
  dVar6 = (double)FUN_0005cb50();
  cVar1 = DAT_0007ad48;
  if (dVar5 <= dVar6) {
    *(undefined1 *)(param_2 + 0x6eb) = 1;
    DAT_0007b8b8 = DAT_0007b8b8 + 1;
    *(int *)(*(int *)(param_2 + 0x6d4) + 0x20) = *(int *)(*(int *)(param_2 + 0x6d4) + 0x20) + 1;
    *(undefined1 *)(param_2 + 0x6ea) = 1;
    if (((cVar1 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
      snprintf(acStack_1020,0x1000,"Found block for pool %d!",**(int **)(param_2 + 0x6d4));
      FUN_00038730(5,acStack_1020,0);
    }
  }
  iVar2 = pthread_mutex_lock((pthread_mutex_t *)&DAT_0007ad4c);
  if (iVar2 != 0) {
    FUN_0000b414("update_work_stats",0x20bf);
  }
  dVar5 = *(double *)(param_2 + 0x6b0);
  dVar6 = (double)FUN_0005cb64(DAT_0007b100,DAT_0007b104);
  dVar6 = dVar6 + dVar5;
  uVar7 = FUN_0005cc10(SUB84(dVar6,0),(int)((ulonglong)dVar6 >> 0x20));
  DAT_0007b104 = (undefined4)((ulonglong)uVar7 >> 0x20);
  DAT_0007b100 = (undefined4)uVar7;
  iVar2 = *(int *)(param_1 + 0x24);
  dVar6 = (double)FUN_0005cb64(*(undefined4 *)(iVar2 + 0xc0),*(undefined4 *)(iVar2 + 0xc4));
  dVar5 = dVar5 + dVar6;
  uVar7 = FUN_0005cc10(SUB84(dVar5,0),(int)((ulonglong)dVar5 >> 0x20));
  *(undefined8 *)(iVar2 + 0xc0) = uVar7;
  iVar2 = *(int *)(param_2 + 0x6d4);
  dVar5 = (double)FUN_0005cb64(*(undefined4 *)(iVar2 + 0x28),*(undefined4 *)(iVar2 + 0x2c));
  dVar5 = dVar5 + *(double *)(param_2 + 0x6b0);
  uVar7 = FUN_0005cc10(SUB84(dVar5,0),(int)((ulonglong)dVar5 >> 0x20));
  *(undefined8 *)(iVar2 + 0x28) = uVar7;
  iVar2 = *(int *)(param_1 + 0x24);
  tVar3 = time((time_t *)0x0);
  *(time_t *)(iVar2 + 0xe8) = tVar3;
  iVar2 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_0007ad4c);
  if (iVar2 != 0) {
    FUN_0000b5c4("update_work_stats",0x20c4);
  }
  (*DAT_00075ed8)();
  return;
}

