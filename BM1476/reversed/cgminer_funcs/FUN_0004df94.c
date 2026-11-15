//==================== FUN_0004df94 @ 0004df94 ====================

void FUN_0004df94(byte *param_1,int param_2,int param_3,int param_4,uint param_5,uint param_6)

{
  int iVar1;
  byte *pbVar2;
  byte *pbVar3;
  uint uVar4;
  byte *pbVar5;
  undefined1 *puVar7;
  undefined1 *puVar6;
  uint uVar8;
  uint uVar9;
  uint uVar10;
  
  if (param_5 < 8) {
                    /* WARNING: Subroutine does not return */
    __assert_fail("bit_len >= 8","sort-verify.c",0x40,"expandArray");
  }
  if (0x20 < param_5 + 7) {
                    /* WARNING: Subroutine does not return */
    __assert_fail("8 * sizeof(uint32_t) >= 7 + bit_len","sort-verify.c",0x41,"expandArray");
  }
  uVar4 = param_6 + (param_5 + 7 >> 3);
  iVar1 = FUN_0005c648(uVar4 * param_2 * 8,param_5);
  if (iVar1 != param_4) {
                    /* WARNING: Subroutine does not return */
    __assert_fail("out_len == 8 * out_width * in_len / bit_len","sort-verify.c",0x44,"expandArray");
  }
  if (param_2 != 0) {
    iVar1 = 0;
    uVar10 = 0;
    uVar9 = 0;
    pbVar2 = param_1;
    do {
      uVar9 = uVar9 + 8;
      pbVar3 = pbVar2 + 1;
      uVar10 = (uint)*pbVar2 | uVar10 << 8;
      if (param_5 <= uVar9) {
        uVar9 = uVar9 - param_5;
        if (param_6 != 0) {
          puVar7 = (undefined1 *)(param_3 + iVar1);
          do {
            puVar6 = puVar7 + 1;
            *puVar7 = 0;
            puVar7 = puVar6;
          } while (puVar6 != (undefined1 *)(param_3 + param_6 + iVar1));
        }
        if (param_6 < uVar4) {
          uVar8 = uVar9 + ((0x1fffffff - param_6) + uVar4) * 8;
          pbVar2 = (byte *)(param_3 + iVar1 + param_6);
          do {
            pbVar5 = pbVar2 + 1;
            *pbVar2 = (byte)(uVar10 >> (uVar8 & 0xff)) &
                      (byte)((uint)~(-1 << (param_5 & 0xff)) >> (uVar8 - uVar9 & 0xff));
            uVar8 = uVar8 - 8;
            pbVar2 = pbVar5;
          } while (pbVar5 != (byte *)(param_3 + uVar4 + iVar1));
        }
        iVar1 = iVar1 + uVar4;
      }
      pbVar2 = pbVar3;
    } while (pbVar3 != param_1 + param_2);
  }
  return;
}

