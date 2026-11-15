//==================== FUN_00029c70 @ 00029c70 ====================

void FUN_00029c70(undefined1 *param_1,char *param_2)

{
  longlong lVar1;
  size_t sVar2;
  uint *puVar3;
  uint uVar4;
  uint uVar5;
  undefined1 *puVar6;
  int iVar7;
  size_t sVar8;
  undefined4 *puVar9;
  undefined4 local_44;
  uint local_40 [7];
  
  local_44 = 0;
  local_40[0] = 0;
  local_40[1] = 0;
  local_40[2] = 0;
  local_40[3] = 0;
  local_40[4] = 0;
  local_40[5] = 0;
  sVar2 = strlen(param_2);
  if ((int)sVar2 < 1) {
    uVar4 = 0;
    local_44._0_1_ = 0;
  }
  else {
    sVar8 = 0;
    uVar4 = 0;
    while( true ) {
      uVar5 = *(uint *)(&DAT_000652a8 + (uint)(byte)param_2[sVar8] * 4);
      iVar7 = 6;
      puVar9 = &local_44;
      while( true ) {
        lVar1 = (ulonglong)uVar4 * 0x3a + (ulonglong)uVar5;
        uVar5 = (uint)((ulonglong)lVar1 >> 0x20) & 0x3f;
        puVar9[6] = (int)lVar1;
        if (iVar7 == 0) break;
        uVar4 = puVar9[5];
        iVar7 = iVar7 + -1;
        puVar9 = puVar9 + -1;
      }
      sVar8 = sVar8 + 1;
      if (sVar2 == sVar8) break;
      uVar4 = local_40[5];
    }
    uVar4 = local_40[0] << 0x18 | (local_40[0] >> 8 & 0xff) << 0x10 |
            (local_40[0] >> 0x10 & 0xff) << 8 | local_40[0] >> 0x18;
  }
  puVar3 = local_40;
  puVar6 = param_1 + 5;
  *param_1 = (undefined1)local_44;
  while( true ) {
    *(uint *)(puVar6 + -4) = uVar4;
    puVar6 = puVar6 + 4;
    if (puVar6 == param_1 + 0x1d) break;
    puVar3 = puVar3 + 1;
    uVar4 = *puVar3;
    uVar4 = uVar4 << 0x18 | (uVar4 >> 8 & 0xff) << 0x10 | (uVar4 >> 0x10 & 0xff) << 8 |
            uVar4 >> 0x18;
  }
  return;
}

