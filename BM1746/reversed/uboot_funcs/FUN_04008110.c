//==================== FUN_04008110 @ 04008110 ====================

void FUN_04008110(undefined1 *param_1,undefined1 param_2,int param_3,int param_4)

{
  int iVar1;
  int iVar2;
  undefined1 *puVar3;
  undefined1 *puVar4;
  
  param_1[6] = param_2;
  *param_1 = 99;
  param_1[3] = 99;
  param_1[7] = 0x39;
  param_1[1] = 0x82;
  puVar3 = (undefined1 *)0x53;
  param_1[4] = 0x35;
  param_1[8] = 2;
  param_1[9] = 2;
  param_1[2] = 0x53;
  if (param_3 == 0) {
    puVar3 = param_1 + 0xb;
  }
  param_1[5] = 1;
  param_1[10] = 0x40;
  if (param_3 != 0) {
    param_1[0xb] = 0x36;
    param_1[0xc] = 4;
    puVar3 = param_1 + 0x11;
    param_1[0x10] = (char)((uint)param_3 >> 0x18);
    param_1[0xd] = (char)param_3;
    param_1[0xe] = (char)((uint)param_3 >> 8);
    param_1[0xf] = (char)((uint)param_3 >> 0x10);
  }
  puVar4 = puVar3;
  if (param_4 != 0) {
    puVar3[5] = (char)((uint)param_4 >> 0x18);
    *puVar3 = 0x32;
    puVar3[1] = 4;
    puVar4 = puVar3 + 6;
    puVar3[2] = (char)param_4;
    puVar3[3] = (char)((uint)param_4 >> 8);
    puVar3[4] = (char)((uint)param_4 >> 0x10);
  }
  iVar1 = func_0x03fbefb0(DAT_040082d8);
  if (iVar1 == 0) {
    iVar1 = 0x15;
  }
  else {
    iVar1 = func_0x03fbf064(DAT_040082d8,0x10,0x15);
    puVar3 = puVar4;
    if (iVar1 < 1) goto LAB_04008208;
  }
  puVar4[3] = (char)iVar1;
  *puVar4 = 0x5d;
  puVar4[1] = 2;
  puVar3 = puVar4 + 4;
  puVar4[2] = (char)((uint)iVar1 >> 8);
LAB_04008208:
  *puVar3 = 0x5e;
  iVar1 = DAT_040082dc;
  puVar3[2] = 1;
  puVar4 = puVar3 + 7;
  puVar3[1] = 3;
  puVar3[3] = 0;
  puVar3[4] = 0;
  iVar2 = func_0x03fbefb0(DAT_040082e0);
  if (iVar2 != 0) {
    iVar1 = iVar2;
  }
  iVar2 = FUN_04005b98(iVar1);
  puVar3[5] = 0x3c;
  puVar3[6] = (char)iVar2;
  FUN_04005ea4(puVar4,iVar1,iVar2);
  puVar4[iVar2] = 0x37;
  puVar4[iVar2 + 3] = 0xc;
  puVar4[iVar2 + 4] = 0x11;
  puVar4[iVar2 + 2] = 3;
  puVar4[iVar2 + 1] = 3;
  puVar4[iVar2 + 5] = 0xff;
  for (puVar4 = puVar4 + iVar2 + 6; (int)puVar4 - (int)param_1 < 0x40; puVar4 = puVar4 + 1) {
    *puVar4 = 0;
  }
  return;
}

