//==================== FUN_0004ea58 @ 0004ea58 ====================

undefined4 FUN_0004ea58(int param_1,int param_2,int param_3,uint param_4)

{
  int iVar1;
  void *__s2;
  uint uVar2;
  uint uVar3;
  
  if (param_4 != 0) {
    uVar3 = 0;
    do {
      uVar2 = 0;
      do {
        __s2 = (void *)(param_2 + param_3 + uVar2);
        uVar2 = uVar2 + 4;
        iVar1 = memcmp((void *)(param_1 + param_3 + uVar3),__s2,4);
        if (iVar1 == 0) {
          return 0;
        }
      } while (uVar2 < param_4);
      uVar3 = uVar3 + 4;
    } while (uVar3 < param_4);
  }
  return 1;
}

