//==================== FUN_00046cbc @ 00046cbc ====================

void FUN_00046cbc(void)

{
  FILE *param0;
  int iVar1;
  char *pcVar2;
  int iVar3;
  char *pcVar4;
  int *piVar5;
  int iVar6;
  int iVar7;
  int iStack_1050;
  char local_104c [36];
  undefined4 local_1028;
  undefined4 uStack_1024;
  undefined4 uStack_1020;
  undefined4 uStack_101c;
  undefined4 local_1018;
  undefined4 uStack_1014;
  undefined4 uStack_1010;
  undefined4 uStack_100c;
  undefined4 local_1008;
  undefined4 uStack_1004;
  undefined4 uStack_1000;
  undefined2 local_ffc;
  
  local_104c[0] = '\0';
  local_104c[1] = '\0';
  local_104c[2] = '\0';
  local_104c[3] = '\0';
  local_104c[4] = '\0';
  local_104c[5] = '\0';
  local_104c[6] = '\0';
  local_104c[7] = '\0';
  local_104c[8] = '\0';
  local_104c[9] = '\0';
  local_104c[10] = '\0';
  local_104c[0xb] = '\0';
  FUN_00040454();
  DAT_0007c130 = FUN_00046a48(local_104c);
  if (DAT_0007c130 < 1) {
    iVar1 = access("/config/chips_freq.config",0);
    if (iVar1 != 0) {
      if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
        pcVar2 = "2 Goto scan mode and find the best frequency\n";
LAB_00046ef0:
        local_1028 = *(undefined4 *)pcVar2;
        uStack_1024 = *(undefined4 *)(pcVar2 + 4);
        uStack_1020 = *(undefined4 *)(pcVar2 + 8);
        uStack_101c = *(undefined4 *)(pcVar2 + 0xc);
        local_1018 = *(undefined4 *)(pcVar2 + 0x10);
        uStack_1014 = *(undefined4 *)(pcVar2 + 0x14);
        uStack_1010 = *(undefined4 *)(pcVar2 + 0x18);
        uStack_100c = *(undefined4 *)(pcVar2 + 0x1c);
        local_1008 = *(undefined4 *)(pcVar2 + 0x20);
        uStack_1004 = *(undefined4 *)(pcVar2 + 0x24);
        uStack_1000 = *(undefined4 *)(pcVar2 + 0x28);
        local_ffc = (undefined2)*(undefined4 *)(pcVar2 + 0x2c);
        FUN_00038730(5,&local_1028,0);
      }
LAB_00046f20:
      FUN_00046880();
      return;
    }
    param0 = fopen("/config/chips_freq.config","r");
    if (param0 == (FILE *)0x0) {
      if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 5))
      goto LAB_00046d8c;
      snprintf((char *)&local_1028,0x1000,"open %s failed\n","/config/chips_freq.config");
      FUN_00038730(5,&local_1028,0);
    }
    else {
      pcVar2 = &DAT_0009d4bb;
      iVar1 = 0;
      do {
        pcVar2 = pcVar2 + 1;
        if (*pcVar2 == '\x01') {
          iVar3 = 0;
          piVar5 = &DAT_0007c224 + iVar1 * 3;
          do {
            iVar3 = iVar3 + 1;
            iStack_1050 = 0;
            fscanf(param0,"%d",&iStack_1050);
            iVar6 = iStack_1050;
            if (0x32 < iStack_1050 - 0x307U) {
              iVar6 = 800;
            }
            *piVar5 = iVar6;
            piVar5 = piVar5 + 1;
          } while (iVar3 != 3);
        }
        iVar1 = iVar1 + 1;
      } while (iVar1 != 4);
      fclose(param0);
    }
    if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 5)) goto LAB_00046d8c;
    pcVar2 = "2 Goto turbo mode\n";
  }
  else {
    pcVar4 = &DAT_0009d4bb;
    iVar1 = 0;
    pcVar2 = pcVar4;
    do {
      pcVar2 = pcVar2 + 1;
      if ((*pcVar2 == '\x01') && (local_104c[iVar1] != 'N')) {
        if ((DAT_0007ad48 == '\0') && ((DAT_00078e08 == '\0' && (DAT_000766c4 < 5))))
        goto LAB_00046f20;
        pcVar2 = "1 Goto scan mode and find the best frequency\n";
        goto LAB_00046ef0;
      }
      iVar1 = iVar1 + 9;
    } while (iVar1 != 0x24);
    pcVar2 = local_104c;
    iVar1 = 0;
    do {
      pcVar4 = pcVar4 + 1;
      if (*pcVar4 == '\x01') {
        piVar5 = &DAT_0007c224 + iVar1 * 3;
        iVar3 = 0;
        do {
          iVar7 = iVar3 + 1;
          iVar6 = (&DAT_00069204)[(uint)(byte)pcVar2[iVar7] * 4];
          if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
            snprintf((char *)&local_1028,0x1000,"pic read chain%d asic%d freq %d\n",iVar1,iVar3,
                     iVar6);
            FUN_00038730(5,&local_1028,0);
          }
          if (iVar6 - 0x307U < 0x33) {
            *piVar5 = iVar6;
          }
          else {
            *piVar5 = 800;
          }
          piVar5 = piVar5 + 1;
          iVar3 = iVar7;
        } while (iVar7 != 3);
      }
      iVar1 = iVar1 + 1;
      pcVar2 = pcVar2 + 9;
    } while (iVar1 != 4);
    if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 5)) goto LAB_00046d8c;
    pcVar2 = "1 Goto turbo mode\n";
  }
  local_1028 = *(undefined4 *)pcVar2;
  uStack_1024 = *(undefined4 *)(pcVar2 + 4);
  uStack_1020 = *(undefined4 *)(pcVar2 + 8);
  uStack_101c = *(undefined4 *)(pcVar2 + 0xc);
  local_1018._0_3_ = (undefined3)*(undefined4 *)(pcVar2 + 0x10);
  FUN_00038730(5,&local_1028,0);
LAB_00046d8c:
  FUN_00046964();
  return;
}

