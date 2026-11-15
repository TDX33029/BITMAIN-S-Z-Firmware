//==================== FUN_0001b600 @ 0001b600 ====================

void FUN_0001b600(char *param_1,int param_2)

{
  int iVar1;
  char *pcVar2;
  char *param10;
  char *param11;
  size_t sVar3;
  int iVar4;
  ulong param3;
  uint param9;
  char acStack_1428 [1023];
  undefined1 local_1029;
  undefined4 local_1028;
  undefined4 uStack_1024;
  undefined4 uStack_1020;
  undefined4 uStack_101c;
  undefined4 uStack_1018;
  undefined2 local_1014;
  
  if (DAT_00076cf4 == (FILE *)0x0) {
    return;
  }
  param9 = *(uint *)(param_2 + 0x6d0);
  iVar1 = FUN_0001b124(param9);
  iVar4 = *(int *)(param_2 + 0x6d4);
  param3 = *(ulong *)(param_2 + 0x780);
  iVar1 = *(int *)(iVar1 + 0x24);
  pcVar2 = (char *)FUN_00029730(param_2 + 0xa0,0x20);
  param10 = (char *)FUN_00029730(param_2 + 0xc0,0x20);
  param11 = (char *)FUN_00029730(param_2,0x80);
  sVar3 = snprintf(acStack_1428,0x400,"%lu,%s,%s,%s,%s%u,%u,%s,%s\n",param3,param_1,pcVar2,
                   *(char **)(iVar4 + 0xc4),*(char **)(*(int *)(iVar1 + 4) + 8),*(uint *)(iVar1 + 8)
                   ,param9,param10,param11);
  free(pcVar2);
  free(param10);
  free(param11);
  if ((int)sVar3 < 0x400) {
    if ((int)sVar3 < 0) {
      if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 3)) {
        return;
      }
      pcVar2 = "sharelog printf error";
      goto LAB_0001b760;
    }
  }
  else {
    local_1029 = 0;
  }
  iVar1 = pthread_mutex_lock((pthread_mutex_t *)&DAT_000770b8);
  if (iVar1 != 0) {
    FUN_0000b414("sharelog",0x2a1);
  }
  sVar3 = fwrite(acStack_1428,sVar3,1,DAT_00076cf4);
  fflush(DAT_00076cf4);
  iVar1 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_000770b8);
  if (iVar1 != 0) {
    FUN_0000b5c4("sharelog",0x2a4);
  }
  (*DAT_00075ed8)();
  if (sVar3 == 1) {
    return;
  }
  if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 3)) {
    return;
  }
  pcVar2 = "sharelog fwrite error";
LAB_0001b760:
  local_1028 = *(undefined4 *)pcVar2;
  uStack_1024 = *(undefined4 *)(pcVar2 + 4);
  uStack_1020 = *(undefined4 *)(pcVar2 + 8);
  uStack_101c = *(undefined4 *)(pcVar2 + 0xc);
  uStack_1018 = *(undefined4 *)(pcVar2 + 0x10);
  local_1014 = (undefined2)*(undefined4 *)(pcVar2 + 0x14);
  FUN_00038730(3,&local_1028,0);
  return;
}

