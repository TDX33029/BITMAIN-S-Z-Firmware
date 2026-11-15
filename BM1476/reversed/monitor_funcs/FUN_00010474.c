//==================== FUN_00010474 @ 00010474 ====================

int FUN_00010474(char *param_1,int param_2)

{
  long lVar1;
  long lVar2;
  int iVar3;
  
  printf("change->pools_count = %d\n",*(int *)(param_1 + 0x1ec));
  iVar3 = *(int *)(param_1 + 0x1ec);
  if (iVar3 == 1) {
    lVar1 = strtol(param_1,(char **)0x0,10);
    if (lVar1 == 1) {
      strcpy((char *)(param_2 + 0xa8),param_1 + 4);
      strcpy((char *)(param_2 + 0xf8),param_1 + 0x54);
      strcpy((char *)(param_2 + 0x120),param_1 + 0x7c);
    }
    else if (lVar1 == 2) {
      strcpy((char *)(param_2 + 0x14c),param_1 + 4);
      strcpy((char *)(param_2 + 0x19c),param_1 + 0x54);
      strcpy((char *)(param_2 + 0x1c4),param_1 + 0x7c);
    }
    else if (lVar1 == 0) {
      strcpy((char *)(param_2 + 4),param_1 + 4);
      strcpy((char *)(param_2 + 0x54),param_1 + 0x54);
      strcpy((char *)(param_2 + 0x7c),param_1 + 0x7c);
    }
  }
  else if (iVar3 == 2) {
    lVar1 = strtol(param_1,(char **)0x0,10);
    lVar2 = strtol(param_1 + 0xa4,(char **)0x0,10);
    if (lVar1 == 1) {
      strcpy((char *)(param_2 + 0xa8),param_1 + 4);
      strcpy((char *)(param_2 + 0xf8),param_1 + 0x54);
      strcpy((char *)(param_2 + 0x120),param_1 + 0x7c);
    }
    else if (lVar1 == 2) {
      strcpy((char *)(param_2 + 0x14c),param_1 + 4);
      strcpy((char *)(param_2 + 0x19c),param_1 + 0x54);
      strcpy((char *)(param_2 + 0x1c4),param_1 + 0x7c);
    }
    else if (lVar1 == 0) {
      strcpy((char *)(param_2 + 4),param_1 + 4);
      strcpy((char *)(param_2 + 0x54),param_1 + 0x54);
      strcpy((char *)(param_2 + 0x7c),param_1 + 0x7c);
    }
    if (lVar2 == 1) {
      strcpy((char *)(param_2 + 0xa8),param_1 + 0xa8);
      strcpy((char *)(param_2 + 0xf8),param_1 + 0xf8);
      strcpy((char *)(param_2 + 0x120),param_1 + 0x120);
    }
    else if (lVar2 == 2) {
      strcpy((char *)(param_2 + 0x14c),param_1 + 0xa8);
      strcpy((char *)(param_2 + 0x19c),param_1 + 0xf8);
      strcpy((char *)(param_2 + 0x1c4),param_1 + 0x120);
    }
    else if (lVar2 == 0) {
      strcpy((char *)(param_2 + 4),param_1 + 0xa8);
      strcpy((char *)(param_2 + 0x54),param_1 + 0xf8);
      strcpy((char *)(param_2 + 0x7c),param_1 + 0x120);
    }
  }
  else if (iVar3 == 3) {
    strcpy((char *)(param_2 + 4),param_1 + 4);
    strcpy((char *)(param_2 + 0x54),param_1 + 0x54);
    strcpy((char *)(param_2 + 0x7c),param_1 + 0x7c);
    strcpy((char *)(param_2 + 0xa8),param_1 + 0xa8);
    strcpy((char *)(param_2 + 0xf8),param_1 + 0xf8);
    strcpy((char *)(param_2 + 0x120),param_1 + 0x120);
    strcpy((char *)(param_2 + 0x14c),param_1 + 0x14c);
    strcpy((char *)(param_2 + 0x19c),param_1 + 0x19c);
    strcpy((char *)(param_2 + 0x1c4),param_1 + 0x1c4);
    return param_2;
  }
  return param_2;
}

