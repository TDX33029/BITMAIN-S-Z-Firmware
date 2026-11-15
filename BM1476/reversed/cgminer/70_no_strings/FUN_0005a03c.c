//==================== FUN_0005a03c @ 0005a03c ====================

undefined4 FUN_0005a03c(int *param_1,int *param_2,undefined4 param_3,undefined4 param_4)

{
  size_t __n;
  int iVar1;
  int *piVar3;
  int *piVar2;
  int *__src;
  int iVar4;
  
  if ((((param_1 == (int *)0x0) || (*param_1 != 1)) || (param_2 == (int *)0x0)) ||
     ((*param_2 != 1 || (iVar1 = FUN_00059d20(param_1,param_2[3],1,1,param_4), iVar1 == 0)))) {
    return 0xffffffff;
  }
  iVar1 = param_2[3];
  __src = (int *)param_2[4];
  __n = 0;
  if (iVar1 != 0) {
    __n = iVar1 * 4;
    piVar2 = __src;
    do {
      piVar3 = piVar2 + 1;
      iVar4 = *piVar2;
      if ((iVar4 != 0) && (*(int *)(iVar4 + 4) != -1)) {
        *(int *)(iVar4 + 4) = *(int *)(iVar4 + 4) + 1;
      }
      piVar2 = piVar3;
    } while (piVar3 != __src + iVar1);
  }
  memcpy((void *)(param_1[4] + param_1[3] * 4),__src,__n);
  param_1[3] = param_1[3] + param_2[3];
  return 0;
}

