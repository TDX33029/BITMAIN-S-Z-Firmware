//==================== FUN_000158e4 @ 000158e4 ====================

undefined8 FUN_000158e4(int param_1)

{
  double dVar1;
  double dVar2;
  double dVar3;
  double dVar4;
  double dVar5;
  undefined8 uVar6;
  
  if (param_1 != 0) {
    dVar5 = 1.7668201048317172e+72;
    if (DAT_0007b859 == '\0') {
      dVar5 = 2.695953529101131e+67;
    }
    dVar1 = (double)FUN_0005cb50(*(undefined4 *)(param_1 + 0xd8),*(undefined4 *)(param_1 + 0xdc));
    dVar3 = (double)FUN_0005cb50(*(undefined4 *)(param_1 + 0xd0),*(undefined4 *)(param_1 + 0xd4));
    dVar4 = (double)FUN_0005cb50(*(undefined4 *)(param_1 + 200),*(undefined4 *)(param_1 + 0xcc));
    dVar2 = (double)FUN_0005cb50(*(undefined4 *)(param_1 + 0xc0),*(undefined4 *)(param_1 + 0xc4));
    dVar2 = dVar1 * 6.277101735386681e+57 + dVar3 * 3.402823669209385e+38 +
            dVar4 * 1.8446744073709552e+19 + dVar2;
    if (dVar2 != 0.0) {
      dVar5 = dVar5 / dVar2;
      uVar6 = FUN_0005cc48(SUB84(dVar5,0),(int)((ulonglong)dVar5 >> 0x20));
      return uVar6;
    }
  }
  return 0;
}

