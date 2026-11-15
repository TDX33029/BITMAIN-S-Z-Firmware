//==================== FUN_00014fd0 @ 00014fd0 ====================

undefined4 FUN_00014fd0(int param_1,int param_2,uint param_3)

{
  undefined4 uVar1;
  int iVar2;
  uint param3;
  uint uVar3;
  undefined1 auStack_21288 [131592];
  undefined1 auStack_1080 [28];
  uint local_1064;
  uint local_1060 [19];
  uint local_1014;
  char acStack_1010 [4096];
  
  param3 = *(uint *)(param_2 + 0x1c);
  iVar2 = 0;
  do {
    uVar3 = *(uint *)(param_1 + iVar2);
    *(uint *)(iVar2 + (int)local_1060) =
         uVar3 << 0x18 | (uVar3 >> 8 & 0xff) << 0x10 | (uVar3 >> 0x10 & 0xff) << 8 | uVar3 >> 0x18;
    iVar2 = iVar2 + 4;
  } while (iVar2 != 0x4c);
  local_1014 = param_3 << 0x18 | (param_3 >> 8 & 0xff) << 0x10 | (param_3 >> 0x10 & 0xff) << 8 |
               param_3 >> 0x18;
  FUN_000122e8(local_1060,auStack_21288,auStack_1080);
  uVar3 = local_1064 << 0x18 | (local_1064 >> 8 & 0xff) << 0x10 | (local_1064 >> 0x10 & 0xff) << 8 |
          local_1064 >> 0x18;
  if ((DAT_00078e09 != '\0') &&
     (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
    snprintf(acStack_1010,0x1000,"htarget %08lx diff1 %08lx hash %08lx",param3,0xffff,uVar3);
    FUN_00038730(7,acStack_1010,0);
  }
  if (uVar3 < 0x10000) {
    if (param3 < uVar3) {
      uVar1 = 0;
    }
    else {
      uVar1 = 1;
    }
  }
  else {
    uVar1 = 0xffffffff;
  }
  return uVar1;
}

