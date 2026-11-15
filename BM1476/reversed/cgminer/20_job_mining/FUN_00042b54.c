//==================== FUN_00042b54 @ 00042b54 ====================

undefined4 FUN_00042b54(int *param_1)

{
  char cVar1;
  pthread_t __th;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  uint param3;
  int iVar5;
  uint param3_00;
  undefined4 *puVar6;
  bool bVar7;
  int iStack_10cc;
  int local_10c8;
  int iStack_10c4;
  int local_10c0;
  undefined1 auStack_10bc [2];
  undefined1 local_10ba;
  undefined1 local_10b9;
  undefined1 auStack_10b8 [144];
  char acStack_1028 [4100];
  
  __th = pthread_self();
  pthread_detach(__th);
  iVar5 = *param_1;
  param3 = (uint)*(byte *)(param_1 + 1);
  puVar6 = *(undefined4 **)(iVar5 + 0x276);
  if ((DAT_00078e09 != '\0') &&
     (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
    snprintf(acStack_1028,0x1000,"Start To Fill Work!ChainIndex:[%d]",param3);
    FUN_00038730(7,acStack_1028,0);
  }
  FUN_0002a84c(&iStack_10cc);
  param3_00 = 0;
  FUN_0002a84c(&iStack_10c4);
LAB_00042bf8:
  do {
    cVar1 = (&DAT_000790fc)[param3];
    while( true ) {
      if (cVar1 != '\0') {
        return 0;
      }
      if ((((&DAT_000790ec)[param3] == '\0') || (DAT_000790f0 != '\0')) || (DAT_00078e28 != '\0')) {
        FUN_0002a92c(10);
        goto LAB_00042bf8;
      }
      FUN_0002a84c(&iStack_10cc);
      iVar4 = local_10c8 - local_10c0;
      bVar7 = iVar4 < 0;
      if (bVar7) {
        iVar4 = iVar4 + 1000000;
      }
      iVar3 = iStack_10cc - iStack_10c4;
      if (bVar7) {
        iVar3 = iVar3 + -1;
      }
      if (((&DAT_00078e14)[param3] == '\0') && ((uint)(iVar3 * 1000000 + iVar4) < DAT_0009d4c4))
      break;
      FUN_0002a84c(&iStack_10c4);
      while (iVar4 = FUN_000247d4(puVar6,*puVar6), iVar4 == 0) {
        if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
          snprintf(acStack_1028,0x1000,"Work Error![%d]",param3_00);
          FUN_00038730(3,acStack_1028,0);
        }
      }
      param3_00 = *(uint *)(iVar4 + 0x724) & 0x7f;
      (&DAT_00078e14)[param3] = 0;
      memset(auStack_10bc,0,0x92);
      local_10ba = 0x20;
      local_10b9 = (undefined1)param3_00;
      memcpy(auStack_10b8,(void *)(iVar4 + 0xe0),0x8c);
      iVar3 = iVar5 + (param3_00 + 0x1c) * 4;
      FUN_00042a60(auStack_10bc,0x92);
      pthread_mutex_lock((pthread_mutex_t *)&DAT_00078ea0);
      if (*(int *)(iVar3 + 6) != 0) {
        FUN_00015780(*(int *)(iVar3 + 6));
        *(undefined2 *)(iVar3 + 6) = 0;
        *(undefined2 *)(iVar3 + 8) = 0;
      }
      uVar2 = FUN_0001a44c(iVar4,0);
      *(undefined4 *)(iVar5 + (param3_00 + 0x1c) * 4 + 6) = uVar2;
      pthread_mutex_unlock((pthread_mutex_t *)&DAT_00078ea0);
      if ((DAT_00078e09 != '\0') &&
         (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
        snprintf(acStack_1028,0x1000,"ChainID[%d] Wirte Work. workid=%d, jobid=%s",param3,param3_00,
                 *(char **)(iVar4 + 0x6f0));
        FUN_00038730(7,acStack_1028,0);
      }
      FUN_0003c150(param3,auStack_10bc,0x92);
      DAT_000790f1 = 1;
      FUN_0002a84c(&DAT_000790f4);
      FUN_0002a954(500,0);
      FUN_00015780(iVar4);
      cVar1 = (&DAT_000790fc)[param3];
    }
    FUN_0002a954(500,0);
  } while( true );
}

