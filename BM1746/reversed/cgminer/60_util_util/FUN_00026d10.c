//==================== FUN_00026d10 @ 00026d10 ====================

int FUN_00026d10(undefined4 param_1)

{
  char *__s1;
  char *__ptr;
  char *__ptr_00;
  undefined4 uVar1;
  int iVar2;
  char *pcVar3;
  char local_1020 [4];
  char acStack_101c [4];
  char acStack_1018 [4];
  char acStack_1014 [4];
  char local_1010 [4];
  undefined3 uStack_100c;
  char cStack_1009;
  undefined2 uStack_1008;
  undefined1 local_1006;
  
  immedok(DAT_0007b430,1);
  local_1020[0] = 'I';
  local_1020[1] = 'n';
  local_1020[2] = 'p';
  local_1020[3] = 'u';
  acStack_101c[0] = 't';
  acStack_101c[1] = ' ';
  acStack_101c[2] = 's';
  acStack_101c[3] = 'e';
  acStack_1018[0] = 'r';
  acStack_1018[1] = 'v';
  acStack_1018[2] = 'e';
  acStack_1018[3] = 'r';
  acStack_1014[0] = ' ';
  acStack_1014[1] = 'd';
  acStack_1014[2] = 'e';
  acStack_1014[3] = 't';
  local_1010[0] = 'a';
  local_1010[1] = 'i';
  local_1010[2] = 'l';
  local_1010[3] = 's';
  uStack_100c._0_1_ = '.';
  uStack_100c._1_1_ = '\n';
  uStack_100c._2_1_ = '\0';
  FUN_00018bc0(local_1020);
  __s1 = (char *)FUN_00025fdc("URL");
  pcVar3 = __s1;
  if ((((*__s1 == '-') && (__s1[1] == '1')) && (__ptr = (char *)0x0, __s1[2] == '\0')) ||
     (((__ptr = (char *)FUN_00025fdc("Username"), *__ptr == '-' && (__ptr[1] == '1')) &&
      (__ptr[2] == '\0')))) {
    immedok(DAT_0007b430,0);
    __ptr_00 = (char *)0x0;
  }
  else {
    __ptr_00 = (char *)FUN_00025fdc("Password");
    if (((*__ptr_00 == '-') && (__ptr_00[1] == '1')) && (__ptr_00[2] == '\0')) {
      free(__ptr_00);
      __ptr_00 = calloc(1,1);
    }
    uVar1 = FUN_00017a3c();
    iVar2 = FUN_0001557c(uVar1,__s1);
    if (((iVar2 == 0) && (iVar2 = strncmp(__s1,"http://",7), iVar2 != 0)) &&
       (iVar2 = strncmp(__s1,"https://",8), iVar2 != 0)) {
      pcVar3 = malloc(0x100);
      if (pcVar3 == (char *)0x0) {
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
        local_1010[1] = 'h';
        local_1010[2] = 't';
        local_1010[3] = 't';
        uStack_100c._0_1_ = 'p';
        uStack_100c._1_1_ = 'i';
        uStack_100c._2_1_ = 'n';
        cStack_1009 = 'p';
        uStack_1008 = 0x7475;
        local_1006 = 0;
        FUN_00038730(3,local_1020,1);
        FUN_00016ca8(1);
      }
      builtin_strncpy(pcVar3,"http://",8);
      strncat(pcVar3,__s1,0xf8);
      free(__s1);
    }
    iVar2 = FUN_00026bcc(uVar1,param_1,pcVar3,__ptr,__ptr_00);
    immedok(DAT_0007b430,0);
    if (iVar2 != 0) {
      return iVar2;
    }
  }
  free(pcVar3);
  free(__ptr);
  free(__ptr_00);
  return 0;
}

