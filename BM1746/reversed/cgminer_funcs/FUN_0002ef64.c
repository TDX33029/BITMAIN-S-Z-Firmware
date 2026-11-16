//==================== FUN_0002ef64 @ 0002ef64 ====================

byte * FUN_0002ef64(byte *param_1)

{
  byte bVar1;
  size_t sVar2;
  byte *pbVar3;
  undefined4 *puVar4;
  byte *pbVar5;
  byte *pbVar6;
  char acStack_1010 [4096];
  
  if (param_1 == (byte *)0x0) {
    puVar4 = malloc(7);
    if (puVar4 == (undefined4 *)0x0) {
      snprintf(acStack_1010,0x1000,"Failed to malloc null in %s %s():%d","util.c","str_text",0xe92);
      FUN_00038730(3,acStack_1010,1);
      FUN_00016ca8(1);
    }
    else {
      *(undefined1 *)((int)puVar4 + 6) = 0;
      *puVar4 = 0x6c756e28;
      *(undefined2 *)(puVar4 + 1) = 0x296c;
    }
  }
  sVar2 = strlen((char *)param_1);
  pbVar3 = malloc(sVar2 * 4 + 5);
  pbVar5 = pbVar3;
  if (pbVar3 == (byte *)0x0) {
    snprintf(acStack_1010,0x1000,"Failed to malloc txt in %s %s():%d","util.c","str_text",0xe99);
    FUN_00038730(3,acStack_1010,1);
    FUN_00016ca8(1);
  }
  else {
  }
  do {
    while( true ) {
      bVar1 = *param_1;
      if (bVar1 - 0x20 < 0x5f) break;
      sprintf((char *)pbVar5,"0x%02x",(uint)bVar1);
      bVar1 = *param_1;
      pbVar6 = pbVar5 + 4;
      param_1 = param_1 + 1;
      pbVar5 = pbVar6;
      if (bVar1 == 0) goto LAB_0002efd8;
    }
    pbVar6 = pbVar5 + 1;
    *pbVar5 = bVar1;
    bVar1 = *param_1;
    param_1 = param_1 + 1;
    pbVar5 = pbVar6;
  } while (bVar1 != 0);
LAB_0002efd8:
  *pbVar6 = 0;
  return pbVar3;
}

