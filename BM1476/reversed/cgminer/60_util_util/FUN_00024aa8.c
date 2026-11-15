//==================== FUN_00024aa8 @ 00024aa8 ====================

void FUN_00024aa8(undefined4 *param_1)

{
  longlong lVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  undefined4 uVar7;
  bool bVar8;
  longlong lVar9;
  longlong local_1048;
  int iStack_1038;
  int local_1034;
  int iStack_1030;
  int local_102c;
  char acStack_1028 [4100];
  
  iVar5 = param_1[9];
  uVar7 = *param_1;
  local_1034 = 0;
  iVar6 = *(int *)(iVar5 + 4);
  iStack_1038 = 0;
  local_1048 = 0;
  do {
    if (*(char *)(iVar5 + 0x16c) != '\0') {
LAB_00024d3c:
      *(undefined4 *)(iVar5 + 0x20) = 1;
      return;
    }
    *(undefined1 *)((int)param_1 + 0x3f) = 0;
    do {
      if (*(int *)(iVar5 + 0x164) == 0) {
        uVar2 = FUN_000247d4(param_1,uVar7);
        iVar3 = pthread_rwlock_wrlock((pthread_rwlock_t *)(iVar5 + 0x140));
        if (iVar3 != 0) {
          FUN_0000b480("fill_queue",0x223b);
        }
        bVar8 = *(int *)(iVar5 + 0x164) == 0;
        if (bVar8) {
          *(undefined4 *)(iVar5 + 0x164) = uVar2;
        }
        iVar3 = pthread_rwlock_unlock((pthread_rwlock_t *)(iVar5 + 0x140));
        if (iVar3 != 0) {
          FUN_0000b630("fill_queue",0x2242);
        }
        (*DAT_00075ed8)();
        if (!bVar8) {
          FUN_000159b4(uVar2);
        }
      }
      iVar3 = (**(code **)(iVar6 + 0x48))(iVar5);
    } while (iVar3 == 0);
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
      goto LAB_00024d3c;
    }
    lVar1 = local_1048 + lVar9;
    local_1048 = lVar9 + local_1048;
    FUN_0002a84c(&iStack_1030);
    iVar3 = local_102c - local_1034;
    bVar8 = iVar3 < 0;
    if (bVar8) {
      iVar3 = iVar3 + 0xf4000;
    }
    iVar4 = iStack_1030 - iStack_1038;
    if (bVar8) {
      iVar3 = iVar3 + 0x240;
      iVar4 = iVar4 + -1;
    }
    if (((lVar1 == 0) || ((iVar4 < 1 && (iVar3 < 0x30d41)))) && (iVar4 < DAT_00075dcc)) {
      if (*(char *)(param_1 + 0xf) != '\0') goto LAB_00024cbc;
LAB_00024c64:
      if (*(int *)(iVar5 + 0x20) != 0) goto LAB_00024cbc;
    }
    else {
      FUN_0001bba4(uVar7,iVar4,(int)lVar1,(int)((ulonglong)lVar1 >> 0x20));
      FUN_0002a530(&iStack_1038,&iStack_1030);
      local_1048 = 0;
      if (*(char *)(param_1 + 0xf) == '\0') goto LAB_00024c64;
LAB_00024cbc:
      FUN_00011c8c(param_1,uVar7,iVar6);
    }
    if (*(char *)((int)param_1 + 0x3f) != '\0') {
      (**(code **)(iVar6 + 0x50))(iVar5);
    }
  } while( true );
}

