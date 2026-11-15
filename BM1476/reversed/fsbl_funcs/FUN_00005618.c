//==================== FUN_00005618 @ 00005618 ====================

int FUN_00005618(undefined4 param_1)

{
  char cVar1;
  char cVar2;
  char cVar3;
  char cVar4;
  char cVar5;
  char cVar6;
  int iVar7;
  int iVar8;
  undefined4 uVar9;
  char *pcVar10;
  char *pcVar11;
  char *pcVar12;
  undefined4 local_6c;
  char *local_68;
  char *local_64 [4];
  char *local_54 [7];
  int local_38;
  
  local_6c = param_1;
  iVar7 = FUN_000030bc(&local_6c,&local_68,2);
  if (iVar7 == 0) {
    local_54[0] = local_68;
    iVar7 = FUN_00003f2c(local_54,local_6c);
    if (iVar7 == 0) {
      iVar7 = 8;
    }
    else if (iVar7 == 4) {
      local_64[0] = local_68;
      iVar8 = FUN_00003868(local_64,0);
      if (iVar8 == 0) {
        iVar7 = 7;
        FUN_00006760();
      }
      else if (iVar8 == 1) {
        FUN_00006760();
        iVar7 = 2;
      }
      else if (iVar8 == -1) {
        FUN_00006760();
        iVar7 = 1;
      }
      else {
        uVar9 = FUN_00006760();
        iVar7 = FUN_00002d5c(local_68,iVar8);
        if (iVar7 == 0) {
          pcVar11 = local_68 + 0x40;
          do {
            pcVar10 = pcVar11 + 1;
            *pcVar11 = ' ';
            pcVar11 = pcVar10;
          } while (local_68 + 0x4b != pcVar10);
          local_68[0x56] = (char)uVar9;
          cVar4 = (char)((uint)uVar9 >> 8);
          cVar6 = (char)((uint)iVar8 >> 8);
          cVar5 = (char)((uint)uVar9 >> 0x10);
          local_68[0x40] = '.';
          cVar2 = (char)((uint)uVar9 >> 0x18);
          local_68[0x59] = cVar2;
          local_68[0x5a] = (char)iVar8;
          local_68[0x4b] = '\x10';
          local_68[0x57] = cVar4;
          local_68[0x58] = cVar5;
          local_68[0x5b] = cVar6;
          cVar3 = (char)((uint)iVar8 >> 0x10);
          cVar1 = (char)((uint)iVar8 >> 0x18);
          if (*local_68 == '\x03') {
            local_68[0x54] = cVar3;
            local_68[0x55] = cVar1;
          }
          pcVar10 = local_68 + 0x3c;
          pcVar11 = local_68 + 0x60;
          do {
            pcVar10 = pcVar10 + 4;
            pcVar12 = pcVar11 + 4;
            *(undefined4 *)pcVar11 = *(undefined4 *)pcVar10;
            pcVar11 = pcVar12;
          } while (pcVar12 != local_68 + 0x80);
          local_68[0x61] = '.';
          local_68[0x7a] = (char)local_54[2];
          local_68[0x7b] = (char)((uint)local_54[2] >> 8);
          if (*local_68 == '\x03') {
            local_68[0x74] = (char)((uint)local_54[2] >> 0x10);
            local_68[0x75] = (char)((uint)local_54[2] >> 0x18);
          }
          local_68[3] = '\x01';
          iVar7 = FUN_00003e38(local_54);
          if (iVar7 == 0) {
            *(char *)(local_38 + 0x16) = (char)uVar9;
            *(char *)(local_38 + 0x17) = cVar4;
            *(char *)(local_38 + 0x18) = cVar5;
            *(char *)(local_38 + 0x19) = cVar2;
            *(char *)(local_38 + 0x1a) = (char)iVar8;
            *(char *)(local_38 + 0x1b) = cVar6;
            cVar2 = *local_68;
            *(undefined1 *)(local_38 + 0xb) = 0x10;
            if (cVar2 == '\x03') {
              *(char *)(local_38 + 0x14) = cVar3;
              *(char *)(local_38 + 0x15) = cVar1;
            }
            local_68[3] = '\x01';
            iVar7 = FUN_00002e20(local_68);
            return iVar7;
          }
        }
      }
      FUN_00003a48(local_54,iVar8,0);
    }
  }
  return iVar7;
}

