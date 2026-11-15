//==================== FUN_0003ba70 @ 0003ba70 ====================

void FUN_0003ba70(void)

{
  printf("\n--- %s\n","init_fpga");
  *(undefined4 *)(DAT_00078e10 + 0x80) = 0x8080800f;
  do {
    FUN_0002a954(10000,0);
  } while (*(int *)(DAT_00078e10 + 0x80) < 0);
  FUN_0003b994();
  FUN_0002a954(100000,0);
  return;
}

