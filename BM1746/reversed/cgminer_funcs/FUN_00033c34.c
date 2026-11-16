//==================== FUN_00033c34 @ 00033c34 ====================

void FUN_00033c34(int param_1,undefined4 param_2,char *param_3,uint param_4,byte param_5)

{
  bool bVar1;
  bool bVar2;
  int iVar3;
  __int32_t **pp_Var4;
  char *pcVar5;
  uint uVar6;
  undefined4 uVar7;
  undefined *puVar8;
  __int32_t *p_Var9;
  int *piVar10;
  undefined *puVar11;
  char acStack_84 [100];
  
  if ((param_3 == (char *)0x0) || (*param_3 == '\0')) {
    FUN_0003305c(param_1,0x47,0,0,param_4);
  }
  else {
    if (DAT_0007652c != (char *)0x0) {
      piVar10 = (int *)&DAT_0007652c;
      pcVar5 = DAT_0007652c;
      do {
        iVar3 = strcmp(pcVar5,param_3);
        if (iVar3 == 0) {
          sprintf(acStack_84,"|%s|",param_3);
          pp_Var4 = __ctype_toupper_loc();
          p_Var9 = *pp_Var4;
          if ((p_Var9[param_5] == p_Var9[0x57]) ||
             (pcVar5 = strstr(*(char **)(&DAT_0007bd24 + (p_Var9[param_5] - p_Var9[0x41]) * 4),
                              acStack_84), pcVar5 != (char *)0x0)) {
            bVar1 = true;
            bVar2 = true;
          }
          else {
            bVar1 = true;
            bVar2 = false;
          }
          goto LAB_00033ce8;
        }
        piVar10 = piVar10 + 3;
        pcVar5 = (char *)*piVar10;
      } while (pcVar5 != (char *)0x0);
    }
    bVar1 = false;
    bVar2 = bVar1;
LAB_00033ce8:
    FUN_0003305c(param_1,0x48,0,0,param_4);
    pcVar5 = "CHECK,";
    if (param_4 != 0) {
      pcVar5 = ",\"CHECK\":[";
    }
    puVar11 = &DAT_000670ac;
    uVar6 = FUN_000307c8(param_1,pcVar5);
    if (bVar1) {
      puVar8 = puVar11;
    }
    else {
      puVar8 = &UNK_000670b0;
    }
    uVar7 = FUN_00032e40(0,"Exists",puVar8,0);
    if (!bVar2) {
      puVar11 = &UNK_000670b0;
    }
    uVar7 = FUN_00032e40(uVar7,"Access",puVar11,0);
    FUN_00032534(param_1,uVar7,param_4,0);
    if ((uVar6 & param_4) != 0) {
      *(undefined1 *)(param_1 + 0xd) = 1;
    }
  }
  return;
}

