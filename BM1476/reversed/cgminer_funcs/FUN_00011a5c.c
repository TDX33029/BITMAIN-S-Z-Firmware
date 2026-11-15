//==================== FUN_00011a5c @ 00011a5c ====================

void FUN_00011a5c(int param_1)

{
  undefined4 uVar1;
  int iVar2;
  uint in_fpscr;
  uint uVar3;
  double in_d0;
  double dVar4;
  double dVar5;
  double dVar6;
  double dVar7;
  double dVar8;
  undefined8 uVar9;
  
  uVar3 = in_fpscr & 0xfffffff;
  iVar2 = *(int *)(param_1 + 0x6d4);
  if (in_d0 == 0.0) {
    dVar4 = 65536.0;
    if (DAT_0007b859 == '\0') {
      dVar4 = 2.695953529101131e+67;
    }
    dVar6 = (double)FUN_0005cb50(*(undefined4 *)(param_1 + 0xb8),*(undefined4 *)(param_1 + 0xbc));
    dVar7 = (double)FUN_0005cb50(*(undefined4 *)(param_1 + 0xb0),*(undefined4 *)(param_1 + 0xb4));
    dVar8 = (double)FUN_0005cb50(*(undefined4 *)(param_1 + 0xa8),*(undefined4 *)(param_1 + 0xac));
    dVar5 = (double)FUN_0005cb50(*(undefined4 *)(param_1 + 0xa0),*(undefined4 *)(param_1 + 0xa4));
    dVar5 = dVar6 * 6.277101735386681e+57 + dVar7 * 3.402823669209385e+38 +
            dVar8 * 1.8446744073709552e+19 + dVar5;
    uVar3 = uVar3 & 0xfffffff;
    if (dVar5 == 0.0) {
      dVar5 = 1.0;
    }
    *(double *)(param_1 + 0x748) = dVar4 / dVar5;
  }
  else {
    *(double *)(param_1 + 0x748) = in_d0;
  }
  uVar1 = FUN_00049824(param_1 + 0xa0);
  dVar5 = (double)VectorSignedToFloat(uVar1,(byte)(uVar3 >> 0x16) & 3);
  *(double *)(param_1 + 0x748) = dVar5;
  *(double *)(iVar2 + 0x1f8) = dVar5;
  dVar4 = round(dVar5);
  uVar9 = FUN_0005cc48(SUB84(dVar4,0),(int)((ulonglong)dVar4 >> 0x20));
  FUN_00011580((int)uVar9,(int)((ulonglong)uVar9 >> 0x20),*(int *)(param_1 + 0x6d4) + 0x30,8,0);
  dVar4 = *(double *)(iVar2 + 0x1e8);
  if (dVar5 == dVar4) {
    *(int *)(iVar2 + 0x200) = *(int *)(iVar2 + 0x200) + 1;
LAB_00011ae4:
    dVar4 = *(double *)(iVar2 + 0x1f0);
  }
  else {
    if (dVar4 <= dVar5) {
      if (dVar4 != 0.0) goto LAB_00011ae4;
      *(double *)(iVar2 + 0x1e8) = dVar5;
      *(undefined4 *)(iVar2 + 0x200) = 1;
    }
    else {
      *(double *)(iVar2 + 0x1e8) = dVar5;
      *(undefined4 *)(iVar2 + 0x200) = 1;
    }
    dVar4 = *(double *)(iVar2 + 0x1f0);
  }
  if (dVar5 == dVar4) {
    *(int *)(iVar2 + 0x204) = *(int *)(iVar2 + 0x204) + 1;
  }
  else if (dVar4 < dVar5) {
    *(double *)(iVar2 + 0x1f0) = dVar5;
    *(undefined4 *)(iVar2 + 0x204) = 1;
  }
  return;
}

