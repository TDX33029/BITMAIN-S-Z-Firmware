//==================== FUN_04006520 @ 04006520 ====================

void FUN_04006520(char *param_1,undefined4 param_2,uint param_3,uint param_4)

{
  uint uVar1;
  char *pcVar2;
  char cVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  int iVar7;
  int iVar8;
  uint uVar9;
  uint uVar10;
  int iVar11;
  bool bVar12;
  uint local_28;
  uint uStack_24;
  
  uVar5 = DAT_04006694;
  uVar4 = DAT_04006690;
  bVar12 = DAT_04006694 <= param_4;
  if (param_4 == DAT_04006694) {
    bVar12 = DAT_04006690 <= param_3;
  }
  pcVar2 = param_1;
  local_28 = param_3;
  uStack_24 = param_4;
  if (bVar12 && (param_4 != DAT_04006694 || param_3 != DAT_04006690)) {
    do {
      param_1 = pcVar2 + 5;
      if (uStack_24 == 0) {
        uVar1 = (local_28 >> 5) / 0xc35;
        uVar6 = local_28 + uVar1 * -100000;
        uStack_24 = 0;
        local_28 = uVar1;
      }
      else {
        uVar6 = FUN_04004754(&local_28,100000,(local_28 >> 5) * 0xa7c5ac5);
      }
      uVar9 = (uVar6 & 0xfff) >> 8;
      uVar1 = uVar6 >> 0xc;
      uVar10 = (uVar6 & 0xff) >> 4;
      bVar12 = uVar5 <= uStack_24;
      iVar7 = (uVar6 & 0xf) + (uVar10 + uVar9 + uVar1) * 6;
      if (uStack_24 == uVar5) {
        bVar12 = uVar4 <= local_28;
      }
      uVar6 = (uint)(iVar7 * 0xcd) >> 0xb;
      iVar11 = uVar1 * 9 + uVar10 + uVar9 * 5 + uVar6;
      uVar10 = (uint)(iVar11 * 0xcd) >> 0xb;
      *pcVar2 = (char)iVar7 + (char)uVar6 * -10 + '0';
      iVar8 = uVar10 + uVar9 * 2;
      pcVar2[1] = (char)iVar11 + (char)uVar10 * -10 + '0';
      uVar6 = (uint)(iVar8 * 0xd) >> 7;
      iVar7 = uVar6 + uVar1 * 4;
      pcVar2[2] = (char)iVar8 + (char)uVar6 * -10 + '0';
      cVar3 = (char)((uint)(iVar7 * 0xcd) >> 0xb);
      pcVar2[4] = cVar3 + '0';
      pcVar2[3] = (char)iVar7 + cVar3 * -10 + '0';
      pcVar2 = param_1;
    } while (bVar12 && (uStack_24 != uVar5 || local_28 != uVar4));
  }
  FUN_0400636c(param_1,local_28);
  return;
}

