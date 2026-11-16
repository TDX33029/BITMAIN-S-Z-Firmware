//==================== FUN_000302d0 @ 000302d0 ====================

void FUN_000302d0(undefined4 *param_1)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  
  puVar2 = (undefined4 *)0x760d8;
  puVar1 = param_1;
  do {
    puVar2 = puVar2 + 1;
    puVar1[0x22] = *puVar2;
    puVar1 = puVar1 + 1;
  } while (puVar2 != (undefined4 *)0x760f8);
  param_1[1] = 0;
  *param_1 = 0;
  return;
}

