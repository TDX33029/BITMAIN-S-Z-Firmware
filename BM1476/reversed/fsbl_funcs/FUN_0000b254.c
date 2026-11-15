//==================== FUN_0000b254 @ 0000b254 ====================

bool FUN_0000b254(uint param_1)

{
  uint uVar1;
  bool bVar2;
  bool bVar3;
  
  if (((((param_1 != 0xf800e104 && param_1 != 0xf800e080) &&
        (param_1 != 0xf800e20c && param_1 != 0xf800e188)) &&
       (param_1 != 0xf800e314 && param_1 != 0xf800e290)) &&
      ((((param_1 != 0xf800e41c && param_1 != 0xf800e398 &&
         (param_1 != 0xf800e524 && param_1 != 0xf800e4a0)) &&
        ((param_1 != 0xf800e978 && param_1 != 0xf800e8fc &&
         ((param_1 != 0xf800ea70 && param_1 != 0xf800e9f4 &&
          (param_1 != 0xf800eb68 && param_1 != 0xf800eaec)))))) &&
       (param_1 != 0xf800ec60 && param_1 != 0xf800ebe4)))) &&
     (param_1 != 0xf800ed58 && param_1 != 0xf800ecdc)) {
    uVar1 = param_1 + 0x7ff1fc0;
    bVar3 = 0x3b < uVar1;
    bVar2 = uVar1 == 0x3c;
    if (0x3c < uVar1) {
      bVar3 = 0x1b < param_1 + 0x7ff2000;
      bVar2 = param_1 + 0x7ff2000 == 0x1c;
    }
    if (((bVar3 && !bVar2) && (0x1fc < (param_1 & 0xfffff7ff) + 0x7ff1a00)) &&
       (0x1c < param_1 + 0x7ff17a0)) {
      return param_1 + 0x7ff1800 < 0x3d;
    }
  }
  return true;
}

