//==================== FUN_0003bf0c @ 0003bf0c ====================

uint FUN_0003bf0c(int param_1)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  pthread_mutex_t *__mutex;
  int iVar4;
  
  printf("--- %s\n","clear_uart_send_fifo");
  __mutex = (pthread_mutex_t *)(&DAT_00078ebc + param_1 * 0x18);
  pthread_mutex_lock(__mutex);
  switch(param_1) {
  case 0:
    iVar4 = 0x180;
    iVar3 = 0x18;
    break;
  case 1:
    iVar4 = 0x180;
    iVar3 = 0x10;
    break;
  case 2:
    iVar4 = 0x180;
    iVar3 = 8;
    break;
  case 3:
    iVar4 = 0x180;
    iVar3 = 0;
    break;
  case 4:
    iVar4 = 0x184;
    iVar3 = 0x18;
    break;
  case 5:
    iVar4 = 0x184;
    iVar3 = 0x10;
    break;
  case 6:
    iVar4 = 0x184;
    iVar3 = 8;
    break;
  case 7:
    iVar4 = 0x184;
    iVar3 = 0;
    break;
  case 8:
    iVar4 = 0x188;
    iVar3 = 0x18;
    break;
  case 9:
    iVar4 = 0x188;
    iVar3 = 0x10;
    break;
  default:
    printf("%s: The uart%d is not supported!!!\n","clear_uart_send_fifo",param_1);
    pthread_mutex_unlock(__mutex);
    return 1;
  }
  iVar1 = 0x15;
  do {
    uVar2 = *(uint *)(DAT_00078e10 + iVar4) >> iVar3 & 0xff;
    if (uVar2 == 0xff) {
      pthread_mutex_unlock(__mutex);
      return 0;
    }
    printf("%s: waiting fpga uart%d clear send fifo space ...\n","clear_uart_send_fifo",param_1);
    usleep(3000);
    iVar1 = iVar1 + -1;
  } while (iVar1 != 0);
  printf("%s: uart%d always dose not has enough send fifo space, break\n","clear_uart_send_fifo",
         param_1);
  pthread_mutex_unlock(__mutex);
  return uVar2;
}

