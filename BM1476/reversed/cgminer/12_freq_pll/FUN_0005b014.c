//==================== FUN_0005b014 @ 0005b014 ====================

void FUN_0005b014(undefined4 *param_1,char *param_2)

{
  size_t sVar1;
  
  if (param_1 != (undefined4 *)0x0) {
    *(undefined1 *)(param_1 + 0x17) = 0;
    param_1[2] = 0;
    *param_1 = 0xffffffff;
    param_1[1] = 0xffffffff;
    if (param_2 != (char *)0x0) {
      if (param_1 == (undefined4 *)0x0 || param_2 == (char *)0x0) {
        return;
      }
      sVar1 = strlen(param_2);
      if (sVar1 < 0x50) {
        memcpy(param_1 + 3,param_2,sVar1 + 1);
        return;
      }
      builtin_strncpy((char *)(param_1 + 3),"...",4);
      strcpy((char *)((int)param_1 + 0xf),param_2 + (sVar1 - 0x4c));
      return;
    }
    *(undefined1 *)(param_1 + 3) = 0;
  }
  return;
}

