//==================== FUN_00002bbc @ 00002bbc ====================

undefined4 FUN_00002bbc(undefined4 *param_1,undefined4 *param_2)

{
  uint uVar1;
  char *pcVar2;
  
  if ((((param_1 != (undefined4 *)0x0) && (pcVar2 = (char *)*param_1, pcVar2 != (char *)0x0)) &&
      (*pcVar2 != '\0')) && (*(short *)(param_1 + 1) == *(short *)(pcVar2 + 6))) {
    uVar1 = FUN_000064b8(pcVar2[1]);
    if ((uVar1 & 1) == 0) {
      *param_2 = *param_1;
      return 0;
    }
  }
  *param_2 = 0;
  return 9;
}

