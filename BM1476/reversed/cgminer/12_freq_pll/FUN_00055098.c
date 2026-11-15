//==================== FUN_00055098 @ 00055098 ====================

undefined4 FUN_00055098(undefined4 param_1,int param_2,void *param_3,uint param_4)

{
  int iVar1;
  undefined1 local_d8;
  undefined1 local_d7;
  undefined1 local_d6;
  undefined1 local_d5;
  undefined1 local_d4;
  undefined1 local_d3;
  undefined1 local_d2;
  undefined1 local_d1;
  undefined1 local_d0;
  undefined1 local_cf;
  undefined1 local_ce;
  undefined1 local_cd;
  undefined1 local_cc;
  undefined1 local_cb;
  undefined1 local_ca;
  undefined1 local_c9;
  undefined1 local_c8;
  undefined1 local_c7;
  undefined2 local_c6;
  undefined2 local_c4;
  undefined2 local_c2;
  undefined2 local_c0;
  undefined2 local_be;
  undefined2 local_bc;
  undefined2 local_ba;
  undefined4 local_b8;
  undefined4 local_b4;
  undefined4 local_b0;
  undefined4 local_ac;
  undefined4 local_a8;
  undefined4 local_a4;
  undefined4 local_a0;
  undefined4 local_9c;
  undefined1 auStack_98 [132];
  
  if (((param_2 - 1U < 0x40) && (param_3 != (void *)0x0 && param_4 != 0)) && (param_4 < 0x41)) {
    local_d7 = (undefined1)param_4;
    local_d8 = (undefined1)param_2;
    local_d4 = 0;
    local_d3 = 0;
    local_d2 = 0;
    local_d1 = 0;
    local_d0 = 0;
    local_cf = 0;
    local_ce = 0;
    local_cd = 0;
    local_cc = 0;
    local_cb = 0;
    local_ca = 0;
    local_c9 = 0;
    local_c8 = 0;
    local_c7 = 0;
    local_c6 = 0;
    local_c4 = 0;
    local_c2 = 0;
    local_c0 = 0;
    local_be = 0;
    local_bc = 0;
    local_ba = 0;
    local_b8 = 0;
    local_b4 = 0;
    local_b0 = 0;
    local_ac = 0;
    local_a8 = 0;
    local_a4 = 0;
    local_a0 = 0;
    local_9c = 0;
    local_d6 = 1;
    local_d5 = 1;
    iVar1 = FUN_00054dcc(param_1,&local_d8);
    if (-1 < iVar1) {
      memset(auStack_98,0,0x80);
      memcpy(auStack_98,param_3,param_4);
      FUN_00054f8c(param_1,auStack_98,0x80,0);
      (*DAT_00076b94)(auStack_98,0,0x80);
      return 0;
    }
  }
  return 0xffffffff;
}

