//==================== FUN_0001c3f4 @ 0001c3f4 ====================

void FUN_0001c3f4(undefined4 *param_1)

{
  char cVar1;
  longlong lVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  undefined4 uVar7;
  bool bVar8;
  longlong lVar9;
  int iStack_1038;
  int local_1034;
  int iStack_1030;
  int local_102c;
  char acStack_1028 [4100];
  
  iVar5 = param_1[9];
  uVar7 = *param_1;
  local_1034 = 0;
  iVar6 = *(int *)(iVar5 + 4);
  lVar2 = 0;
  iStack_1038 = 0;
LAB_0001c440:
  cVar1 = *(char *)(iVar5 + 0x16c);
  do {
    if (cVar1 != '\0') {
LAB_0001c568:
      *(undefined4 *)(iVar5 + 0x20) = 1;
      return;
    }
    *(undefined1 *)((int)param_1 + 0x3f) = 0;
    lVar9 = (**(code **)(iVar6 + 0x44))(param_1);
    *(undefined1 *)((int)param_1 + 0x3e) = 0;
    if (lVar9 == -1) {
      if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
        snprintf(acStack_1028,0x1000,"%s %d failure, disabling!",*(char **)(iVar6 + 8),
                 *(int *)(iVar5 + 8));
        FUN_00038730(3,acStack_1028,0);
      }
      *(undefined4 *)(iVar5 + 0x20) = 1;
      FUN_0002ed88(iVar5);
      goto LAB_0001c568;
    }
    lVar2 = lVar9 + lVar2;
    FUN_0002a84c(&iStack_1030);
    iVar4 = local_102c - local_1034;
    bVar8 = iVar4 < 0;
    if (bVar8) {
      iVar4 = iVar4 + 0xf4000;
    }
    iVar3 = iStack_1030 - iStack_1038;
    if (bVar8) {
      iVar4 = iVar4 + 0x240;
      iVar3 = iVar3 + -1;
    }
    if (((lVar2 == 0) || ((iVar3 < 1 && (iVar4 < 0x30d41)))) && (iVar3 < DAT_00075dcc)) {
      lVar9 = lVar2;
      if (*(char *)(param_1 + 0xf) != '\0') goto LAB_0001c5ac;
LAB_0001c508:
      lVar2 = lVar9;
      if (*(int *)(iVar5 + 0x20) != 0) goto LAB_0001c5ac;
    }
    else {
      FUN_0001bba4(uVar7,iVar3,(int)lVar2,(int)((ulonglong)lVar2 >> 0x20));
      lVar2 = 0;
      FUN_0002a530(&iStack_1038,&iStack_1030);
      lVar9 = 0;
      if (*(char *)(param_1 + 0xf) == '\0') goto LAB_0001c508;
LAB_0001c5ac:
      FUN_00011c8c(param_1,uVar7,iVar6);
    }
    if (*(char *)((int)param_1 + 0x3f) == '\0') goto LAB_0001c440;
    iVar4 = pthread_mutex_lock((pthread_mutex_t *)&DAT_0007b85c);
    if (iVar4 != 0) {
      FUN_0000b414("hash_driver_work",0x23a0);
    }
    (**(code **)(iVar6 + 0x50))(iVar5);
    iVar4 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_0007b85c);
    if (iVar4 != 0) {
      FUN_0000b5c4("hash_driver_work",0x23a2);
    }
    (*DAT_00075ed8)();
    cVar1 = *(char *)(iVar5 + 0x16c);
  } while( true );
}

