//==================== FUN_00016244 @ 00016244 ====================

void FUN_00016244(undefined8 *param_1)

{
  char *param3;
  double in_d0;
  double dVar1;
  double dVar2;
  undefined8 local_1038;
  undefined8 local_1030;
  undefined8 local_1028;
  undefined8 local_1020;
  char local_1018 [4];
  char acStack_1014 [4];
  char acStack_1010 [4];
  char acStack_100c [4];
  char local_1008 [4];
  char acStack_1004 [4];
  char acStack_1000 [4];
  undefined2 uStack_ffc;
  undefined1 local_ffa;
  
  if (in_d0 == 0.0) {
    if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 3)) {
      in_d0 = 1.0;
    }
    else {
      local_1018[0] = 'D';
      local_1018[1] = 'i';
      local_1018[2] = 'f';
      local_1018[3] = 'f';
      acStack_1014[0] = ' ';
      acStack_1014[1] = 'z';
      acStack_1014[2] = 'e';
      acStack_1014[3] = 'r';
      acStack_1010[0] = 'o';
      acStack_1010[1] = ' ';
      acStack_1010[2] = 'p';
      acStack_1010[3] = 'a';
      acStack_100c[0] = 's';
      acStack_100c[1] = 's';
      acStack_100c[2] = 'e';
      acStack_100c[3] = 'd';
      local_1008[0] = ' ';
      local_1008[1] = 't';
      local_1008[2] = 'o';
      local_1008[3] = ' ';
      acStack_1004[0] = 's';
      acStack_1004[1] = 'e';
      acStack_1004[2] = 't';
      acStack_1004[3] = '_';
      acStack_1000[0] = 't';
      acStack_1000[1] = 'a';
      acStack_1000[2] = 'r';
      acStack_1000[3] = 'g';
      uStack_ffc = 0x7465;
      local_ffa = 0;
      FUN_00038730(3,local_1018,0);
      in_d0 = 1.0;
    }
  }
  dVar1 = 1.7668201048317172e+72;
  if (DAT_0007b859 == '\0') {
    dVar1 = 2.695953529101131e+67;
  }
  dVar2 = (dVar1 / in_d0) * 1.5930919111324523e-58;
  local_1020 = FUN_0005cc48(SUB84(dVar2,0),(int)((ulonglong)dVar2 >> 0x20));
  dVar2 = (double)FUN_0005cb50();
  dVar1 = dVar1 / in_d0 - dVar2 * 6.277101735386681e+57;
  dVar2 = dVar1 * 2.938735877055719e-39;
  local_1028 = FUN_0005cc48(SUB84(dVar2,0),(int)((ulonglong)dVar2 >> 0x20));
  dVar2 = (double)FUN_0005cb50();
  dVar1 = dVar1 - dVar2 * 3.402823669209385e+38;
  dVar2 = dVar1 * 5.421010862427522e-20;
  local_1030 = FUN_0005cc48(SUB84(dVar2,0),(int)((ulonglong)dVar2 >> 0x20));
  dVar2 = (double)FUN_0005cb50();
  dVar1 = dVar1 - dVar2 * 1.8446744073709552e+19;
  local_1038 = FUN_0005cc48(SUB84(dVar1,0),(int)((ulonglong)dVar1 >> 0x20));
  if (DAT_00078e09 != '\0') {
    param3 = (char *)FUN_00029730(&local_1038,0x20);
    if ((DAT_00078e09 != '\0') &&
       (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
      snprintf(local_1018,0x1000,"Generated target %s",param3);
      FUN_00038730(7,local_1018,0);
    }
    free(param3);
  }
  *param_1 = local_1038;
  param_1[1] = local_1030;
  param_1[2] = local_1028;
  param_1[3] = local_1020;
  return;
}

