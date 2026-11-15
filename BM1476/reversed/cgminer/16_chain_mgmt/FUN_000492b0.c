//==================== FUN_000492b0 @ 000492b0 ====================

void FUN_000492b0(int param_1)

{
  short *psVar1;
  uint uVar2;
  uint uVar3;
  char cVar4;
  int iVar5;
  char acStack_1028 [4100];
  
  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
    snprintf(acStack_1028,0x1000,"%d %s",param_1,"calibration_sensor_offset_chain");
    FUN_00038730(5,acStack_1028,0);
  }
  uVar3 = 0;
  psVar1 = (short *)(&DAT_0009d534 + param_1 * 0x18);
  do {
    uVar2 = uVar3 & 0xff;
    cVar4 = '\v';
    do {
      FUN_0004886c(param_1,uVar2,0x4c,1);
      FUN_0004886c(param_1,uVar2,0x4c,0);
      if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
        snprintf(acStack_1028,0x1000,"chain:%d local:%d remote:%d",param_1,(int)*psVar1,
                 (int)psVar1[1]);
        FUN_00038730(5,acStack_1028,0);
      }
      iVar5 = (int)*psVar1 - (int)psVar1[1];
      if (iVar5 < 0) {
        iVar5 = -iVar5;
      }
      (&DAT_00079be4)[uVar3 + param_1 * 4] =
           ((char)*psVar1 + (&DAT_00079be4)[uVar3 + param_1 * 4]) - (char)psVar1[1];
      if (iVar5 < 3) break;
      cVar4 = cVar4 + -1;
      FUN_000490fc(param_1,uVar2);
    } while (cVar4 != '\0');
    uVar3 = uVar3 + 1;
    psVar1 = psVar1 + 4;
    if (uVar3 == 3) {
      return;
    }
  } while( true );
}

