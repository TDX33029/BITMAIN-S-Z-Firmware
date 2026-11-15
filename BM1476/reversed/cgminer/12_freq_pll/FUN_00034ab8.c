//==================== FUN_00034ab8 @ 00034ab8 ====================

void FUN_00034ab8(int param_1,undefined4 param_2,undefined4 param_3,uint param_4)

{
  uint uVar1;
  undefined4 uVar2;
  char *pcVar3;
  undefined4 local_20;
  undefined4 local_1c [2];
  
  local_20 = 0;
  local_1c[0] = 0;
  FUN_0003305c(param_1,0x21,0,0,param_4);
  pcVar3 = "CONFIG,";
  if (param_4 != 0) {
    pcVar3 = ",\"CONFIG\":[";
  }
  uVar1 = FUN_000307c8(param_1,pcVar3);
  uVar2 = FUN_00032ed0(0,"ASC Count",&local_20,0);
  uVar2 = FUN_00032ed0(uVar2,"PGA Count",local_1c,0);
  uVar2 = FUN_00032ed0(uVar2,"Pool Count",&DAT_0007b198,0);
  uVar2 = FUN_00032e40(uVar2,"Strategy",(&DAT_00075e10)[DAT_00076efc],0);
  uVar2 = FUN_00032ed0(uVar2,"Log Interval",&DAT_00075dcc,0);
  uVar2 = FUN_00032e40(uVar2,"Device Code","",0);
  uVar2 = FUN_00032e40(uVar2,&DAT_00067184,"Linux",0);
  uVar2 = FUN_00032ff0(uVar2,"Failover-Only",&DAT_0007b625,0);
  uVar2 = FUN_00032ed0(uVar2,"ScanTime",&DAT_00075df0,0);
  uVar2 = FUN_00032ed0(uVar2,"Queue",&DAT_00075ddc,0);
  uVar2 = FUN_00032ed0(uVar2,"Expiry",&DAT_00075df4,0);
  uVar2 = FUN_00032e40(uVar2,"Hotplug",&DAT_000671c4,0);
  FUN_00032534(param_1,uVar2,param_4,0);
  if ((uVar1 & param_4) != 0) {
    *(undefined1 *)(param_1 + 0xd) = 1;
  }
  return;
}

