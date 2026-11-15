//==================== FUN_00042958 @ 00042958 ====================

uint FUN_00042958(int param_1,int param_2)

{
  switch(param_2 % 8) {
  case 0:
    if (param_2 < 0) {
      param_2 = param_2 + 7;
    }
    break;
  case 1:
    if (param_2 < 0) {
      param_2 = param_2 + 7;
    }
    return (*(byte *)(param_1 + (param_2 >> 3)) & 3) >> 1;
  case 2:
    if (param_2 < 0) {
      param_2 = param_2 + 7;
    }
    return (*(byte *)(param_1 + (param_2 >> 3)) & 7) >> 2;
  case 3:
    if (param_2 < 0) {
      param_2 = param_2 + 7;
    }
    return (*(byte *)(param_1 + (param_2 >> 3)) & 0xf) >> 3;
  case 4:
    if (param_2 < 0) {
      param_2 = param_2 + 7;
    }
    return (*(byte *)(param_1 + (param_2 >> 3)) & 0x1f) >> 4;
  case 5:
    if (param_2 < 0) {
      param_2 = param_2 + 7;
    }
    return (*(byte *)(param_1 + (param_2 >> 3)) & 0x3f) >> 5;
  case 6:
    if (param_2 < 0) {
      param_2 = param_2 + 7;
    }
    return (*(byte *)(param_1 + (param_2 >> 3)) & 0x7f) >> 6;
  case 7:
    if (param_2 < 0) {
      param_2 = param_2 + 7;
    }
    return (uint)(*(byte *)(param_1 + (param_2 >> 3)) >> 7);
  default:
    return 0;
  }
  return *(byte *)(param_1 + (param_2 >> 3)) & 1;
}

