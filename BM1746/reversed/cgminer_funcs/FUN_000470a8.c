//==================== FUN_000470a8 @ 000470a8 ====================

undefined4 FUN_000470a8(void)

{
  int *piVar1;
  undefined4 uVar2;
  int iVar3;
  FILE *__stream;
  char cVar4;
  int iVar5;
  int iVar6;
  int param4;
  int iVar7;
  int iVar8;
  char *pcVar9;
  undefined4 *puVar10;
  char *pcVar11;
  char local_104c [4136];
  
  puVar10 = &DAT_0007c130;
  if (DAT_0007c130 < 1) {
    __stream = fopen("/config/chips_freq.config","w+");
    if (__stream == (FILE *)0x0) {
      if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 5)) {
        uVar2 = 0xffffffff;
      }
      else {
        snprintf(local_104c + 0x24,0x1000,"%s open %s failed\n","scan_freq_save_freq",
                 "/config/chips_freq.config");
        FUN_00038730(5,local_104c + 0x24,0);
        uVar2 = 0xffffffff;
      }
    }
    else {
      pcVar9 = &DAT_0009d4bb;
      do {
        pcVar9 = pcVar9 + 1;
        if (*pcVar9 == '\x01') {
          iVar6 = 0;
          do {
            iVar7 = iVar6 + 4;
            fprintf(__stream,"%d ",*(undefined4 *)((int)puVar10 + iVar6 + 0xf4));
            iVar6 = iVar7;
          } while (iVar7 != 0xc);
        }
        puVar10 = puVar10 + 3;
      } while (pcVar9 != &DAT_0009d4bf);
      fclose(__stream);
      uVar2 = 0;
    }
  }
  else {
    pcVar9 = &DAT_0009d4bb;
    iVar7 = 0;
    iVar6 = 0;
    local_104c[0] = '\0';
    local_104c[1] = '\0';
    local_104c[2] = '\0';
    local_104c[3] = '\0';
    local_104c[4] = '\0';
    local_104c[5] = '\0';
    local_104c[6] = '\0';
    local_104c[7] = '\0';
    local_104c[8] = '\0';
    local_104c[9] = '\0';
    local_104c[10] = '\0';
    local_104c[0xb] = '\0';
    local_104c[0xc] = '\0';
    local_104c[0xd] = '\0';
    local_104c[0xe] = '\0';
    local_104c[0xf] = '\0';
    local_104c[0x10] = '\0';
    local_104c[0x11] = '\0';
    local_104c[0x12] = '\0';
    local_104c[0x13] = '\0';
    local_104c[0x14] = '\0';
    local_104c[0x15] = '\0';
    local_104c[0x16] = '\0';
    local_104c[0x17] = '\0';
    local_104c[0x18] = '\0';
    local_104c[0x19] = '\0';
    local_104c[0x1a] = '\0';
    local_104c[0x1b] = '\0';
    local_104c[0x1c] = '\0';
    local_104c[0x1d] = '\0';
    local_104c[0x1e] = '\0';
    local_104c[0x1f] = '\0';
    local_104c[0x20] = '\0';
    local_104c[0x21] = '\0';
    local_104c[0x22] = '\0';
    local_104c[0x23] = '\0';
    do {
      pcVar9 = pcVar9 + 1;
      if (*pcVar9 == '\x01') {
        pcVar11 = local_104c + iVar6 * 9;
        iVar8 = iVar6 * 0xc + 0x7c220;
        param4 = 0;
        local_104c[iVar7] = 'N';
        do {
          if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 5)) {
            iVar3 = *(int *)(iVar8 + 4);
            if (iVar3 != 100) goto LAB_0004719c;
LAB_0004723c:
            cVar4 = '\0';
          }
          else {
            snprintf(local_104c + 0x24,0x1000,"pic write chain%d asic%d freq %d\n",iVar6,param4,
                     *(int *)(iVar8 + 4));
            FUN_00038730(5,local_104c + 0x24,0);
            iVar3 = *(int *)(iVar8 + 4);
            if (iVar3 == 100) goto LAB_0004723c;
LAB_0004719c:
            puVar10 = &DAT_00069204;
            iVar5 = 1;
            do {
              piVar1 = puVar10 + 4;
              puVar10 = puVar10 + 4;
              if (iVar3 == *piVar1) {
                cVar4 = (char)iVar5;
                goto LAB_000471c8;
              }
              iVar5 = iVar5 + 1;
            } while (iVar5 != 0x73);
            cVar4 = -1;
          }
LAB_000471c8:
          iVar8 = iVar8 + 4;
          param4 = param4 + 1;
          pcVar11 = pcVar11 + 1;
          *pcVar11 = cVar4;
        } while (param4 != 3);
        FUN_00056b3c(local_104c + iVar6 * 9 + 1,2,&DAT_00069af8 + iVar6 * 0x10);
      }
      iVar6 = iVar6 + 1;
      iVar7 = iVar7 + 9;
    } while (iVar6 != 4);
    FUN_00046b94(local_104c);
    uVar2 = 0;
  }
  return uVar2;
}

