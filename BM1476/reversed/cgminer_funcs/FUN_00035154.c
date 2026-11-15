//==================== FUN_00035154 @ 00035154 ====================

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_00035154(undefined4 param_1,int param_2,undefined4 param_3,int param_4,int param_5,
                undefined1 param_6)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int local_2c;
  double local_28;
  
  local_28 = _DAT_0007acc0 / DAT_000755f8;
  local_2c = param_2;
  uVar1 = FUN_00032ed0(0,"STATS",&local_2c,0);
  uVar1 = FUN_00032e1c(uVar1,&DAT_000670e8,param_3,0);
  uVar1 = FUN_00032fcc(uVar1,"Elapsed",&DAT_000755f8,0);
  uVar1 = FUN_00032f18(uVar1,"Calls",param_4,0);
  uVar1 = FUN_00033014(uVar1,&DAT_0006725c,param_4 + 4,0);
  uVar1 = FUN_00033014(uVar1,&DAT_00067264,param_4 + 0xc,0);
  uVar1 = FUN_00033014(uVar1,&DAT_00067268,param_4 + 0x14,0);
  uVar1 = FUN_00032e1c(uVar1,"GHS 5s",&DAT_00078b84,0);
  iVar2 = FUN_00034eb0(uVar1,"GHS av",&local_28,0);
  if (param_5 != 0) {
    if (iVar2 == 0) {
      iVar2 = param_5;
    }
    else {
      iVar3 = *(int *)(param_5 + 0x10);
      *(undefined4 *)(param_5 + 0x10) = *(undefined4 *)(iVar2 + 0x10);
      *(int *)(*(int *)(iVar2 + 0x10) + 0x14) = param_5;
      *(int *)(iVar3 + 0x14) = iVar2;
      *(int *)(iVar2 + 0x10) = iVar3;
    }
  }
  FUN_00032534(param_1,iVar2,param_6);
  return local_2c + 1;
}

