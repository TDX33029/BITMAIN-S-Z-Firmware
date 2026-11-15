//==================== FUN_00039460 @ 00039460 ====================

void FUN_00039460(undefined4 *param_1,undefined4 *param_2,char *param_3,char *param_4,int param_5)

{
  undefined4 uVar1;
  char acStack_1018 [4096];
  
  if ((char *)*param_2 != (char *)*param_1) {
    snprintf(acStack_1018,0x1000,
             "List %s can\'t %s() a %s item - from %s %s() line %d in %s %s():%d",(char *)*param_1,
             "_k_add_tail",(char *)*param_2,param_3,param_4,param_5,"klist.c","_k_add_tail",0xec);
    FUN_00038730(3,acStack_1018,1);
    FUN_00016ca8(1);
  }
  if (*(char *)(param_1 + 0xb) == '\0') {
    snprintf(acStack_1018,0x1000,
             "List %s can\'t %s() - do_tail is false - from %s %s() line %d in %s %s():%d",
             (char *)*param_1,"_k_add_tail",param_3,param_4,param_5,"klist.c","_k_add_tail",0xf1);
    FUN_00038730(3,acStack_1018,1);
    FUN_00016ca8(1);
  }
  uVar1 = param_1[4];
  param_2[2] = 0;
  param_2[1] = uVar1;
  if (param_1[4] != 0) {
    *(undefined4 **)(param_1[4] + 8) = param_2;
  }
  param_1[4] = param_2;
  if (param_1[3] == 0) {
    param_1[3] = param_2;
  }
  param_1[7] = param_1[7] + 1;
  param_1[8] = param_1[8] + 1;
  return;
}

