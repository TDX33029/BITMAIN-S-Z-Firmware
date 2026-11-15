//==================== FUN_0000e88c @ 0000e88c ====================

int FUN_0000e88c(undefined4 param_1,undefined4 param_2)

{
  byte *pbVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  byte local_144 [32];
  undefined1 auStack_124 [264];
  
  FUN_000027e0(auStack_124,0,0x104);
  iVar3 = -0x7ff1f80;
  iVar4 = 0;
  FUN_0000be74(param_1,auStack_124,0x100);
  FUN_0000af74();
  do {
    iVar5 = 0;
    FUN_0000b0e4(auStack_124 + iVar4 * 0x1a,local_144);
    do {
      iVar2 = FUN_0000b254(iVar3);
      pbVar1 = local_144 + iVar5;
      if (iVar2 == 1) {
        iVar3 = iVar3 + 4;
      }
      iVar5 = iVar5 + 1;
      if (((*pbVar1 & 1) != 0) && (iVar2 = FUN_0000e7c4(iVar3,param_2), iVar2 != 0)) {
        return iVar2;
      }
      iVar3 = iVar3 + 4;
    } while (iVar5 != 0x1f);
    iVar4 = iVar4 + 1;
    if (iVar4 == 10) {
      return 0;
    }
  } while( true );
}

