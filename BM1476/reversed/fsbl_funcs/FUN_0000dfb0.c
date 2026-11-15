//==================== FUN_0000dfb0 @ 0000dfb0 ====================

uint * FUN_0000dfb0(uint *param_1,uint param_2)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  
  param_2 = param_2 & 0xff;
  if (param_2 == 0) {
    if (((uint)param_1 & 3) != 0) {
      if ((byte)*param_1 == 0) {
        return param_1;
      }
      while (param_1 = (uint *)((int)param_1 + 1), ((uint)param_1 & 3) != 0) {
        if (*(byte *)param_1 == 0) {
          return param_1;
        }
      }
    }
    uVar1 = *param_1 + 0xfefefeff & ~*param_1;
    while ((uVar1 & 0x80808080) == 0) {
      param_1 = param_1 + 1;
      uVar1 = *param_1 + 0xfefefeff & ~*param_1;
    }
    if ((byte)*param_1 == 0) {
      return param_1;
    }
    do {
      param_1 = (uint *)((int)param_1 + 1);
    } while (*(byte *)param_1 != 0);
    return param_1;
  }
  if (((uint)param_1 & 3) != 0) {
    if ((byte)*param_1 == 0) {
      return (uint *)0x0;
    }
    if (param_2 == (byte)*param_1) {
      return param_1;
    }
    while (param_1 = (uint *)((int)param_1 + 1), ((uint)param_1 & 3) != 0) {
      if (*(byte *)param_1 == 0) {
        return (uint *)0x0;
      }
      if (*(byte *)param_1 == param_2) {
        return param_1;
      }
    }
  }
  uVar2 = param_2 | param_2 << 8;
  uVar1 = *param_1;
  uVar2 = uVar2 | uVar2 << 0x10;
  uVar3 = uVar2 ^ uVar1;
  for (uVar1 = uVar3 + 0xfefefeff & ~uVar3 | uVar1 + 0xfefefeff & ~uVar1; (uVar1 & 0x80808080) == 0;
      uVar1 = uVar1 + 0xfefefeff & ~uVar1 | uVar3 + 0xfefefeff & ~uVar3) {
    param_1 = param_1 + 1;
    uVar1 = *param_1;
    uVar3 = uVar1 ^ uVar2;
  }
  if ((byte)*param_1 != 0) {
    if (param_2 == (byte)*param_1) {
      return param_1;
    }
    while( true ) {
      param_1 = (uint *)((int)param_1 + 1);
      if (*(byte *)param_1 == 0) break;
      if (*(byte *)param_1 == param_2) {
        return param_1;
      }
    }
  }
  return (uint *)0x0;
}

