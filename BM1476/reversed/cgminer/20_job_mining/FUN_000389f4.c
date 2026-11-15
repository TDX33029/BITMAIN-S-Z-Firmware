//==================== FUN_000389f4 @ 000389f4 ====================

void FUN_000389f4(int param_1,char *param_2,char *param_3,int param_4)

{
  int iVar1;
  
  if ((DAT_0007bc78 != '\0') && (param_1 != 3)) {
    return;
  }
  if (param_4 != 0) {
    pthread_mutex_trylock((pthread_mutex_t *)&DAT_0007b7a8);
    FUN_0003896c(0x26);
    (*DAT_00075ed8)();
  }
  if ((DAT_00075dd0 == '\0') || (iVar1 = FUN_000199e8(param_1,param_2,param_3), iVar1 == 0)) {
    FUN_000388e4();
    printf("%s%s%s",param_2,param_3,"                    \n");
    FUN_0003896c(0x31);
    (*DAT_00075ed8)();
    return;
  }
  return;
}

