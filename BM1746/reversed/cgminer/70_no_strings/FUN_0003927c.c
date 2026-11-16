//==================== FUN_0003927c @ 0003927c ====================

int FUN_0003927c(int param_1)

{
  int iVar1;
  
  iVar1 = FUN_000391fc();
  if (iVar1 != 0) {
    memset(*(void **)(iVar1 + 0xc),0,*(size_t *)(param_1 + 0x14));
  }
  return iVar1;
}

