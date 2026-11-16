//==================== FUN_0400636c @ 0400636c ====================

char * FUN_0400636c(char *param_1,uint param_2)

{
  uint uVar1;
  char cVar2;
  int iVar3;
  char *pcVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  char *pcVar8;
  bool bVar9;
  
  uVar7 = (param_2 & 0xff) >> 4;
  uVar6 = (param_2 & 0xfff) >> 8;
  uVar1 = param_2 >> 0xc;
  iVar3 = (param_2 & 0xf) + (uVar7 + uVar6 + uVar1) * 6;
  uVar5 = (uint)(iVar3 * 0xcd) >> 0xb;
  pcVar4 = (char *)(uVar5 + uVar1 * 9 + uVar7 + uVar6 * 5);
  bVar9 = pcVar4 == (char *)0x0;
  if (bVar9) {
    pcVar4 = param_1 + 1;
  }
  *param_1 = (char)iVar3 + (char)uVar5 * -10 + '0';
  if (!bVar9) {
    uVar5 = (uint)((int)pcVar4 * 0xcd) >> 0xb;
    iVar3 = uVar5 + uVar6 * 2;
    param_1[1] = (char)pcVar4 + (char)uVar5 * -10 + '0';
    if (uVar1 == 0 && iVar3 == 0) {
      pcVar4 = param_1 + 2;
    }
    else {
      pcVar4 = (char *)((uint)(iVar3 * 0xd) >> 7);
      pcVar8 = pcVar4 + uVar1 * 4;
      cVar2 = (char)pcVar4;
      if (pcVar8 == (char *)0x0) {
        pcVar4 = param_1 + 3;
      }
      param_1[2] = (char)iVar3 + cVar2 * -10 + '0';
      if (pcVar8 != (char *)0x0) {
        uVar1 = (uint)((int)pcVar8 * 0xcd) >> 0xb;
        uVar5 = uVar1;
        if (uVar1 != 0) {
          uVar5 = uVar1 + 0x30;
        }
        if (uVar1 != 0) {
          pcVar4 = param_1 + 5;
          param_1[4] = (char)uVar5;
        }
        else {
          pcVar4 = param_1 + 4;
        }
        param_1[3] = (char)pcVar8 + (char)uVar1 * -10 + '0';
      }
    }
  }
  return pcVar4;
}

