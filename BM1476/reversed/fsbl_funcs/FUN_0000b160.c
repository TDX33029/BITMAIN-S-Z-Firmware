//==================== FUN_0000b160 @ 0000b160 ====================

undefined1 FUN_0000b160(int param_1,byte *param_2)

{
  char *pcVar1;
  char *pcVar2;
  byte bVar3;
  byte *pbVar4;
  char *pcVar5;
  
  pbVar4 = param_2 + -1;
  pcVar5 = &DAT_00028eac;
  do {
    bVar3 = 0;
    pbVar4 = pbVar4 + 1;
    *pbVar4 = 0;
    pcVar1 = pcVar5;
    pcVar2 = (char *)(param_1 + -1);
    do {
      pcVar2 = pcVar2 + 1;
      bVar3 = *pcVar1 * *pcVar2 + bVar3;
      *pbVar4 = bVar3;
      pcVar1 = pcVar1 + 5;
    } while (pcVar2 != (char *)(param_1 + 0x1e));
    *pbVar4 = bVar3 & 1;
    pcVar5 = pcVar5 + 1;
  } while (pbVar4 != param_2 + 4);
  return (&DAT_00028e8c)
         [(uint)param_2[3] * 8 + (uint)param_2[4] * 0x10 + (uint)*param_2 + (uint)param_2[2] * 4 +
          (uint)param_2[1] * 2 & 0xff];
}

