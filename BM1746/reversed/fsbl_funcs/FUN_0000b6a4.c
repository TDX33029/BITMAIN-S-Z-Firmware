//==================== FUN_0000b6a4 @ 0000b6a4 ====================

undefined4 FUN_0000b6a4(undefined4 param_1)

{
  undefined4 uVar1;
  int iVar2;
  uint *puVar3;
  undefined8 uVar4;
  undefined4 local_10;
  undefined4 uStack_c;
  
  local_10 = 0;
  uStack_c = 0;
  uVar4 = FUN_0000b254(param_1,param_1);
  puVar3 = (uint *)((ulonglong)uVar4 >> 0x20);
  if ((int)uVar4 == 0) {
    if ((*puVar3 & 1) == 0) {
      *puVar3 = 1;
      FUN_0000bcd8(&local_10,puVar3,0x96,0);
      do {
        iVar2 = FUN_0000bd28(local_10,uStack_c);
      } while (iVar2 != 1);
    }
    uVar1 = 0;
  }
  else {
    uVar1 = 1;
  }
  return uVar1;
}

