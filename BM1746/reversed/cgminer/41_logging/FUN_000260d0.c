//==================== FUN_000260d0 @ 000260d0 ====================

long FUN_000260d0(void)

{
  char *__nptr;
  long lVar1;
  
  __nptr = (char *)FUN_00025fdc();
  lVar1 = strtol(__nptr,(char **)0x0,10);
  free(__nptr);
  return lVar1;
}

