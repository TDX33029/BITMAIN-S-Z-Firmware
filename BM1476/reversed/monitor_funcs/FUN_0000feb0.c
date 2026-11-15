//==================== FUN_0000feb0 @ 0000feb0 ====================

int FUN_0000feb0(char *param_1,int param_2)

{
  char *pcVar1;
  char *pcVar2;
  char *pcVar3;
  undefined4 uVar4;
  char *pcVar5;
  int iVar6;
  int iVar7;
  
  pcVar1 = strchr(param_1,0x3b);
  iVar7 = 0;
  uVar4 = 0;
  *pcVar1 = '\0';
  strcpy((char *)&DAT_00026ab8,param_1);
  pcVar1 = pcVar1 + 1;
  while( true ) {
    pcVar2 = strchr(pcVar1,0x3b);
    pcVar5 = pcVar2;
    if (pcVar2 != (char *)0x0) {
      pcVar5 = pcVar2 + 1;
      *pcVar2 = '\0';
    }
    if (*pcVar1 != '\0') break;
LAB_0000ffa4:
    if (pcVar5 == (char *)0x0) {
      return param_2;
    }
    pcVar1 = pcVar5;
  }
  iVar6 = iVar7 * 0xa4;
LAB_0000ff20:
  pcVar3 = strchr(pcVar1,0x2c);
  pcVar2 = pcVar3;
  if (pcVar3 != (char *)0x0) {
    pcVar2 = pcVar3 + 1;
    *pcVar3 = '\0';
  }
  if (*pcVar1 == '\0') goto LAB_0000ff6c;
  switch(uVar4) {
  case 0:
    strcpy((char *)(param_2 + iVar6),pcVar1);
    uVar4 = 1;
    *(int *)(param_2 + 0x1ec) = *(int *)(param_2 + 0x1ec) + 1;
    if (pcVar2 != (char *)0x0) goto LAB_0000ff74;
LAB_0000ffa0:
    iVar7 = iVar7 + 1;
    goto LAB_0000ffa4;
  case 1:
    uVar4 = 2;
    strcpy((char *)(param_2 + iVar6 + 4),pcVar1);
    break;
  case 2:
    uVar4 = 3;
    strcpy((char *)(param_2 + iVar6 + 0x54),pcVar1);
    break;
  case 3:
    strcpy((char *)(param_2 + iVar6 + 0x7c),pcVar1);
    uVar4 = 0;
    break;
  default:
    puts("Pools Config Data is Wrong");
  }
LAB_0000ff6c:
  if (pcVar2 == (char *)0x0) goto LAB_0000ffa0;
LAB_0000ff74:
  pcVar1 = pcVar2;
  goto LAB_0000ff20;
}

