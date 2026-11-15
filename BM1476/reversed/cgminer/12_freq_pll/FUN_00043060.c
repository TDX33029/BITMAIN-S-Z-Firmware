//==================== FUN_00043060 @ 00043060 ====================

void FUN_00043060(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 extraout_r1;
  undefined4 extraout_r2;
  int iVar1;
  char *pcVar2;
  
  iVar1 = 0;
  pcVar2 = &DAT_0009d4bb;
  do {
    pcVar2 = pcVar2 + 1;
    if (*pcVar2 == '\x01') {
      FUN_00042f7c(iVar1);
      param_2 = extraout_r1;
      param_3 = extraout_r2;
    }
    iVar1 = iVar1 + 1;
  } while (iVar1 != 4);
  FUN_0002a92c(10,param_2,param_3,param_4);
  return;
}

