//==================== FUN_00019bf8 @ 00019bf8 ====================

void FUN_00019bf8(int *param_1)

{
  uint uVar1;
  int *piVar2;
  int iVar3;
  int *piVar4;
  int iVar5;
  int iVar6;
  int *piVar7;
  int iVar8;
  char acStack_1020 [4100];
  
  iVar3 = pthread_mutex_lock((pthread_mutex_t *)&DAT_0007ba10);
  if (iVar3 != 0) {
    FUN_0000b414("switch_pools",0x13ed);
  }
  iVar3 = pthread_rwlock_wrlock((pthread_rwlock_t *)&DAT_0007ba28);
  if (iVar3 != 0) {
    FUN_0000b480("switch_pools",0x13ed);
  }
  piVar2 = DAT_0007705c;
  iVar3 = *DAT_0007705c;
  if ((param_1 != (int *)0x0) && (iVar8 = param_1[1], iVar8 != 0)) {
    if (0 < DAT_0007b198) {
      piVar4 = DAT_0007b438 + DAT_0007b198;
      piVar7 = DAT_0007b438;
      while( true ) {
        iVar6 = *(int *)(*piVar7 + 4);
        if (iVar6 < iVar8) {
          *(int *)(*piVar7 + 4) = iVar6 + 1;
        }
        if (piVar7 + 1 == piVar4) break;
        iVar8 = param_1[1];
        piVar7 = piVar7 + 1;
      }
    }
    param_1[1] = 0;
  }
  if (DAT_00076efc < 5) {
    uVar1 = 1 << (DAT_00076efc & 0xff);
    if ((uVar1 & 0x19) == 0) {
      if ((uVar1 & 6) != 0) {
        if ((param_1 != (int *)0x0) && (*(char *)((int)param_1 + 0x81) == '\0')) {
          piVar7 = (int *)DAT_0007b438[*param_1];
          goto LAB_00019d00;
        }
        if (1 < DAT_0007b198) {
          iVar6 = 1;
          iVar8 = iVar3;
          do {
            iVar8 = iVar8 + 1;
            iVar6 = iVar6 + 1;
            iVar5 = iVar8 * 4;
            if (DAT_0007b198 <= iVar8) {
              iVar5 = 0;
              iVar8 = iVar5;
            }
            piVar7 = *(int **)((int)DAT_0007b438 + iVar5);
            if ((*(char *)((int)piVar7 + 0x81) == '\0') && (piVar7[0x21] == 1)) goto LAB_00019d00;
          } while (iVar6 != DAT_0007b198);
          piVar7 = (int *)DAT_0007b438[iVar3];
          goto LAB_00019d00;
        }
      }
    }
    else if (0 < DAT_0007b198) {
      iVar8 = 0;
      do {
        iVar6 = iVar8 + 1;
        piVar7 = (int *)FUN_00012038(iVar8);
        if ((*(char *)((int)piVar7 + 0x81) == '\0') && (piVar7[0x21] == 1)) {
          piVar7 = (int *)DAT_0007b438[*piVar7];
          goto LAB_00019d00;
        }
        iVar8 = iVar6;
      } while (iVar6 < DAT_0007b198);
    }
  }
  piVar7 = (int *)DAT_0007b438[iVar3];
LAB_00019d00:
  DAT_0007705c = piVar7;
  iVar3 = pthread_rwlock_unlock((pthread_rwlock_t *)&DAT_0007ba28);
  if (iVar3 != 0) {
    FUN_0000b630("switch_pools",0x142b);
  }
  iVar3 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_0007ba10);
  if (iVar3 != 0) {
    FUN_0000b5c4("switch_pools",0x142b);
  }
  (*DAT_00075ed8)();
  if (DAT_0007b625 != '\0') {
    FUN_00018834(piVar7,(int)piVar7 + 0x82);
  }
  if (((piVar2 != piVar7) && (DAT_00076efc != 3)) && (DAT_00076efc != 4)) {
    if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
      snprintf(acStack_1020,0x1000,"Switching to pool %d %s",*piVar7,(char *)piVar7[0x31]);
      FUN_00038730(4,acStack_1020,0);
    }
    FUN_000185c8(piVar2);
  }
  iVar3 = pthread_mutex_lock((pthread_mutex_t *)&DAT_00077060);
  if (iVar3 != 0) {
    FUN_0000b414("switch_pools",0x143a);
  }
  pthread_cond_broadcast((pthread_cond_t *)&DAT_00077078);
  iVar3 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_00077060);
  if (iVar3 != 0) {
    FUN_0000b5c4("switch_pools",0x143c);
  }
  (*DAT_00075ed8)();
  return;
}

