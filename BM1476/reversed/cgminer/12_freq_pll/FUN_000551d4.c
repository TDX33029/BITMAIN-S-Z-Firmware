//==================== FUN_000551d4 @ 000551d4 ====================

undefined4 FUN_000551d4(int param_1,void *param_2,uint param_3)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  undefined4 uVar4;
  size_t __n;
  undefined4 uVar5;
  uint uVar6;
  undefined4 *puVar7;
  undefined1 *puVar8;
  undefined1 local_68 [68];
  
  puVar8 = local_68;
  memset(local_68,0,0x40);
  if (((param_2 == (void *)0x0 || param_3 == 0) || (0x40 < param_3)) ||
     (*(int *)(param_1 + 0x50) != 0 || *(int *)(param_1 + 0x54) != 0)) {
    uVar4 = 0xffffffff;
  }
  else {
    uVar3 = *(uint *)(param_1 + 0x160);
    if (0x80 < uVar3) {
      uVar1 = *(uint *)(param_1 + 0x48);
      uVar3 = *(uint *)(param_1 + 0x40) + 0x80;
      iVar2 = *(int *)(param_1 + 0x44) + (uint)(0xffffff7f < *(uint *)(param_1 + 0x40));
      *(uint *)(param_1 + 0x40) = uVar3;
      *(int *)(param_1 + 0x44) = iVar2;
      uVar3 = (uint)(iVar2 == 0 && uVar3 < 0x80);
      *(uint *)(param_1 + 0x48) = uVar1 + uVar3;
      *(uint *)(param_1 + 0x4c) = *(int *)(param_1 + 0x4c) + (uint)CARRY4(uVar1,uVar3);
      FUN_0004ef68(param_1,(void *)(param_1 + 0x60));
      __n = *(int *)(param_1 + 0x160) - 0x80;
      *(size_t *)(param_1 + 0x160) = __n;
      memcpy((void *)(param_1 + 0x60),(void *)(param_1 + 0xe0),__n);
      uVar3 = *(uint *)(param_1 + 0x160);
    }
    uVar6 = *(uint *)(param_1 + 0x48);
    uVar1 = *(uint *)(param_1 + 0x40) + uVar3;
    iVar2 = *(int *)(param_1 + 0x44) + (uint)CARRY4(*(uint *)(param_1 + 0x40),uVar3);
    *(uint *)(param_1 + 0x40) = uVar1;
    *(int *)(param_1 + 0x44) = iVar2;
    uVar1 = (uint)(iVar2 == 0 && uVar1 < uVar3);
    *(uint *)(param_1 + 0x48) = uVar6 + uVar1;
    *(uint *)(param_1 + 0x4c) = *(int *)(param_1 + 0x4c) + (uint)CARRY4(uVar6,uVar1);
    if (*(char *)(param_1 + 0x164) != '\0') {
      *(undefined8 *)(param_1 + 0x58) = 0xffffffffffffffff;
    }
    *(undefined8 *)(param_1 + 0x50) = 0xffffffffffffffff;
    memset((void *)(param_1 + uVar3 + 0x60),0,0x100 - uVar3);
    FUN_0004ef68(param_1,param_1 + 0x60);
    puVar7 = (undefined4 *)(param_1 + -8);
    do {
      uVar4 = puVar7[2];
      uVar5 = puVar7[3];
      puVar8[1] = (char)((uint)uVar4 >> 8);
      *puVar8 = (char)uVar4;
      puVar8[4] = (char)uVar5;
      puVar8[2] = (char)((uint)uVar4 >> 0x10);
      puVar8[3] = (char)((uint)uVar4 >> 0x18);
      puVar8[5] = (char)((uint)uVar5 >> 8);
      puVar8[6] = (char)((uint)uVar5 >> 0x10);
      puVar8[7] = (char)((uint)uVar5 >> 0x18);
      puVar8 = puVar8 + 8;
      puVar7 = puVar7 + 2;
    } while (puVar8 != local_68 + 0x40);
    memcpy(param_2,local_68,param_3);
    uVar4 = 0;
  }
  return uVar4;
}

