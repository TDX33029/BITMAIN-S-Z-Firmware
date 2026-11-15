//==================== FUN_0001a3d4 @ 0001a3d4 ====================

void * FUN_0001a3d4(void)

{
  void *pvVar1;
  undefined4 uVar2;
  char local_1008 [4];
  char acStack_1004 [4];
  char acStack_1000 [4];
  char acStack_ffc [4];
  char local_ff8 [4];
  char acStack_ff4 [4];
  char acStack_ff0 [4];
  char acStack_fec [4];
  undefined2 uStack_fe8;
  undefined1 local_fe6;
  
  pvVar1 = calloc(1,0x7b0);
  if (pvVar1 == (void *)0x0) {
    local_1008[0] = 'F';
    local_1008[1] = 'a';
    local_1008[2] = 'i';
    local_1008[3] = 'l';
    acStack_1004[0] = 'e';
    acStack_1004[1] = 'd';
    acStack_1004[2] = ' ';
    acStack_1004[3] = 't';
    acStack_1000[0] = 'o';
    acStack_1000[1] = ' ';
    acStack_1000[2] = 'c';
    acStack_1000[3] = 'a';
    acStack_ffc[0] = 'l';
    acStack_ffc[1] = 'l';
    acStack_ffc[2] = 'o';
    acStack_ffc[3] = 'c';
    local_ff8[0] = ' ';
    local_ff8[1] = 'w';
    local_ff8[2] = 'o';
    local_ff8[3] = 'r';
    acStack_ff4[0] = 'k';
    acStack_ff4[1] = ' ';
    acStack_ff4[2] = 'i';
    acStack_ff4[3] = 'n';
    acStack_ff0[0] = ' ';
    acStack_ff0[1] = 'm';
    acStack_ff0[2] = 'a';
    acStack_ff0[3] = 'k';
    acStack_fec[0] = 'e';
    acStack_fec[1] = '_';
    acStack_fec[2] = 'w';
    acStack_fec[3] = 'o';
    uStack_fe8 = 0x6b72;
    local_fe6 = 0;
    FUN_00038730(3,local_1008,1);
    FUN_00016ca8(1);
  }
  uVar2 = FUN_0001a1e8();
  *(undefined4 *)((int)pvVar1 + 0x724) = uVar2;
  return pvVar1;
}

