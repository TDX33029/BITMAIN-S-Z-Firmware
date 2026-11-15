//==================== FUN_0001aa18 @ 0001aa18 ====================

void FUN_0001aa18(int param_1)

{
  int iVar1;
  int iVar2;
  char local_1010 [4];
  char acStack_100c [4];
  char acStack_1008 [4];
  char acStack_1004 [4];
  char local_1000 [4];
  char acStack_ffc [4];
  undefined2 uStack_ff8;
  undefined1 local_ff6;
  
  if (param_1 != 0) {
    iVar1 = pthread_rwlock_trywrlock((pthread_rwlock_t *)(param_1 + 0x140));
    if (iVar1 == 0) {
      iVar2 = *(int *)(param_1 + 0x164);
      *(undefined4 *)(param_1 + 0x164) = 0;
      iVar1 = pthread_rwlock_unlock((pthread_rwlock_t *)(param_1 + 0x140));
      if (iVar1 != 0) {
        FUN_0000b630("flush_queue",0x232c);
      }
      (*DAT_00075ed8)();
      if (((iVar2 != 0) && (FUN_00015780(iVar2), DAT_00078e09 != '\0')) &&
         ((DAT_0007ad48 != '\0' || ((DAT_00078e08 != '\0' || (6 < DAT_000766c4)))))) {
        local_1010[0] = 'D';
        local_1010[1] = 'i';
        local_1010[2] = 's';
        local_1010[3] = 'c';
        acStack_100c[0] = 'a';
        acStack_100c[1] = 'r';
        acStack_100c[2] = 'd';
        acStack_100c[3] = 'e';
        acStack_1008[0] = 'd';
        acStack_1008[1] = ' ';
        acStack_1008[2] = 'q';
        acStack_1008[3] = 'u';
        acStack_1004[0] = 'e';
        acStack_1004[1] = 'u';
        acStack_1004[2] = 'e';
        acStack_1004[3] = 'd';
        local_1000[0] = ' ';
        local_1000[1] = 'w';
        local_1000[2] = 'o';
        local_1000[3] = 'r';
        acStack_ffc[0] = 'k';
        acStack_ffc[1] = ' ';
        acStack_ffc[2] = 'i';
        acStack_ffc[3] = 't';
        uStack_ff8 = 0x6d65;
        local_ff6 = 0;
        FUN_00038730(7,local_1010,0);
      }
    }
  }
  return;
}

