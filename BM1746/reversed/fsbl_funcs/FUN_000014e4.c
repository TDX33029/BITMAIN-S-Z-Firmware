//==================== FUN_000014e4 @ 000014e4 ====================

void FUN_000014e4(void)

{
  if (DAT_00021540 == Reset) {
    do {
      if (DAT_00019704 == 0) {
        return;
      }
    } while (DAT_00019704 != 0);
    return;
  }
  (*DAT_00021540)();
  do {
    if (DAT_00019704 == 0) {
      return;
    }
  } while (DAT_00019704 != 0);
  return;
}

