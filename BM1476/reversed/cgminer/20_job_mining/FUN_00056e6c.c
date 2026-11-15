//==================== FUN_00056e6c @ 00056e6c ====================

undefined4 FUN_00056e6c(char *param_1,code *param_2,undefined4 param_3,uint param_4)

{
  int iVar1;
  char *pcVar2;
  char *pcVar3;
  undefined4 uVar4;
  uint uVar5;
  uint uVar6;
  uint local_34;
  char acStack_30 [20];
  
  iVar1 = (*param_2)(&DAT_00060810,1);
  pcVar3 = param_1;
  if (iVar1 != 0) {
    return 0xffffffff;
  }
  do {
    while (*param_1 != '\0') {
      pcVar2 = (char *)FUN_00059c18(param_1,&local_34);
      if (pcVar2 == (char *)0x0) {
        return 0xffffffff;
      }
      if ((((local_34 == 0x5c || local_34 == 0x22) || ((int)local_34 < 0x20)) ||
          (((param_4 & 0x400) != 0 && (local_34 == 0x2f)))) ||
         (((param_4 & 0x40) != 0 && (0x7f < (int)local_34)))) goto LAB_00056ef8;
      param_1 = pcVar2;
    }
    pcVar2 = param_1;
LAB_00056ef8:
    if ((param_1 != pcVar3) &&
       (iVar1 = (*param_2)(pcVar3,(int)param_1 - (int)pcVar3,param_3), iVar1 != 0)) {
      return 0xffffffff;
    }
    if (pcVar2 == param_1) {
      uVar4 = (*param_2)(&DAT_00060810,1,param_3);
      return uVar4;
    }
    if (local_34 == 0xc) {
      uVar4 = 2;
      pcVar3 = "\\f";
LAB_00056ed0:
      iVar1 = (*param_2)(pcVar3,uVar4,param_3);
    }
    else {
      if ((int)local_34 < 0xd) {
        if (local_34 == 9) {
          uVar4 = 2;
          pcVar3 = "\\t";
        }
        else if (local_34 == 10) {
          uVar4 = 2;
          pcVar3 = "\\n";
        }
        else {
          if (local_34 != 8) goto LAB_00056eb4;
          uVar4 = 2;
          pcVar3 = "\\b";
        }
        goto LAB_00056ed0;
      }
      if (local_34 == 0x22) {
        pcVar3 = "\\\"";
        uVar4 = 2;
        goto LAB_00056ed0;
      }
      if (0x22 < (int)local_34) {
        if (local_34 == 0x2f) {
          uVar4 = 2;
          pcVar3 = "\\/";
        }
        else {
          if (local_34 != 0x5c) goto LAB_00056eb4;
          uVar4 = 2;
          pcVar3 = "\\\\";
        }
        goto LAB_00056ed0;
      }
      if (local_34 != 0xd) {
LAB_00056eb4:
        if ((int)local_34 < 0x10000) {
          sprintf(acStack_30,"\\u%04x",local_34);
          pcVar3 = acStack_30;
          uVar4 = 6;
        }
        else {
          uVar6 = local_34 - 0x10000;
          uVar5 = local_34 & 0x3ff;
          local_34 = uVar6;
          sprintf(acStack_30,"\\u%04x\\u%04x",(uVar6 & 0xfffff) >> 10 | 0xd800,uVar5 | 0xdc00);
          pcVar3 = acStack_30;
          uVar4 = 0xc;
        }
        goto LAB_00056ed0;
      }
      iVar1 = (*param_2)(&DAT_0006cb30,2,param_3);
    }
    if (iVar1 != 0) {
      return 0xffffffff;
    }
    param_1 = pcVar2;
    pcVar3 = pcVar2;
  } while( true );
}

