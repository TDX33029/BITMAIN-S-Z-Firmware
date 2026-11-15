//==================== FUN_04009008 @ 04009008 ====================

int FUN_04009008(int param_1,int param_2,undefined4 param_3)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  int *piVar5;
  int iVar6;
  uint local_20;
  int local_1c;
  undefined4 uStack_18;
  
  local_20 = 0;
  local_1c = param_2;
  uStack_18 = param_3;
  uVar1 = FUN_04005b98(DAT_040090f4);
  iVar2 = FUN_04005a90(param_1,DAT_040090f4,uVar1);
  if ((iVar2 == 0) && (uVar3 = FUN_04005b98(param_1), uVar1 < uVar3)) {
    uVar1 = param_1 + uVar1;
    iVar2 = FUN_04007718(uVar1,&local_20,10);
  }
  else {
    uVar1 = 0;
    iVar2 = -1;
  }
  func_0x03fd0cb8(0x13,&local_1c);
  piVar5 = *(int **)(local_1c + 8);
  do {
    iVar6 = (int)(piVar5 + -0xc);
    if (piVar5 == (int *)(local_1c + 8)) {
      return 0;
    }
    iVar4 = func_0x03fd031c(iVar6);
    if (iVar4 == 0) {
      iVar4 = FUN_04005a60(piVar5[-0xb],param_1);
      if (iVar4 == 0) {
        return iVar6;
      }
      if ((uVar1 < local_20) && (piVar5[8] == iVar2)) {
        return iVar6;
      }
    }
    piVar5 = (int *)*piVar5;
  } while( true );
}

