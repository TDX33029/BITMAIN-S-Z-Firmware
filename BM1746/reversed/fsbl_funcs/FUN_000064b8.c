//==================== FUN_000064b8 @ 000064b8 ====================

void FUN_000064b8(int param_1)

{
  byte bVar1;
  int iVar2;
  uint uVar3;
  
  bVar1 = (&DAT_00019be8)[param_1];
  iVar2 = *(int *)(&DAT_00021564 + param_1 * 0x220);
  if (iVar2 == 0) {
    DAT_000219a0 = -0x1ff00000;
    DAT_000219a4 = 1;
    DAT_000219a8 = iVar2;
    if ((uRame01000fe & 0xff) == 2) {
      uVar3 = uRame0100040 & 0xc0000000;
      *(uint *)(&DAT_000219ac + param_1 * 4) = uVar3;
      if (uVar3 == 0x40000000) goto LAB_00006568;
    }
    else {
      *(undefined4 *)(&DAT_000219ac + param_1 * 4) = 0;
    }
    if ((uRame0100024 & 0x10000) == 0) {
LAB_00006524:
      iVar2 = 500;
      do {
        func_0xffffe2ac(10000);
        uVar3 = *(uint *)(DAT_000219a0 + 0x24);
        if ((uVar3 & 0x10000) != 0) goto LAB_0000654c;
        iVar2 = iVar2 + -1;
      } while (iVar2 != 0);
      bVar1 = 3;
      goto LAB_0000656c;
    }
  }
  else {
    uVar3 = *(uint *)(DAT_000219a0 + 0x24);
    if (*(int *)(&DAT_000219ac + param_1 * 4) != 0x40000000) {
      if ((DAT_000219a4 != 0) && ((uVar3 & 0x10000) == 0)) goto LAB_00006524;
LAB_0000654c:
      if ((DAT_000219a8 != 0) && ((uVar3 & 0x80000) == 0)) {
        bVar1 = bVar1 & 0xfd | 4;
        goto LAB_0000656c;
      }
    }
  }
LAB_00006568:
  bVar1 = bVar1 & 0xf9;
LAB_0000656c:
  (&DAT_00019be8)[param_1] = bVar1;
  return;
}

