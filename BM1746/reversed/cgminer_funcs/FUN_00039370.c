//==================== FUN_00039370 @ 00039370 ====================

void FUN_00039370(undefined4 *param_1,undefined4 *param_2,char *param_3,char *param_4,int param_5)

{
  char acStack_1010 [4096];
  
  if ((char *)*param_2 != (char *)*param_1) {
    snprintf(acStack_1010,0x1000,
             "List %s can\'t %s() a %s item - from %s %s() line %d in %s %s():%d",(char *)*param_1,
             "_k_add_head",(char *)*param_2,param_3,param_4,param_5,"klist.c","_k_add_head",0xcd);
    FUN_00038730(3,acStack_1010,1);
    FUN_00016ca8(1);
  }
  param_2[1] = 0;
  param_2[2] = param_1[3];
  if (param_1[3] != 0) {
    *(undefined4 **)(param_1[3] + 4) = param_2;
  }
  param_1[3] = param_2;
  if ((*(char *)(param_1 + 0xb) != '\0') && (param_1[4] == 0)) {
    param_1[4] = param_2;
  }
  param_1[7] = param_1[7] + 1;
  param_1[8] = param_1[8] + 1;
  return;
}

