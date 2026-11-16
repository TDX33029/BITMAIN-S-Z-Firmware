//==================== FUN_0004601c @ 0004601c ====================

void FUN_0004601c(uint param_1)

{
  uint param4;
  undefined1 *puVar1;
  undefined4 uStack_1024;
  undefined4 uStack_1020;
  undefined2 uStack_101c;
  undefined1 local_101a;
  char acStack_1018 [4100];
  
  uStack_1020 = 0;
  uStack_101c = 0;
  local_101a = 0;
  param4 = DAT_00076b5c | 0x4060;
  uStack_1024 = 0;
  DAT_00076b5c = param4;
  if ((&DAT_0009d4bc)[param_1] == '\x01') {
    if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
      snprintf(acStack_1018,0x1000,"%s: reg_value=0x%08x chainid %u",
               "enable_read_temperature_from_asic_chain",param4,param_1);
      FUN_00038730(5,acStack_1018,0);
    }
    puVar1 = (undefined1 *)((int)&DAT_0007912c + 3);
    do {
      puVar1 = puVar1 + 1;
      FUN_0003cdcc(&uStack_1024,0,*puVar1,0x1c,param4);
      FUN_0003c150(param_1 & 0xff,&uStack_1024,0xb);
      FUN_0002a92c(0x32);
    } while (puVar1 != &DAT_00079132);
  }
  return;
}

