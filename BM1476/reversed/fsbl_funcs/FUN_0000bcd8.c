//==================== FUN_0000bcd8 @ 0000bcd8 ====================

void FUN_0000bcd8(longlong *param_1,undefined4 param_2,uint param_3,int param_4)

{
  longlong lVar1;
  
  lVar1 = FUN_0000bc74();
  *param_1 = lVar1 + CONCAT44(DAT_00028f50 * param_4 +
                              (int)((ulonglong)param_3 * (ulonglong)DAT_00028f50 >> 0x20),
                              (int)((ulonglong)param_3 * (ulonglong)DAT_00028f50));
  return;
}

