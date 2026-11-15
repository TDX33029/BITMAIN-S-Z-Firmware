//==================== FUN_00005224 @ 00005224 ====================

int FUN_00005224(undefined4 param_1,uint *param_2,undefined4 *param_3)

{
  int iVar1;
  int iVar2;
  char *pcVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  char *pcVar8;
  int iVar9;
  int iVar10;
  undefined4 local_2c [2];
  char *local_24;
  
  local_2c[0] = param_1;
  iVar4 = FUN_000030bc(local_2c,&local_24,0);
  pcVar3 = local_24;
  if (iVar4 == 0) {
    iVar9 = *(int *)(local_24 + 0x14);
    uVar6 = *(uint *)(local_24 + 0x10);
    *param_3 = local_24;
    if (iVar9 - 2U < uVar6) {
      if (*local_24 == '\x01') {
        uVar7 = 2;
        uVar6 = 0;
        do {
          iVar9 = FUN_000036fc(pcVar3,uVar7);
          uVar7 = uVar7 + 1;
          pcVar8 = local_24;
          if (iVar9 == -1) {
            iVar4 = 1;
            break;
          }
          if (iVar9 == 1) {
            iVar4 = 2;
            break;
          }
          if (iVar9 == 0) {
            uVar6 = uVar6 + 1;
          }
        } while (uVar7 < *(uint *)(local_24 + 0x14));
      }
      else {
        uVar7 = 0;
        uVar6 = 0;
        iVar5 = *(int *)(local_24 + 0x20);
        pcVar8 = local_24;
        do {
          while( true ) {
            iVar10 = iVar5;
            if (uVar7 == 0) {
              iVar10 = iVar5 + 1;
              if (iVar5 == *(int *)(pcVar8 + 0x2c)) {
              }
              else {
                iVar5 = FUN_00002f3c(pcVar8);
                pcVar8 = local_24;
                if (iVar5 != 0) {
                  iVar4 = iVar5;
                  goto LAB_00005318;
                }
              }
            }
            iVar1 = uVar7 + 0x40;
            iVar2 = uVar7 + 0x41;
            iVar5 = iVar10;
            if (*pcVar8 != '\x02') break;
            uVar7 = uVar7 + 2 & 0x1ff;
            if (CONCAT11(pcVar8[iVar2],pcVar8[iVar1]) == 0) {
              uVar6 = uVar6 + 1;
            }
            iVar9 = iVar9 + -1;
            if (iVar9 == 0) goto LAB_00005318;
          }
          iVar10 = uVar7 + 0x42;
          uVar7 = uVar7 + 4 & 0x1ff;
          if ((CONCAT31(CONCAT21(*(undefined2 *)(pcVar8 + iVar10),pcVar8[iVar2]),pcVar8[iVar1]) &
              0xfffffff) == 0) {
            uVar6 = uVar6 + 1;
          }
          iVar9 = iVar9 + -1;
        } while (iVar9 != 0);
      }
LAB_00005318:
      *param_2 = uVar6;
      *(uint *)(pcVar8 + 0x10) = uVar6;
      pcVar8[4] = pcVar8[4] | 1;
    }
    else {
      *param_2 = uVar6;
    }
  }
  return iVar4;
}

