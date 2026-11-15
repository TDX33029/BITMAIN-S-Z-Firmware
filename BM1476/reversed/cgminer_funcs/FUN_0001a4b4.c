//==================== FUN_0001a4b4 @ 0001a4b4 ====================

undefined8 FUN_0001a4b4(int param_1)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  bool bVar4;
  bool bVar5;
  double dVar6;
  double dVar7;
  double dVar8;
  double dVar9;
  double dVar10;
  undefined8 uVar11;
  char acStack_1028 [4100];
  
  dVar6 = 1.7668201048317172e+72;
  if (DAT_0007b859 == '\0') {
    dVar6 = 2.695953529101131e+67;
  }
  dVar7 = (double)FUN_0005cb50(*(undefined4 *)(param_1 + 0xd8),*(undefined4 *)(param_1 + 0xdc));
  dVar8 = (double)FUN_0005cb50(*(undefined4 *)(param_1 + 0xd0),*(undefined4 *)(param_1 + 0xd4));
  dVar9 = (double)FUN_0005cb50(*(undefined4 *)(param_1 + 200),*(undefined4 *)(param_1 + 0xcc));
  dVar10 = (double)FUN_0005cb50(*(undefined4 *)(param_1 + 0xc0),*(undefined4 *)(param_1 + 0xc4));
  dVar10 = dVar7 * 6.277101735386681e+57 + dVar8 * 3.402823669209385e+38 +
           dVar9 * 1.8446744073709552e+19 + dVar10;
  if (dVar10 == 0.0) {
    dVar10 = 0.0;
  }
  dVar6 = round(dVar6 / dVar10);
  uVar11 = FUN_0005cc48(SUB84(dVar6,0),(int)((ulonglong)dVar6 >> 0x20));
  uVar3 = (uint)((ulonglong)uVar11 >> 0x20);
  uVar1 = (uint)uVar11;
  iVar2 = pthread_mutex_lock((pthread_mutex_t *)&DAT_0007ba10);
  if (iVar2 != 0) {
    FUN_0000b414("share_diff",0x137a);
  }
  iVar2 = pthread_rwlock_wrlock((pthread_rwlock_t *)&DAT_0007ba28);
  if (iVar2 != 0) {
    FUN_0000b480("share_diff",0x137a);
  }
  bVar4 = uVar3 <= DAT_00076f1c;
  if (DAT_00076f1c == uVar3) {
    bVar4 = uVar1 <= DAT_00076f18;
  }
  if (!bVar4) {
    DAT_00076f18 = uVar1;
    DAT_00076f1c = uVar3;
    FUN_00011580(uVar1,uVar3,&DAT_00075dc4,8,0);
  }
  iVar2 = *(int *)(param_1 + 0x6d4);
  bVar5 = uVar3 <= *(uint *)(iVar2 + 0x194);
  if (*(uint *)(iVar2 + 0x194) == uVar3) {
    bVar5 = uVar1 <= *(uint *)(iVar2 + 400);
  }
  if (!bVar5) {
    *(undefined8 *)(iVar2 + 400) = uVar11;
  }
  iVar2 = pthread_rwlock_unlock((pthread_rwlock_t *)&DAT_0007ba28);
  if (iVar2 != 0) {
    FUN_0000b630("share_diff",0x1383);
  }
  iVar2 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_0007ba10);
  if (iVar2 != 0) {
    FUN_0000b5c4("share_diff",0x1383);
  }
  (*DAT_00075ed8)();
  if (((!bVar4) && (DAT_00078e09 != '\0')) &&
     ((DAT_0007ad48 != '\0' || ((DAT_00078e08 != '\0' || (6 < DAT_000766c4)))))) {
    snprintf(acStack_1028,0x1000,"New best share: %s",(char *)&DAT_00075dc4);
    FUN_00038730(7,acStack_1028,0);
  }
  return uVar11;
}

