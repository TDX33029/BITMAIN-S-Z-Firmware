//==================== FUN_000160b4 @ 000160b4 ====================

void FUN_000160b4(void)

{
  int *piVar1;
  int iVar2;
  int *piVar4;
  int *piVar3;
  
  DAT_00075dc4 = 0;
  DAT_00075dc8 = 0;
  DAT_00076f18 = 0;
  DAT_00076f1c = 0;
  FUN_00011580(0,0,&DAT_00075dc4,8,0);
  if (0 < DAT_0007b198) {
    piVar1 = DAT_0007b438 + DAT_0007b198;
    piVar3 = DAT_0007b438;
    do {
      piVar4 = piVar3 + 1;
      iVar2 = *piVar3;
      *(undefined4 *)(iVar2 + 400) = 0;
      *(undefined4 *)(iVar2 + 0x194) = 0;
      piVar3 = piVar4;
    } while (piVar4 != piVar1);
  }
  return;
}

