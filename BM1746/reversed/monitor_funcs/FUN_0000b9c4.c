//==================== FUN_0000b9c4 @ 0000b9c4 ====================

void FUN_0000b9c4(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  printf("g_var_num == %d\n",DAT_0001b3a0);
  if (DAT_0001b3a0 < 1) {
    return;
  }
  iVar3 = 0;
  do {
    iVar1 = iVar3 * 0x81;
    iVar2 = iVar3 * 0x201;
    iVar3 = iVar3 + 1;
    printf("%s = %s\n",&DAT_00024490 + iVar1,&DAT_0001f468 + iVar2);
  } while (iVar3 < DAT_0001b3a0);
  return;
}

