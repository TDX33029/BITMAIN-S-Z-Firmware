//==================== FUN_0400644c @ 0400644c ====================

undefined1 *
FUN_0400644c(undefined1 *param_1,undefined1 *param_2,undefined1 *param_3,int param_4,
            undefined4 param_5,uint param_6)

{
  undefined1 uVar1;
  int iVar2;
  int iVar3;
  undefined1 *puVar4;
  undefined1 *puVar5;
  undefined1 *puVar6;
  
  puVar5 = DAT_0400651c;
  if (param_3 != (undefined1 *)0x0) {
    puVar5 = param_3;
  }
  iVar2 = FUN_04005bc4(puVar5,param_5);
  iVar3 = param_4 + -1;
  if ((param_6 & 0x10) == 0) {
    if (iVar2 < param_4) {
      puVar6 = param_1 + (param_4 - iVar2);
      puVar4 = param_1;
      do {
        param_1 = puVar4 + 1;
        if (puVar4 < param_2) {
          *puVar4 = 0x20;
        }
        puVar4 = param_1;
      } while (param_1 != puVar6);
      param_4 = iVar2 + -1;
      iVar3 = iVar2 + -2;
    }
    else {
      iVar3 = param_4 + -2;
      param_4 = param_4 + -1;
    }
  }
  if (0 < iVar2) {
    puVar6 = param_1 + iVar2;
    puVar4 = param_1;
    do {
      param_1 = puVar4 + 1;
      if (puVar4 < param_2) {
        uVar1 = *puVar5;
        puVar5 = puVar5 + 1;
        *puVar4 = uVar1;
      }
      puVar4 = param_1;
    } while (puVar6 != param_1);
  }
  puVar5 = param_1;
  if (iVar2 < param_4) {
    puVar4 = param_1;
    do {
      puVar5 = puVar4 + 1;
      if (puVar4 < param_2) {
        *puVar4 = 0x20;
      }
      puVar4 = puVar5;
    } while (iVar2 < (int)(param_1 + ((iVar3 + 1) - (int)puVar5)));
  }
  return puVar5;
}

