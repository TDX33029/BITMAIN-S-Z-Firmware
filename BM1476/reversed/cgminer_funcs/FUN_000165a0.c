//==================== FUN_000165a0 @ 000165a0 ====================

bool FUN_000165a0(int param_1,undefined4 param_2)

{
  uint uVar1;
  bool bVar2;
  double in_d0;
  double dVar3;
  undefined8 uVar4;
  
  *(undefined4 *)(param_1 + 0x4c) = param_2;
  if (DAT_0007b859 == '\0') {
    FUN_00011f60();
  }
  else {
    FUN_00014f30();
  }
  dVar3 = 281470681743360.0;
  if (DAT_0007b859 == '\0') {
    dVar3 = 4294901760.0;
  }
  uVar4 = FUN_0005cc48(SUB84(dVar3 / in_d0,0),(int)((ulonglong)(dVar3 / in_d0) >> 0x20));
  uVar1 = (uint)((ulonglong)uVar4 >> 0x20);
  bVar2 = *(uint *)(param_1 + 0xdc) <= uVar1;
  if (uVar1 == *(uint *)(param_1 + 0xdc)) {
    bVar2 = *(uint *)(param_1 + 0xd8) <= (uint)uVar4;
  }
  return bVar2;
}

