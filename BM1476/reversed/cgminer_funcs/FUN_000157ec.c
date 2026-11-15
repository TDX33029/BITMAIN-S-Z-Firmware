//==================== FUN_000157ec @ 000157ec ====================

undefined4 FUN_000157ec(int param_1)

{
  undefined4 uVar1;
  undefined4 extraout_s1;
  double dVar2;
  undefined1 auStack_10 [8];
  
  if (*(int *)(param_1 + 0x170) == 0) {
    dVar2 = DAT_000755f8;
  }
  else {
    FUN_0002a84c(auStack_10);
    uVar1 = FUN_0002aa24(auStack_10,param_1 + 0x170);
    dVar2 = (double)CONCAT44(extraout_s1,uVar1);
  }
  if (dVar2 < 1.0) {
    dVar2 = 0.0;
  }
  return SUB84(dVar2,0);
}

