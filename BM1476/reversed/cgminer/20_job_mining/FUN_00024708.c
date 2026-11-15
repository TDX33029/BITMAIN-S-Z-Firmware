//==================== FUN_00024708 @ 00024708 ====================

int FUN_00024708(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar2 = *(int *)(param_1 + 0x164);
  iVar3 = iVar2;
  if (iVar2 != 0) {
    iVar1 = FUN_00022b44(iVar2,0,param_3,param_4,param_4);
    if (iVar1 == 0) {
      FUN_00016624(param_1,iVar2);
    }
    else {
      iVar3 = 0;
      FUN_000159b4(iVar2);
      FUN_00019840();
    }
    *(undefined4 *)(param_1 + 0x164) = 0;
  }
  return iVar3;
}

