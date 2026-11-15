//==================== FUN_000150f8 @ 000150f8 ====================

undefined1
FUN_000150f8(int param_1,undefined4 param_2,int param_3,undefined4 param_4,undefined4 param_5,
            int param_6,uint param_7,uint *param_8,uint param_9)

{
  void *__ptr;
  uint uVar1;
  int iVar2;
  undefined1 uVar3;
  uint uVar4;
  uint local_1078 [19];
  uint local_102c;
  char local_1028 [4];
  char acStack_1024 [4];
  char acStack_1020 [4];
  char acStack_101c [4];
  char local_1018 [4];
  char acStack_1014 [4];
  char acStack_1010 [4];
  uint local_100c;
  char local_1008 [4];
  char acStack_1004 [4];
  char acStack_1000 [4];
  undefined2 uStack_ffc;
  undefined1 local_ffa;
  
  uVar4 = *(uint *)(param_6 + 0x1c);
  iVar2 = 0;
  do {
    uVar1 = *(uint *)(param_3 + iVar2);
    *(uint *)((int)local_1078 + iVar2) =
         uVar1 << 0x18 | (uVar1 >> 8 & 0xff) << 0x10 | (uVar1 >> 0x10 & 0xff) << 8 | uVar1 >> 0x18;
    iVar2 = iVar2 + 4;
  } while (iVar2 != 0x4c);
  __ptr = malloc(0x20200);
  if (__ptr == (void *)0x0) {
    if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (uVar3 = 0, 2 < DAT_000766c4)) {
      uVar3 = 0;
      local_1028[0] = 'F';
      local_1028[1] = 'a';
      local_1028[2] = 'i';
      local_1028[3] = 'l';
      acStack_1024[0] = 'e';
      acStack_1024[1] = 'd';
      acStack_1024[2] = ' ';
      acStack_1024[3] = 't';
      acStack_1020[0] = 'o';
      acStack_1020[1] = ' ';
      acStack_1020[2] = 'm';
      acStack_1020[3] = 'a';
      acStack_101c[0] = 'l';
      acStack_101c[1] = 'l';
      acStack_101c[2] = 'o';
      acStack_101c[3] = 'c';
      local_1018[0] = ' ';
      local_1018[1] = 's';
      local_1018[2] = 'c';
      local_1018[3] = 'r';
      acStack_1014[0] = 'a';
      acStack_1014[1] = 't';
      acStack_1014[2] = 'c';
      acStack_1014[3] = 'h';
      acStack_1010[0] = 'b';
      acStack_1010[1] = 'u';
      acStack_1010[2] = 'f';
      acStack_1010[3] = ' ';
      local_100c._0_1_ = 'i';
      local_100c._1_1_ = 'n';
      local_100c._2_1_ = ' ';
      local_100c._3_1_ = 's';
      local_1008[0] = 'c';
      local_1008[1] = 'a';
      local_1008[2] = 'n';
      local_1008[3] = 'h';
      acStack_1004[0] = 'a';
      acStack_1004[1] = 's';
      acStack_1004[2] = 'h';
      acStack_1004[3] = '_';
      acStack_1000[0] = 's';
      acStack_1000[1] = 'c';
      acStack_1000[2] = 'r';
      acStack_1000[3] = 'y';
      uStack_ffc = 0x7470;
      local_ffa = 0;
      FUN_00038730(3,local_1028,0);
    }
  }
  else {
    do {
      param_9 = param_9 + 1;
      *(uint *)(param_3 + 0x4c) = param_9;
      uVar1 = param_9 * 0x1000000 | (param_9 >> 8 & 0xff) << 0x10 | (param_9 >> 0x10 & 0xff) << 8 |
              param_9 >> 0x18;
      local_102c = uVar1;
      FUN_000122e8(local_1078,__ptr,local_1028);
      if ((local_100c << 0x18 | (local_100c >> 8 & 0xff) << 0x10 | (local_100c >> 0x10 & 0xff) << 8
          | local_100c >> 0x18) <= uVar4) {
        *(uint *)(param_3 + 0x4c) = uVar1;
        uVar3 = 1;
        *param_8 = param_9;
        goto LAB_000151c8;
      }
    } while ((param_9 < param_7) && (*(char *)(param_1 + 0x3e) == '\0'));
    uVar3 = 0;
    *param_8 = param_9;
LAB_000151c8:
    free(__ptr);
  }
  return uVar3;
}

