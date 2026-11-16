//==================== FUN_00011c8c @ 00011c8c ====================

void FUN_00011c8c(int param_1,int param_2,int param_3)

{
  char cVar1;
  int iVar2;
  char local_1020 [4];
  char acStack_101c [4];
  char acStack_1018 [4];
  char acStack_1014 [4];
  char local_1010 [4];
  char acStack_100c [4];
  char acStack_1008 [4];
  undefined2 uStack_1004;
  undefined1 local_1002;
  
  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
    snprintf(local_1020,0x1000,"Thread %d being disabled",param_2);
    FUN_00038730(4,local_1020,0);
  }
  cVar1 = DAT_00078e09;
  iVar2 = *(int *)(param_1 + 0x24);
  *(undefined4 *)(iVar2 + 0x30) = 0;
  *(undefined4 *)(iVar2 + 0x34) = 0;
  if ((cVar1 != '\0') && (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4))))
  {
    local_1020[0] = 'W';
    local_1020[1] = 'a';
    local_1020[2] = 'i';
    local_1020[3] = 't';
    acStack_101c[0] = 'i';
    acStack_101c[1] = 'n';
    acStack_101c[2] = 'g';
    acStack_101c[3] = ' ';
    acStack_1018[0] = 'o';
    acStack_1018[1] = 'n';
    acStack_1018[2] = ' ';
    acStack_1018[3] = 's';
    acStack_1014[0] = 'e';
    acStack_1014[1] = 'm';
    acStack_1014[2] = ' ';
    acStack_1014[3] = 'i';
    local_1010[0] = 'n';
    local_1010[1] = ' ';
    local_1010[2] = 'm';
    local_1010[3] = 'i';
    acStack_100c[0] = 'n';
    acStack_100c[1] = 'e';
    acStack_100c[2] = 'r';
    acStack_100c[3] = ' ';
    acStack_1008[0] = 't';
    acStack_1008[1] = 'h';
    acStack_1008[2] = 'r';
    acStack_1008[3] = 'e';
    uStack_1004 = 0x6461;
    local_1002 = 0;
    FUN_00038730(7,local_1020,0);
  }
  FUN_0002f280(param_1 + 0x10,"cgminer.c","mt_disable",0x2171);
  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
    snprintf(local_1020,0x1000,"Thread %d being re-enabled",param_2);
    FUN_00038730(4,local_1020,0);
  }
  (**(code **)(param_3 + 0x5c))(param_1);
  return;
}

