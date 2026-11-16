//==================== FUN_00043734 @ 00043734 ====================

void FUN_00043734(int param_1,int param_2,int param_3)

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
  if ((DAT_00078e09 != '\0') &&
     (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
    snprintf(acStack_1010,0x1000,"%s %d %d freq=%d\n","set_freq_asic",param_1,param_2,param_3);
    FUN_00038730(7,acStack_1010,0);
  }
  iVar3 = 0;
  do {
    iVar1 = iVar3 * 4;
    iVar2 = iVar3 << 4;
    iVar3 = iVar3 + 1;
    if (param_3 == (&DAT_00069204)[iVar1]) goto LAB_000437f8;
  } while (iVar3 != 0x73);
  iVar2 = 0x40;
LAB_000437f8:
  FUN_0003cdcc(&uStack_101c,0,(uint)DAT_0009d6f3 * param_2 & 0xff,0xc,
               *(undefined4 *)((int)&DAT_00069210 + iVar2));
  FUN_0003c150(param_1,&uStack_101c,0xb);
  FUN_0002a92c(100);
  return;
}

