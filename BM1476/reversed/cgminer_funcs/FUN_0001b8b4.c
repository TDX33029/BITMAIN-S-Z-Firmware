//==================== FUN_0001b8b4 @ 0001b8b4 ====================

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0001b8b4(void)

{
  int iVar1;
  int *piVar3;
  int *piVar2;
  int iVar4;
  int *piVar5;
  int iVar6;
  undefined4 *puVar8;
  undefined4 *puVar7;
  
  FUN_0002a84c(&DAT_000770d0);
  FUN_0002a530(&DAT_000770d8,&DAT_000770d0);
  _DAT_0007bc98 = 0;
  DAT_0007bc9c = 0;
  _DAT_0007bb10 = 0;
  DAT_0007bb14 = 0;
  _DAT_0007b2f8 = 0;
  DAT_0007b2fc = 0;
  _DAT_0007bc88 = 0;
  DAT_0007bc8c = 0;
  _DAT_0007acc0 = 0;
  DAT_0007acc4 = 0;
  puVar7 = &DAT_000770e0;
  do {
    puVar8 = puVar7 + 2;
    *puVar7 = 0;
    puVar7[1] = 0;
    puVar7 = puVar8;
  } while (puVar8 != &DAT_00077140);
  DAT_00077144 = 1;
  DAT_0007b8a8 = 0;
  DAT_0007bb20 = 0;
  DAT_00075de8 = 0x3ff0000000000000;
  DAT_000755f8 = 0x3ff0000000000000;
  DAT_00077140 = 0;
  DAT_0007ae68 = 0;
  DAT_0007b648 = 0;
  DAT_0007b64c = 0;
  DAT_0007b8a4 = 0;
  DAT_0007b5e8 = 0;
  DAT_0007b4cc = 0;
  DAT_0007b670 = 0;
  DAT_0007b43c = 0;
  DAT_0007b428 = 0;
  DAT_0007b8b8 = 0;
  DAT_0007b420 = 0;
  DAT_0007b424 = 0;
  DAT_0007b6f8 = 0;
  DAT_0007b6fc = 0;
  DAT_0007bc80 = 0;
  DAT_0007bc84 = 0;
  DAT_0007b7c8 = 0;
  DAT_0007b7cc = 0;
  DAT_0007b100 = 0;
  DAT_0007b104 = 0;
  if (0 < DAT_0007b198) {
    piVar5 = DAT_0007b438 + DAT_0007b198;
    piVar2 = DAT_0007b438;
    do {
      piVar3 = piVar2 + 1;
      iVar4 = *piVar2;
      *(undefined4 *)(iVar4 + 0x68) = 0;
      *(undefined4 *)(iVar4 + 0x6c) = 0;
      *(undefined4 *)(iVar4 + 0x70) = 0;
      *(undefined4 *)(iVar4 + 0x74) = 0;
      *(undefined4 *)(iVar4 + 0x78) = 0;
      *(undefined4 *)(iVar4 + 0x7c) = 0;
      *(undefined4 *)(iVar4 + 0x188) = 0;
      *(undefined4 *)(iVar4 + 0x18c) = 0;
      *(undefined4 *)(iVar4 + 0x94) = 0;
      *(undefined4 *)(iVar4 + 8) = 0;
      *(undefined4 *)(iVar4 + 0xc) = 0;
      *(undefined4 *)(iVar4 + 0x10) = 0;
      *(undefined4 *)(iVar4 + 0x14) = 0;
      *(undefined4 *)(iVar4 + 0x98) = 0;
      *(undefined4 *)(iVar4 + 0x9c) = 0;
      *(undefined4 *)(iVar4 + 0xa0) = 0;
      *(undefined4 *)(iVar4 + 0xa4) = 0;
      *(undefined4 *)(iVar4 + 0x180) = 0;
      *(undefined4 *)(iVar4 + 0x28) = 0;
      *(undefined4 *)(iVar4 + 0x2c) = 0;
      piVar2 = piVar3;
    } while (piVar3 != piVar5);
  }
  FUN_000160b4();
  if (DAT_0007b720 < 1) {
    return;
  }
  iVar4 = 0;
  do {
    iVar6 = iVar4 + 1;
    iVar4 = FUN_0001b83c(iVar4);
    FUN_0002a530(iVar4 + 0x170,&DAT_000770d0);
    iVar1 = pthread_mutex_lock((pthread_mutex_t *)&DAT_0007b384);
    if (iVar1 != 0) {
      FUN_0000b414("zero_stats",0x16f2);
    }
    *(undefined4 *)(iVar4 + 0x50) = 0;
    *(undefined4 *)(iVar4 + 0x54) = 0;
    *(undefined4 *)(iVar4 + 0x24) = 0;
    *(undefined8 *)(iVar4 + 0xc0) = 0;
    *(undefined4 *)(iVar4 + 0x58) = 0;
    *(undefined4 *)(iVar4 + 0x5c) = 0;
    *(undefined4 *)(iVar4 + 0x28) = 0;
    *(undefined4 *)(iVar4 + 200) = 0;
    *(undefined4 *)(iVar4 + 0xcc) = 0;
    *(undefined4 *)(iVar4 + 0x2c) = 0;
    *(undefined4 *)(iVar4 + 0xd0) = 0;
    *(undefined4 *)(iVar4 + 0xd4) = 0;
    *(undefined4 *)(iVar4 + 0xdc) = 0;
    *(undefined4 *)(iVar4 + 0xe0) = 0;
    *(undefined4 *)(iVar4 + 0xe4) = 0;
    iVar1 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_0007b384);
    if (iVar1 != 0) {
      FUN_0000b5c4("zero_stats",0x16fd);
    }
    (*DAT_00075ed8)();
    (**(code **)(*(int *)(iVar4 + 4) + 0x60))(iVar4);
    iVar4 = iVar6;
  } while (iVar6 < DAT_0007b720);
  return;
}

