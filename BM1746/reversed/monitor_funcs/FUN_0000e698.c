//==================== FUN_0000e698 @ 0000e698 ====================

undefined4 FUN_0000e698(int param_1,int param_2,int param_3,code *param_4)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 *puVar5;
  undefined1 auStack_1038 [4096];
  int local_38;
  int local_34;
  int local_30;
  int local_2c;
  
  local_34 = param_1;
  local_30 = param_2;
  local_2c = param_3;
  if (param_2 != param_3) {
    local_38 = param_1;
    do {
      puVar5 = (undefined4 *)(local_38 + param_2 * 0x100f);
      uVar3 = *puVar5;
      uVar4 = puVar5[1];
      uVar2 = puVar5[2];
      uVar1 = puVar5[3];
      memcpy(auStack_1038,puVar5 + 4,0xfff);
      (*param_4)(uVar3,uVar4,uVar2,uVar1);
      param_2 = (param_2 + 1) % 9;
    } while (param_2 != param_3);
  }
  putchar(10);
  return 1;
}

