//==================== FUN_00031fc4 @ 00031fc4 ====================

void FUN_00031fc4(undefined4 *param_1,undefined4 *param_2)

{
  char cVar1;
  char *pcVar2;
  char *pcVar3;
  undefined1 *puVar4;
  char *pcVar5;
  
  pcVar3 = (char *)*param_1;
  cVar1 = *pcVar3;
  do {
    if (cVar1 == '\0') {
LAB_00032034:
      puVar4 = (undefined1 *)*param_2;
      *puVar4 = 0;
      *param_2 = puVar4 + 1;
      return;
    }
    if (cVar1 == ',') {
      *param_1 = pcVar3 + 1;
      goto LAB_00032034;
    }
    if ((cVar1 == '\\') && (pcVar5 = pcVar3 + 1, pcVar3[1] != '\0')) {
      *param_1 = pcVar5;
    }
    else {
      pcVar5 = (char *)*param_1;
    }
    pcVar2 = (char *)*param_2;
    pcVar3 = pcVar5 + 1;
    *pcVar2 = *pcVar5;
    *param_2 = pcVar2 + 1;
    *param_1 = pcVar3;
    cVar1 = pcVar5[1];
  } while( true );
}

