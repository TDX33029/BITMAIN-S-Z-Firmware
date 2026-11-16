//==================== FUN_0000be74 @ 0000be74 ====================

void FUN_0000be74(byte *param_1,int param_2,int param_3)

{
  uint uVar1;
  undefined1 *puVar2;
  int iVar3;
  undefined1 *puVar4;
  uint uVar5;
  byte *pbVar6;
  
  if (param_3 == 0) {
    return;
  }
  puVar2 = (undefined1 *)(param_2 + -1);
  puVar4 = puVar2 + param_3;
  do {
    puVar2 = puVar2 + 1;
    *puVar2 = 0;
  } while (puVar2 != puVar4);
  iVar3 = 0;
  do {
    uVar5 = 0;
    pbVar6 = (byte *)(param_2 + iVar3);
    do {
      param_3 = param_3 + -1;
      uVar1 = uVar5 & 0xff;
      uVar5 = uVar5 + 1;
      *pbVar6 = (byte)((int)(uint)*param_1 >> uVar1) & 1;
      iVar3 = (int)(pbVar6 + 1) - param_2;
      if (param_3 == 0) {
        return;
      }
      pbVar6 = pbVar6 + 1;
    } while (uVar5 != 8);
    param_1 = param_1 + 1;
  } while( true );
}

