//==================== FUN_0000e284 @ 0000e284 ====================

undefined4 FUN_0000e284(char *param_1,char *param_2)

{
  int __fd;
  int iVar1;
  char *__src;
  undefined4 uVar2;
  char acStack_30 [15];
  undefined1 local_21;
  in_addr iStack_1c;
  
  __fd = socket(2,2,0);
  if (__fd == -1) {
    perror("socket");
    uVar2 = 0xffffffff;
  }
  else {
    strncpy(acStack_30,param_1,0x14);
    local_21 = 0x30;
    iVar1 = ioctl(__fd,0x8915,acStack_30);
    if (iVar1 < 0) {
      printf(":No Such Device %s/n",param_1);
      uVar2 = 0xffffffff;
    }
    else {
      __src = inet_ntoa(iStack_1c);
      strcpy(param_2,__src);
      close(__fd);
      uVar2 = 1;
    }
  }
  return uVar2;
}

