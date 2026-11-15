//==================== FUN_00026bcc @ 00026bcc ====================

undefined1 FUN_00026bcc(void *param_1,uint param_2,undefined4 param_3,char *param_4,char *param_5)

{
  undefined1 uVar1;
  undefined4 uVar2;
  size_t sVar3;
  size_t sVar4;
  char *param0;
  int *piVar5;
  bool bVar6;
  char local_1018 [4];
  char acStack_1014 [4];
  char acStack_1010 [4];
  char acStack_100c [4];
  char local_1008 [4];
  char acStack_1004 [4];
  undefined2 local_1000;
  
  uVar2 = FUN_0002ace0(param_3,param_1);
  *(char **)((int)param_1 + 0xcc) = param_4;
  *(char **)((int)param_1 + 0xd0) = param_5;
  *(undefined4 *)((int)param_1 + 0xc4) = uVar2;
  sVar3 = strlen(param_4);
  sVar4 = strlen(param_5);
  sVar3 = sVar3 + sVar4 + 2;
  param0 = malloc(sVar3);
  *(char **)((int)param_1 + 200) = param0;
  if (param0 == (char *)0x0) {
    local_1018[0] = 'F';
    local_1018[1] = 'a';
    local_1018[2] = 'i';
    local_1018[3] = 'l';
    acStack_1014[0] = 'e';
    acStack_1014[1] = 'd';
    acStack_1014[2] = ' ';
    acStack_1014[3] = 't';
    acStack_1010[0] = 'o';
    acStack_1010[1] = ' ';
    acStack_1010[2] = 'm';
    acStack_1010[3] = 'a';
    acStack_100c[0] = 'l';
    acStack_100c[1] = 'l';
    acStack_100c[2] = 'o';
    acStack_100c[3] = 'c';
    local_1008[0] = ' ';
    local_1008[1] = 'u';
    local_1008[2] = 's';
    local_1008[3] = 'e';
    acStack_1004[0] = 'r';
    acStack_1004[1] = 'p';
    acStack_1004[2] = 'a';
    acStack_1004[3] = 's';
    local_1000 = 0x73;
    FUN_00038730(3,local_1018,1);
    FUN_00016ca8(1);
    param0 = *(char **)((int)param_1 + 200);
    param_4 = *(char **)((int)param_1 + 0xcc);
    param_5 = *(char **)((int)param_1 + 0xd0);
  }
  snprintf(param0,sVar3,"%s:%s",param_4,param_5);
  piVar5 = (int *)(param_2 ^ 1);
  bVar6 = *(int *)((int)param_1 + 0x84) != 1;
  *(char *)((int)param_1 + 0x8b) = (char)piVar5;
  if (bVar6) {
    *(undefined4 *)((int)param_1 + 0x84) = 1;
    piVar5 = &DAT_0007b5e4;
  }
  *(undefined1 *)((int)param_1 + 0x13c) = 1;
  *(undefined1 *)((int)param_1 + 0x81) = 1;
  if (bVar6) {
    *piVar5 = *piVar5 + 1;
  }
  pthread_create((pthread_t *)((int)param_1 + 0x138),(pthread_attr_t *)0x0,
                 (__start_routine *)&LAB_0001fe0c,param_1);
  if (param_2 == 0) {
    pthread_join(*(pthread_t *)((int)param_1 + 0x138),(void **)0x0);
    uVar1 = DAT_0007730c;
  }
  else {
    uVar1 = 1;
  }
  return uVar1;
}

