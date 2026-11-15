//==================== FUN_00004d90 @ 00004d90 ====================

uint FUN_00004d90(undefined4 *param_1,uint param_2)

{
  undefined1 uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  uint uVar7;
  uint uVar8;
  int local_24 [2];
  
  uVar2 = FUN_00002bbc(param_1,local_24);
  if (uVar2 != 0) {
    return uVar2;
  }
  if (*(byte *)((int)param_1 + 0x11) != 0) {
    return (uint)*(byte *)((int)param_1 + 0x11);
  }
  if (((uint)param_1[3] < param_2) && ((*(byte *)(param_1 + 4) & 2) == 0)) {
    param_2 = param_1[3];
  }
  iVar6 = param_1[5];
  param_1[5] = 0;
  if (param_2 == 0) {
    return 0;
  }
  uVar7 = (uint)*(ushort *)(local_24[0] + 10);
  uVar2 = uVar7 * 0x200;
  if (iVar6 == 0) {
LAB_00004eec:
    uVar3 = param_1[2];
    if (uVar3 != 0) {
      uVar8 = 0;
LAB_00004efc:
      param_1[6] = uVar3;
      goto LAB_00004f00;
    }
    uVar3 = FUN_00003868(param_1,0);
    if (uVar3 != 1) {
      if (uVar3 == 0xffffffff) goto LAB_00004fc0;
      param_1[2] = uVar3;
      uVar8 = param_1[5];
      goto LAB_00004efc;
    }
LAB_00004fd4:
    uVar2 = 2;
    *(undefined1 *)((int)param_1 + 0x11) = 2;
  }
  else {
    uVar8 = iVar6 - 1;
    uVar3 = FUN_00001a50(param_2 - 1,uVar2);
    uVar4 = FUN_00001a50(uVar8,uVar2);
    if (uVar3 < uVar4) goto LAB_00004eec;
    uVar3 = param_1[6];
    uVar8 = uVar7 * -0x200 & uVar8;
    param_1[5] = uVar8;
    param_2 = param_2 - uVar8;
LAB_00004f00:
    iVar6 = 0;
    if (uVar3 != 0) {
      while (uVar2 < param_2) {
        param_1[5] = uVar2 + uVar8;
        param_2 = param_2 + uVar7 * -0x200;
        if ((*(byte *)(param_1 + 4) & 2) == 0) {
          uVar3 = FUN_000036fc(*param_1);
        }
        else {
          uVar3 = FUN_00003868(param_1,uVar3);
          if (uVar3 == 0) {
            uVar8 = param_1[5];
            iVar6 = 0;
            goto LAB_00004e7c;
          }
        }
        if (uVar3 == 0xffffffff) goto LAB_00004fc0;
        if ((uVar3 < 2) || (*(uint *)(local_24[0] + 0x14) <= uVar3)) goto LAB_00004fd4;
        param_1[6] = uVar3;
        uVar8 = param_1[5];
      }
      uVar8 = uVar8 + param_2;
      param_1[5] = uVar8;
      if ((param_2 & 0x1ff) == 0) {
        iVar6 = 0;
      }
      else {
        if ((*(int *)(local_24[0] + 0x14) - 2U <= uVar3 - 2) ||
           (iVar6 = (uint)*(ushort *)(local_24[0] + 10) * (uVar3 - 2) + *(int *)(local_24[0] + 0x28)
           , iVar6 == 0)) goto LAB_00004fd4;
        iVar6 = iVar6 + (param_2 >> 9);
      }
    }
LAB_00004e7c:
    if ((uint)param_1[3] < uVar8) {
      param_1[3] = uVar8;
      *(byte *)(param_1 + 4) = *(byte *)(param_1 + 4) | 0x40;
    }
    if ((uVar8 & 0x1ff) == 0) {
      return 0;
    }
    if (param_1[7] == iVar6) {
      return 0;
    }
    uVar1 = *(undefined1 *)(local_24[0] + 1);
    if (*(char *)(param_1 + 4) < '\0') {
      iVar5 = FUN_0000676c(uVar1,param_1 + 0x10,param_1[7],1);
      if (iVar5 != 0) goto LAB_00004fc0;
      *(byte *)(param_1 + 4) = *(byte *)(param_1 + 4) & 0x7f;
      uVar1 = *(undefined1 *)(local_24[0] + 1);
    }
    iVar5 = FUN_0000667c(uVar1,param_1 + 0x10,iVar6,1);
    if (iVar5 != 0) {
LAB_00004fc0:
      *(undefined1 *)((int)param_1 + 0x11) = 1;
      return 1;
    }
    param_1[7] = iVar6;
    uVar2 = 0;
  }
  return uVar2;
}

