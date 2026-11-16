//==================== FUN_000071f8 @ 000071f8 ====================

/* WARNING: Control flow encountered bad instruction data */

void FUN_000071f8(int param_1,int param_2,int param_3)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  uint uVar6;
  int iVar7;
  int iVar8;
  
  iVar5 = *(int *)(param_1 + 4);
  uVar6 = *(ushort *)(iVar5 + 4) & 0xfff;
  uVar3 = param_2 * uVar6;
  if (uVar3 < 0x10000) {
    iVar7 = 0;
  }
  else {
    uVar4 = uVar3 >> 0x10;
    if ((uVar3 & 0xffff) != 0) {
      uVar4 = uVar4 + 1;
    }
    if (uVar4 == 1) {
      iVar7 = 0;
    }
    else {
      iVar2 = param_3;
      iVar1 = 0;
      iVar8 = param_1;
      do {
        iVar7 = iVar1 + 1;
        iVar1 = param_1 + iVar1 * 0xc;
        *(int *)(iVar1 + 0x84) = iVar2;
        iVar2 = iVar2 + 0x10000;
        *(undefined4 *)(iVar1 + 0x88) = 0;
        *(undefined2 *)(iVar8 + 0x80) = 0x21;
        *(undefined2 *)(iVar8 + 0x82) = 0;
        iVar1 = iVar7;
        iVar8 = iVar8 + 0xc;
      } while (iVar7 != uVar4 - 1);
      param_3 = param_3 + iVar7 * 0x10000;
    }
  }
  iVar7 = param_1 + iVar7 * 0xc;
  *(int *)(iVar7 + 0x84) = param_3;
  *(undefined4 *)(iVar7 + 0x88) = 0;
  *(short *)(iVar7 + 0x82) = (short)param_2 * (short)uVar6;
  *(undefined2 *)(iVar7 + 0x80) = 0x23;
  *(int *)(iVar5 + 0x58) = param_1 + 0x80;
  if (*(char *)(param_1 + 0x20) == '\0') {
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  return;
}

