//==================== FUN_00034c60 @ 00034c60 ====================

void FUN_00034c60(int param_1,undefined4 param_2,undefined4 param_3,int param_4)

{
  int iVar1;
  undefined4 uVar2;
  
  FUN_0003305c(param_1,0x16,0,0,param_4);
  if (param_4 == 0) {
    FUN_000307c8(param_1,"VERSION,");
    uVar2 = FUN_00032e1c(0,"CGMiner","4.9.0",0);
    uVar2 = FUN_00032e40(uVar2,&DAT_00060570,&DAT_000671e4,0);
    uVar2 = FUN_00032e1c(uVar2,"Miner",&DAT_00078b94,0);
    uVar2 = FUN_00032e1c(uVar2,"CompileTime",&DAT_00078750,0);
    uVar2 = FUN_00032e1c(uVar2,&DAT_000671fc,&DAT_00078850,0);
    FUN_00032534(param_1,uVar2,0,0);
    return;
  }
  iVar1 = FUN_000307c8(param_1,",\"VERSION\":[");
  uVar2 = FUN_00032e1c(0,"CGMiner","4.9.0",0);
  uVar2 = FUN_00032e40(uVar2,&DAT_00060570,&DAT_000671e4,0);
  uVar2 = FUN_00032e1c(uVar2,"Miner",&DAT_00078b94,0);
  uVar2 = FUN_00032e1c(uVar2,"CompileTime",&DAT_00078750,0);
  uVar2 = FUN_00032e1c(uVar2,&DAT_000671fc,&DAT_00078850,0);
  FUN_00032534(param_1,uVar2,1,0);
  if (iVar1 != 0) {
    *(undefined1 *)(param_1 + 0xd) = 1;
  }
  return;
}

