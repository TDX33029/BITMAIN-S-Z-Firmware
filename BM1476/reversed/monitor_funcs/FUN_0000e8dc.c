//==================== FUN_0000e8dc @ 0000e8dc ====================

undefined4 FUN_0000e8dc(void)

{
  char cVar1;
  uint param1;
  uint param1_00;
  bool bVar2;
  undefined1 auStack_1090 [4096];
  undefined1 local_90;
  undefined1 local_8f;
  undefined1 auStack_8c [104];
  
  memset(auStack_8c,0,100);
  printf("r_connect = %d.\n",DAT_0001b380);
  DAT_00026f94 = (*DAT_0001d434)(DAT_00026f98,DAT_000258bc,&DAT_0001e454,2);
  if (DAT_00026f94 == 0xffffffff) {
    puts("recv data failed1.");
    DAT_0001b380 = 0;
    memcpy(auStack_1090,&DAT_0001c3e4,0xfff);
    FUN_0000e7f4(DAT_0001c3d4,DAT_0001c3d8,DAT_0001c3dc,DAT_0001c3e0);
    FUN_0000e4cc(&DAT_0001b3a8);
    FUN_0000e4cc(&DAT_0001d410);
    return 0;
  }
  printf("recv package.pkg_head:%s.\n",&DAT_0001e454);
  bVar2 = DAT_00026f94 == 0;
  (&DAT_0001e454)[DAT_00026f94] = 0;
  cVar1 = DAT_0001e456;
  if (bVar2) {
    puts("the first two byte not 00   it is 0");
    return 0;
  }
  if (((DAT_0001e454 == '0') && (DAT_0001e455 == '0')) && (DAT_0001e456 == '\0')) {
    DAT_00026f94 = (*DAT_0001d434)(DAT_00026f98,DAT_000258bc,&DAT_0001e457,2);
    if (DAT_00026f94 == 0xffffffff) {
      DAT_0001b380 = 0;
      return 0;
    }
    if (DAT_00026f94 == 0) {
      return 0;
    }
    if (DAT_00026f94 == 2) {
      DAT_0001e458._1_1_ = cVar1;
      printf("rev from server cmd = \n%s\n",&DAT_0001e457);
      DAT_00026f94 = (*DAT_0001d434)(DAT_00026f98,DAT_000258bc,&local_90,2);
      if (DAT_00026f94 == 0xffffffff) {
        DAT_0001b380 = 0;
        return 0;
      }
      if (DAT_00026f94 == 0) {
        return 0;
      }
      if (DAT_00026f94 == 2) {
        param1_00 = (uint)CONCAT11(local_90,local_8f);
        printf("rev from server sum=\n%d\n",param1_00);
        if (param1_00 == 0) {
          DAT_0001f45c = local_8f;
          return 1;
        }
        param1 = (*DAT_0001d434)(DAT_00026f98,DAT_000258bc,&DAT_0001f45c,param1_00);
        if (param1 == 0xffffffff) {
          DAT_0001b380 = 0;
          DAT_00026f94 = param1;
          return 0;
        }
        DAT_00026f94 = param1;
        (&DAT_0001f45c)[param1_00] = 0;
        printf("recvbytes:%d,sum:%d,pkgnonce=%s\n",param1,param1_00,&DAT_0001f45c);
        if (DAT_00026f94 == 0) {
          DAT_0001b380 = 0;
          return 0;
        }
        if (param1_00 == DAT_00026f94) {
          return 1;
        }
        puts("PAKGE ERROR!!!!!");
      }
    }
  }
  return 0xffffffff;
}

