//==================== FUN_00013bec @ 00013bec ====================

void FUN_00013bec(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  undefined4 *puVar2;
  
  FUN_000106e8();
  iVar1 = 0;
  puVar2 = &DAT_00024f08;
  do {
    iVar1 = iVar1 + 1;
    (*(code *)*puVar2)(param_1,param_2,param_3,(code *)*puVar2,param_4);
    puVar2 = puVar2 + 1;
  } while (iVar1 != 1);
  return;
}

