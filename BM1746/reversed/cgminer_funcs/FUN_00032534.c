//==================== FUN_00032534 @ 00032534 ====================

undefined4 FUN_00032534(undefined4 param_1,int *param_2,int param_3,int param_4)

{
  int iVar1;
  int iVar2;
  void *__ptr;
  int *piVar3;
  char *pcVar4;
  pthread_mutex_t *ppVar5;
  void *pvVar6;
  char acStack_1070 [64];
  char acStack_1030 [4100];
  
  ppVar5 = *(pthread_mutex_t **)(DAT_00078cd4 + 8);
  iVar1 = pthread_mutex_lock(ppVar5);
  if (iVar1 != 0) {
    FUN_0000b774("print_data",0x4ac);
  }
  iVar1 = pthread_rwlock_wrlock((pthread_rwlock_t *)(ppVar5 + 1));
  if (iVar1 != 0) {
    piVar3 = __errno_location();
    snprintf(acStack_1030,0x1000,"WTF WRLOCK ERROR ON LOCK! errno=%d in %s %s():%d",*piVar3,"api.c",
             "print_data",0x4ac);
    FUN_00038730(3,acStack_1030,1);
    FUN_00016ca8(1);
  }
  iVar1 = FUN_000391fc(DAT_00078cd4,"api.c","print_data",0x4ad);
  ppVar5 = *(pthread_mutex_t **)(DAT_00078cd4 + 8);
  iVar2 = pthread_rwlock_unlock((pthread_rwlock_t *)(ppVar5 + 1));
  if (iVar2 != 0) {
    FUN_0000b7e0("print_data",0x4ae);
  }
  iVar2 = pthread_mutex_unlock(ppVar5);
  if (iVar2 != 0) {
    FUN_0000b84c("print_data",0x4ae);
  }
  (*DAT_00075ed8)();
  *(undefined4 *)(*(int *)(iVar1 + 0xc) + 4) = 0;
  if (param_4 != 0) {
    FUN_000306b4(iVar1,&DAT_00060764);
  }
  if (param_3 != 0) {
    FUN_000306b4(iVar1,&DAT_00066aa8);
  }
  if (param_2 == (int *)0x0) {
LAB_00032760:
    if (param_3 == 0) {
      pcVar4 = "|";
    }
    else {
      pcVar4 = "}";
    }
    FUN_000306b4(iVar1,pcVar4);
    FUN_000307c8(param_1,**(undefined4 **)(iVar1 + 0xc));
    ppVar5 = *(pthread_mutex_t **)(DAT_00078cd4 + 8);
    iVar2 = pthread_mutex_lock(ppVar5);
    if (iVar2 != 0) {
      FUN_0000b774("print_data",0x558);
    }
    iVar2 = pthread_rwlock_wrlock((pthread_rwlock_t *)(ppVar5 + 1));
    if (iVar2 != 0) {
      piVar3 = __errno_location();
      snprintf(acStack_1030,0x1000,"WTF WRLOCK ERROR ON LOCK! errno=%d in %s %s():%d",*piVar3,
               "api.c","print_data",0x558);
      FUN_00038730(3,acStack_1030,1);
      FUN_00016ca8(1);
    }
    FUN_00039370(DAT_00078cd4,iVar1,"api.c","print_data",0x559);
    ppVar5 = *(pthread_mutex_t **)(DAT_00078cd4 + 8);
    iVar1 = pthread_rwlock_unlock((pthread_rwlock_t *)(ppVar5 + 1));
    if (iVar1 != 0) {
      FUN_0000b7e0("print_data",0x55a);
    }
    iVar1 = pthread_mutex_unlock(ppVar5);
    if (iVar1 != 0) {
      FUN_0000b84c("print_data",0x55a);
    }
    (*DAT_00075ed8)();
    return 0;
  }
  if (param_3 != 0) goto LAB_00032714;
LAB_0003260c:
  FUN_000306b4(iVar1,param_2[1]);
  FUN_000306b4(iVar1,&DAT_00066fbc);
  piVar3 = param_2;
  do {
    switch(*piVar3) {
    case 0:
      pvVar6 = (void *)piVar3[2];
      __ptr = (void *)FUN_00030500(pvVar6,param_3);
      if (param_3 == 0) {
        FUN_000306b4(iVar1,__ptr);
      }
      else {
        FUN_000306b4(iVar1,&DAT_00060810);
        FUN_000306b4(iVar1,__ptr);
        FUN_000306b4(iVar1,&DAT_00060810);
      }
      if (__ptr != pvVar6) {
        free(__ptr);
      }
      goto LAB_000326c4;
    case 1:
    case 2:
      if (param_3 == 0) {
        FUN_000306b4(iVar1,piVar3[2]);
      }
      else {
        FUN_000306b4(iVar1,&DAT_00060810);
        FUN_000306b4(iVar1,piVar3[2]);
        FUN_000306b4(iVar1,&DAT_00060810);
      }
      goto LAB_000326c4;
    case 3:
      snprintf(acStack_1070,0x40,"%u",(uint)*(byte *)piVar3[2]);
      break;
    case 4:
      snprintf(acStack_1070,0x40,"%d",(int)*(short *)piVar3[2]);
      break;
    case 5:
      snprintf(acStack_1070,0x40,"%u",(uint)*(ushort *)piVar3[2]);
      break;
    case 6:
      snprintf(acStack_1070,0x40,"%d",*(int *)piVar3[2]);
      break;
    case 7:
    case 8:
      snprintf(acStack_1070,0x40,"%u",*(undefined4 *)piVar3[2]);
      break;
    case 9:
      if (param_3 == 0) {
        snprintf(acStack_1070,0x40,"0x%08x",*(uint *)piVar3[2]);
        FUN_000306b4(iVar1,acStack_1070);
      }
      else {
        FUN_000306b4(iVar1,&DAT_00060810);
        snprintf(acStack_1070,0x40,"0x%08x",*(uint *)piVar3[2]);
        FUN_000306b4(iVar1,acStack_1070);
        FUN_000306b4(iVar1,&DAT_00060810);
      }
      goto LAB_000326c4;
    case 10:
      snprintf(acStack_1070,0x40,"%llu",(undefined4 *)piVar3[2],*(undefined4 *)piVar3[2]);
      break;
    case 0xb:
      snprintf(acStack_1070,0x40,"%lld",*(longlong *)piVar3[2]);
      break;
    case 0xc:
      snprintf(acStack_1070,0x40,"%f",((undefined4 *)piVar3[2])[1],*(undefined4 *)piVar3[2]);
      break;
    case 0xd:
      snprintf(acStack_1070,0x40,"%.0f",((undefined4 *)piVar3[2])[1],*(undefined4 *)piVar3[2]);
      break;
    case 0xe:
      pcVar4 = "false";
      if (*(char *)piVar3[2] != '\0') {
        pcVar4 = "true";
      }
      snprintf(acStack_1070,0x40,"%s",pcVar4);
      break;
    case 0xf:
      snprintf(acStack_1070,0x40,"%ld.%06ld",*(long *)piVar3[2],((long *)piVar3[2])[1]);
      break;
    case 0x10:
      snprintf(acStack_1070,0x40,"%lu",*(ulong *)piVar3[2]);
      break;
    case 0x11:
    case 0x14:
    case 0x15:
      snprintf(acStack_1070,0x40,"%.2f",((undefined4 *)piVar3[2])[1],*(undefined4 *)piVar3[2]);
      break;
    case 0x12:
      snprintf(acStack_1070,0x40,"%.4f",((undefined4 *)piVar3[2])[1],*(undefined4 *)piVar3[2]);
      break;
    case 0x13:
      snprintf(acStack_1070,0x40,"%.2f",(float *)piVar3[2],(double)*(float *)piVar3[2]);
      break;
    case 0x16:
    case 0x1a:
      snprintf(acStack_1070,0x40,"%.3f",(float *)piVar3[2],(double)*(float *)piVar3[2]);
      break;
    case 0x17:
      snprintf(acStack_1070,0x40,"%.15f",*(double *)piVar3[2]);
      break;
    case 0x18:
      snprintf(acStack_1070,0x40,"%.8f",((undefined4 *)piVar3[2])[1],*(undefined4 *)piVar3[2]);
      break;
    case 0x19:
      snprintf(acStack_1070,0x40,"%.4f",(double *)piVar3[2],*(double *)piVar3[2] * 100.0);
      break;
    default:
      if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
        snprintf(acStack_1030,0x1000,"API: unknown2 data type %d ignored",*piVar3);
        FUN_00038730(3,acStack_1030,0);
      }
      if (param_3 == 0) {
        FUN_000306b4(iVar1,"Unknown");
      }
      else {
        FUN_000306b4(iVar1,&DAT_00060810);
        FUN_000306b4(iVar1,"Unknown");
        FUN_000306b4(iVar1,&DAT_00060810);
      }
      goto LAB_000326c4;
    }
    FUN_000306b4(iVar1,acStack_1070);
LAB_000326c4:
    free((void *)piVar3[1]);
    if ((char)piVar3[3] == '\0') {
      param_2 = (int *)piVar3[5];
    }
    else {
      free((void *)piVar3[2]);
      param_2 = (int *)piVar3[5];
    }
    if (param_2 == piVar3) {
      free(piVar3);
      goto LAB_00032760;
    }
    iVar2 = piVar3[4];
    param_2[4] = iVar2;
    *(int **)(iVar2 + 0x14) = param_2;
    free(piVar3);
    FUN_000306b4(iVar1,&DAT_00060764);
    if (param_3 == 0) goto LAB_0003260c;
LAB_00032714:
    FUN_000306b4(iVar1,&DAT_00060810);
    FUN_000306b4(iVar1,param_2[1]);
    FUN_000306b4(iVar1,&DAT_00060810);
    FUN_000306b4(iVar1,&DAT_00065964);
    piVar3 = param_2;
  } while( true );
}

