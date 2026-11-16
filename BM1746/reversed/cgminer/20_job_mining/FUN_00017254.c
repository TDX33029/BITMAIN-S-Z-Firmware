//==================== FUN_00017254 @ 00017254 ====================

undefined4 FUN_00017254(void *param_1)

{
  char *pcVar1;
  undefined4 uVar2;
  size_t sVar3;
  size_t sVar4;
  ulong param2;
  uint uVar5;
  uchar *__s;
  uchar **ppuVar6;
  int iVar7;
  int param4;
  int param7;
  uchar *local_1524;
  int local_1520 [2];
  char *local_1518;
  char *local_1514;
  uint local_1510 [16];
  undefined1 auStack_14d0 [136];
  undefined4 local_1448;
  undefined4 uStack_1444;
  undefined4 uStack_1440;
  undefined4 uStack_143c;
  undefined4 local_1438;
  undefined4 uStack_1434;
  undefined4 uStack_1430;
  undefined4 uStack_142c;
  uchar local_1428 [1024];
  char local_1028 [4];
  char acStack_1024 [4];
  char acStack_1020 [4];
  char acStack_101c [4];
  char local_1018 [4];
  char acStack_1014 [4];
  char acStack_1010 [4];
  char acStack_100c [4];
  undefined1 local_1008;
  char acStack_fa0 [3964];
  
  ppuVar6 = &local_1524;
  if (DAT_00076ff8 == (FILE *)0x0) {
    if (DAT_00076ffc == (char *)0x0) {
      local_1028[0] = 'B';
      local_1028[1] = 'E';
      local_1028[2] = 'N';
      local_1028[3] = 'C';
      acStack_1024[0] = 'H';
      acStack_1024[1] = 'F';
      acStack_1024[2] = 'I';
      acStack_1024[3] = 'L';
      acStack_1020[0] = 'E';
      acStack_1020[1] = ' ';
      acStack_1020[2] = 'I';
      acStack_1020[3] = 'n';
      acStack_101c[0] = 'v';
      acStack_101c[1] = 'a';
      acStack_101c[2] = 'l';
      acStack_101c[3] = 'i';
      local_1018[0] = 'd';
      local_1018[1] = ' ';
      local_1018[2] = 'b';
      local_1018[3] = 'e';
      acStack_1014[0] = 'n';
      acStack_1014[1] = 'c';
      acStack_1014[2] = 'h';
      acStack_1014[3] = 'f';
      acStack_1010[0] = 'i';
      acStack_1010[1] = 'l';
      acStack_1010[2] = 'e';
      acStack_1010[3] = ' ';
      acStack_100c[0] = 'N';
      acStack_100c[1] = 'U';
      acStack_100c[2] = 'L';
      acStack_100c[3] = 'L';
      local_1008 = 0;
      FUN_00038730(3,local_1028,1);
      FUN_00016ca8(1);
    }
    else {
      DAT_00076ff8 = fopen(DAT_00076ffc,"r");
    }
    if (DAT_00076ff8 == (FILE *)0x0) {
      snprintf(local_1028,0x1000,"BENCHFILE Failed to open benchfile \'%s\'",DAT_00076ffc);
      FUN_00038730(3,local_1028,1);
      FUN_00016ca8(1);
    }
    DAT_00077000 = 0;
    pcVar1 = fgets((char *)local_1428,0x400,DAT_00076ff8);
    if (pcVar1 == (char *)0x0) {
      snprintf(local_1028,0x1000,"BENCHFILE Failed to read benchfile \'%s\'",DAT_00076ffc);
      FUN_00038730(3,local_1028,1);
      FUN_00016ca8(1);
    }
    DAT_00077004 = 0;
  }
  else {
    pcVar1 = fgets((char *)local_1428,0x400,DAT_00076ff8);
    if (pcVar1 == (char *)0x0) goto joined_r0x000172e4;
  }
  __s = local_1428;
  do {
    DAT_00077000 = DAT_00077000 + 1;
    if ((local_1428[0] != '#' && local_1428[0] != '\0') && (local_1428[0] != '/')) {
      iVar7 = 0;
      param4 = 1;
      local_1524 = __s;
      do {
        pcVar1 = strchr((char *)__s,0x2c);
        ppuVar6[1] = (uchar *)pcVar1;
        if (pcVar1 == (char *)0x0) {
          snprintf(local_1028,0x1000,
                   "BENCHFILE Invalid input file line %d - field count is %d but should be %d",
                   DAT_00077000,param4,5);
          FUN_00038730(3,local_1028,1);
          FUN_00016ca8(1);
        }
        param7 = *(int *)((int)&DAT_0005d6ac + iVar7);
        if ((param7 != 0) && ((int)pcVar1 - (int)*ppuVar6 != param7)) {
          snprintf(local_1028,0x1000,
                   "BENCHFILE Invalid input file line %d field %d (%s) length is %d but should be %d"
                   ,DAT_00077000,param4,*(char **)((int)&DAT_0005d6b0 + iVar7),
                   (int)pcVar1 - (int)*ppuVar6,param7);
          FUN_00038730(3,local_1028,1);
          FUN_00016ca8(1);
        }
        param4 = param4 + 1;
        __s = (uchar *)(pcVar1 + 1);
        *pcVar1 = '\0';
        iVar7 = iVar7 + 8;
        ppuVar6[1] = __s;
        ppuVar6 = ppuVar6 + 1;
      } while (param4 != 5);
      sVar3 = strlen(local_1514);
      if ((int)sVar3 < 10) {
        snprintf(local_1028,0x1000,
                 "BENCHFILE Invalid input file line %d field %d (%s) length is %d but should be least %d"
                 ,DAT_00077000,5,"NonceTime",sVar3,10);
        FUN_00038730(3,local_1028,1);
        FUN_00016ca8(1);
      }
      sprintf(local_1028,"0000000%c",*local_1524);
      sVar4 = strlen(local_1028);
      local_1520[1] = local_1520[1] + 0x38;
      sVar3 = sVar4;
      do {
        pcVar1 = local_1028 + sVar3;
        sVar3 = sVar3 + 8;
        sprintf(pcVar1,"%.8s",local_1520[1]);
        local_1520[1] = local_1520[1] + -8;
      } while (sVar3 != sVar4 + 0x40);
      iVar7 = local_1520[0] + 0x38;
      do {
        pcVar1 = local_1028 + sVar3;
        sVar3 = sVar3 + 8;
        sprintf(pcVar1,"%.8s",iVar7);
        iVar7 = iVar7 + -8;
      } while (sVar3 != sVar4 + 0x80);
      param2 = strtol(local_1514,(char **)0x0,10);
      sprintf(local_1028 + sVar3,"%08lx",param2);
      iVar7 = 0;
      strcpy(local_1028 + sVar4 + 0x88,local_1518);
      memset(param_1,0,0x7b0);
      FUN_00029830(param_1,local_1028,(int)(sVar4 + 0x90) >> 1);
      do {
        uVar5 = *(uint *)((int)param_1 + iVar7);
        *(uint *)((int)local_1510 + iVar7) =
             uVar5 << 0x18 | (uVar5 >> 8 & 0xff) << 0x10 | (uVar5 >> 0x10 & 0xff) << 8 |
             uVar5 >> 0x18;
        iVar7 = iVar7 + 4;
      } while (iVar7 != 0x40);
      FUN_000302d0(auStack_14d0);
      FUN_00030304(auStack_14d0,local_1510,0x40);
      DAT_00077004 = DAT_00077004 + 1;
      *(undefined4 *)((int)param_1 + 0x80) = local_1448;
      *(undefined4 *)((int)param_1 + 0x84) = uStack_1444;
      *(undefined4 *)((int)param_1 + 0x88) = uStack_1440;
      *(undefined4 *)((int)param_1 + 0x8c) = uStack_143c;
      *(undefined4 *)((int)param_1 + 0x90) = local_1438;
      *(undefined4 *)((int)param_1 + 0x94) = uStack_1434;
      *(undefined4 *)((int)param_1 + 0x98) = uStack_1430;
      *(undefined4 *)((int)param_1 + 0x9c) = uStack_142c;
      return 1;
    }
    pcVar1 = fgets((char *)__s,0x400,DAT_00076ff8);
  } while (pcVar1 != (char *)0x0);
joined_r0x000172e4:
  if (DAT_00077004 == 0) {
    snprintf(local_1028,0x1000,"BENCHFILE No work in benchfile \'%s\'",DAT_00076ffc);
    FUN_00038730(3,local_1028,1);
    FUN_00016ca8(1);
  }
  fclose(DAT_00076ff8);
  DAT_00076ff8 = (FILE *)0x0;
  uVar2 = FUN_00017254(param_1);
  return uVar2;
}

