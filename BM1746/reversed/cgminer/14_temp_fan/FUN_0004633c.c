//==================== FUN_0004633c @ 0004633c ====================

void FUN_0004633c(undefined4 param_1,int param_2,uint param_3,uint param_4,byte param_5)

{
  undefined4 local_1c;
  undefined4 local_18;
  undefined2 local_14;
  undefined1 local_12;
  
  local_1c = 0;
  local_18 = 0;
  local_14 = 0;
  local_12 = 0;
  FUN_0003cdcc(&local_1c,0,(&DAT_00079130)[param_2],0x20,
               DAT_00076b6c & 0xff010000 | 0x1000000 | (param_3 & 0x7f) << 0x11 |
               (param_4 & 0xff) << 8 | (uint)param_5 | 0x10000);
  FUN_0003c150(param_1,&local_1c,0xb);
  FUN_0002a92c(0x32);
  return;
}

