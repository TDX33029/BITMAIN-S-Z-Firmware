//==================== FUN_040092ec @ 040092ec ====================

int FUN_040092ec(undefined4 param_1,undefined4 param_2)

{
  int *piVar1;
  undefined4 uVar2;
  code *pcVar3;
  int iVar4;
  int iVar5;
  undefined4 local_14;
  
  local_14 = param_2;
  piVar1 = (int *)FUN_04008f68();
  if (piVar1 == (int *)0x0) {
    iVar4 = -0x13;
  }
  else if ((piVar1[0x12] & 1U) == 0) {
    iVar4 = -0x16;
  }
  else {
    iVar5 = 0x20;
    uVar2 = 1;
    do {
      pcVar3 = *(code **)(*(int *)(*piVar1 + 0x3c) + 8);
      iVar4 = (*pcVar3)(piVar1,uVar2,&local_14,pcVar3,param_1);
      if (iVar4 < 1) {
        if (iVar4 != 0) goto LAB_0400938c;
      }
      else {
        FUN_0400a0a8(local_14,iVar4);
      }
      pcVar3 = *(code **)(*(int *)(*piVar1 + 0x3c) + 0xc);
      if (pcVar3 != (code *)0x0) {
        (*pcVar3)(piVar1,local_14,iVar4);
      }
      if (iVar4 < 1) {
LAB_0400938c:
        if (iVar4 == -0xb) {
          return 0;
        }
        return iVar4;
      }
      iVar5 = iVar5 + -1;
      uVar2 = 0;
    } while (iVar5 != 0);
  }
  return iVar4;
}

