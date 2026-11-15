//==================== FUN_00029f54 @ 00029f54 ====================

int FUN_00029f54(uint *param_1,uint *param_2)

{
  char *param3;
  char *param4;
  uint uVar1;
  uint *puVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  uint *puVar9;
  char *param5;
  bool bVar10;
  uint uStack_1068;
  uint local_1064;
  uint local_1060;
  uint local_105c;
  uint local_1058;
  uint local_1054;
  uint local_1050;
  uint local_104c;
  uint uStack_1048;
  uint local_1044;
  uint local_1040;
  uint local_103c;
  uint local_1038;
  uint local_1034;
  uint local_1030;
  uint local_102c;
  char acStack_1028 [4100];
  
  puVar9 = param_1 + 8;
  puVar2 = param_2 + 8;
  iVar3 = 7;
  do {
    puVar9 = puVar9 + -1;
    puVar2 = puVar2 + -1;
    if (*puVar2 < *puVar9) {
      iVar3 = 0;
      goto LAB_00029f94;
    }
  } while ((*puVar2 <= *puVar9) && (bVar10 = iVar3 != 0, iVar3 = iVar3 + -1, bVar10));
  iVar3 = 1;
LAB_00029f94:
  if (DAT_00078e09 != '\0') {
    uVar8 = param_1[7];
    uVar1 = param_1[6];
    uVar6 = param_1[5];
    uVar7 = param_1[4];
    uVar5 = param_1[3];
    local_1060 = uVar6 << 0x18 | (uVar6 >> 8 & 0xff) << 0x10 | (uVar6 >> 0x10 & 0xff) << 8 |
                 uVar6 >> 0x18;
    uVar4 = param_1[2];
    uStack_1068 = uVar8 << 0x18 | (uVar8 >> 8 & 0xff) << 0x10 | (uVar8 >> 0x10 & 0xff) << 8 |
                  uVar8 >> 0x18;
    uVar8 = param_1[1];
    local_105c = uVar7 << 0x18 | (uVar7 >> 8 & 0xff) << 0x10 | (uVar7 >> 0x10 & 0xff) << 8 |
                 uVar7 >> 0x18;
    uVar7 = *param_1;
    uVar6 = param_2[7];
    local_1058 = uVar5 << 0x18 | (uVar5 >> 8 & 0xff) << 0x10 | (uVar5 >> 0x10 & 0xff) << 8 |
                 uVar5 >> 0x18;
    uVar5 = param_2[6];
    local_1054 = uVar4 << 0x18 | (uVar4 >> 8 & 0xff) << 0x10 | (uVar4 >> 0x10 & 0xff) << 8 |
                 uVar4 >> 0x18;
    local_1050 = uVar8 << 0x18 | (uVar8 >> 8 & 0xff) << 0x10 | (uVar8 >> 0x10 & 0xff) << 8 |
                 uVar8 >> 0x18;
    uVar4 = param_2[5];
    local_104c = uVar7 << 0x18 | (uVar7 >> 8 & 0xff) << 0x10 | (uVar7 >> 0x10 & 0xff) << 8 |
                 uVar7 >> 0x18;
    uStack_1048 = uVar6 << 0x18 | (uVar6 >> 8 & 0xff) << 0x10 | (uVar6 >> 0x10 & 0xff) << 8 |
                  uVar6 >> 0x18;
    uVar6 = param_2[4];
    local_1064 = uVar1 << 0x18 | (uVar1 >> 8 & 0xff) << 0x10 | (uVar1 >> 0x10 & 0xff) << 8 |
                 uVar1 >> 0x18;
    uVar1 = param_2[1];
    local_103c = uVar6 << 0x18 | (uVar6 >> 8 & 0xff) << 0x10 | (uVar6 >> 0x10 & 0xff) << 8 |
                 uVar6 >> 0x18;
    uVar6 = *param_2;
    local_1044 = uVar5 << 0x18 | (uVar5 >> 8 & 0xff) << 0x10 | (uVar5 >> 0x10 & 0xff) << 8 |
                 uVar5 >> 0x18;
    local_1030 = uVar1 << 0x18 | (uVar1 >> 8 & 0xff) << 0x10 | (uVar1 >> 0x10 & 0xff) << 8 |
                 uVar1 >> 0x18;
    uVar5 = param_2[3];
    local_102c = uVar6 << 0x18 | (uVar6 >> 8 & 0xff) << 0x10 | (uVar6 >> 0x10 & 0xff) << 8 |
                 uVar6 >> 0x18;
    local_1040 = uVar4 << 0x18 | (uVar4 >> 8 & 0xff) << 0x10 | (uVar4 >> 0x10 & 0xff) << 8 |
                 uVar4 >> 0x18;
    uVar1 = param_2[2];
    local_1038 = uVar5 << 0x18 | (uVar5 >> 8 & 0xff) << 0x10 | (uVar5 >> 0x10 & 0xff) << 8 |
                 uVar5 >> 0x18;
    local_1034 = uVar1 << 0x18 | (uVar1 >> 8 & 0xff) << 0x10 | (uVar1 >> 0x10 & 0xff) << 8 |
                 uVar1 >> 0x18;
    param3 = (char *)FUN_00029730(&uStack_1068,0x20);
    param4 = (char *)FUN_00029730(&uStack_1048,0x20);
    if ((DAT_00078e09 != '\0') &&
       (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
      param5 = "no (false positive; hash > target)";
      if (iVar3 != 0) {
        param5 = "YES (hash <= target)";
      }
      snprintf(acStack_1028,0x1000," Proof: %s\nTarget: %s\nTrgVal? %s",param3,param4,param5);
      FUN_00038730(7,acStack_1028,0);
    }
    free(param3);
    free(param4);
  }
  return iVar3;
}

