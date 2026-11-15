//==================== FUN_0000a7f4 @ 0000a7f4 ====================

undefined4 FUN_0000a7f4(void)

{
  char *pcVar1;
  char acStack_20 [16];
  
  while (DAT_0001b2ec != '\0') {
    pcVar1 = (char *)FUN_0000f990(DAT_0001b2f0,"api-switch",acStack_20);
    if ((((*pcVar1 == 'o') && (pcVar1[1] == 'f')) && (pcVar1[2] == 'f')) && (pcVar1[3] == '\0')) {
      DAT_0001b394 = 1;
    }
    printf("key_switch = %s\n",acStack_20);
    sleep(0xf);
  }
  puts("will bi exited###################");
  return 0;
}

