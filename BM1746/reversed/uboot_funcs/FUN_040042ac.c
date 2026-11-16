//==================== FUN_040042ac @ 040042ac ====================

void FUN_040042ac(uint param_1,uint param_2,undefined4 param_3)

{
  uint uVar1;
  char cVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  int iVar8;
  
  uVar7 = 0x3c;
  iVar8 = 0;
  do {
    uVar3 = 0x20 - uVar7;
    uVar4 = uVar7 - 0x20;
    uVar5 = param_1 >> (uVar7 & 0xff) | param_2 << (uVar3 & 0xff) | param_2 >> (uVar4 & 0xff);
    if (uVar5 != 0 || param_2 >> (uVar7 & 0xff) != 0) {
      cVar2 = *(char *)(DAT_040043e0 + iVar8);
      if (cVar2 != '\0') {
        param_1 = param_1 & ~(-1 << (uVar7 & 0xff));
        param_2 = param_2 & ~(-1 << (uVar7 & 0xff) | -1 << (uVar4 & 0xff) |
                             0xffffffffU >> (uVar3 & 0xff));
        if (param_1 == 0 && param_2 == 0) {
          FUN_04007548(DAT_040043e4,uVar5);
        }
        else {
          uVar1 = 1 << (uVar7 - 1 & 0xff);
          uVar6 = (1 << (uVar7 - 0x21 & 0xff) | 1U >> (0x20 - (uVar7 - 1) & 0xff)) +
                  ((param_2 << 2 | param_1 >> 0x1e) + param_2 + (uint)CARRY4(param_1 * 4,param_1)) *
                  2 + (uint)CARRY4(param_1 * 5,param_1 * 5) + (uint)CARRY4(uVar1,param_1 * 10);
          uVar7 = uVar1 + param_1 * 10 >> (uVar7 & 0xff) | uVar6 << (uVar3 & 0xff) |
                  uVar6 >> (uVar4 & 0xff);
          if (9 < uVar7) {
            uVar5 = uVar5 + 1;
            uVar7 = uVar7 - 10;
          }
          FUN_04007548(DAT_040043e4,uVar5);
          if (uVar7 != 0) {
            FUN_04007548(DAT_040043ec,uVar7);
          }
        }
        FUN_04007548(DAT_040043e8,cVar2,param_3);
        return;
      }
      break;
    }
    uVar7 = uVar7 - 10;
    iVar8 = iVar8 + 1;
  } while (uVar7 != 0);
  FUN_04007548(DAT_040043dc,param_2,param_1,param_2,param_3);
  return;
}

