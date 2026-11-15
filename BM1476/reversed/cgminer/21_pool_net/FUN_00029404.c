//==================== FUN_00029404 @ 00029404 ====================

undefined4 FUN_00029404(int param_1,char *param_2,int param_3)

{
  size_t sVar1;
  int iVar2;
  int *piVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  size_t __n;
  uint uVar7;
  undefined8 uVar8;
  undefined8 uVar9;
  timeval local_b0;
  fd_set local_a8;
  
  sVar1 = strlen(param_2);
  __n = param_3 + 1;
  iVar4 = *(int *)(param_1 + 0x264);
  (param_2 + sVar1)[0] = '\n';
  (param_2 + sVar1)[1] = '\0';
  if (0 < (int)__n) {
    iVar5 = iVar4 + 0x1f;
    if (-1 < iVar4) {
      iVar5 = iVar4;
    }
    iVar6 = 0;
LAB_00029484:
    local_b0.tv_usec = 0;
    local_b0.tv_sec = 1;
    uVar7 = 0;
    do {
      if (0x1f < uVar7) {
        local_a8.fds_bits[iVar5 >> 5] = 1 << (iVar4 % 0x20 & 0xffU) | local_a8.fds_bits[iVar5 >> 5];
        iVar2 = select(iVar4 + 1,(fd_set *)0x0,&local_a8,(fd_set *)0x0,&local_b0);
        if (0 < iVar2) goto LAB_000294f0;
        piVar3 = __errno_location();
        uVar7 = 0;
        if (*piVar3 != 4) {
          return 1;
        }
      }
      local_a8.fds_bits[uVar7] = 0;
      uVar7 = uVar7 + 1;
    } while( true );
  }
  uVar8 = 0;
LAB_00029528:
  uVar9 = VectorAdd(*(undefined8 *)(param_1 + 0x210),uVar8,8);
  *(undefined8 *)(param_1 + 0x210) = uVar9;
  uVar7 = *(uint *)(param_1 + 0x208);
  uVar8 = VectorAdd(*(undefined8 *)(param_1 + 0x218),uVar8,8);
  *(undefined8 *)(param_1 + 0x218) = uVar8;
  *(uint *)(param_1 + 0x208) = uVar7 + 1;
  *(uint *)(param_1 + 0x20c) = *(int *)(param_1 + 0x20c) + (uint)(0xfffffffe < uVar7);
  return 0;
LAB_000294f0:
  iVar2 = send(*(int *)(param_1 + 0x264),param_2 + iVar6,__n,0x4000);
  if (iVar2 < 0) {
    piVar3 = __errno_location();
    if (*piVar3 != 0xb) {
      return 2;
    }
    iVar2 = 0;
  }
  __n = __n - iVar2;
  iVar6 = iVar6 + iVar2;
  if ((int)__n < 1) goto code_r0x00029520;
  goto LAB_00029484;
code_r0x00029520:
  uVar8 = VectorShiftRight(CONCAT44(iVar6,iVar6),0x20);
  goto LAB_00029528;
}

