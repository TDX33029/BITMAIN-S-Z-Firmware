//==================== FUN_00014f30 @ 00014f30 ====================

void FUN_00014f30(int param_1)

{
  int iVar1;
  uint *puVar2;
  uint *puVar3;
  uint uVar4;
  uint auStack_20260 [19];
  uint uStack_20214;
  undefined4 uStack_20210;
  undefined1 auStack_2020c [131580];
  
  uStack_20210 = 0;
  memset(auStack_2020c,0,0x201fc);
  iVar1 = 0;
  do {
    uVar4 = *(uint *)(param_1 + iVar1);
    *(uint *)((int)auStack_20260 + iVar1) =
         uVar4 << 0x18 | (uVar4 >> 8 & 0xff) << 0x10 | (uVar4 >> 0x10 & 0xff) << 8 | uVar4 >> 0x18;
    iVar1 = iVar1 + 4;
  } while (iVar1 != 0x4c);
  uVar4 = *(uint *)(param_1 + 0x4c);
  uStack_20214 = uVar4 << 0x18 | (uVar4 >> 8 & 0xff) << 0x10 | (uVar4 >> 0x10 & 0xff) << 8 |
                 uVar4 >> 0x18;
  FUN_000122e8(auStack_20260,&uStack_20210,(uint *)(param_1 + 0xc0));
  puVar2 = (uint *)(param_1 + 0xc0);
  do {
    uVar4 = *puVar2;
    puVar3 = puVar2 + 1;
    *puVar2 = uVar4 << 0x18 | (uVar4 >> 8 & 0xff) << 0x10 | (uVar4 >> 0x10 & 0xff) << 8 |
              uVar4 >> 0x18;
    puVar2 = puVar3;
  } while (puVar3 != (uint *)(param_1 + 0xe0));
  return;
}

