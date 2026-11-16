//==================== FUN_040034a8 @ 040034a8 ====================

uint FUN_040034a8(byte *param_1,int param_2,undefined4 param_3,undefined4 param_4,int param_5,
                 int *param_6,int *param_7,undefined4 param_8)

{
  int iVar1;
  uint extraout_r1;
  int extraout_r1_00;
  undefined4 uVar2;
  uint uVar3;
  byte *pbVar4;
  uint uVar5;
  uint uVar6;
  int iVar7;
  int iVar8;
  code *pcVar9;
  uint *puVar10;
  uint uVar11;
  uint local_34;
  
  iVar1 = FUN_04005b98(param_1);
  if (iVar1 == 0) {
    iVar8 = param_7[1];
    iVar1 = 0x14;
    uVar6 = 1;
  }
  else {
    pbVar4 = param_1 + iVar1;
    do {
      pbVar4 = pbVar4 + -1;
      iVar1 = (uint)*pbVar4 + iVar1 * 0x10;
    } while (pbVar4 != param_1);
    iVar8 = param_7[1];
    FUN_0400c7d4(iVar1,iVar8);
    if (extraout_r1 == 0) {
      iVar1 = 0x14;
      uVar6 = 1;
    }
    else {
      iVar1 = extraout_r1 * 0x14;
      uVar6 = extraout_r1;
    }
  }
  uVar5 = *(uint *)(*param_7 + iVar1);
  puVar10 = (uint *)(*param_7 + iVar1);
  local_34 = uVar6;
  if (uVar5 == 0) {
    if (param_5 == 1) {
      if (param_7[2] != iVar8) {
LAB_04003600:
        *puVar10 = uVar6;
        uVar6 = FUN_04005c20(param_1);
        iVar7 = *param_7;
        puVar10[1] = uVar6;
        uVar2 = FUN_04005c20(param_2);
        iVar8 = *param_7;
        *(undefined4 *)(iVar7 + iVar1 + 8) = uVar2;
        iVar8 = iVar8 + iVar1;
        if ((*(int *)(iVar8 + 4) != 0) && (*(int *)(iVar8 + 8) != 0)) {
          param_7[2] = param_7[2] + 1;
          func_0x03fc3288(iVar8 + 4);
          func_0x03fc35fc(*param_7 + iVar1 + 4);
          if (((code *)param_7[3] == (code *)0x0) ||
             (iVar8 = (*(code *)param_7[3])(*param_7 + iVar1 + 4,param_2,0,param_8), iVar8 == 0)) {
            iVar8 = *param_7 + iVar1;
            if (*(code **)(iVar8 + 0xc) == (code *)0x0) {
LAB_040036d4:
              *param_6 = iVar8 + 4;
              return 1;
            }
            iVar8 = (**(code **)(iVar8 + 0xc))(param_1,param_2,0,param_8);
            if (iVar8 == 0) {
              iVar8 = *param_7 + iVar1;
              goto LAB_040036d4;
            }
            FUN_040030a8(param_7,param_7 + 2,*param_7 + iVar1 + 4,local_34);
LAB_0400391c:
            uVar2 = 0x16;
            goto LAB_040035c8;
          }
          FUN_040030a8(param_7,param_7 + 2,*param_7 + iVar1 + 4,local_34);
          goto LAB_04003944;
        }
      }
LAB_04003840:
      uVar2 = 0xc;
      goto LAB_040035c8;
    }
  }
  else {
    if (uVar5 != 0xffffffff) {
      local_34 = 0;
    }
    if (uVar6 == uVar5) {
      iVar8 = FUN_04005a60(param_1,puVar10[1]);
      if (iVar8 != 0) {
        iVar8 = param_7[1];
        goto LAB_04003538;
      }
      iVar7 = *param_7;
      iVar8 = iVar7 + iVar1;
      if (param_2 == 0 || param_5 != 1) goto LAB_040038ec;
      if ((code *)param_7[3] == (code *)0x0) {
LAB_040037dc:
        pcVar9 = *(code **)(iVar7 + iVar1 + 0xc);
        if (pcVar9 == (code *)0x0) {
LAB_0400380c:
          func_0x03fc8570(*(undefined4 *)(iVar7 + iVar1 + 8));
          iVar7 = *param_7;
          uVar2 = FUN_04005c20(param_2);
          iVar8 = *param_7;
          *(undefined4 *)(iVar7 + iVar1 + 8) = uVar2;
          iVar8 = iVar8 + iVar1;
          if (*(int *)(iVar8 + 8) != 0) {
LAB_040038ec:
            *param_6 = iVar8 + 4;
            return uVar6;
          }
          goto LAB_04003840;
        }
        iVar8 = (*pcVar9)(param_1,param_2,2,param_8);
        if (iVar8 == 0) {
          iVar7 = *param_7;
          goto LAB_0400380c;
        }
        goto LAB_0400391c;
      }
      iVar8 = (*(code *)param_7[3])(iVar8 + 4,param_2,2,param_8);
      if (iVar8 == 0) {
        iVar7 = *param_7;
        goto LAB_040037dc;
      }
LAB_04003944:
      uVar2 = 1;
      goto LAB_040035c8;
    }
LAB_04003538:
    FUN_0400c7d4(uVar6,iVar8 + -2);
    uVar11 = extraout_r1_00 + 1;
    uVar5 = uVar6;
    do {
      while( true ) {
        if (uVar11 < uVar5) {
          iVar1 = -uVar11;
        }
        else {
          iVar1 = param_7[1] - uVar11;
        }
        uVar5 = uVar5 + iVar1;
        if (uVar6 == uVar5) goto LAB_040035b4;
        iVar1 = uVar5 * 0x14;
        uVar3 = *(uint *)(*param_7 + uVar5 * 0x14);
        if (uVar6 == uVar3) break;
LAB_04003550:
        if (uVar3 == 0) goto LAB_040035b4;
      }
      iVar8 = FUN_04005a60(param_1,*(undefined4 *)(*param_7 + iVar1 + 4));
      if (iVar8 == 0) {
        puVar10 = (uint *)(*param_7 + iVar1);
        if (param_2 != 0 && param_5 == 1) {
          if ((code *)param_7[3] != (code *)0x0) {
            iVar8 = (*(code *)param_7[3])(puVar10 + 1,param_2,2,param_8);
            if (iVar8 != 0) goto LAB_04003944;
            puVar10 = (uint *)(*param_7 + iVar1);
          }
          if ((code *)puVar10[3] != (code *)0x0) {
            iVar8 = (*(code *)puVar10[3])(param_1,param_2,2,param_8);
            if (iVar8 != 0) goto LAB_0400391c;
            puVar10 = (uint *)(*param_7 + iVar1);
          }
          func_0x03fc8570(puVar10[2]);
          iVar7 = *param_7;
          uVar2 = FUN_04005c20(param_2);
          iVar8 = *param_7;
          *(undefined4 *)(iVar7 + iVar1 + 8) = uVar2;
          puVar10 = (uint *)(iVar8 + iVar1);
          if (puVar10[2] == 0) goto LAB_04003840;
        }
        *param_6 = (int)(puVar10 + 1);
        if (uVar5 != 0xffffffff) {
          return uVar5;
        }
        uVar3 = *puVar10;
        goto LAB_04003550;
      }
    } while (*(int *)(*param_7 + uVar5 * 0x14) != 0);
LAB_040035b4:
    if (param_5 == 1) {
      if (param_7[2] != param_7[1]) {
        if (local_34 == 0) {
          local_34 = uVar5;
        }
        iVar1 = local_34 * 0x14;
        puVar10 = (uint *)(*param_7 + iVar1);
        goto LAB_04003600;
      }
      goto LAB_04003840;
    }
  }
  uVar2 = 3;
LAB_040035c8:
  *DAT_04003950 = uVar2;
  *param_6 = 0;
  return 0;
}

