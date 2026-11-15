//==================== FUN_000020ec @ 000020ec ====================

void FUN_000020ec(undefined4 param_1)

{
  int iVar1;
  undefined4 *puVar2;
  
  FUN_000029dc(param_1,0);
  if (*(code **)(DAT_00015b74 + 0x3c) != Reset) {
    (**(code **)(DAT_00015b74 + 0x3c))();
  }
  FUN_00002b40(param_1);
  iVar1 = 1;
  puVar2 = (undefined4 *)0x20010;
  do {
    iVar1 = iVar1 + -1;
    puVar2 = puVar2 + -1;
    (*(code *)*puVar2)();
  } while (iVar1 != 0);
  return;
}

