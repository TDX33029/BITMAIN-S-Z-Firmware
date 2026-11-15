//==================== FUN_0002b040 @ 0002b040 ====================

char * FUN_0002b040(int param_1)

{
  char *pcVar1;
  size_t sVar2;
  size_t sVar3;
  uint uVar4;
  int iVar5;
  ssize_t sVar6;
  int *piVar7;
  int iVar8;
  void *pvVar9;
  char *__s;
  uint uVar10;
  uint uVar11;
  uint in_fpscr;
  double dVar12;
  double dVar13;
  int iStack_3040;
  int local_303c;
  int iStack_3038;
  int local_3034;
  char local_3030 [4];
  char acStack_302c [4];
  char acStack_3028 [4];
  char acStack_3024 [4];
  char local_3020 [4];
  char acStack_301c [4];
  char acStack_3018 [4];
  char acStack_3014 [4];
  ushort local_3010;
  undefined1 local_300e;
  char local_2030 [4];
  char acStack_202c [4];
  char acStack_2028 [4];
  char acStack_2024 [4];
  char local_2020 [4];
  char acStack_201c [4];
  char acStack_2018 [4];
  char acStack_2014 [4];
  char local_2010 [4];
  char acStack_200c [4];
  undefined2 local_2008 [2];
  char acStack_2004 [4];
  char local_2000 [8148];
  
  __s = *(char **)(param_1 + 0x268);
  pcVar1 = strchr(__s,10);
  if (pcVar1 == (char *)0x0) {
    FUN_0002a84c(&iStack_3040);
    iVar5 = FUN_00028afc(param_1,0x3c);
    if (iVar5 == 0) {
      if ((DAT_00078e09 != '\0') &&
         (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
        local_2030[0] = 'T';
        local_2030[1] = 'i';
        local_2030[2] = 'm';
        local_2030[3] = 'e';
        acStack_202c[0] = 'd';
        acStack_202c[1] = ' ';
        acStack_202c[2] = 'o';
        acStack_202c[3] = 'u';
        acStack_2028[0] = 't';
        acStack_2028[1] = ' ';
        acStack_2028[2] = 'w';
        acStack_2028[3] = 'a';
        acStack_2024[0] = 'i';
        acStack_2024[1] = 't';
        acStack_2024[2] = 'i';
        acStack_2024[3] = 'n';
        local_2020[0] = 'g';
        local_2020[1] = ' ';
        local_2020[2] = 'f';
        local_2020[3] = 'o';
        acStack_201c[0] = 'r';
        acStack_201c[1] = ' ';
        acStack_201c[2] = 'd';
        acStack_201c[3] = 'a';
        acStack_2018[0] = 't';
        acStack_2018[1] = 'a';
        acStack_2018[2] = ' ';
        acStack_2018[3] = 'o';
        acStack_2014[0] = 'n';
        acStack_2014[1] = ' ';
        acStack_2014[2] = 's';
        acStack_2014[3] = 'o';
        local_2010[0] = 'c';
        local_2010[1] = 'k';
        local_2010[2] = 'e';
        local_2010[3] = 't';
        acStack_200c[0] = '_';
        acStack_200c[1] = 'f';
        acStack_200c[2] = 'u';
        acStack_200c[3] = 'l';
        local_2008[0] = 0x6c;
        FUN_00038730(7,local_2030,0);
      }
      goto LAB_0002b224;
    }
    while( true ) {
      memset(local_2030,0,0x2000);
      sVar6 = recv(*(int *)(param_1 + 0x264),local_2030,0x1ffc,0);
      if (sVar6 == 0) break;
      FUN_0002a84c(&iStack_3038);
      dVar12 = (double)VectorSignedToFloat(local_3034 - local_303c,(byte)(in_fpscr >> 0x16) & 3);
      dVar13 = (double)VectorSignedToFloat(iStack_3038 - iStack_3040,(byte)(in_fpscr >> 0x16) & 3);
      iVar5 = VectorFloatToSigned(dVar13 + dVar12 / 1000000.0,3);
      if (sVar6 < 0) {
        piVar7 = __errno_location();
        if ((*piVar7 != 0xb) || (iVar8 = FUN_00028afc(param_1,0x3c - iVar5), iVar8 == 0)) {
          if ((DAT_00078e09 != '\0') &&
             (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
            local_3030[0] = 'F';
            local_3030[1] = 'a';
            local_3030[2] = 'i';
            local_3030[3] = 'l';
            acStack_302c[0] = 'e';
            acStack_302c[1] = 'd';
            acStack_302c[2] = ' ';
            acStack_302c[3] = 't';
            acStack_3028[0] = 'o';
            acStack_3028[1] = ' ';
            acStack_3028[2] = 'r';
            acStack_3028[3] = 'e';
            acStack_3024[0] = 'c';
            acStack_3024[1] = 'v';
            acStack_3024[2] = ' ';
            acStack_3024[3] = 's';
            local_3020[0] = 'o';
            local_3020[1] = 'c';
            local_3020[2] = 'k';
            local_3020[3] = ' ';
            acStack_301c[0] = 'i';
            acStack_301c[1] = 'n';
            acStack_301c[2] = ' ';
            acStack_301c[3] = 'r';
            acStack_3018[0] = 'e';
            acStack_3018[1] = 'c';
            acStack_3018[2] = 'v';
            acStack_3018[3] = '_';
            acStack_3014[0] = 'l';
            acStack_3014[1] = 'i';
            acStack_3014[2] = 'n';
            acStack_3014[3] = 'e';
            local_3010 = local_3010 & 0xff00;
            FUN_00038730(7,local_3030,0);
          }
          goto LAB_0002b3fc;
        }
      }
      else {
        sVar2 = strlen(local_2030);
        pcVar1 = *(char **)(param_1 + 0x268);
        sVar3 = strlen(pcVar1);
        uVar4 = sVar2 + sVar3 + 1;
        if (*(uint *)(param_1 + 0x26c) <= uVar4) {
          sVar2 = (uVar4 & 0xffffe000) + 0x2000;
          pvVar9 = realloc(pcVar1,sVar2);
          *(void **)(param_1 + 0x268) = pvVar9;
          if (pvVar9 == (void *)0x0) {
            snprintf(local_3030,0x1000,"Failed to realloc pool sockbuf in %s %s():%d","util.c",
                     "recalloc_sock",0x7cd);
            FUN_00038730(3,local_3030,1);
            FUN_00016ca8(1);
            pvVar9 = *(void **)(param_1 + 0x268);
          }
          memset((void *)((int)pvVar9 + sVar3),0,sVar2 - sVar3);
          pcVar1 = *(char **)(param_1 + 0x268);
          *(size_t *)(param_1 + 0x26c) = sVar2;
        }
        strcat(pcVar1,local_2030);
      }
      __s = *(char **)(param_1 + 0x268);
      if ((0x3b < iVar5) || (pcVar1 = strchr(__s,10), pcVar1 != (char *)0x0)) goto LAB_0002b06c;
    }
    if ((DAT_00078e09 != '\0') &&
       (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
      local_3030[0] = 'S';
      local_3030[1] = 'o';
      local_3030[2] = 'c';
      local_3030[3] = 'k';
      acStack_302c[0] = 'e';
      acStack_302c[1] = 't';
      acStack_302c[2] = ' ';
      acStack_302c[3] = 'c';
      acStack_3028[0] = 'l';
      acStack_3028[1] = 'o';
      acStack_3028[2] = 's';
      acStack_3028[3] = 'e';
      acStack_3024[0] = 'd';
      acStack_3024[1] = ' ';
      acStack_3024[2] = 'w';
      acStack_3024[3] = 'a';
      local_3020[0] = 'i';
      local_3020[1] = 't';
      local_3020[2] = 'i';
      local_3020[3] = 'n';
      acStack_301c[0] = 'g';
      acStack_301c[1] = ' ';
      acStack_301c[2] = 'i';
      acStack_301c[3] = 'n';
      acStack_3018[0] = ' ';
      acStack_3018[1] = 'r';
      acStack_3018[2] = 'e';
      acStack_3018[3] = 'c';
      acStack_3014[0] = 'v';
      acStack_3014[1] = '_';
      acStack_3014[2] = 'l';
      acStack_3014[3] = 'i';
      local_3010 = 0x656e;
      local_300e = 0;
      FUN_00038730(7,local_3030,0);
    }
LAB_0002b3fc:
    FUN_0002aee0(param_1);
    __s = *(char **)(param_1 + 0x268);
  }
LAB_0002b06c:
  sVar2 = strlen(__s);
  pcVar1 = strtok(__s,"\n");
  if (pcVar1 == (char *)0x0) {
    if ((DAT_00078e09 != '\0') &&
       (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
      local_2030[0] = 'F';
      local_2030[1] = 'a';
      local_2030[2] = 'i';
      local_2030[3] = 'l';
      acStack_202c[0] = 'e';
      acStack_202c[1] = 'd';
      acStack_202c[2] = ' ';
      acStack_202c[3] = 't';
      acStack_2028[0] = 'o';
      acStack_2028[1] = ' ';
      acStack_2028[2] = 'p';
      acStack_2028[3] = 'a';
      acStack_2024[0] = 'r';
      acStack_2024[1] = 's';
      acStack_2024[2] = 'e';
      acStack_2024[3] = ' ';
      local_2020[0] = 'a';
      local_2020[1] = ' ';
      local_2020[2] = '\\';
      local_2020[3] = 'n';
      acStack_201c[0] = ' ';
      acStack_201c[1] = 't';
      acStack_201c[2] = 'e';
      acStack_201c[3] = 'r';
      acStack_2018[0] = 'm';
      acStack_2018[1] = 'i';
      acStack_2018[2] = 'n';
      acStack_2018[3] = 'a';
      acStack_2014[0] = 't';
      acStack_2014[1] = 'e';
      acStack_2014[2] = 'd';
      acStack_2014[3] = ' ';
      local_2010[0] = 's';
      local_2010[1] = 't';
      local_2010[2] = 'r';
      local_2010[3] = 'i';
      acStack_200c[0] = 'n';
      acStack_200c[1] = 'g';
      acStack_200c[2] = ' ';
      acStack_200c[3] = 'i';
      local_2008[0]._0_1_ = 'n';
      local_2008[0]._1_1_ = ' ';
      local_2008[1]._0_1_ = 'r';
      local_2008[1]._1_1_ = 'e';
      acStack_2004[0] = 'c';
      acStack_2004[1] = 'v';
      acStack_2004[2] = '_';
      acStack_2004[3] = 'l';
      local_2000[0] = 'i';
      local_2000[1] = 'n';
      local_2000[2] = 'e';
      local_2000[3] = '\0';
      FUN_00038730(7,local_2030,0);
    }
  }
  else {
    pcVar1 = (char *)__strdup();
    sVar3 = strlen(pcVar1);
    if ((int)(sVar3 + 1) < (int)sVar2) {
      memmove(*(void **)(param_1 + 0x268),(void *)((int)*(void **)(param_1 + 0x268) + sVar3 + 1),
              (sVar2 - sVar3) + 1);
    }
    else {
      **(undefined1 **)(param_1 + 0x268) = 0;
    }
    uVar11 = *(uint *)(param_1 + 0x228);
    uVar4 = *(uint *)(param_1 + 0x230);
    uVar10 = *(uint *)(param_1 + 0x220);
    *(uint *)(param_1 + 0x220) = uVar10 + 1;
    *(uint *)(param_1 + 0x224) = *(int *)(param_1 + 0x224) + (uint)(0xfffffffe < uVar10);
    *(uint *)(param_1 + 0x228) = uVar11 + sVar3;
    *(uint *)(param_1 + 0x22c) =
         *(int *)(param_1 + 0x22c) + ((int)sVar3 >> 0x1f) + (uint)CARRY4(uVar11,sVar3);
    *(size_t *)(param_1 + 0x230) = sVar3 + uVar4;
    *(uint *)(param_1 + 0x234) =
         ((int)sVar3 >> 0x1f) + *(int *)(param_1 + 0x234) + (uint)CARRY4(sVar3,uVar4);
    if (pcVar1 != (char *)0x0) {
      if (DAT_0007b380 == '\0') {
        return pcVar1;
      }
      if (DAT_00078e09 == '\0') {
        return pcVar1;
      }
      if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 7)) {
        return pcVar1;
      }
      snprintf(local_2030,0x1000,"RECVD: %s",pcVar1);
      FUN_00038730(7,local_2030,0);
      return pcVar1;
    }
  }
LAB_0002b224:
  FUN_000295a8(param_1);
  return (char *)0x0;
}

