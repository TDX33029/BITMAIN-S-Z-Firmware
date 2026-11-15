//==================== FUN_00059154 @ 00059154 ====================

int FUN_00059154(int param_1,uint param_2,int param_3,undefined4 param_4)

{
  int iVar1;
  int iVar2;
  
  FUN_0005852c(param_1,param_3,param_3,param_4,param_4);
  if (((param_2 & 4) == 0) && ((*(uint *)(param_1 + 0x34) & 0xffffffdf) != 0x5b)) {
    FUN_00058034(param_3,param_1,"\'[\' or \'{\' expected");
  }
  else {
    iVar1 = FUN_00058d00(param_1,param_2,param_3);
    if (iVar1 != 0) {
      if (((param_2 & 2) != 0) || (FUN_0005852c(param_1,param_3), *(int *)(param_1 + 0x34) == 0)) {
        if (param_3 == 0) {
          return iVar1;
        }
        *(undefined4 *)(param_3 + 8) = *(undefined4 *)(param_1 + 0x24);
        return iVar1;
      }
      FUN_00058034(param_3,param_1,"end of file expected");
      if ((*(int *)(iVar1 + 4) != -1) &&
         (iVar2 = *(int *)(iVar1 + 4) + -1, *(int *)(iVar1 + 4) = iVar2, iVar2 == 0)) {
        FUN_0005a3e4(iVar1);
        return 0;
      }
    }
  }
  return 0;
}

