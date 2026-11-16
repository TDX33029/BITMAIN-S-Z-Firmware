//==================== FUN_0000f410 @ 0000f410 ====================

undefined4 FUN_0000f410(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uStack_8;
  undefined4 uStack_4;
  
  uStack_8 = param_3;
  uStack_4 = param_4;
  uVar1 = FUN_0000feb0(&uStack_8,&DAT_000258c4,param_3,param_4,param_4);
  uVar2 = FUN_00010004(PTR_s__config_cgminer_conf_0001b36c,&DAT_00026d6c);
  uVar1 = FUN_00010474(uVar1,uVar2);
  FUN_00010210(uVar1,PTR_s__config_cgminer_conf_0001b36c);
  return 1;
}

