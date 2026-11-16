//==================== FUN_0003c5e8 @ 0003c5e8 ====================

undefined4 FUN_0003c5e8(int param_1,undefined4 param_2,uint param_3)

{
  uint uVar1;
  undefined4 uVar2;
  
  pthread_mutex_lock((pthread_mutex_t *)(&DAT_00078fac + param_1 * 0x18));
  uVar1 = FUN_0003bb78(param_1);
  if (uVar1 < param_3) {
    if (uVar1 == 0) {
      uVar2 = 0;
    }
    else {
      uVar2 = FUN_0003bd44(param_1,param_2,uVar1);
    }
  }
  else {
    uVar2 = FUN_0003bd44(param_1,param_2,param_3);
  }
  pthread_mutex_unlock((pthread_mutex_t *)(&DAT_00078fac + param_1 * 0x18));
  return uVar2;
}

