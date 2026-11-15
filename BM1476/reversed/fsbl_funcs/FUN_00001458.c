//==================== FUN_00001458 @ 00001458 ====================

undefined4 FUN_00001458(uint param_1)

{
  longlong lVar1;
  uint uVar2;
  bool bVar3;
  uint local_18;
  uint uStack_14;
  
  func_0xffffe2d4(&local_18);
  lVar1 = (ulonglong)param_1 * 0x13de435f + CONCAT44(uStack_14,local_18);
  uVar2 = (uint)((ulonglong)lVar1 >> 0x20);
  do {
    func_0xffffe2d4(&local_18);
    bVar3 = uVar2 <= uStack_14;
    if (uStack_14 == uVar2) {
      bVar3 = (uint)lVar1 <= local_18;
    }
  } while (!bVar3);
  return 0;
}

