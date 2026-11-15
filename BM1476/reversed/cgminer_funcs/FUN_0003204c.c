//==================== FUN_0003204c @ 0003204c ====================

undefined4 FUN_0003204c(char *param_1,undefined4 *param_2,undefined4 *param_3,undefined4 *param_4)

{
  size_t sVar1;
  void *__ptr;
  char *apcStack_102c [2];
  void *pvStack_1024;
  char local_1020 [4];
  char acStack_101c [4];
  char acStack_1018 [4];
  char acStack_1014 [4];
  char local_1010 [4];
  char acStack_100c [4];
  char acStack_1008 [4];
  char acStack_1004 [4];
  undefined1 local_1000;
  
  apcStack_102c[0] = param_1;
  sVar1 = strlen(param_1);
  __ptr = malloc(sVar1 + 1);
  pvStack_1024 = __ptr;
  if (__ptr == (void *)0x0) {
    local_1020[0] = 'F';
    local_1020[1] = 'a';
    local_1020[2] = 'i';
    local_1020[3] = 'l';
    acStack_101c[0] = 'e';
    acStack_101c[1] = 'd';
    acStack_101c[2] = ' ';
    acStack_101c[3] = 't';
    acStack_1018[0] = 'o';
    acStack_1018[1] = ' ';
    acStack_1018[2] = 'm';
    acStack_1018[3] = 'a';
    acStack_1014[0] = 'l';
    acStack_1014[1] = 'l';
    acStack_1014[2] = 'o';
    acStack_1014[3] = 'c';
    local_1010[0] = ' ';
    local_1010[1] = 'p';
    local_1010[2] = 'o';
    local_1010[3] = 'o';
    acStack_100c[0] = 'l';
    acStack_100c[1] = 'd';
    acStack_100c[2] = 'e';
    acStack_100c[3] = 't';
    acStack_1008[0] = 'a';
    acStack_1008[1] = 'i';
    acStack_1008[2] = 'l';
    acStack_1008[3] = 's';
    acStack_1004[0] = ' ';
    acStack_1004[1] = 'b';
    acStack_1004[2] = 'u';
    acStack_1004[3] = 'f';
    local_1000 = 0;
    FUN_00038730(3,local_1020,1);
    FUN_00016ca8(1);
  }
  *param_2 = pvStack_1024;
  FUN_00031fc4(apcStack_102c,&pvStack_1024);
  if (*apcStack_102c[0] != '\0') {
    *param_3 = pvStack_1024;
    FUN_00031fc4(apcStack_102c,&pvStack_1024);
    if (*apcStack_102c[0] != '\0') {
      *param_4 = pvStack_1024;
      FUN_00031fc4(apcStack_102c,&pvStack_1024);
      return 1;
    }
  }
  free(__ptr);
  return 0;
}

