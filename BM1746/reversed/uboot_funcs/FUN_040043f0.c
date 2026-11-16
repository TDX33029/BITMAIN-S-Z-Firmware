//==================== FUN_040043f0 @ 040043f0 ====================

undefined4 FUN_040043f0(int param_1,byte *param_2,int param_3,uint param_4,uint param_5)

{
  undefined4 uVar1;
  byte *pbVar2;
  byte *pbVar3;
  uint uVar4;
  int iVar5;
  uint uVar6;
  byte *pbVar7;
  uint local_7c;
  int local_78;
  byte *local_74;
  byte local_64 [68];
  
  if (0x40 < param_3 * param_5) {
    param_5 = FUN_0400c66c(0x40,param_3);
  }
  if (param_5 == 0) {
    param_5 = FUN_0400c66c(0x10,param_3);
  }
  uVar1 = DAT_04004608;
  if (param_4 != 0) {
    local_7c = param_4;
    local_78 = param_1;
    local_74 = param_2;
    do {
      FUN_04007548(DAT_0400460c,local_78);
      if (local_7c < param_5) {
        uVar6 = local_7c;
      }
      else {
        uVar6 = param_5;
      }
      if (uVar6 == 0) {
        if (param_5 != 0) {
          iVar5 = 0;
          goto LAB_04004524;
        }
        iVar5 = 0;
      }
      else {
        uVar4 = 0;
        pbVar7 = local_74;
        do {
          if (param_3 == 4) {
            *(undefined4 *)(local_64 + uVar4 * 4) = *(undefined4 *)pbVar7;
          }
          else if (param_3 == 2) {
            *(undefined2 *)(local_64 + uVar4 * 2) = *(undefined2 *)pbVar7;
          }
          else {
            local_64[uVar4] = *pbVar7;
          }
          uVar4 = uVar4 + 1;
          pbVar7 = pbVar7 + param_3;
          FUN_04007548(DAT_04004610,param_3 * 2);
        } while (uVar6 != uVar4);
        iVar5 = uVar6 * param_3;
        local_74 = local_74 + iVar5;
        for (; uVar6 < param_5; param_5 = param_5 - 1) {
LAB_04004524:
          uVar4 = 0;
          do {
            uVar4 = uVar4 + 1;
            func_0x03fc7fbc(uVar1);
          } while (uVar4 < param_3 * 2 + 1U);
        }
        if (iVar5 != 0) {
          pbVar2 = local_64 + iVar5;
          pbVar7 = local_64;
          do {
            while( true ) {
              pbVar3 = pbVar7 + 1;
              if (((*(byte *)(DAT_04004614 + (uint)*pbVar7) & 0x97) != 0) && ((*pbVar7 & 0x80) == 0)
                 ) break;
              *pbVar7 = 0x2e;
              pbVar7 = pbVar3;
              if (pbVar2 == pbVar3) goto LAB_04004590;
            }
            pbVar7 = pbVar3;
          } while (pbVar2 != pbVar3);
LAB_04004590:
          local_78 = local_78 + iVar5;
        }
      }
      local_64[iVar5] = 0;
      local_7c = local_7c - uVar6;
      FUN_04007548(DAT_04004618,local_64);
      iVar5 = func_0x03fc7fe4();
      if (iVar5 != 0) {
        return 0xffffffff;
      }
    } while (local_7c != 0);
  }
  return 0;
}

