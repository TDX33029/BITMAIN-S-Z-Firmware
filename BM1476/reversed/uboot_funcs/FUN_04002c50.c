//==================== FUN_04002c50 @ 04002c50 ====================

uint FUN_04002c50(void)

{
  int iVar1;
  uint uVar2;
  int in_r3;
  uint uVar3;
  int iVar4;
  uint in_stack_00000000;
  uint in_stack_00000004;
  uint *in_stack_00000008;
  uint local_14;
  
  if (((in_stack_00000000 < 2) && (in_stack_00000004 < 2)) &&
     (iVar1 = func_0x03ffae84(), iVar1 != 0)) {
    in_r3 = in_r3 * (in_stack_00000004 + in_stack_00000000);
    iVar4 = iVar1 + in_r3 * 4;
    if (iVar4 + in_stack_00000000 * 4 + in_stack_00000004 * 4 <= iVar1 + (local_14 & 0xfffffffc)) {
      if (in_stack_00000000 == 0) {
        uVar2 = 0;
      }
      else {
        uVar2 = *(uint *)(iVar1 + in_r3 * 4);
        uVar2 = uVar2 << 0x18 | (uVar2 >> 8 & 0xff) << 0x10 | (uVar2 >> 0x10 & 0xff) << 8 |
                uVar2 >> 0x18;
      }
      if (in_stack_00000008 == (uint *)0x0) {
        return uVar2;
      }
      uVar3 = 0;
      if (in_stack_00000004 != 0) {
        uVar3 = *(uint *)(iVar4 + in_stack_00000000 * 4);
        uVar3 = uVar3 << 0x18 | (uVar3 >> 8 & 0xff) << 0x10 | (uVar3 >> 0x10 & 0xff) << 8 |
                uVar3 >> 0x18;
      }
      *in_stack_00000008 = uVar3;
      return uVar2;
    }
  }
  return 0xffffffff;
}

