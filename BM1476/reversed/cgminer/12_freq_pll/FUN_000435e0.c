//==================== FUN_000435e0 @ 000435e0 ====================

void FUN_000435e0(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined4 uStack_101c;
  undefined4 uStack_1018;
  undefined2 uStack_1014;
  undefined1 local_1012;
  char acStack_1010 [4096];
  
  uStack_1018 = 0;
  uStack_101c = 0;
  uStack_1014 = 0;
  local_1012 = 0;
  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
    snprintf(acStack_1010,0x1000,"chainid%d %s freq=%d",param_1,"set_frequency_chain",param_2);
    FUN_00038730(5,acStack_1010,0);
  }
  iVar2 = 0;
  do {
    iVar1 = iVar2 * 4;
    iVar3 = iVar2 << 4;
    iVar2 = iVar2 + 1;
    if (param_2 == (&DAT_00069204)[iVar1]) goto LAB_00043694;
  } while (iVar2 != 0x73);
  iVar3 = 0x40;
LAB_00043694:
  DAT_00076b58 = *(undefined4 *)((int)&DAT_00069210 + iVar3);
  FUN_0003cdcc(&uStack_101c,1,0,0xc,DAT_00076b58);
  FUN_0003c150(param_1,&uStack_101c,0xb);
  *(short *)(&DAT_0009d4b5 + (param_1 + 0x128) * 2) = (short)param_2;
  FUN_0002a92c(0x32);
  return;
}

