//==================== FUN_0000bfbc @ 0000bfbc ====================

char * FUN_0000bfbc(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  if (param_1 == 0) {
    return s_Calculated_checksum_0000f018 + 0xb;
  }
  if (param_2 == 0) {
    return s_Calculated_checksum_0000f018 + 9;
  }
  iVar1 = FUN_00002900();
  if (iVar1 != param_2) {
    return s_Calculated_checksum_0000f018 + 8;
  }
  iVar2 = param_1;
  do {
    iVar3 = iVar2 + 1;
    iVar2 = FUN_0000bf60(iVar2);
    if (iVar2 != 0) {
      return s_Calculated_checksum_0000f018 + 10;
    }
    iVar2 = iVar3;
  } while (param_1 + iVar1 != iVar3);
  return (char *)0x0;
}

