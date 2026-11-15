//==================== FUN_0004ae48 @ 0004ae48 ====================

void FUN_0004ae48(int param_1,undefined4 param_2,undefined1 param_3)

{
  undefined1 uVar1;
  int iVar2;
  uint uVar3;
  uint param4;
  char local_1010 [4];
  char acStack_100c [4];
  char acStack_1008 [4088];
  
  uVar3 = FUN_0003cc6c(param_1 + 2,0x33);
  if ((*(byte *)(param_1 + 8) & 0x1f) == uVar3) {
    if ((DAT_0007bd9c < 100) && (DAT_0007bd94 < 100)) {
      pthread_mutex_lock((pthread_mutex_t *)&DAT_00079b78);
      uVar3 = DAT_0007bd94;
      iVar2 = DAT_0007bd94 * 7;
      param4 = DAT_0007bd94;
      if (0x62 < DAT_0007bd94) {
        param4 = 0;
      }
      *(undefined4 *)(&DAT_0007bda1 + iVar2) = *(undefined4 *)(param_1 + 3);
      (&DAT_0007bda5)[iVar2] = *(undefined1 *)(param_1 + 7);
      uVar1 = *(undefined1 *)(param_1 + 2);
      (&DAT_0007bda6)[iVar2] = param_3;
      if (0x62 < uVar3) {
        DAT_0007bd94 = param4;
      }
      (&DAT_0007bda0)[iVar2] = uVar1;
      if (uVar3 < 99) {
        if ((DAT_00078e09 != '\0') &&
           (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
          snprintf(local_1010,0x1000,"%s: p_wr = %d reg_value_num = %d","reg_handle",param4,
                   DAT_0007bd9c);
          FUN_00038730(7,local_1010,0);
          param4 = DAT_0007bd94;
        }
        DAT_0007bd94 = param4 + 1;
      }
      if (DAT_0007bd9c < 100) {
        DAT_0007bd9c = DAT_0007bd9c + 1;
      }
      else {
        DAT_0007bd9c = 100;
        if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
          snprintf(local_1010,0x1000,"%s: reg fifo full!!!","reg_handle");
          FUN_00038730(4,local_1010,0);
        }
      }
      pthread_mutex_unlock((pthread_mutex_t *)&DAT_00079b78);
    }
    else {
      if ((DAT_0007ad48 != '\0') || ((DAT_00078e08 != '\0' || (4 < DAT_000766c4)))) {
        local_1010[0] = 'W';
        local_1010[1] = 'i';
        local_1010[2] = 'l';
        local_1010[3] = 'l';
        acStack_100c[0] = ' ';
        acStack_100c[1] = 'C';
        acStack_100c[2] = 'l';
        acStack_100c[3] = 'e';
        acStack_1008[0] = 'a';
        acStack_1008[1] = 'n';
        acStack_1008[2] = '!';
        acStack_1008[3] = '\0';
        FUN_00038730(5,local_1010,0);
      }
      FUN_000481b4();
    }
  }
  else if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
    snprintf(local_1010,0x1000,"%s CRC error crc = %02x\n","reg_handle",uVar3);
    FUN_00038730(3,local_1010,0);
  }
  return;
}

