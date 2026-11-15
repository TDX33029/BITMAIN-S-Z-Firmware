//==================== FUN_04004714 @ 04004714 ====================

void FUN_04004714(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  uint uVar1;
  uint local_c;
  
  uVar1 = FUN_0400461c(0xffffffff,param_1,param_2);
  uVar1 = ~uVar1;
  local_c = uVar1 << 0x18 | (uVar1 >> 8 & 0xff) << 0x10 | (uVar1 >> 0x10 & 0xff) << 8 |
            uVar1 >> 0x18;
  FUN_04005ea4(param_3,&local_c,4);
  return;
}

