//==================== FUN_00030500 @ 00030500 ====================

byte * FUN_00030500(byte *param_1,int param_2)

{
  size_t sVar1;
  byte *pbVar2;
  byte *pbVar3;
  byte bVar4;
  byte *pbVar5;
  byte *pbVar6;
  byte bVar7;
  int iVar8;
  char acStack_1020 [4100];
  
  bVar7 = *param_1;
  pbVar2 = param_1;
  if (bVar7 != 0) {
    iVar8 = 0;
    pbVar3 = param_1;
    bVar4 = bVar7;
    do {
      while (bVar4 != 0x3d) {
        if (0x3d < bVar4) {
          if (bVar4 != 0x5c) {
            if (bVar4 != 0x7c) goto LAB_00030544;
            break;
          }
          goto LAB_00030574;
        }
        if (bVar4 == 0x22) {
          if (param_2 != 0) goto LAB_00030574;
        }
        else if (bVar4 == 0x2c) break;
LAB_00030544:
        pbVar3 = pbVar3 + 1;
        bVar4 = *pbVar3;
        if (bVar4 == 0) goto LAB_00030584;
      }
      if (param_2 != 0) goto LAB_00030544;
LAB_00030574:
      pbVar3 = pbVar3 + 1;
      bVar4 = *pbVar3;
      iVar8 = iVar8 + 1;
    } while (bVar4 != 0);
LAB_00030584:
    if (iVar8 != 0) {
      sVar1 = strlen((char *)param_1);
      sVar1 = iVar8 + 1 + sVar1;
      pbVar2 = malloc(sVar1);
      pbVar6 = pbVar2;
      pbVar3 = param_1;
      if (pbVar2 == (byte *)0x0) {
        snprintf(acStack_1020,0x1000,"Failed to malloc escape buf %d in %s %s():%d",sVar1,"api.c",
                 "escape_string",0x348);
        FUN_00038730(3,acStack_1020,1);
        FUN_00016ca8(1);
        bVar7 = *param_1;
      }
      while (bVar7 != 0) {
        if (bVar7 == 0x3d) {
LAB_00030618:
          if (param_2 == 0) {
LAB_00030620:
            *pbVar6 = 0x5c;
            bVar7 = *pbVar3;
            pbVar6 = pbVar6 + 1;
          }
LAB_000305e8:
          pbVar5 = pbVar6 + 1;
          *pbVar6 = bVar7;
        }
        else {
          if (bVar7 < 0x3e) {
            if (bVar7 == 0x22) {
              if (param_2 != 0) goto LAB_00030620;
            }
            else if (bVar7 == 0x2c) goto LAB_00030618;
            goto LAB_000305e8;
          }
          if (bVar7 != 0x5c) {
            if (bVar7 != 0x7c) goto LAB_000305e8;
            goto LAB_00030618;
          }
          *pbVar6 = 0x5c;
          pbVar6[1] = *pbVar3;
          pbVar5 = pbVar6 + 2;
        }
        param_1 = param_1 + 1;
        pbVar6 = pbVar5;
        pbVar3 = pbVar3 + 1;
        bVar7 = *param_1;
      }
      *pbVar6 = 0;
    }
  }
  return pbVar2;
}

