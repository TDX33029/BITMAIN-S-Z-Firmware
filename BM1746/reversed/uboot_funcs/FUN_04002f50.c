//==================== FUN_04002f50 @ 04002f50 ====================

undefined4 FUN_04002f50(void)

{
  uint *puVar1;
  undefined4 uVar2;
  uint *puVar4;
  uint *puVar3;
  uint uVar5;
  int in_r3;
  uint *puVar6;
  int in_stack_00000000;
  int local_14;
  
  puVar1 = (uint *)func_0x03ffae84();
  if (puVar1 == (uint *)0x0) {
    uVar2 = 0xffffffff;
  }
  else if (local_14 < in_stack_00000000 * 4) {
    uVar2 = 0xfffffff4;
  }
  else {
    if (0 < in_stack_00000000) {
      puVar6 = (uint *)(in_r3 + -4);
      puVar3 = puVar1;
      do {
        puVar4 = puVar3 + 1;
        uVar5 = *puVar3;
        puVar6 = puVar6 + 1;
        *puVar6 = uVar5 << 0x18 | (uVar5 >> 8 & 0xff) << 0x10 | (uVar5 >> 0x10 & 0xff) << 8 |
                  uVar5 >> 0x18;
        puVar3 = puVar4;
      } while (puVar4 != puVar1 + in_stack_00000000);
    }
    uVar2 = 0;
  }
  return uVar2;
}

