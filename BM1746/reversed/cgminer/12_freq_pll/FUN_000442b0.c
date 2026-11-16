//==================== FUN_000442b0 @ 000442b0 ====================

void FUN_000442b0(void)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  char *pcVar5;
  uint uVar6;
  int local_1030;
  char acStack_1028 [4100];
  
  iVar4 = 0;
  pcVar5 = &DAT_0009d4bb;
  do {
    if (pcVar5[1] != '\0') {
      if (pcVar5[0x20a] == '\0') {
        local_1030 = iVar4 << 3;
        uVar2 = 0;
      }
      else {
        local_1030 = iVar4 * 8;
        uVar3 = 0;
        uVar2 = 0;
        do {
          if ((uVar2 & 7) == 0) {
            uVar6 = uVar2 + uVar3;
            if ((0x13 < uVar6) &&
               (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)))) {
              snprintf(acStack_1028,0x1000,"offset[%d] ERR",uVar6);
              FUN_00038730(3,acStack_1028,0);
            }
            uVar3 = uVar3 + 1 & 0xff;
            (&DAT_0009d4b4)[uVar6 + iVar4 * 0x13 + 0x163] = 0x20;
          }
          uVar6 = uVar2 + uVar3;
          if ((0x13 < uVar6) &&
             (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)))) {
            snprintf(acStack_1028,0x1000,"offset[%d] ERR",uVar6);
            FUN_00038730(3,acStack_1028,0);
          }
          (&DAT_0009d4b4)[uVar6 + iVar4 * 0x13 + 0x163] = 0x6f;
          iVar1 = (iVar4 * 3 + uVar2 + 0x40) * 4;
          uVar2 = uVar2 + 1 & 0xff;
          uVar6 = *(uint *)(iVar1 + 0x9d4b8);
          (&DAT_0009d4b7)[iVar1] = 0;
          *(uint *)(iVar1 + 0x9d4b8) = uVar6 & 0xff000000;
        } while (uVar2 < (byte)pcVar5[0x20a]);
        uVar2 = uVar2 + uVar3;
        if ((0x13 < uVar2) &&
           (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)))) {
          snprintf(acStack_1028,0x1000,"offset[%d] ERR",uVar2);
          FUN_00038730(3,acStack_1028,0);
        }
      }
      (&DAT_0009d617)[local_1030 * 2 + iVar4 * 3 + uVar2] = 0;
    }
    iVar4 = iVar4 + 1;
    pcVar5 = pcVar5 + 1;
  } while (iVar4 != 4);
  return;
}

