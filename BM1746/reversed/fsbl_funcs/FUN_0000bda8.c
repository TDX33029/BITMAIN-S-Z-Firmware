//==================== FUN_0000bda8 @ 0000bda8 ====================

undefined4 FUN_0000bda8(int param_1,int param_2,uint param_3)

{
  undefined4 uVar1;
  int iVar2;
  uint extraout_r2;
  int iVar3;
  uint uVar4;
  byte *pbVar5;
  byte local_12;
  char local_11;
  
  if ((param_3 & 1) == 0) {
    pbVar5 = (byte *)(param_2 + (param_3 >> 3));
    uVar4 = 0;
    do {
      iVar2 = FUN_0000ba3c(*(undefined1 *)(param_1 + uVar4),&local_11);
      iVar3 = param_1 + uVar4;
      uVar4 = uVar4 + 2;
      if ((iVar2 != 0) || (iVar2 = FUN_0000ba3c(*(undefined1 *)(iVar3 + 1),&local_12), iVar2 != 0))
      {
        return 0x11;
      }
      pbVar5 = pbVar5 + -1;
      *pbVar5 = local_12 | local_11 << 4;
      uVar1 = 0;
    } while (uVar4 < extraout_r2);
  }
  else {
    uVar1 = 0x10;
  }
  return uVar1;
}

