//==================== FUN_0004ea38 @ 0004ea38 ====================

uint FUN_0004ea38(int param_1,int param_2,int param_3,size_t param_4)

{
  uint uVar1;
  
  uVar1 = memcmp((void *)(param_1 + param_3),(void *)(param_2 + param_3),param_4);
  return uVar1 >> 0x1f;
}

