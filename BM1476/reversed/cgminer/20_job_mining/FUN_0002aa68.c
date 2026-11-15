//==================== FUN_0002aa68 @ 0002aa68 ====================

void FUN_0002aa68(int *param_1,char *param_2)

{
  char *__src;
  int iVar1;
  char acStack_1020 [16];
  char acStack_1010 [4096];
  
  __src = strchr(param_2,0x23);
  if ((__src != (char *)0x0) && ((char)param_1[0x97] == '\0')) {
    strcpy(acStack_1020,__src);
    *__src = '\0';
    iVar1 = strcmp(acStack_1020,"#xnsub");
    if (((iVar1 == 0) && (*(undefined1 *)(param_1 + 0x97) = 1, DAT_00078e09 != '\0')) &&
       (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
      snprintf(acStack_1010,0x1000,"Pool %d extranonce subscribing enabled.",*param_1);
      FUN_00038730(7,acStack_1010,0);
    }
  }
  return;
}

