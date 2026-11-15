//==================== FUN_0003ca38 @ 0003ca38 ====================

undefined4 FUN_0003ca38(char param_1,int param_2,int param_3,uint param_4)

{
  byte bVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  byte abStack_1044 [5];
  undefined1 local_103f;
  undefined1 local_103e;
  undefined1 local_103d;
  undefined1 local_103c;
  undefined4 local_1038;
  undefined4 local_1034;
  undefined4 local_1030;
  undefined4 local_102c;
  char acStack_1028 [4100];
  
  abStack_1044[4] = param_1 << 1 | 1;
  local_103f = (undefined1)param_2;
  abStack_1044[1] = 0xaa;
  abStack_1044[3] = 0x32;
  iVar4 = (param_4 & 0xff) + param_2 + ((byte)(param_1 << 1) | 1) + 0x39;
  local_103e = (undefined1)param_4;
  local_1034 = 0;
  local_1030 = 0;
  uVar2 = 0;
  abStack_1044[2] = 7;
  local_103d = (undefined1)((uint)iVar4 >> 8);
  local_103c = (undefined1)iVar4;
  local_102c = 0;
  local_1038 = 0xff;
  abStack_1044[0] = 0x55;
  pthread_mutex_lock((pthread_mutex_t *)&DAT_000790a0);
  bVar1 = 0x55;
  while( true ) {
    FUN_0003c7b0(bVar1);
    uVar2 = uVar2 + 1 & 0xff;
    if (uVar2 == 9) break;
    bVar1 = abStack_1044[uVar2];
  }
  uVar2 = 0;
  FUN_0002a92c(200);
  local_1038 = 0;
  local_1034 = 0;
  local_1030 = 0;
  local_102c = 0;
  if ((int)(param_4 + 4) < 0) {
    pthread_mutex_unlock((pthread_mutex_t *)&DAT_000790a0);
  }
  else {
    do {
      bVar1 = FUN_0003c7c8();
      uVar3 = uVar2 + 1 & 0xff;
      abStack_1044[uVar2 + 0xc] = bVar1;
      uVar2 = uVar3;
    } while ((int)uVar3 <= (int)(param_4 + 4));
    uVar2 = local_1038 >> 8 & 0xff;
    pthread_mutex_unlock((pthread_mutex_t *)&DAT_000790a0);
    if ((uVar2 == 0x32) && (local_1038._2_1_ == '\x01')) {
      if (0 < (int)param_4) {
        uVar2 = 0;
        do {
          *(char *)(param_3 + uVar2) = acStack_1028[uVar2 - 0xd];
          uVar2 = uVar2 + 1 & 0xff;
        } while ((int)uVar2 < (int)param_4);
      }
      return 0;
    }
  }
  if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (4 < DAT_000766c4)) {
    snprintf(acStack_1028,0x1000,
             "--- %s failed! read_back_data[0] = 0x%02x, read_back_data[1] = 0x%02x, read_back_data[2] = 0x%02x, read_back_data[3] = 0x%02x\n"
             ,"read_dc_dc",local_1038 & 0xff,uVar2,local_1038 >> 0x10 & 0xff,local_1038 >> 0x18);
    FUN_00038730(5,acStack_1028,0);
  }
  return 0xffffffff;
}

