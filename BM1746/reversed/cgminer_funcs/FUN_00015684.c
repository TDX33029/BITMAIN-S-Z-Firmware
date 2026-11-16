//==================== FUN_00015684 @ 00015684 ====================

void FUN_00015684(void *param_1,void *param_2,int param_3)

{
  uint uVar1;
  undefined4 uVar2;
  uint local_1c [2];
  
  uVar2 = *(undefined4 *)((int)param_1 + 0x724);
  FUN_00015648();
  memcpy(param_1,param_2,0x7b0);
  *(undefined4 *)((int)param_1 + 0x724) = uVar2;
  if (*(int *)((int)param_2 + 0x6f0) != 0) {
    uVar2 = __strdup();
    *(undefined4 *)((int)param_1 + 0x6f0) = uVar2;
  }
  if (*(int *)((int)param_2 + 0x710) != 0) {
    uVar2 = __strdup();
    *(undefined4 *)((int)param_1 + 0x710) = uVar2;
  }
  if (*(int *)((int)param_2 + 0x704) == 0) {
    if (param_3 != 0) {
      uVar1 = *(uint *)((int)param_1 + 0x44);
      uVar1 = (uVar1 << 0x18 | (uVar1 >> 8 & 0xff) << 0x10 | (uVar1 >> 0x10 & 0xff) << 8 |
              uVar1 >> 0x18) + param_3;
      *(uint *)((int)param_1 + 0x44) =
           uVar1 * 0x1000000 | (uVar1 >> 8 & 0xff) << 0x10 | (uVar1 >> 0x10 & 0xff) << 8 |
           uVar1 >> 0x18;
    }
  }
  else if (param_3 == 0) {
    uVar2 = __strdup();
    *(undefined4 *)((int)param_1 + 0x704) = uVar2;
  }
  else {
    uVar1 = *(uint *)((int)param_1 + 0x44);
    uVar1 = (uVar1 << 0x18 | (uVar1 >> 8 & 0xff) << 0x10 | (uVar1 >> 0x10 & 0xff) << 8 |
            uVar1 >> 0x18) + param_3;
    *(uint *)((int)param_1 + 0x44) =
         uVar1 * 0x1000000 | (uVar1 >> 8 & 0xff) << 0x10 | (uVar1 >> 0x10 & 0xff) << 8 |
         uVar1 >> 0x18;
    FUN_00029830(local_1c,*(undefined4 *)((int)param_2 + 0x704));
    uVar1 = (local_1c[0] << 0x18 | (local_1c[0] >> 8 & 0xff) << 0x10 |
             (local_1c[0] >> 0x10 & 0xff) << 8 | local_1c[0] >> 0x18) + param_3;
    local_1c[0] = uVar1 * 0x1000000 | (uVar1 >> 8 & 0xff) << 0x10 | (uVar1 >> 0x10 & 0xff) << 8 |
                  uVar1 >> 0x18;
    uVar2 = FUN_00029730(local_1c,4);
    *(undefined4 *)((int)param_1 + 0x704) = uVar2;
  }
  if (*(int *)((int)param_2 + 0x718) != 0) {
    uVar2 = __strdup();
    *(undefined4 *)((int)param_1 + 0x718) = uVar2;
  }
  return;
}

