//==================== FUN_0002a4c8 @ 0002a4c8 ====================

int * FUN_0002a4c8(int *param_1,int *param_2)

{
  int iVar1;
  int iVar2;
  
  iVar1 = *param_1;
  iVar2 = *param_2;
  if (iVar1 == iVar2) {
    return (int *)(uint)(param_2[1] < param_1[1]);
  }
  if (iVar1 < iVar2) {
    param_1 = (int *)0x0;
  }
  if (iVar2 < iVar1) {
    param_1 = (int *)0x1;
  }
  return param_1;
}

