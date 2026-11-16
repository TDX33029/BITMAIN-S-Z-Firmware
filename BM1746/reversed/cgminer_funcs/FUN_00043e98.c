//==================== FUN_00043e98 @ 00043e98 ====================

void FUN_00043e98(undefined4 param_1,uint param_2)

{
  undefined4 local_1c;
  undefined4 local_18;
  undefined2 local_14;
  undefined1 local_12;
  
  DAT_00076b5c = DAT_00076b5c & 0xffffe0ff | (param_2 & 0x1f) << 8;
  local_1c = 0;
  local_18 = 0;
  local_14 = 0;
  local_12 = 0;
  FUN_0003cdcc(&local_1c,1,0,0x1c,DAT_00076b5c);
  FUN_0003c150(param_1,&local_1c,0xb);
  FUN_0002a92c(1);
  return;
}

