//==================== FUN_000363fc @ 000363fc ====================

void FUN_000363fc(undefined4 param_1,int param_2,int param_3,undefined4 param_4)

{
  undefined4 uVar1;
  char *pcVar2;
  int local_1c [2];
  
  if (*(int *)(param_3 + 0xf4) == 0) {
    pcVar2 = "None";
  }
  else if (*(uint *)(param_3 + 0xf8) < 9) {
    pcVar2 = (&PTR_s_Thread_failed_to_init_00066a1c)[*(uint *)(param_3 + 0xf8)];
  }
  else {
    pcVar2 = "Unknown reason - code bug";
  }
  local_1c[0] = param_2;
  uVar1 = FUN_00032ed0(0,"NOTIFY",local_1c,0);
  uVar1 = FUN_00032e1c(uVar1,"Name",*(undefined4 *)(*(int *)(param_3 + 4) + 8),0);
  uVar1 = FUN_00032ed0(uVar1,&DAT_000670e8,param_3 + 8,0);
  uVar1 = FUN_00033038(uVar1,"Last Well",param_3 + 0xf0,0);
  uVar1 = FUN_00033038(uVar1,"Last Not Well",param_3 + 0xf4,0);
  uVar1 = FUN_00032e1c(uVar1,"Reason Not Well",pcVar2,0);
  uVar1 = FUN_00032ed0(uVar1,"*Thread Fail Init",param_3 + 0xfc,0);
  uVar1 = FUN_00032ed0(uVar1,"*Thread Zero Hash",param_3 + 0x100,0);
  uVar1 = FUN_00032ed0(uVar1,"*Thread Fail Queue",param_3 + 0x104,0);
  uVar1 = FUN_00032ed0(uVar1,"*Dev Sick Idle 60s",param_3 + 0x108,0);
  uVar1 = FUN_00032ed0(uVar1,"*Dev Dead Idle 600s",param_3 + 0x10c,0);
  uVar1 = FUN_00032ed0(uVar1,"*Dev Nostart",param_3 + 0x110,0);
  uVar1 = FUN_00032ed0(uVar1,"*Dev Over Heat",param_3 + 0x114,0);
  uVar1 = FUN_00032ed0(uVar1,"*Dev Thermal Cutoff",param_3 + 0x118,0);
  uVar1 = FUN_00032ed0(uVar1,"*Dev Comms Error",param_3 + 0x11c,0);
  uVar1 = FUN_00032ed0(uVar1,"*Dev Throttle",param_3 + 0x120,0);
  FUN_00032534(param_1,uVar1,param_4);
  return;
}

