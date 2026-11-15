//==================== FUN_00054f8c @ 00054f8c ====================

undefined4 FUN_00054f8c(int param_1,void *param_2,uint param_3,int param_4)

{
  uint uVar1;
  int iVar2;
  uint __n;
  bool bVar3;
  
  if (param_3 != 0 || param_4 != 0) {
    iVar2 = *(int *)(param_1 + 0x160);
    do {
      __n = 0x100 - iVar2;
      bVar3 = param_4 == 0;
      if (param_4 == 0) {
        bVar3 = param_3 <= __n;
      }
      if (bVar3) {
        memcpy((void *)(param_1 + iVar2 + 0x60),param_2,param_3);
        *(uint *)(param_1 + 0x160) = *(int *)(param_1 + 0x160) + param_3;
        return 0;
      }
      memcpy((void *)(param_1 + iVar2 + 0x60),param_2,__n);
      uVar1 = *(uint *)(param_1 + 0x40) + 0x80;
      iVar2 = *(int *)(param_1 + 0x44) + (uint)(0xffffff7f < *(uint *)(param_1 + 0x40));
      *(uint *)(param_1 + 0x40) = uVar1;
      *(int *)(param_1 + 0x44) = iVar2;
      bVar3 = iVar2 == 0;
      if (iVar2 == 0) {
        bVar3 = uVar1 < 0x80;
      }
      uVar1 = *(uint *)(param_1 + 0x48);
      param_2 = (void *)((int)param_2 + __n);
      *(uint *)(param_1 + 0x48) = uVar1 + bVar3;
      *(uint *)(param_1 + 0x4c) = *(int *)(param_1 + 0x4c) + (uint)CARRY4(uVar1,(uint)bVar3);
      *(uint *)(param_1 + 0x160) = *(int *)(param_1 + 0x160) + __n;
      bVar3 = param_3 < __n;
      param_3 = param_3 - __n;
      param_4 = param_4 - (uint)bVar3;
      FUN_0004ef68(param_1,(void *)(param_1 + 0x60));
      memcpy((void *)(param_1 + 0x60),(void *)(param_1 + 0xe0),0x80);
      iVar2 = *(int *)(param_1 + 0x160) + -0x80;
      *(int *)(param_1 + 0x160) = iVar2;
    } while (param_3 != 0 || param_4 != 0);
  }
  return 0;
}

