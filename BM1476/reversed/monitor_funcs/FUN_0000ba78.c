//==================== FUN_0000ba78 @ 0000ba78 ====================

undefined * FUN_0000ba78(char *param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  FUN_0000b46c(DAT_0001c3d0);
  iVar1 = DAT_0001b3a0;
  if (0 < DAT_0001b3a0) {
    iVar3 = 0;
    do {
      iVar2 = strcasecmp(&DAT_00024490 + iVar3 * 0x81,param_1);
      if (iVar2 == 0) {
        DAT_0001b3a0 = 0;
        memset(&DAT_00024490,0,0x1428);
        return &DAT_0001f468 + iVar3 * 0x201;
      }
      iVar3 = iVar3 + 1;
    } while (iVar3 != iVar1);
  }
  memset(&DAT_00024490,0,0x1428);
  DAT_0001b3a0 = 0;
  printf("get %s failed\n",param_1);
  return (undefined *)0x0;
}

