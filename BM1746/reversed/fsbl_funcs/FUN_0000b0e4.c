//==================== FUN_0000b0e4 @ 0000b0e4 ====================

void FUN_0000b0e4(int param_1,int param_2)

{
  char *pcVar1;
  char *pcVar2;
  byte bVar3;
  char *pcVar4;
  byte *pbVar5;
  char *pcVar6;
  
  pcVar4 = (char *)(param_2 + -1);
  pcVar6 = (char *)(param_1 + -1);
  do {
    pcVar6 = pcVar6 + 1;
    pcVar4 = pcVar4 + 1;
    *pcVar4 = *pcVar6;
  } while (pcVar6 != (char *)(param_1 + 0x19));
  pbVar5 = (byte *)(param_2 + 0x19);
  pcVar4 = &DAT_00028eac;
  do {
    bVar3 = 0;
    pbVar5 = pbVar5 + 1;
    *pbVar5 = 0;
    pcVar1 = pcVar4;
    pcVar2 = (char *)(param_1 + -1);
    do {
      pcVar2 = pcVar2 + 1;
      bVar3 = *pcVar1 * *pcVar2 + bVar3;
      *pbVar5 = bVar3;
      pcVar1 = pcVar1 + 5;
    } while (pcVar2 != pcVar6);
    *pbVar5 = bVar3 & 1;
    pcVar4 = pcVar4 + 1;
  } while (pbVar5 != (byte *)(param_2 + 0x1e));
  return;
}

