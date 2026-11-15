//==================== FUN_00029830 @ 00029830 ====================

int FUN_00029830(byte *param_1,byte *param_2,uint param_3)

{
  int iVar1;
  uint uVar2;
  char local_1010 [4];
  char acStack_100c [4];
  char acStack_1008 [4];
  char acStack_1004 [4];
  char local_1000 [4];
  undefined2 local_ffc;
  
  if (*param_2 == 0) {
LAB_000298bc:
    iVar1 = 1 - param_3;
    if (1 < param_3) {
      iVar1 = 0;
    }
  }
  else {
    if (param_3 != 0) {
      uVar2 = (uint)param_2[1];
      if (uVar2 != 0) {
        iVar1 = (uint)*param_2 * 4;
        do {
          if ((int)(*(uint *)(&DAT_00064e8c + uVar2 * 4) | *(uint *)(&DAT_00064e8c + iVar1)) < 0) {
            if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
              local_1010[0] = 'h';
              local_1010[1] = 'e';
              local_1010[2] = 'x';
              local_1010[3] = '2';
              acStack_100c[0] = 'b';
              acStack_100c[1] = 'i';
              acStack_100c[2] = 'n';
              acStack_100c[3] = ' ';
              acStack_1008[0] = 's';
              acStack_1008[1] = 'c';
              acStack_1008[2] = 'a';
              acStack_1008[3] = 'n';
              acStack_1004[0] = ' ';
              acStack_1004[1] = 'f';
              acStack_1004[2] = 'a';
              acStack_1004[3] = 'i';
              local_1000[0] = 'l';
              local_1000[1] = 'e';
              local_1000[2] = 'd';
              local_1000[3] = '\0';
              FUN_00038730(3,local_1010,0);
              return 0;
            }
            goto LAB_00029924;
          }
          param_3 = param_3 - 1;
          *param_1 = (byte)*(uint *)(&DAT_00064e8c + uVar2 * 4) |
                     (byte)((*(uint *)(&DAT_00064e8c + iVar1) & 0xff) << 4);
          uVar2 = (uint)param_2[2];
          iVar1 = uVar2 << 2;
          if (uVar2 == 0) goto LAB_000298bc;
          if (param_3 == 0) goto LAB_00029924;
          uVar2 = (uint)param_2[3];
          param_1 = param_1 + 1;
          param_2 = param_2 + 2;
        } while (uVar2 != 0);
      }
      if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
        local_1010[0] = 'h';
        local_1010[1] = 'e';
        local_1010[2] = 'x';
        local_1010[3] = '2';
        acStack_100c[0] = 'b';
        acStack_100c[1] = 'i';
        acStack_100c[2] = 'n';
        acStack_100c[3] = ' ';
        acStack_1008[0] = 's';
        acStack_1008[1] = 't';
        acStack_1008[2] = 'r';
        acStack_1008[3] = ' ';
        acStack_1004[0] = 't';
        acStack_1004[1] = 'r';
        acStack_1004[2] = 'u';
        acStack_1004[3] = 'n';
        local_1000[0] = 'c';
        local_1000[1] = 'a';
        local_1000[2] = 't';
        local_1000[3] = 'e';
        local_ffc = 100;
        FUN_00038730(3,local_1010,0);
      }
    }
LAB_00029924:
    iVar1 = 0;
  }
  return iVar1;
}

