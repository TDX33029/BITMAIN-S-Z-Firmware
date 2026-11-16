//==================== FUN_040082e4 @ 040082e4 ====================

void FUN_040082e4(byte *param_1,byte *param_2)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  int iVar7;
  
  uVar2 = DAT_040084a8;
  uVar1 = DAT_040084a4;
  do {
    iVar7 = DAT_040084bc;
    if (param_2 <= param_1) {
      return;
    }
    uVar4 = (uint)*param_1;
    if (uVar4 == 0xff) {
      return;
    }
    uVar5 = (uint)param_1[1];
    if (uVar4 == 0x1c) goto LAB_040083f0;
    if (uVar4 < 0x1d) {
      if (uVar4 == 6) {
        uVar3 = DAT_040084c8;
        goto LAB_04008430;
      }
      if (uVar4 < 7) {
        if (uVar4 == 1) {
          uVar3 = DAT_040084c0;
        }
        else {
          if (uVar4 == 0) {
            uVar5 = 0xffffffff;
            goto LAB_040083f0;
          }
          if (uVar4 != 3) goto LAB_04008340;
          uVar3 = DAT_040084c4;
        }
        goto LAB_04008430;
      }
      if (uVar4 != 0xf) {
        uVar6 = uVar5;
        if (uVar4 == 0x11) {
          if (0x3f < uVar5) {
            FUN_04007548(DAT_040084b0,DAT_040084cc,uVar5,0x40);
            uVar6 = 0x3f;
          }
          iVar7 = DAT_040084d0;
        }
        else {
          if (uVar4 != 0xc) goto LAB_04008340;
          if (0x1f < uVar5) {
            FUN_04007548(DAT_040084b0,DAT_040084ac,uVar5,0x20);
            uVar6 = 0x1f;
          }
          iVar7 = DAT_040084b4;
        }
        FUN_04005ea4(iVar7,param_1 + 2,uVar6);
        *(undefined1 *)(iVar7 + uVar6) = 0;
      }
    }
    else {
      if (uVar4 != 0x36) {
        if (uVar4 < 0x37) {
          if (uVar4 == 0x34) {
            *DAT_040084d4 = param_1[2];
          }
          else if (uVar4 < 0x35) {
            if (uVar4 != 0x33) goto LAB_04008340;
            uVar3 = DAT_040084b8;
            goto LAB_04008430;
          }
        }
        else if (uVar4 != 0x42) {
          if (uVar4 < 0x43) {
            if (1 < uVar4 - 0x3a) {
LAB_04008340:
              FUN_04007548(uVar1);
            }
          }
          else {
            if (uVar4 != 0x43) goto LAB_04008340;
            FUN_04005ea4(DAT_040084bc,param_1 + 2,uVar5);
            *(undefined1 *)(iVar7 + uVar5) = 0;
          }
        }
        goto LAB_040083f0;
      }
      uVar3 = uVar2;
LAB_04008430:
      FUN_04005ea4(uVar3,param_1 + 2,4);
    }
LAB_040083f0:
    param_1 = param_1 + uVar5 + 2;
  } while( true );
}

