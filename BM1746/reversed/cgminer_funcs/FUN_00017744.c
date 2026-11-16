//==================== FUN_00017744 @ 00017744 ====================

void FUN_00017744(char *param_1,size_t param_2,int param_3)

{
  size_t sVar1;
  size_t sVar2;
  undefined4 uVar3;
  undefined4 extraout_s1;
  double dVar4;
  double dVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  char acStack_1098 [16];
  char acStack_1088 [16];
  char acStack_1078 [64];
  char acStack_1038 [4100];
  
  uVar3 = FUN_000157ec(param_3);
  dVar5 = (double)FUN_0005cb64(*(undefined4 *)(param_3 + 0xc0),*(undefined4 *)(param_3 + 0xc4));
  uVar6 = FUN_0005cc48(*(undefined4 *)(param_3 + 0x30),*(undefined4 *)(param_3 + 0x34));
  dVar4 = *(double *)(param_3 + 0x50) / (double)CONCAT44(extraout_s1,uVar3);
  uVar7 = FUN_0005cc48(SUB84(dVar4,0),(int)((ulonglong)dVar4 >> 0x20));
  FUN_00011580((int)uVar7,(int)((ulonglong)uVar7 >> 0x20),acStack_1098,0x10,4);
  FUN_00011580((int)uVar6,(int)((ulonglong)uVar6 >> 0x20),acStack_1088,0x10,4);
  snprintf(param_1,param_2,"%s%d ",*(char **)(*(int *)(param_3 + 4) + 8),*(int *)(param_3 + 8));
  (**(code **)(*(int *)(param_3 + 4) + 0x14))(param_1,param_2,param_3);
  sVar1 = strlen(param_1);
  snprintf(acStack_1078,0x40,"(%ds):%s (avg):%sh/s | A:%.0f R:%.0f HW:%d WU:%.1f/m",DAT_00075dcc,
           acStack_1088,acStack_1098,*(double *)(param_3 + 200),*(double *)(param_3 + 0xd0),
           *(int *)(param_3 + 0x2c),(dVar5 / (double)CONCAT44(extraout_s1,uVar3)) * 60.0);
  sVar2 = strlen(acStack_1078);
  if (param_2 <= sVar2 + sVar1) {
    snprintf(acStack_1038,0x1000,"tailsprintf buffer overflow in %s %s line %d","cgminer.c",
             "get_statline",0xc09);
    FUN_00038730(3,acStack_1038,1);
    FUN_00016ca8(1);
  }
  strcat(param_1,acStack_1078);
  (**(code **)(*(int *)(param_3 + 4) + 0x18))(param_1,param_2,param_3);
  return;
}

