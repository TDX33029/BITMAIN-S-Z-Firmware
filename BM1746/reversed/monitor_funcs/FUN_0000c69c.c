//==================== FUN_0000c69c @ 0000c69c ====================

undefined1 * FUN_0000c69c(byte *param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  byte bVar1;
  char *pcVar2;
  undefined1 *puVar3;
  uint uVar4;
  byte *pbVar5;
  int iVar6;
  byte *pbVar7;
  bool bVar8;
  
  if (param_1 == (byte *)0x0) {
    puVar3 = (undefined1 *)FUN_0000c574("",param_2,param_3,param_4);
    return puVar3;
  }
  if (*param_1 == 0) {
    iVar6 = 3;
  }
  else {
    iVar6 = 0;
    uVar4 = (uint)*param_1;
    pbVar7 = param_1;
    do {
      while (pcVar2 = strchr("\"\\\b\f\n\r\t",uVar4), pcVar2 == (char *)0x0) {
        pbVar7 = pbVar7 + 1;
        if (uVar4 < 0x20) {
          iVar6 = iVar6 + 6;
        }
        else {
          iVar6 = iVar6 + 1;
        }
        uVar4 = (uint)*pbVar7;
        if (*pbVar7 == 0) goto LAB_0000c6fc;
      }
      iVar6 = iVar6 + 2;
      pbVar7 = pbVar7 + 1;
      uVar4 = (uint)*pbVar7;
    } while (uVar4 != 0);
LAB_0000c6fc:
    iVar6 = iVar6 + 3;
  }
  puVar3 = (undefined1 *)(*DAT_0001b374)(iVar6);
  if (puVar3 == (undefined1 *)0x0) {
    return (undefined1 *)0x0;
  }
  pbVar7 = puVar3 + 1;
  *puVar3 = 0x22;
LAB_0000c734:
  bVar1 = *param_1;
  pbVar5 = pbVar7;
  pbVar7 = param_1;
  do {
    if (bVar1 == 0) {
      pbVar5[1] = 0;
      *pbVar5 = 0x22;
      return puVar3;
    }
    param_1 = pbVar7 + 1;
    bVar8 = 0x21 < bVar1;
    if (bVar1 != 0x22) {
      bVar8 = 0x1e < bVar1;
    }
    if ((bVar8 && (bVar1 != 0x22 && bVar1 != 0x1f)) && (bVar1 != 0x5c)) {
      pbVar7 = pbVar5 + 1;
      *pbVar5 = bVar1;
      goto LAB_0000c734;
    }
    *pbVar5 = 0x5c;
    uVar4 = (uint)*pbVar7;
    if (uVar4 == 0xc) {
      pbVar5[1] = 0x66;
      pbVar7 = pbVar5 + 2;
      goto LAB_0000c734;
    }
    if (uVar4 < 0xd) {
      if (uVar4 == 9) {
        pbVar5[1] = 0x74;
        pbVar7 = pbVar5 + 2;
        goto LAB_0000c734;
      }
      if (uVar4 == 10) {
        pbVar5[1] = 0x6e;
        pbVar7 = pbVar5 + 2;
        goto LAB_0000c734;
      }
      if (uVar4 == 8) break;
    }
    else {
      if ((uVar4 == 0x22) || (uVar4 == 0x5c)) {
        pbVar5[1] = *pbVar7;
        pbVar7 = pbVar5 + 2;
        goto LAB_0000c734;
      }
      if (uVar4 == 0xd) {
        pbVar5[1] = 0x72;
        pbVar7 = pbVar5 + 2;
        goto LAB_0000c734;
      }
    }
    sprintf((char *)(pbVar5 + 1),"u%04x",uVar4);
    bVar1 = *param_1;
    pbVar5 = pbVar5 + 6;
    pbVar7 = param_1;
  } while( true );
  pbVar5[1] = 0x62;
  pbVar7 = pbVar5 + 2;
  goto LAB_0000c734;
}

