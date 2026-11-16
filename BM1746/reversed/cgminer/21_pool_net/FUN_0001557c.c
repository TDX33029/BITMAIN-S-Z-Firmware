//==================== FUN_0001557c @ 0001557c ====================

undefined4 FUN_0001557c(int param_1,char *param_2)

{
  int iVar1;
  undefined4 uVar2;
  
  FUN_0002aa68();
  iVar1 = FUN_0002ab54(param_2,param_1 + 0x270,param_1 + 0x260);
  if ((iVar1 == 0) || (iVar1 = strncasecmp(param_2,"stratum+tcp://",0xe), iVar1 != 0)) {
    return 0;
  }
  uVar2 = __strdup(param_2);
  *(undefined1 *)(param_1 + 0x298) = 1;
  *(undefined4 *)(param_1 + 600) = *(undefined4 *)(param_1 + 0x270);
  *(undefined4 *)(param_1 + 0xc4) = uVar2;
  return 1;
}

