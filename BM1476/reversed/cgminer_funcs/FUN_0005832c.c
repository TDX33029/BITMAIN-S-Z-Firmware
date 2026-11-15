//==================== FUN_0005832c @ 0005832c ====================

uint FUN_0005832c(undefined4 *param_1,undefined4 param_2)

{
  undefined1 uVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  undefined4 uVar6;
  undefined4 *puVar7;
  
  if (param_1[5] != 0) {
    return param_1[5];
  }
  uVar2 = (uint)*(byte *)((int)param_1 + param_1[4] + 8);
  if (uVar2 == 0) {
    uVar3 = (*(code *)*param_1)(param_1[1]);
    if (uVar3 == 0xffffffff) {
      param_1[5] = 0xffffffff;
      return 0xffffffff;
    }
    param_1[4] = 0;
    uVar2 = uVar3 & 0xff;
    if (0x7f < uVar3 - 0x80) {
      *(undefined1 *)((int)param_1 + 9) = 0;
    }
    *(char *)(param_1 + 2) = (char)uVar3;
    if (uVar3 - 0x80 < 0x80) {
      iVar5 = FUN_00059ac4();
      if (iVar5 != 0) {
        if (iVar5 < 2) {
                    /* WARNING: Subroutine does not return */
          __assert_fail("count >= 2","load.c",0xaf,"stream_get");
        }
        puVar7 = param_1;
        do {
          uVar1 = (*(code *)*param_1)(param_1[1]);
          *(undefined1 *)((int)puVar7 + 9) = uVar1;
          puVar7 = (undefined4 *)((int)puVar7 + 1);
        } while (puVar7 != (undefined4 *)((int)param_1 + iVar5 + -1));
        iVar4 = FUN_00059b40(param_1 + 2,iVar5,0);
        if (iVar4 != 0) {
          iVar4 = param_1[4];
          *(undefined1 *)((int)param_1 + iVar5 + 8) = 0;
          iVar5 = iVar4 + 1;
          uVar2 = (uint)*(byte *)((int)param_1 + iVar4 + 8);
          goto LAB_0005835c;
        }
      }
      param_1[5] = 0xfffffffe;
      FUN_00058034(param_2,param_1,"unable to decode byte 0x%x",uVar3);
      return 0xfffffffe;
    }
    iVar5 = 1;
  }
  else {
    iVar5 = param_1[4] + 1;
  }
LAB_0005835c:
  param_1[4] = iVar5;
  param_1[9] = param_1[9] + 1;
  if (uVar2 == 10) {
    uVar6 = param_1[7];
    param_1[7] = 0;
    param_1[8] = uVar6;
    param_1[6] = param_1[6] + 1;
    return 10;
  }
  iVar5 = FUN_00059ac4();
  if (iVar5 != 0) {
    param_1[7] = param_1[7] + 1;
  }
  return uVar2;
}

