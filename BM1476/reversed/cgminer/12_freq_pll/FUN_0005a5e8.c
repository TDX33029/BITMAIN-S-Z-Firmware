//==================== FUN_0005a5e8 @ 0005a5e8 ====================

undefined4 FUN_0005a5e8(int *param_1,uint param_2,int *param_3)

{
  void *__src;
  void *__dest;
  int iVar1;
  
  if (param_3 != (int *)0x0) {
    if ((((param_1 != (int *)0x0) && (*param_1 == 1)) && (param_1 != param_3)) &&
       ((param_2 <= (uint)param_1[3] &&
        (__src = (void *)FUN_00059d20(param_1,1,0), __src != (void *)0x0)))) {
      __dest = (void *)param_1[4];
      if (__dest == __src) {
        memmove((void *)((int)__dest + (param_2 + 1) * 4),(void *)((int)__dest + param_2 * 4),
                (param_1[3] - param_2) * 4);
      }
      else {
        memcpy(__dest,__src,param_2 * 4);
        memcpy((void *)(param_1[4] + (param_2 + 1) * 4),(void *)((int)__src + param_2 * 4),
               (param_1[3] - param_2) * 4);
        FUN_00059594(__src);
      }
      iVar1 = param_1[3];
      *(int **)(param_1[4] + param_2 * 4) = param_3;
      param_1[3] = iVar1 + 1;
      return 0;
    }
    if (param_3[1] != -1) {
      iVar1 = param_3[1] + -1;
      param_3[1] = iVar1;
      if (iVar1 == 0) {
        FUN_0005a3e4(param_3);
      }
    }
  }
  return 0xffffffff;
}

