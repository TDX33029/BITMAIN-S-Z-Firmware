//==================== FUN_0000f854 @ 0000f854 ====================

undefined4
FUN_0000f854(undefined4 param_1,uint param_2,undefined4 param_3,undefined4 param_4,
            undefined1 param_5)

{
  int iVar1;
  undefined1 uStack00000000;
  undefined1 auStack_1020 [4096];
  undefined4 local_10;
  undefined4 local_c;
  undefined4 uStack_8;
  undefined4 uStack_4;
  
  local_10._3_1_ = (char)((uint)param_1 >> 0x18);
  local_10 = param_1;
  local_c = param_2;
  uStack_8 = param_3;
  uStack_4 = param_4;
  if (local_10._3_1_ == '0') {
    local_c._1_1_ = (char)(param_2 >> 8);
    if ((param_2 & 0xff) == 0x31) {
      if (local_c._1_1_ == '\0') {
        FUN_0000edfc(&DAT_0001163c);
        goto LAB_0000f898;
      }
    }
    else if (((param_2 & 0xff) == 0x36) && (local_c._1_1_ == '\0')) {
      memcpy(auStack_1020,&param_5,0xfff);
      FUN_0000f410(local_10,local_c,uStack_8,uStack_4);
      iVar1 = FUN_0000f4c0();
      if (iVar1 != 1) {
        FUN_0000f69c(&DAT_00011650,&DAT_00011d94);
        puts("restart cgminer FAILD ");
        return 0;
      }
      FUN_0000f69c(&DAT_00011650,&DAT_00011d78);
      puts("restart cgminer OK OK ");
      goto LAB_0000f898;
    }
  }
  iVar1 = FUN_0000f108((int)&local_10 + 3);
  if (iVar1 == 0) {
    puts("after send_getstatus_res()");
    return 0;
  }
LAB_0000f898:
  memcpy(auStack_1020,&param_5,0xfff);
  FUN_0000e7f4(local_10,local_c,uStack_8,uStack_4);
  return 1;
}

