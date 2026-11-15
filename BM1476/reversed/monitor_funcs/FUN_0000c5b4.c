//==================== FUN_0000c5b4 @ 0000c5b4 ====================

void FUN_0000c5b4(undefined4 *param_1)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  
  puVar2 = (undefined4 *)FUN_0000c540();
  if (puVar2 == (undefined4 *)0x0) {
    return;
  }
  puVar1 = puVar2;
  puVar5 = param_1;
  do {
    puVar4 = puVar5;
    puVar3 = puVar1;
    uVar7 = puVar4[1];
    uVar6 = puVar4[2];
    uVar8 = puVar4[3];
    puVar5 = puVar4 + 4;
    *puVar3 = *puVar4;
    puVar3[1] = uVar7;
    puVar3[2] = uVar6;
    puVar3[3] = uVar8;
    puVar1 = puVar3 + 4;
  } while (puVar5 != param_1 + 8);
  uVar6 = puVar4[5];
  puVar3[4] = *puVar5;
  puVar3[5] = uVar6;
  puVar2[8] = 0;
  puVar2[1] = 0;
  puVar2[3] = puVar2[3] | 0x100;
  *puVar2 = 0;
  return;
}

