//==================== FUN_0003c0a0 @ 0003c0a0 ====================

undefined4 FUN_0003c0a0(int param_1)

{
  size_t __size;
  void *__ptr;
  size_t param4;
  undefined4 uVar1;
  pthread_mutex_t *__mutex;
  
  __mutex = (pthread_mutex_t *)(&DAT_00078fac + param_1 * 0x18);
  pthread_mutex_lock(__mutex);
  __size = FUN_0003bb78(param_1);
  if (__size == 0) {
    pthread_mutex_unlock(__mutex);
    uVar1 = 0;
  }
  else {
    __ptr = malloc(__size);
    if (__ptr != (void *)0x0) {
      param4 = FUN_0003bd44(param_1,__ptr,__size);
      if (param4 != __size) {
        printf("%s: uart%d clear rx fifo error. nbytes = %d, len = %d\n","clear_uart_rx_fifo",
               param_1,__size,param4);
      }
      free(__ptr);
    }
    usleep(500000);
    pthread_mutex_unlock(__mutex);
    uVar1 = 1;
  }
  return uVar1;
}

