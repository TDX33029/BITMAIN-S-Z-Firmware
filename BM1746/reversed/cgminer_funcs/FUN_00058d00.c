//==================== FUN_00058d00 @ 00058d00 ====================

int FUN_00058d00(int param_1,uint param_2,undefined4 param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined4 local_30 [3];
  
  iVar3 = *(int *)(param_1 + 0x34);
  if (iVar3 == 0x101) {
    if ((param_2 & 8) == 0) {
      iVar3 = FUN_0005a21c(*(undefined4 *)(param_1 + 0x38),*(undefined4 *)(param_1 + 0x3c));
      return iVar3;
    }
    iVar3 = FUN_000597e8(param_1 + 0x28,local_30);
    if (iVar3 == 0) {
      iVar3 = FUN_0005a29c(local_30[0]);
      return iVar3;
    }
    FUN_00058034(param_3,param_1,"real number overflow");
    return 0;
  }
  if (iVar3 < 0x102) {
    if (iVar3 == 0x5b) {
      iVar3 = FUN_00059f80();
      if (iVar3 == 0) {
        return 0;
      }
      FUN_0005852c(param_1,param_3);
      iVar1 = *(int *)(param_1 + 0x34);
      if (iVar1 == 0x5d) {
        return iVar3;
      }
      while (iVar1 != 0) {
        iVar1 = FUN_00058d00(param_1,param_2,param_3);
        if (iVar1 == 0) goto LAB_00058fcc;
        if (*(int *)(iVar1 + 4) != -1) {
          *(int *)(iVar1 + 4) = *(int *)(iVar1 + 4) + 1;
        }
        iVar4 = FUN_0005a6e8(iVar3,iVar1);
        if (iVar4 != 0) {
          iVar4 = *(int *)(iVar1 + 4);
          goto joined_r0x0005906c;
        }
        iVar4 = *(int *)(iVar1 + 4) + -1;
        if ((*(int *)(iVar1 + 4) != -1) && (*(int *)(iVar1 + 4) = iVar4, iVar4 == 0)) {
          FUN_0005a3e4(iVar1);
        }
        FUN_0005852c(param_1,param_3);
        if (*(int *)(param_1 + 0x34) != 0x2c) {
          if (*(int *)(param_1 + 0x34) == 0x5d) {
            return iVar3;
          }
          break;
        }
        FUN_0005852c(param_1,param_3);
        iVar1 = *(int *)(param_1 + 0x34);
      }
      FUN_00058034(param_3,param_1,"\']\' expected");
LAB_00058fcc:
      if ((*(int *)(iVar3 + 4) == -1) ||
         (iVar1 = *(int *)(iVar3 + 4) + -1, *(int *)(iVar3 + 4) = iVar1, iVar1 != 0)) {
        return 0;
      }
      FUN_0005a3e4(iVar3);
      return 0;
    }
    if (iVar3 < 0x5c) {
      if (iVar3 == -1) {
        FUN_00058034(param_3,param_1,"invalid token");
        return 0;
      }
    }
    else {
      if (iVar3 == 0x7b) {
        iVar3 = FUN_00059da8();
        if (iVar3 == 0) {
          return 0;
        }
        FUN_0005852c(param_1,param_3);
        if (*(int *)(param_1 + 0x34) == 0x7d) {
          return iVar3;
        }
        if (*(int *)(param_1 + 0x34) == 0x100) {
          iVar4 = *(int *)(param_1 + 0x38);
          *(undefined4 *)(param_1 + 0x38) = 0;
          if (iVar4 == 0) {
            return 0;
          }
          do {
            if (((param_2 & 1) != 0) && (iVar1 = FUN_00059e20(iVar3,iVar4), iVar1 != 0)) {
              FUN_00059594(iVar4);
              FUN_00058034(param_3,param_1,"duplicate object key");
              goto LAB_00058fcc;
            }
            FUN_0005852c(param_1,param_3);
            if (*(int *)(param_1 + 0x34) != 0x3a) {
              FUN_00059594(iVar4);
              FUN_00058034(param_3,param_1,"\':\' expected");
              goto LAB_00058fcc;
            }
            FUN_0005852c(param_1,param_3);
            iVar1 = FUN_00058d00(param_1,param_2,param_3);
            if (iVar1 == 0) {
              FUN_00059594(iVar4);
              goto LAB_00058fcc;
            }
            if (*(int *)(iVar1 + 4) != -1) {
              *(int *)(iVar1 + 4) = *(int *)(iVar1 + 4) + 1;
            }
            iVar2 = FUN_0005a820(iVar3,iVar4,iVar1);
            if (iVar2 != 0) {
              FUN_00059594(iVar4);
              iVar4 = *(int *)(iVar1 + 4);
joined_r0x0005906c:
              if ((iVar4 != -1) && (*(int *)(iVar1 + 4) = iVar4 + -1, iVar4 + -1 == 0)) {
                FUN_0005a3e4(iVar1);
              }
              goto LAB_00058fcc;
            }
            if ((*(int *)(iVar1 + 4) != -1) &&
               (iVar2 = *(int *)(iVar1 + 4) + -1, *(int *)(iVar1 + 4) = iVar2, iVar2 == 0)) {
              FUN_0005a3e4(iVar1);
            }
            FUN_00059594(iVar4);
            FUN_0005852c(param_1,param_3);
            if (*(int *)(param_1 + 0x34) != 0x2c) {
              if (*(int *)(param_1 + 0x34) == 0x7d) {
                return iVar3;
              }
              FUN_00058034(param_3,param_1,"\'}\' expected");
              goto LAB_00058fcc;
            }
            FUN_0005852c(param_1,param_3);
            if (*(int *)(param_1 + 0x34) != 0x100) break;
            iVar4 = *(int *)(param_1 + 0x38);
            *(undefined4 *)(param_1 + 0x38) = 0;
            if (iVar4 == 0) {
              return 0;
            }
          } while( true );
        }
        FUN_00058034(param_3,param_1,"string or \'}\' expected");
        goto LAB_00058fcc;
      }
      if (iVar3 == 0x100) {
        iVar3 = FUN_0005a0ec(*(undefined4 *)(param_1 + 0x38));
        return iVar3;
      }
    }
  }
  else {
    if (iVar3 == 0x103) {
      iVar3 = FUN_0005a3c0();
      return iVar3;
    }
    if (iVar3 < 0x103) {
      iVar3 = FUN_0005a29c((int)*(undefined8 *)(param_1 + 0x38));
      return iVar3;
    }
    if (iVar3 == 0x104) {
      iVar3 = FUN_0005a3cc();
      return iVar3;
    }
    if (iVar3 == 0x105) {
      iVar3 = FUN_0005a3d8();
      return iVar3;
    }
  }
  FUN_00058034(param_3,param_1,"unexpected token");
  return 0;
}

