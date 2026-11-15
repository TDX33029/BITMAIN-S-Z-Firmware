//==================== FUN_00016494 @ 00016494 ====================

undefined4 FUN_00016494(undefined1 *param_1,undefined4 param_2,uint param_3,int param_4)

{
  size_t __n;
  undefined4 uVar1;
  bool bVar2;
  uint local_18;
  int iStack_14;
  
  if (param_4 == 0 && param_3 < 0xfd) {
    *param_1 = (char)param_3;
    uVar1 = 1;
    __n = 0;
  }
  else {
    bVar2 = param_4 == 0;
    if (param_4 == 0) {
      bVar2 = param_3 < 0x10000;
    }
    if (bVar2) {
      uVar1 = 3;
      *param_1 = 0xfd;
      __n = 2;
    }
    else if (param_4 == 0) {
      uVar1 = 5;
      __n = 4;
      *param_1 = 0xfe;
    }
    else {
      uVar1 = 9;
      __n = 8;
      *param_1 = 0xff;
    }
  }
  local_18 = param_3;
  iStack_14 = param_4;
  memcpy(param_1 + 1,&local_18,__n);
  return uVar1;
}

