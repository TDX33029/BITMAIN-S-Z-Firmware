//==================== FUN_0003561c @ 0003561c ====================

void FUN_0003561c(int param_1,undefined4 param_2,undefined4 param_3,uint param_4)

{
  uint uVar1;
  undefined4 uVar2;
  int iVar3;
  char *pcVar4;
  
  FUN_0003305c(param_1,0x4e,0,0,param_4);
  pcVar4 = "COIN,";
  if (param_4 != 0) {
    pcVar4 = ",\"COIN\":[";
  }
  uVar1 = FUN_000307c8(param_1,pcVar4);
  uVar2 = FUN_00032e40(0,"Hash Method","sha256",0);
  iVar3 = pthread_mutex_lock((pthread_mutex_t *)&DAT_0007b9d8);
  if (iVar3 != 0) {
    FUN_0000b774("minecoin",0xe72);
  }
  FUN_00032190(&DAT_0007b9f0,"minecoin",0xe72);
  iVar3 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_0007b9d8);
  if (iVar3 != 0) {
    FUN_0000b84c("minecoin",0xe72);
  }
  uVar2 = FUN_00033014(uVar2,"Current Block Time",&DAT_0007b070,1);
  uVar2 = FUN_00032e1c(uVar2,"Current Block Hash",&DAT_0007bb28,1);
  iVar3 = pthread_rwlock_unlock((pthread_rwlock_t *)&DAT_0007b9f0);
  if (iVar3 != 0) {
    FUN_0000b7e0("minecoin",0xe75);
  }
  (*DAT_00075ed8)();
  uVar2 = FUN_00032ff0(uVar2,&DAT_00062288,&DAT_0007bc79,0);
  uVar2 = FUN_000355f8(uVar2,"Network Difficulty",&DAT_00075de0,1);
  FUN_00032534(param_1,uVar2,param_4,0);
  if ((uVar1 & param_4) != 0) {
    *(undefined1 *)(param_1 + 0xd) = 1;
  }
  return;
}

