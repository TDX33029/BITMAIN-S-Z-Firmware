//==================== FUN_00038e94 @ 00038e94 ====================

undefined4 * FUN_00038e94(undefined4 *param_1)

{
  undefined4 *puVar1;
  char acStack_1010 [4100];
  
  puVar1 = calloc(1,0x40);
  if (puVar1 == (undefined4 *)0x0) {
    snprintf(acStack_1010,0x1000,"Failed to calloc store for %s in %s %s():%d",(char *)*param_1,
             "klist.c","k_new_store",0x55);
    FUN_00038730(3,acStack_1010,1);
    FUN_00016ca8(1);
  }
  *(undefined1 *)(puVar1 + 1) = 1;
  puVar1[2] = param_1[2];
  *puVar1 = *param_1;
  *(undefined1 *)(puVar1 + 0xb) = *(undefined1 *)(param_1 + 0xb);
  return puVar1;
}

