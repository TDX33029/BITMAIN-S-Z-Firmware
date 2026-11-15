//==================== FUN_00057178 @ 00057178 ====================

undefined4
FUN_00057178(undefined4 *param_1,uint param_2,int param_3,code *param_4,undefined4 param_5)

{
  longlong param3;
  undefined4 uVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  size_t __nmemb;
  void *__base;
  undefined *puVar7;
  __compar_fn_t __compar;
  undefined4 *puVar8;
  uint uVar9;
  int iVar10;
  char acStack_8c [104];
  
  if (param_1 == (undefined4 *)0x0) goto switchD_000571a0_default;
  switch(*param_1) {
  case 0:
    puVar7 = &DAT_0006cb54;
    if ((param_2 & 0x20) == 0) {
      uVar1 = 2;
    }
    else {
      uVar1 = 1;
      puVar7 = &DAT_00065964;
    }
    if (param_1[8] == 0) {
      param_1[8] = 1;
      iVar3 = FUN_00059e9c(param_1);
      iVar4 = (*param_4)(&DAT_00066aa8,1,param_5);
      if (iVar4 == 0) {
        if (iVar3 == 0) {
          param_1[8] = 0;
          uVar1 = (*param_4)("}",1,param_5);
          return uVar1;
        }
        iVar10 = param_3 + 1;
        iVar4 = FUN_000570d4(param_2,iVar10,0,param_4,param_5);
        if (iVar4 == 0) {
          if ((param_2 & 0x180) == 0) {
            while( true ) {
              iVar4 = FUN_00059ee8(param_1,iVar3);
              uVar2 = FUN_00059f14(iVar3);
              FUN_00056e6c(uVar2,param_4,param_5,param_2);
              iVar5 = (*param_4)(puVar7,uVar1,param_5);
              if (iVar5 != 0) break;
              uVar2 = FUN_00059f20(iVar3);
              iVar3 = FUN_00057178(uVar2,param_2,iVar10,param_4,param_5);
              if (iVar3 != 0) break;
              if (iVar4 == 0) {
                iVar3 = FUN_000570d4(param_2,param_3,0,param_4,param_5);
                if (iVar3 == 0) goto LAB_000574fc;
                break;
              }
              iVar3 = (*param_4)(&DAT_00060764,1,param_5);
              if ((iVar3 != 0) ||
                 (iVar5 = FUN_000570d4(param_2,iVar10,1,param_4,param_5), iVar3 = iVar4, iVar5 != 0)
                 ) break;
            }
          }
          else {
            __nmemb = FUN_00059e04(param_1);
            __base = (void *)FUN_0005956c(__nmemb << 3);
            if (__base != (void *)0x0) {
              iVar4 = 0;
              uVar6 = 0;
              while( true ) {
                uVar2 = FUN_00057f3c(iVar3);
                puVar8 = (undefined4 *)((int)__base + iVar4);
                *puVar8 = uVar2;
                uVar2 = FUN_00059f14(iVar3);
                iVar4 = iVar4 + 8;
                puVar8[1] = uVar2;
                iVar3 = FUN_00059ee8(param_1,iVar3);
                if (iVar3 == 0) break;
                uVar6 = uVar6 + 1;
              }
              if (__nmemb != uVar6 + 1) {
                    /* WARNING: Subroutine does not return */
                __assert_fail("i == size","dump.c",0x141,"do_dump");
              }
              __compar = (__compar_fn_t)&LAB_00057090;
              if ((param_2 & 0x80) == 0) {
                __compar = (__compar_fn_t)&LAB_00056e48;
              }
              qsort(__base,__nmemb,8,__compar);
              if (__nmemb != 0) {
                uVar9 = 0;
                do {
                  uVar2 = *(undefined4 *)((int)__base + uVar9 * 8 + 4);
                  iVar3 = FUN_00059e20(param_1,uVar2);
                  if (iVar3 == 0) {
                    /* WARNING: Subroutine does not return */
                    __assert_fail("value","dump.c",0x151,"do_dump");
                  }
                  FUN_00056e6c(uVar2,param_4,param_5,param_2);
                  iVar4 = (*param_4)(puVar7,uVar1,param_5);
                  if ((iVar4 != 0) ||
                     (iVar3 = FUN_00057178(iVar3,param_2,iVar10,param_4,param_5), iVar3 != 0)) {
LAB_000576ec:
                    FUN_00059594(__base);
                    goto LAB_00057344;
                  }
                  if (uVar9 < uVar6) {
                    iVar3 = (*param_4)(&DAT_00060764,1,param_5);
                    if (iVar3 != 0) goto LAB_000576ec;
                    iVar3 = FUN_000570d4(param_2,iVar10,1,param_4,param_5);
                  }
                  else {
                    iVar3 = FUN_000570d4(param_2,param_3,0,param_4,param_5);
                  }
                  if (iVar3 != 0) goto LAB_000576ec;
                  uVar9 = uVar9 + 1;
                } while (__nmemb != uVar9);
              }
              FUN_00059594(__base);
LAB_000574fc:
              param_1[8] = 0;
              uVar1 = (*param_4)("}",1,param_5);
              return uVar1;
            }
          }
        }
      }
    }
LAB_00057344:
    uVar1 = 0xffffffff;
    param_1[8] = 0;
    break;
  case 1:
    if (param_1[5] == 0) {
      param_1[5] = 1;
      iVar3 = FUN_00059fec(param_1);
      iVar4 = (*param_4)(&DAT_0006070c,1,param_5);
      if (iVar4 == 0) {
        if (iVar3 == 0) {
          param_1[5] = 0;
          uVar1 = (*param_4)("]",1,param_5);
          return uVar1;
        }
        iVar10 = param_3 + 1;
        iVar4 = FUN_000570d4(param_2,iVar10,0,param_4,param_5);
        if (iVar4 == 0) {
          if (0 < iVar3) {
            iVar4 = 0;
            do {
              uVar1 = FUN_0005a008(param_1,iVar4);
              iVar5 = FUN_00057178(uVar1,param_2,iVar10,param_4,param_5);
              if (iVar5 != 0) goto LAB_0005745c;
              if (iVar4 < iVar3 + -1) {
                iVar5 = (*param_4)(&DAT_00060764,1,param_5);
                if (iVar5 != 0) goto LAB_0005745c;
                iVar5 = FUN_000570d4(param_2,iVar10,1,param_4,param_5);
              }
              else {
                iVar5 = FUN_000570d4(param_2,param_3,0,param_4,param_5);
              }
              if (iVar5 != 0) goto LAB_0005745c;
              iVar4 = iVar4 + 1;
            } while (iVar3 != iVar4);
          }
          param_1[5] = 0;
          uVar1 = (*param_4)("]",1,param_5);
          return uVar1;
        }
      }
    }
LAB_0005745c:
    uVar1 = 0xffffffff;
    param_1[5] = 0;
    break;
  case 2:
    uVar1 = FUN_0005a178(param_1);
    uVar1 = FUN_00056e6c(uVar1,param_4,param_5,param_2);
    break;
  case 3:
    param3 = FUN_0005a248(param_1);
    uVar6 = snprintf(acStack_8c,100,"%lld",param3);
    if (uVar6 < 100) {
LAB_000574a4:
      uVar1 = (*param_4)(acStack_8c,uVar6,param_5);
      return uVar1;
    }
  default:
switchD_000571a0_default:
    uVar1 = 0xffffffff;
    break;
  case 4:
    FUN_0005a2f8(param_1);
    uVar6 = FUN_000598a8(acStack_8c,100);
    if (-1 < (int)uVar6) goto LAB_000574a4;
    goto switchD_000571a0_default;
  case 5:
    uVar1 = (*param_4)(&DAT_00066f44,4,param_5);
    break;
  case 6:
    uVar1 = (*param_4)("false",5,param_5);
    break;
  case 7:
    uVar1 = (*param_4)(&DAT_0006cb58,4,param_5);
  }
  return uVar1;
}

