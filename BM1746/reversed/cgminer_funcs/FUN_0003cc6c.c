//==================== FUN_0003cc6c @ 0003cc6c ====================

uint FUN_0003cc6c(byte *param_1,uint param_2)

{
  bool bVar1;
  uint uVar2;
  char cVar3;
  bool bVar4;
  bool bVar5;
  bool bVar6;
  bool bVar7;
  bool bVar8;
  bool bVar9;
  bool bVar10;
  uint uVar11;
  
  if (param_2 == 0) {
    bVar4 = true;
    bVar5 = true;
    bVar9 = true;
    bVar10 = true;
  }
  else {
    cVar3 = '\0';
    uVar11 = 0;
    uVar2 = 0x80;
    bVar1 = true;
    bVar7 = true;
    bVar4 = true;
    bVar8 = true;
    bVar9 = true;
    do {
      bVar10 = bVar9;
      bVar5 = bVar4;
      bVar6 = bVar1;
      cVar3 = cVar3 + '\x01';
      bVar4 = bVar7;
      if ((uVar2 & *param_1) != 0) {
        bVar4 = (bool)(bVar7 ^ 1);
      }
      uVar11 = uVar11 + 1 & 0xff;
      uVar2 = uVar2 >> 1;
      if (cVar3 == '\b') {
        param_1 = param_1 + 1;
      }
      bVar9 = (bool)(bVar8 ^ bVar4);
      if (cVar3 == '\b') {
        cVar3 = '\0';
        uVar2 = 0x80;
      }
      bVar1 = bVar10;
      bVar7 = bVar6;
      bVar8 = bVar5;
    } while (uVar11 != param_2);
    if (!bVar6) {
      uVar2 = 0;
      goto LAB_0003cd08;
    }
  }
  uVar2 = 0x10;
LAB_0003cd08:
  if (bVar10) {
    uVar2 = uVar2 | 8;
  }
  if (bVar9) {
    uVar2 = uVar2 | 4;
  }
  if (bVar5) {
    uVar2 = uVar2 | 2;
  }
  if (bVar4) {
    uVar2 = uVar2 | 1;
  }
  return uVar2;
}

