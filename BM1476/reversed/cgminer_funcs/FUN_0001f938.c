//==================== FUN_0001f938 @ 0001f938 ====================

void FUN_0001f938(int param_1,int param_2,uint param_3)

{
  int iVar1;
  time_t tVar2;
  int iVar3;
  uint uVar4;
  bool bVar5;
  
  iVar1 = pthread_mutex_lock((pthread_mutex_t *)&DAT_0007ad4c);
  if (iVar1 != 0) {
    FUN_0000b414("inc_work_stats",0x20c9);
  }
  iVar1 = (int)param_3 >> 0x1f;
  iVar3 = *(int *)(param_1 + 0x24);
  uVar4 = *(uint *)(iVar3 + 0xc0);
  bVar5 = CARRY4(DAT_0007b100,param_3);
  DAT_0007b100 = DAT_0007b100 + param_3;
  DAT_0007b104 = DAT_0007b104 + iVar1 + (uint)bVar5;
  *(uint *)(iVar3 + 0xc0) = uVar4 + param_3;
  *(uint *)(iVar3 + 0xc4) = *(int *)(iVar3 + 0xc4) + iVar1 + (uint)CARRY4(uVar4,param_3);
  if (param_2 == 0) {
    iVar3 = FUN_0001da58();
    uVar4 = *(uint *)(iVar3 + 0x28);
    *(uint *)(iVar3 + 0x28) = param_3 + uVar4;
    *(uint *)(iVar3 + 0x2c) = iVar1 + *(int *)(iVar3 + 0x2c) + (uint)CARRY4(param_3,uVar4);
  }
  else {
    uVar4 = *(uint *)(param_2 + 0x28);
    *(uint *)(param_2 + 0x28) = param_3 + uVar4;
    *(uint *)(param_2 + 0x2c) = iVar1 + *(int *)(param_2 + 0x2c) + (uint)CARRY4(param_3,uVar4);
  }
  iVar1 = *(int *)(param_1 + 0x24);
  tVar2 = time((time_t *)0x0);
  *(time_t *)(iVar1 + 0xe8) = tVar2;
  iVar1 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_0007ad4c);
  if (iVar1 != 0) {
    FUN_0000b5c4("inc_work_stats",0x20d6);
  }
  (*DAT_00075ed8)();
  return;
}

