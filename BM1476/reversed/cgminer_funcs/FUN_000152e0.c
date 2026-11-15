//==================== FUN_000152e0 @ 000152e0 ====================

void FUN_000152e0(void)

{
  int iVar1;
  undefined4 uVar2;
  int extraout_r1;
  uint uVar3;
  int *piVar5;
  int *piVar4;
  uint param3;
  int iVar6;
  int iVar7;
  int iVar8;
  int *piVar9;
  int *piVar10;
  char acStack_1020 [4096];
  
  piVar10 = DAT_0007b438;
  iVar1 = DAT_0007b198;
  if (DAT_0007b198 < 1) {
    param3 = 1;
  }
  else {
    param3 = 0xffffffff;
    piVar9 = DAT_0007b438 + DAT_0007b198;
    piVar4 = DAT_0007b438;
    do {
      piVar5 = piVar4 + 1;
      uVar3 = *(uint *)(*piVar4 + 0x38);
      if ((uVar3 != 0) && (uVar3 <= param3)) {
        param3 = uVar3;
      }
      piVar4 = piVar5;
    } while (piVar5 != piVar9);
    piVar4 = DAT_0007b438;
    if (param3 == 0xffffffff) {
      param3 = 1;
    }
    else {
      do {
        while( true ) {
          piVar5 = piVar4 + 1;
          iVar6 = *(int *)(*piVar4 + 0x38);
          piVar4 = piVar5;
          if (iVar6 != 0) break;
          if (piVar5 == piVar9) goto LAB_00015360;
        }
        while (FUN_0005c6f0(iVar6,param3), extraout_r1 != 0) {
          param3 = param3 - 1;
        }
      } while (piVar5 != piVar9);
    }
LAB_00015360:
    iVar6 = DAT_000755f0;
    piVar10 = piVar10 + -1;
    iVar8 = 0;
    do {
      piVar10 = piVar10 + 1;
      iVar7 = *piVar10;
      iVar8 = iVar8 + 1;
      uVar2 = FUN_0005c648(*(int *)(iVar7 + 0x40) * iVar6,param3);
      *(undefined4 *)(iVar7 + 0x40) = uVar2;
      uVar2 = FUN_0005c648(*(undefined4 *)(iVar7 + 0x38),param3);
      *(undefined4 *)(iVar7 + 0x3c) = uVar2;
    } while (iVar8 < iVar1);
  }
  DAT_000755f0 = param3;
  if ((DAT_00078e09 != '\0') &&
     (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
    snprintf(acStack_1020,0x1000,"Global quota greatest common denominator set to %lu",param3);
    FUN_00038730(7,acStack_1020,0);
  }
  return;
}

