//==================== FUN_00010ec0 @ 00010ec0 ====================

void FUN_00010ec0(int param_1,undefined4 param_2,int param_3)

{
  undefined4 *puVar1;
  void *pvVar2;
  undefined4 uVar3;
  
  puVar1 = malloc(8);
  if (puVar1 == (undefined4 *)0x0) {
                    /* WARNING: Subroutine does not return */
    exit(0);
  }
  pvVar2 = malloc(0x100f);
  strcpy((char *)((int)pvVar2 + 3),(char *)(param_1 + 3));
  strcpy((char *)((int)pvVar2 + 0x1008),(char *)(param_1 + 0x1008));
  uVar3 = *(undefined4 *)(param_3 + 4);
  *puVar1 = pvVar2;
  puVar1[1] = uVar3;
  DAT_0001d440 = DAT_0001d440 + 1;
  *(undefined4 **)(param_3 + 4) = puVar1;
  if (DAT_0001d440 < 0x15) {
    return;
  }
  puts("L_cmd is full!");
  return;
}

