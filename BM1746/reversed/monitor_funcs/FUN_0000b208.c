//==================== FUN_0000b208 @ 0000b208 ====================

void FUN_0000b208(char *param_1,uint param_2)

{
  byte *pbVar1;
  byte bVar2;
  size_t sVar3;
  
  sVar3 = strlen(param_1);
  if (sVar3 == 0) {
    return;
  }
  param_1 = param_1 + sVar3;
  bVar2 = param_1[-1];
  if (bVar2 != param_2) {
    return;
  }
  do {
    sVar3 = sVar3 - 1;
    param_1[-1] = '\0';
    if (sVar3 == 0) {
      return;
    }
    pbVar1 = (byte *)(param_1 + -2);
    param_1 = param_1 + -1;
  } while ((uint)*pbVar1 == (uint)bVar2);
  return;
}

