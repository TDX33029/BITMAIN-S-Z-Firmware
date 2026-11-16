//==================== FUN_0000c0dc @ 0000c0dc ====================

undefined4 FUN_0000c0dc(int param_1)

{
  uint uVar1;
  undefined4 uVar2;
  undefined1 *puVar3;
  undefined1 *puVar4;
  int iVar5;
  undefined1 auStack_54 [4];
  undefined1 auStack_50 [7];
  undefined1 uStack_49;
  undefined1 local_48 [64];
  
  FUN_000027e0(local_48,0,0x40);
  uVar1 = FUN_00002900(param_1);
  if (uVar1 < 0x41) {
    if (uVar1 == 0x40) {
      puVar3 = (undefined1 *)(param_1 + -1);
      puVar4 = &uStack_49;
      do {
        puVar3 = puVar3 + 1;
        puVar4 = puVar4 + 1;
        *puVar4 = *puVar3;
      } while (puVar4 != local_48 + 0x40);
    }
    else {
      puVar3 = local_48 + 0x40 + -uVar1;
      puVar4 = (undefined1 *)(param_1 + -1);
      do {
        puVar4 = puVar4 + 1;
        puVar3 = puVar3 + 1;
        *puVar3 = *puVar4;
      } while (puVar4 != (undefined1 *)(param_1 + uVar1));
    }
    iVar5 = 8;
    do {
      FUN_0000ba90(auStack_50,auStack_54);
      iVar5 = iVar5 + -1;
    } while (iVar5 != 0);
    uVar2 = 0;
  }
  else {
    uVar2 = 0x20;
  }
  return uVar2;
}

