//==================== FUN_00040e94 @ 00040e94 ====================

undefined4 FUN_00040e94(void)

{
  FILE *__stream;
  ulong uVar1;
  int iVar2;
  undefined4 uVar3;
  char *__format;
  undefined4 *puVar4;
  int param3;
  char acStack_23c8 [8];
  undefined4 local_23c0 [4];
  char local_23b0 [4];
  char acStack_23ac [4];
  char acStack_23a8 [4];
  char acStack_23a4 [4];
  char local_23a0 [4];
  char acStack_239c [4];
  char acStack_2398 [4];
  char acStack_2394 [4];
  undefined2 uStack_2390;
  undefined1 local_238e;
  undefined4 local_13b0;
  undefined4 local_13ac [1250];
  
  memset(&local_13b0,0,5000);
  local_23c0[1] = 0;
  acStack_23c8[0] = '\0';
  local_23c0[2] = 0;
  local_23c0[0] = 0;
  local_23c0[3] = 0;
  acStack_23c8[1] = 0;
  acStack_23c8[2] = 0;
  acStack_23c8[3] = 0;
  acStack_23c8[4] = 0;
  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
    local_23b0[0] = 'P';
    local_23b0[1] = 'I';
    local_23b0[2] = 'C';
    local_23b0[3] = '1';
    acStack_23ac[0] = '7';
    acStack_23ac[1] = '0';
    acStack_23ac[2] = '4';
    acStack_23ac[3] = '_';
    acStack_23a8[0] = 'u';
    acStack_23a8[1] = 'p';
    acStack_23a8[2] = 'd';
    acStack_23a8[3] = 'a';
    acStack_23a4[0] = 't';
    acStack_23a4[1] = 'e';
    acStack_23a4[2] = '_';
    acStack_23a4[3] = 'p';
    local_23a0[0] = 'i';
    local_23a0[1] = 'c';
    local_23a0[2] = '_';
    local_23a0[3] = 'a';
    acStack_239c[0] = 'p';
    acStack_239c[1] = 'p';
    acStack_239c[2] = '_';
    acStack_239c[3] = 'p';
    acStack_2398[0] = 'r';
    acStack_2398[1] = 'o';
    acStack_2398[2] = 'g';
    acStack_2398[3] = 'r';
    acStack_2394[0] = 'a';
    acStack_2394[1] = 'm';
    acStack_2394[2] = '_';
    acStack_2394[3] = 'n';
    uStack_2390 = 0x7765;
    local_238e = 0;
    FUN_00038730(5,local_23b0,0);
  }
  __stream = fopen("/sbin/pic.txt","r");
  if (__stream == (FILE *)0x0) {
    if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 3)) {
      return 0;
    }
    snprintf(local_23b0,0x1000,"%s: open pic16f1704_app_new.txt failed\n",
             "PIC1704_update_pic_app_program_new");
    uVar3 = FUN_00038730(3,local_23b0,0);
    return uVar3;
  }
  fseek(__stream,0,0);
  memset(&local_13b0,0,5000);
  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
    snprintf(local_23b0,0x1000,"%s: pic_flash_length = %d\n","PIC1704_update_pic_app_program_new",
             0x980);
    FUN_00038730(5,local_23b0,0);
  }
  puVar4 = &local_13b0;
  do {
    fgets(acStack_23c8,0x3ff,__stream);
    uVar1 = strtoul(acStack_23c8,(char **)0x0,0x10);
    *(char *)((int)puVar4 + 1) = (char)uVar1;
    *(char *)puVar4 = (char)(uVar1 >> 8);
    puVar4 = (undefined4 *)((int)puVar4 + 2);
  } while (puVar4 != local_13ac + 0x4bf);
  fclose(__stream);
  iVar2 = FUN_0003df78();
  if (iVar2 != 0) {
    iVar2 = FUN_00040dec();
    if (iVar2 == 0) {
      if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 3)) {
        return 0;
      }
      __format = "%s: erase flash error!\n\n";
      goto LAB_00041074;
    }
    iVar2 = FUN_0003d000(6,0);
    if (iVar2 == 0) {
      if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 3)) {
        return 0;
      }
      __format = "%s: set flash pointer error!\n\n";
      goto LAB_00041074;
    }
    iVar2 = 0;
    do {
      local_23c0[0] = local_13ac[iVar2 * 4 + -1];
      local_23c0[1] = local_13ac[iVar2 * 4];
      local_23c0[2] = local_13ac[iVar2 * 4 + 1];
      local_23c0[3] = local_13ac[iVar2 * 4 + 2];
      if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
        snprintf(local_23b0,0x1000,"send pic program time: %d",iVar2);
        FUN_00038730(5,local_23b0,0);
      }
      param3 = 0;
      do {
        if ((DAT_00078e09 != '\0') &&
           (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
          snprintf(local_23b0,0x1000,"buf[%d] = 0x%02x",param3,
                   (uint)*(byte *)((int)local_23c0 + param3));
          FUN_00038730(7,local_23b0,0);
        }
        param3 = param3 + 1;
      } while (param3 != 0x10);
      iVar2 = iVar2 + 1;
      FUN_0003d1ac(local_23c0);
      FUN_0003dc28();
    } while (iVar2 != 0x130);
    iVar2 = FUN_0003df78();
    if (iVar2 != 0) {
      return 1;
    }
  }
  if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 3)) {
    return 0;
  }
  __format = "%s: reset pic error!\n\n";
LAB_00041074:
  snprintf(local_23b0,0x1000,__format,"PIC1704_update_pic_app_program_new");
  FUN_00038730(3,local_23b0,0);
  return 0;
}

