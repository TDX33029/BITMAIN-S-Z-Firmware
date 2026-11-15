//==================== FUN_000267b0 @ 000267b0 ====================

void FUN_000267b0(void)

{
  int iVar1;
  char *__src;
  FILE *__stream;
  uint uVar2;
  char cStack_30b1;
  stat sStack_30b0;
  char acStack_3054 [4096];
  char local_2054 [4];
  char acStack_2050 [4];
  char acStack_204c [4];
  char acStack_2048 [4];
  char local_2044 [4];
  char acStack_2040 [4];
  uint local_203c;
  char local_1054 [4];
  char acStack_1050 [4];
  char acStack_104c [4];
  undefined3 uStack_1048;
  char cStack_1045;
  undefined3 local_1044;
  char cStack_1041;
  char acStack_1040 [4];
  char acStack_103c [4];
  char acStack_1038 [4];
  char local_1034 [4];
  char acStack_1030 [4];
  char acStack_102c [4];
  undefined1 local_1028;
  
  DAT_0007bac8 = 1;
  immedok(DAT_0007b430,1);
  FUN_00018ad8();
LAB_000267f4:
  while( true ) {
    snprintf(local_1054,0x1000,
             "[Q]ueue: %d\n[S]cantime: %d\n[E]xpiry: %d\n[W]rite config file\n[C]gminer restart\n",
             DAT_00075ddc,DAT_00075df0,DAT_00075df4);
    FUN_00018bc0(local_1054);
    local_1054[0] = 'S';
    local_1054[1] = 'e';
    local_1054[2] = 'l';
    local_1054[3] = 'e';
    acStack_1050[0] = 'c';
    acStack_1050[1] = 't';
    acStack_1050[2] = ' ';
    acStack_1050[3] = 'a';
    acStack_104c[0] = 'n';
    acStack_104c[1] = ' ';
    acStack_104c[2] = 'o';
    acStack_104c[3] = 'p';
    uStack_1048._0_1_ = 't';
    uStack_1048._1_1_ = 'i';
    uStack_1048._2_1_ = 'o';
    cStack_1045 = 'n';
    local_1044._0_1_ = ' ';
    local_1044._1_1_ = 'o';
    local_1044._2_1_ = 'r';
    cStack_1041 = ' ';
    acStack_1040[0] = 'a';
    acStack_1040[1] = 'n';
    acStack_1040[2] = 'y';
    acStack_1040[3] = ' ';
    acStack_103c[0] = 'o';
    acStack_103c[1] = 't';
    acStack_103c[2] = 'h';
    acStack_103c[3] = 'e';
    acStack_1038[0] = 'r';
    acStack_1038[1] = ' ';
    acStack_1038[2] = 'k';
    acStack_1038[3] = 'e';
    local_1034[0] = 'y';
    local_1034[1] = ' ';
    local_1034[2] = 't';
    local_1034[3] = 'o';
    acStack_1030[0] = ' ';
    acStack_1030[1] = 'r';
    acStack_1030[2] = 'e';
    acStack_1030[3] = 't';
    acStack_102c[0] = 'u';
    acStack_102c[1] = 'r';
    acStack_102c[2] = 'n';
    acStack_102c[3] = '\n';
    local_1028 = 0;
    FUN_00018bc0(local_1054);
    FUN_00018b44();
    cStack_30b1 = wgetch(stdscr);
    iVar1 = strncasecmp(&cStack_30b1,"q",1);
    if (iVar1 != 0) break;
    uVar2 = FUN_000260d0("Extra work items to queue");
    if (uVar2 < 10000) {
      DAT_00075ddc = uVar2;
      if ((int)uVar2 < (int)DAT_00075dd8) {
        DAT_00075dd8 = uVar2;
      }
    }
    else {
LAB_000269b0:
      local_1054[0] = 'I';
      local_1054[1] = 'n';
      local_1054[2] = 'v';
      local_1054[3] = 'a';
      acStack_1050[0] = 'l';
      acStack_1050[1] = 'i';
      acStack_1050[2] = 'd';
      acStack_1050[3] = ' ';
      acStack_104c[0] = 's';
      acStack_104c[1] = 'e';
      acStack_104c[2] = 'l';
      acStack_104c[3] = 'e';
      uStack_1048._0_1_ = 'c';
      uStack_1048._1_1_ = 't';
      uStack_1048._2_1_ = 'i';
      cStack_1045 = 'o';
      local_1044._0_1_ = 'n';
      local_1044._1_1_ = '\n';
      local_1044._2_1_ = '\0';
      FUN_00018bc0(local_1054);
    }
  }
  iVar1 = strncasecmp(&cStack_30b1,"s",1);
  if (iVar1 == 0) {
    uVar2 = FUN_000260d0("Set scantime in seconds");
    if (9999 < uVar2) goto LAB_000269b0;
    DAT_00075df0 = uVar2;
    goto LAB_000267f4;
  }
  iVar1 = strncasecmp(&cStack_30b1,"e",1);
  if (iVar1 == 0) {
    uVar2 = FUN_000260d0("Set expiry time in seconds");
    if (9999 < uVar2) goto LAB_000269b0;
    DAT_00075df4 = uVar2;
    goto LAB_000267f4;
  }
  iVar1 = strncasecmp(&cStack_30b1,"w",1);
  if (iVar1 == 0) {
    FUN_00016150(acStack_3054);
    snprintf(local_1054,0x1032,"Config filename to write (Enter for default) [%s]",acStack_3054);
    __src = (char *)FUN_00025fdc(local_1054);
    if (((*__src == '-') && (__src[1] == '1')) && (__src[2] == '\0')) {
      free(__src);
LAB_00026928:
      __stream = fopen(acStack_3054,"w");
      if (__stream == (FILE *)0x0) {
        local_2054[0] = 'C';
        local_2054[1] = 'a';
        local_2054[2] = 'n';
        local_2054[3] = 'n';
        acStack_2050[0] = 'o';
        acStack_2050[1] = 't';
        acStack_2050[2] = ' ';
        acStack_2050[3] = 'o';
        acStack_204c[0] = 'p';
        acStack_204c[1] = 'e';
        acStack_204c[2] = 'n';
        acStack_204c[3] = ' ';
        acStack_2048[0] = 'o';
        acStack_2048[1] = 'r';
        acStack_2048[2] = ' ';
        acStack_2048[3] = 'c';
        local_2044[0] = 'r';
        local_2044[1] = 'e';
        local_2044[2] = 'a';
        local_2044[3] = 't';
        acStack_2040[0] = 'e';
        acStack_2040[1] = ' ';
        acStack_2040[2] = 'f';
        acStack_2040[3] = 'i';
        local_203c._0_1_ = 'l';
        local_203c._1_1_ = 'e';
        local_203c._2_1_ = '\n';
        local_203c._3_1_ = '\0';
        FUN_00018bc0(local_2054);
      }
      else {
        FUN_00015b64();
        fclose(__stream);
      }
    }
    else {
      strcpy(acStack_3054,__src);
      free(__src);
      iVar1 = __xstat(3,acStack_3054,&sStack_30b0);
      if (iVar1 != 0) goto LAB_00026928;
      local_2054[0] = 'F';
      local_2054[1] = 'i';
      local_2054[2] = 'l';
      local_2054[3] = 'e';
      acStack_2050[0] = ' ';
      acStack_2050[1] = 'e';
      acStack_2050[2] = 'x';
      acStack_2050[3] = 'i';
      acStack_204c[0] = 's';
      acStack_204c[1] = 't';
      acStack_204c[2] = 's';
      acStack_204c[3] = ',';
      acStack_2048[0] = ' ';
      acStack_2048[1] = 'o';
      acStack_2048[2] = 'v';
      acStack_2048[3] = 'e';
      local_2044[0] = 'r';
      local_2044[1] = 'w';
      local_2044[2] = 'r';
      local_2044[3] = 'i';
      acStack_2040[0] = 't';
      acStack_2040[1] = 'e';
      acStack_2040[2] = '?';
      acStack_2040[3] = '\n';
      local_203c = local_203c & 0xffffff00;
      FUN_00018bc0(local_2054);
      cStack_30b1 = wgetch(stdscr);
      iVar1 = strncasecmp(&cStack_30b1,"y",1);
      if (iVar1 == 0) goto LAB_00026928;
    }
    goto LAB_000267f4;
  }
  iVar1 = strncasecmp(&cStack_30b1,"c",1);
  if (iVar1 == 0) {
    local_1054[0] = 'A';
    local_1054[1] = 'r';
    local_1054[2] = 'e';
    local_1054[3] = ' ';
    acStack_1050[0] = 'y';
    acStack_1050[1] = 'o';
    acStack_1050[2] = 'u';
    acStack_1050[3] = ' ';
    acStack_104c[0] = 's';
    acStack_104c[1] = 'u';
    acStack_104c[2] = 'r';
    acStack_104c[3] = 'e';
    uStack_1048._0_1_ = '?';
    uStack_1048._1_1_ = '\n';
    uStack_1048._2_1_ = '\0';
    FUN_00018bc0(local_1054);
    cStack_30b1 = wgetch(stdscr);
    iVar1 = strncasecmp(&cStack_30b1,"y",1);
    if (iVar1 == 0) {
      FUN_0001d7c8();
      goto LAB_00026b54;
    }
  }
  FUN_00018ad8();
LAB_00026b54:
  immedok(DAT_0007b430,0);
  DAT_0007bac8 = 0;
  return;
}

