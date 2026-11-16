//==================== FUN_0003305c @ 0003305c ====================

void FUN_0003305c(undefined4 param_1,int param_2,int param_3,undefined4 param_4,char param_5)

{
  int *piVar1;
  undefined4 uVar2;
  undefined4 *puVar3;
  char *pcVar4;
  int iVar5;
  int iVar6;
  int iStack_202c;
  undefined1 uStack_2028;
  undefined1 local_2027;
  undefined4 uStack_2024;
  char acStack_2020 [8192];
  
  iStack_202c = param_2;
  if (param_5 != '\0') {
    FUN_000307c8(param_1,"{\"STATUS\":[");
  }
  if (DAT_000760fc == 4) {
LAB_000330f4:
    uVar2 = FUN_00032e1c(0,"STATUS",&DAT_00066ff4,0);
    uVar2 = FUN_00033038(uVar2,&DAT_00066fd4,&DAT_00078cd8,0);
    uStack_2024 = 0xffffffff;
    uVar2 = FUN_00032ed0(uVar2,&DAT_00066fdc,&uStack_2024,0);
    sprintf(acStack_2020,"%d",iStack_202c);
  }
  else {
    if (DAT_00076100 == iStack_202c) {
      iVar6 = 0;
      iVar5 = DAT_000760fc;
    }
    else {
      iVar6 = 0;
      puVar3 = &DAT_000760fc;
      do {
        iVar5 = puVar3[4];
        iVar6 = iVar6 + 1;
        if (iVar5 == 4) goto LAB_000330f4;
        piVar1 = puVar3 + 5;
        puVar3 = puVar3 + 4;
      } while (*piVar1 != iStack_202c);
    }
    if (iVar5 == 2) {
      uStack_2028 = 0x49;
    }
    else if (iVar5 == 3) {
      uStack_2028 = 0x53;
    }
    else if (iVar5 == 1) {
      uStack_2028 = 0x57;
    }
    else {
      uStack_2028 = 0x45;
    }
    local_2027 = 0;
    switch((&DAT_00076104)[iVar6 * 4]) {
    case 0:
    case 1:
    case 2:
    case 0xe:
      sprintf(acStack_2020,(char *)(&DAT_00076108)[iVar6 * 4],param_3);
      break;
    default:
      strcpy(acStack_2020,(char *)(&DAT_00076108)[iVar6 * 4]);
      break;
    case 5:
      sprintf(acStack_2020,(char *)(&DAT_00076108)[iVar6 * 4],DAT_0007b198);
      break;
    case 6:
      sprintf(acStack_2020,(char *)(&DAT_00076108)[iVar6 * 4],param_3,DAT_0007b198 + -1);
      break;
    case 7:
      sprintf(acStack_2020,(char *)(&DAT_00076108)[iVar6 * 4]);
      break;
    case 8:
      sprintf(acStack_2020,(char *)(&DAT_00076108)[iVar6 * 4],"command");
      break;
    case 9:
      sprintf(acStack_2020,(char *)(&DAT_00076108)[iVar6 * 4],param_3,
              *(undefined4 *)(*(int *)(DAT_0007b438 + param_3 * 4) + 0xc4));
      break;
    case 10:
      sprintf(acStack_2020,(char *)(&DAT_00076108)[iVar6 * 4],param_4);
      break;
    case 0xb:
      sprintf(acStack_2020,(char *)(&DAT_00076108)[iVar6 * 4],param_3,param_4);
      break;
    case 0xc:
      pcVar4 = "false";
      if (param_3 != 0) {
        pcVar4 = "true";
      }
      sprintf(acStack_2020,(char *)(&DAT_00076108)[iVar6 * 4],pcVar4);
      break;
    case 0xd:
      sprintf(acStack_2020,(char *)(&DAT_00076108)[iVar6 * 4],param_4,param_3);
    }
    uVar2 = FUN_00032e1c(0,"STATUS",&uStack_2028,0);
    uVar2 = FUN_00033038(uVar2,&DAT_00066fd4,&DAT_00078cd8,0);
    uVar2 = FUN_00032ed0(uVar2,&DAT_00066fdc,&iStack_202c,0);
  }
  uVar2 = FUN_00032df8(uVar2,&DAT_00066fe4,acStack_2020,0);
  uVar2 = FUN_00032df8(uVar2,"Description",DAT_00075ed4,0);
  FUN_00032534(param_1,uVar2,param_5,0);
  if (param_5 != '\0') {
    FUN_000307c8(param_1,"]");
  }
  return;
}

