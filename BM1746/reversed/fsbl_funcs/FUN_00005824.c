//==================== FUN_00005824 @ 00005824 ====================

int FUN_00005824(undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  undefined4 *puVar2;
  char **ppcVar3;
  uint uVar4;
  undefined4 *puVar5;
  int *piVar6;
  int *piVar7;
  undefined4 local_90;
  undefined4 local_8c [2];
  char *local_84;
  undefined4 auStack_80 [2];
  byte local_75;
  undefined4 local_73;
  undefined4 local_6f;
  undefined4 local_6b;
  undefined4 local_67;
  undefined1 local_63;
  undefined1 local_62;
  undefined1 local_61;
  char *local_60 [2];
  int local_58;
  int local_50;
  int local_44;
  byte local_35;
  int aiStack_34 [11];
  int iStack_8;
  
  local_90 = param_2;
  local_8c[0] = param_1;
  FUN_00002b44(&local_90);
  iVar1 = FUN_000030bc(local_8c,&local_84,2);
  if (iVar1 == 0) {
    local_60[0] = local_84;
    iVar1 = FUN_00003f2c(local_60,local_8c[0]);
    if (iVar1 == 0) {
      if ((local_35 & 0xa0) == 0) {
        puVar5 = (undefined4 *)(local_44 + -4);
        puVar2 = auStack_80;
        do {
          puVar5 = puVar5 + 1;
          *puVar2 = *puVar5;
          puVar2 = puVar2 + 1;
        } while ((undefined4 *)(local_44 + 0x1c) != puVar5);
        ppcVar3 = local_60;
        piVar6 = aiStack_34;
        do {
          piVar7 = piVar6 + 1;
          *piVar6 = (int)*ppcVar3;
          ppcVar3 = ppcVar3 + 1;
          piVar6 = piVar7;
        } while (piVar7 != &iStack_8);
        iVar1 = FUN_00003f2c(aiStack_34,local_90);
        if (iVar1 == 0) {
          if ((aiStack_34[2] != local_58) || (aiStack_34[4] != local_50)) {
            return 8;
          }
        }
        else if (iVar1 != 4) {
          return iVar1;
        }
        iVar1 = FUN_00003e38(aiStack_34);
        if (iVar1 == 0) {
          *(undefined4 *)(aiStack_34[7] + 0x11) = local_6f;
          if ((local_75 & 0x10) == 0) {
            local_75 = local_75 | 0x20;
          }
          *(undefined4 *)(aiStack_34[7] + 0xd) = local_73;
          *(undefined4 *)(aiStack_34[7] + 0x15) = local_6b;
          *(undefined1 *)(aiStack_34[7] + 0x1e) = local_62;
          *(byte *)(aiStack_34[7] + 0xb) = local_75;
          *(undefined4 *)(aiStack_34[7] + 0x19) = local_67;
          *(undefined1 *)(aiStack_34[7] + 0x1d) = local_63;
          *(undefined1 *)(aiStack_34[7] + 0x1f) = local_61;
          local_84[3] = '\x01';
          if (((*(byte *)(aiStack_34[7] + 0xb) & 0x10) != 0) && (local_58 != aiStack_34[2])) {
            uVar4 = (uint)*(ushort *)(aiStack_34[7] + 0x1a);
            if (*local_84 == '\x03') {
              aiStack_34[7] = (int)*(ushort *)(aiStack_34[7] + 0x14);
            }
            if (*local_84 == '\x03') {
              uVar4 = uVar4 | aiStack_34[7] << 0x10;
            }
            if ((*(int *)(local_84 + 0x14) - 2U <= uVar4 - 2) ||
               (iVar1 = (uVar4 - 2) * (uint)*(ushort *)(local_84 + 10) + *(int *)(local_84 + 0x28),
               iVar1 == 0)) {
              return 2;
            }
            if ((iVar1 != *(int *)(local_84 + 0x2c)) &&
               (iVar1 = FUN_00002f3c(local_84,iVar1), iVar1 != 0)) {
              return iVar1;
            }
            if (local_84[0x61] == '.') {
              local_84[0x7a] = (char)aiStack_34[2];
              local_84[0x7b] = (char)((uint)aiStack_34[2] >> 8);
              if (*local_84 == '\x03') {
                local_84[0x74] = (char)((uint)aiStack_34[2] >> 0x10);
                local_84[0x75] = (char)((uint)aiStack_34[2] >> 0x18);
              }
              local_84[3] = '\x01';
            }
          }
          iVar1 = FUN_00002f8c(local_60);
          if (iVar1 == 0) {
            iVar1 = FUN_00002e20(local_84);
          }
        }
      }
      else {
        iVar1 = 6;
      }
    }
  }
  return iVar1;
}

