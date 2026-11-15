//==================== FUN_00033dcc @ 00033dcc ====================

void FUN_00033dcc(int param_1,undefined4 param_2,undefined4 param_3,int param_4)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  char *pcVar4;
  char *pcVar5;
  undefined4 uVar6;
  int local_24;
  
  if (DAT_0007b720 == 0) {
    FUN_0003305c(param_1,10,0,0,param_4);
  }
  else {
    FUN_0003305c(param_1,0x45,0,0,param_4);
    if (param_4 == 0) {
      iVar3 = 0;
    }
    else {
      iVar3 = FUN_000307c8(param_1,",\"DEVDETAILS\":[");
    }
    uVar6 = 0;
    local_24 = 0;
    if (0 < DAT_0007b720) {
      do {
        iVar1 = FUN_0001b83c(local_24);
        pcVar5 = "";
        uVar6 = FUN_00032ed0(uVar6,"DEVDETAILS",&local_24,0);
        uVar6 = FUN_00032e1c(uVar6,"Name",*(undefined4 *)(*(int *)(iVar1 + 4) + 8),0);
        uVar6 = FUN_00032ed0(uVar6,&DAT_000670e8,iVar1 + 8,0);
        uVar6 = FUN_00032e1c(uVar6,"Driver",*(undefined4 *)(*(int *)(iVar1 + 4) + 4),0);
        pcVar4 = *(char **)(iVar1 + 0xa8);
        if (*(char **)(iVar1 + 0xa8) == (char *)0x0) {
          pcVar4 = pcVar5;
        }
        uVar6 = FUN_00032e40(uVar6,"Kernel",pcVar4);
        pcVar4 = *(char **)(iVar1 + 0xc);
        if (*(char **)(iVar1 + 0xc) == (char *)0x0) {
          pcVar4 = pcVar5;
        }
        uVar6 = FUN_00032e40(uVar6,"Model",pcVar4);
        pcVar4 = *(char **)(iVar1 + 0x10);
        if (*(char **)(iVar1 + 0x10) == (char *)0x0) {
          pcVar4 = pcVar5;
        }
        uVar2 = FUN_00032e40(uVar6,"Device Path",pcVar4);
        uVar6 = 0;
        if (param_4 != 0) {
          if (local_24 < 1) {
            uVar6 = 0;
          }
          else {
            uVar6 = 1;
          }
        }
        uVar6 = FUN_00032534(param_1,uVar2,param_4,uVar6);
        local_24 = local_24 + 1;
      } while (local_24 < DAT_0007b720);
    }
    if (iVar3 != 0) {
      *(undefined1 *)(param_1 + 0xd) = 1;
    }
  }
  return;
}

