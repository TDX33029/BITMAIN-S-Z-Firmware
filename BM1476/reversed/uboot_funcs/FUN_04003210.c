//==================== FUN_04003210 @ 04003210 ====================

undefined4
FUN_04003210(undefined4 *param_1,undefined4 *param_2,uint param_3,int param_4,int param_5)

{
  int iVar1;
  undefined4 *puVar2;
  int iVar3;
  undefined1 auStack_234 [528];
  undefined4 local_24;
  
  if (0 < param_4) {
    iVar3 = 0;
    puVar2 = (undefined4 *)(param_5 + -4);
    do {
      while( true ) {
        puVar2 = puVar2 + 1;
        iVar1 = FUN_04005718(auStack_234,*puVar2);
        if (iVar1 == 0) {
          FUN_04007548(DAT_040032fc,local_24);
          return 0;
        }
        if (((param_3 & 0x10) != 0) &&
           (iVar1 = FUN_04003148(param_3,*param_1,*puVar2,auStack_234), iVar1 != 0)) {
          return 1;
        }
        if ((param_3 & 0x20) != 0) break;
        iVar3 = iVar3 + 1;
        if (iVar3 == param_4) {
          return 0;
        }
      }
      iVar1 = FUN_04003148(param_3,*param_2,*puVar2,auStack_234);
      if (iVar1 != 0) {
        return 1;
      }
      iVar3 = iVar3 + 1;
    } while (iVar3 != param_4);
  }
  return 0;
}

