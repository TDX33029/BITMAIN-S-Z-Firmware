//==================== FUN_00049824 @ 00049824 ====================

int FUN_00049824(int param_1)

{
  undefined1 *puVar1;
  int iVar2;
  undefined1 *puVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  undefined1 uStack_39;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  
  puVar1 = (undefined1 *)(param_1 + 0x20);
  puVar3 = &uStack_39;
  local_38 = 0;
  local_34 = 0;
  local_30 = 0;
  local_2c = 0;
  local_28 = 0;
  local_24 = 0;
  local_20 = 0;
  local_1c = 0;
  do {
    puVar1 = puVar1 + -1;
    puVar3 = puVar3 + 1;
    *puVar3 = *puVar1;
  } while (puVar3 != (undefined1 *)((int)&local_1c + 3));
  iVar7 = 0;
  iVar5 = 0;
  do {
    iVar6 = iVar5 + 8;
    iVar4 = 7;
    do {
      iVar2 = FUN_00042958((int)&local_38 + iVar7,iVar4);
      iVar4 = iVar4 + -1;
      if (iVar2 != 0) {
        return iVar5;
      }
      iVar5 = iVar5 + 1;
    } while (iVar5 != iVar6);
    iVar7 = iVar7 + 1;
  } while (iVar7 != 0x20);
  return iVar5;
}

