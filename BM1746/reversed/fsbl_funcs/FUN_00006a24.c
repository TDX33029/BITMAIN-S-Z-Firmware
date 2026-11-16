//==================== FUN_00006a24 @ 00006a24 ====================

code * FUN_00006a24(int param_1,code *param_2)

{
  code *pcVar1;
  
  if (param_2 == (code *)0x1200) goto LAB_00006b0c;
  if (param_2 < (code *)0x1201) {
    if (param_2 == (code *)0x800) {
      pcVar1 = (code *)0x83a;
      if (*(char *)(param_1 + 0x34) == '\x01') {
        pcVar1 = FUN_00000814;
      }
      return pcVar1;
    }
    if (param_2 < (code *)0x801) {
      if (param_2 == (code *)0x300) {
        pcVar1 = (code *)0x31a;
        if (*(char *)(param_1 + 0x34) == '\x01') {
          pcVar1 = (code *)0x31b;
        }
        return pcVar1;
      }
      if (param_2 < (code *)0x301) {
        if (param_2 == (code *)0x100) {
          return (code *)0x102;
        }
        if (param_2 == (code *)0x200) {
          return (code *)0x209;
        }
      }
      else {
        if (param_2 == (code *)0x600) {
          pcVar1 = (code *)0x61b;
          if (*(char *)(param_1 + 0x34) == '\x01') {
            pcVar1 = (code *)0x63a;
          }
          return pcVar1;
        }
        if (param_2 == (code *)0x700) {
          return FUN_00000714;
        }
        if (param_2 == (code *)0x500) {
          return (code *)0x51b;
        }
      }
      return param_2;
    }
    if (param_2 != (code *)0xb00) {
      if (param_2 < (code *)0xb01) {
        if (param_2 == (code *)0x900) {
          return (code *)0x909;
        }
        if (param_2 != (code *)0xa00) {
          return param_2;
        }
      }
      else if (param_2 != (code *)0x1000) {
        if (param_2 == (code *)(s_0_8_0__0_0_0___000010f5 + 0xb)) goto LAB_00006b0c;
        if (param_2 != (code *)0xc00) {
          return param_2;
        }
      }
    }
  }
  else if (param_2 != FUN_000036fc + 4) {
    if (param_2 < FUN_000036fc + 5) {
      if (param_2 != (code *)0x1700) {
        if (param_2 < (code *)0x1701) {
          if ((param_2 != (code *)(s_0___000012f9 + 7)) && (param_2 != (code *)0x1500)) {
            return param_2;
          }
        }
        else if (param_2 != (code *)0x1900) {
          if (param_2 == (code *)0x3400) goto LAB_00006af4;
          if (param_2 != (code *)0x1800) {
            return param_2;
          }
        }
      }
    }
    else if (param_2 != (code *)0x9700) {
      if ((code *)0x9700 < param_2) {
        if (param_2 == (code *)0xaa00) {
          return (code *)0xaa1a;
        }
        if (param_2 == (code *)0xb300) {
          return (code *)0xb33a;
        }
        if (param_2 == (code *)0xa900) {
          return (code *)0xa902;
        }
        return param_2;
      }
      if (param_2 == (code *)0x8600) {
        return (code *)0x861a;
      }
      if (param_2 != (code *)0x8d00) {
        return param_2;
      }
      goto LAB_00006af4;
    }
LAB_00006b0c:
    return (code *)((uint)param_2 | 0x3a);
  }
LAB_00006af4:
  return (code *)((uint)param_2 | 0x1a);
}

