//==================== FUN_0003a940 @ 0003a940 ====================

void FUN_0003a940(void)

{
  void *pvVar1;
  void *pvVar2;
  int iVar3;
  char local_1010 [4];
  char acStack_100c [4];
  char acStack_1008 [4];
  char acStack_1004 [4];
  char local_1000 [4];
  char acStack_ffc [4];
  char acStack_ff8 [4];
  char acStack_ff4 [4072];
  
  pvVar1 = calloc(1,0x188);
  if (pvVar1 == (void *)0x0) {
                    /* WARNING: Subroutine does not return */
    __assert_fail("cgpu","driver-btm-zcash.c",0x1d38,"bitmain_ZCASH_detect");
  }
  *(undefined4 *)((int)pvVar1 + 0x94) = 1;
  *(undefined **)((int)pvVar1 + 4) = &DAT_00076ac8;
  *(undefined4 *)((int)pvVar1 + 0x20) = 0;
  pvVar2 = calloc(0x4b4,1);
  *(void **)((int)pvVar1 + 0x14) = pvVar2;
  if (pvVar2 == (void *)0x0) {
    local_1010[0] = 'F';
    local_1010[1] = 'a';
    local_1010[2] = 'i';
    local_1010[3] = 'l';
    acStack_100c[0] = 'e';
    acStack_100c[1] = 'd';
    acStack_100c[2] = ' ';
    acStack_100c[3] = 't';
    acStack_1008[0] = 'o';
    acStack_1008[1] = ' ';
    acStack_1008[2] = 'c';
    acStack_1008[3] = 'a';
    acStack_1004[0] = 'l';
    acStack_1004[1] = 'l';
    acStack_1004[2] = 'o';
    acStack_1004[3] = 'c';
    local_1000[0] = ' ';
    local_1000[1] = 'c';
    local_1000[2] = 'g';
    local_1000[3] = 'p';
    acStack_ffc[0] = 'u';
    acStack_ffc[1] = '_';
    acStack_ffc[2] = 'i';
    acStack_ffc[3] = 'n';
    acStack_ff8[0] = 'f';
    acStack_ff8[1] = 'o';
    acStack_ff8[2] = ' ';
    acStack_ff8[3] = 'd';
    acStack_ff4[0] = 'a';
    acStack_ff4[1] = 't';
    acStack_ff4[2] = 'a';
    acStack_ff4[3] = '\0';
    FUN_00038730(3,local_1010,1);
    FUN_00016ca8(1);
  }
  iVar3 = FUN_00028008(pvVar1);
  if (iVar3 == 0) {
                    /* WARNING: Subroutine does not return */
    __assert_fail("add_cgpu(cgpu)","driver-btm-zcash.c",0x1d40,"bitmain_ZCASH_detect");
  }
  if ((DAT_00078e09 != '\0') &&
     (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
    snprintf(local_1010,0x1000,"%s detect new device","bitmain_ZCASH_detect");
    FUN_00038730(7,local_1010,0);
  }
  return;
}

