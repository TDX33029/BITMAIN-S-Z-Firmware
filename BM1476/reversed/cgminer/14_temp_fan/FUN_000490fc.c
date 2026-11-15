//==================== FUN_000490fc @ 000490fc ====================

void FUN_000490fc(int param_1,int param_2)

{
  int iVar1;
  int param6;
  char acStack_1018 [4100];
  
  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
    snprintf(acStack_1018,0x1000,"Chain %d %s",param_1,"set_temperature_offset_value_chain_sensor");
    FUN_00038730(4,acStack_1018,0);
  }
  iVar1 = param_2 + param_1 * 4;
  FUN_0004633c(param_1,param_2,0x4c,0x11,*(undefined1 *)((int)&DAT_00078e10 + iVar1 + 0xdd4));
  FUN_0002a92c(0x32);
  FUN_0004886c(param_1,param_2,0x4c,0x11);
  param6 = (int)*(char *)((int)&DAT_00078e10 + iVar1 + 0xd84);
  iVar1 = (int)*(char *)((int)&DAT_00078e10 + iVar1 + 0xdd4);
  if (param6 == iVar1) {
    if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
      snprintf(acStack_1018,0x1000,"%s: Chain%d Sensor%d temp offset : %02d, ",
               "set_temperature_offset_value_chain_sensor",param_1,param_2,param6);
      FUN_00038730(5,acStack_1018,0);
    }
  }
  else if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
    snprintf(acStack_1018,0x1000,
             "%s: Chain%d Sensor%d temp offset value set error. It should be %02d, but read back is %02d\n"
             ,"set_temperature_offset_value_chain_sensor",param_1,param_2,iVar1,param6);
    FUN_00038730(3,acStack_1018,0);
  }
  return;
}

