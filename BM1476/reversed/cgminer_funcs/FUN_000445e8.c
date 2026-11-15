//==================== FUN_000445e8 @ 000445e8 ====================

void FUN_000445e8(int param_1,int param_2)

{
  char cVar1;
  int iVar2;
  char *pcVar3;
  int iStack_1028;
  undefined1 local_1024;
  char local_1020 [4100];
  
  cVar1 = DAT_0007ad48;
  if ((&DAT_0009d4bc)[param_1] == '\0') {
    return;
  }
  *(int *)(&DAT_0007c064 + param_1 * 4) = param_1;
  if (((cVar1 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
    snprintf(local_1020,0x1000,"%s chainid = %d","tty_init_chain",param_1);
    FUN_00038730(5,local_1020,0);
  }
  (&DAT_00079118)[param_1] = 1;
  iVar2 = FUN_0002f174(param_2 + param_1 * 0x40 + 0x37a,0,FUN_0004b0c0,&DAT_0007c064 + param_1 * 4);
  if (iVar2 == 0) {
    if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
      pcVar3 = "create rx read thread for chain %d ok";
LAB_000446e8:
      snprintf(local_1020,0x1000,pcVar3,param_1);
      FUN_00038730(3,local_1020,0);
    }
  }
  else if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
    pcVar3 = "create rx read thread for chain %d failed";
    goto LAB_000446e8;
  }
  FUN_0002a92c(0x32);
  local_1024 = (undefined1)param_1;
  iStack_1028 = param_2;
  iVar2 = FUN_0002f174(param_2 + param_1 * 0x40 + 0x27a,0,FUN_00042b54,&iStack_1028);
  FUN_0002a92c(200);
  if (iVar2 == 0) {
    if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 3)) goto LAB_000447fc;
    pcVar3 = "create tx read thread for chain %d ok";
LAB_0004478c:
    snprintf(local_1020,0x1000,pcVar3,param_1);
    FUN_00038730(3,local_1020,0);
    if ((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) goto LAB_000447fc;
  }
  else {
    if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
      pcVar3 = "create tx read thread for chain %d failed";
      goto LAB_0004478c;
    }
LAB_000447fc:
    if (DAT_000766c4 < 5) goto LAB_000447e4;
  }
  builtin_strncpy(local_1020,"open device over",0x11);
  FUN_00038730(5,local_1020,0);
LAB_000447e4:
  FUN_0002a92c(10);
  return;
}

