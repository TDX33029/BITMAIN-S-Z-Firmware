//==================== FUN_0003b844 @ 0003b844 ====================

void FUN_0003b844(void)

{
  int iVar1;
  
  printf("\n\n--- %s\n","bitmain_axi_close");
  iVar1 = munmap(DAT_00078e10,0x1200);
  if (iVar1 < 0) {
    puts("munmap failed!");
  }
  iVar1 = munmap(DAT_00078eb8,0x1000000);
  if (iVar1 < 0) {
    puts("munmap failed!");
  }
  close(DAT_0007bd90);
  close(DAT_0007c0e8);
  return;
}

