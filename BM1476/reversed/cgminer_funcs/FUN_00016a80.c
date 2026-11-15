//==================== FUN_00016a80 @ 00016a80 ====================

int FUN_00016a80(int param_1,void *param_2,size_t param_3,void *param_4,int param_5,size_t param_6)

{
  int iVar1;
  int iVar2;
  
  if (param_1 != 0) {
    iVar1 = *(int *)(param_1 + 0x730);
    while( true ) {
      iVar2 = iVar1;
      iVar1 = memcmp((void *)(param_1 + 0x80),param_2,param_3);
      if ((iVar1 == 0) && (iVar1 = memcmp((void *)(param_1 + param_5),param_4,param_6), iVar1 == 0))
      break;
      if (iVar2 == 0) {
        return 0;
      }
      iVar1 = *(int *)(iVar2 + 0x730);
      param_1 = iVar2;
    }
  }
  return param_1;
}

