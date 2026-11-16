//==================== FUN_04007c5c @ 04007c5c ====================

uint FUN_04007c5c(ushort *param_1,uint param_2)

{
  int iVar1;
  ushort *puVar2;
  uint uVar3;
  uint local_c;
  
  uVar3 = 0;
  for (puVar2 = param_1; 1 < (int)param_1 + (param_2 - (int)puVar2); puVar2 = puVar2 + 1) {
    uVar3 = uVar3 + *puVar2;
  }
  if ((param_2 >> 1) * -2 + param_2 == 1) {
    local_c = (uint)(byte)param_1[param_2 >> 1];
    uVar3 = uVar3 + local_c;
  }
  iVar1 = (uVar3 & 0xffff) + ((int)uVar3 >> 0x10);
  return ~(iVar1 + (iVar1 >> 0x10)) & 0xffff;
}

