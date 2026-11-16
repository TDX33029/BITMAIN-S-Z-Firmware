//==================== FUN_0003049c @ 0003049c ====================

void FUN_0003049c(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  undefined4 local_b8;
  undefined4 local_b4;
  undefined4 uStack_34;
  undefined4 local_30 [8];
  
  puVar1 = (undefined4 *)0x760d8;
  puVar2 = &uStack_34;
  do {
    puVar1 = puVar1 + 1;
    puVar2 = puVar2 + 1;
    *puVar2 = *puVar1;
  } while (puVar1 != (undefined4 *)0x760f8);
  local_b8 = 0;
  local_b4 = 0;
  FUN_00030304(&local_b8,param_1,param_2);
  FUN_000303a4(&local_b8,param_3);
  return;
}

