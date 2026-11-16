//==================== FUN_04003954 @ 04003954 ====================

undefined4 FUN_04003954(undefined4 param_1,int *param_2,undefined4 param_3)

{
  int iVar1;
  int iVar2;
  code *pcVar3;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 uStack_2c;
  undefined4 uStack_28;
  undefined4 uStack_24;
  
  local_30 = param_1;
  iVar1 = FUN_040034a8(param_1,uStack_2c,uStack_28,uStack_24,0,&local_34,param_2,0);
  if (iVar1 == 0) {
    *DAT_04003a64 = 3;
    return 0;
  }
  if (((code *)param_2[3] != (code *)0x0) &&
     (iVar2 = (*(code *)param_2[3])(local_34,0,1,param_3), iVar2 != 0)) {
    *DAT_04003a64 = 1;
    return 0;
  }
  pcVar3 = *(code **)(*param_2 + iVar1 * 0x14 + 0xc);
  if ((pcVar3 == (code *)0x0) || (iVar2 = (*pcVar3)(param_1,0,1,param_3), iVar2 == 0)) {
    FUN_040030a8(param_2,param_2 + 2,local_34,iVar1);
    return 1;
  }
  *DAT_04003a64 = 0x16;
  return 0;
}

