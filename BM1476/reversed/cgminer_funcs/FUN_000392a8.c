//==================== FUN_000392a8 @ 000392a8 ====================

void FUN_000392a8(undefined4 *param_1,char *param_2,char *param_3,int param_4)

{
  int iVar1;
  int iVar2;
  char acStack_1010 [4096];
  
  if (*(char *)(param_1 + 0xb) == '\0') {
    snprintf(acStack_1010,0x1000,
             "List %s can\'t %s() - do_tail is false - from %s %s() line %d in %s %s():%d",
             (char *)*param_1,"_k_unlink_tail",param_2,param_3,param_4,"klist.c","_k_unlink_tail",
             0xb5);
    FUN_00038730(3,acStack_1010,1);
    FUN_00016ca8(1);
  }
  iVar1 = param_1[4];
  if (iVar1 != 0) {
    iVar2 = *(int *)(iVar1 + 4);
    param_1[4] = iVar2;
    if (iVar2 == 0) {
      param_1[3] = 0;
    }
    else {
      *(undefined4 *)(iVar2 + 8) = 0;
    }
    *(undefined4 *)(iVar1 + 8) = 0;
    *(undefined4 *)(iVar1 + 4) = 0;
    param_1[7] = param_1[7] + -1;
  }
  return;
}

