//==================== FUN_00028afc @ 00028afc ====================

bool FUN_00028afc(int param_1,uint param_2)

{
  __suseconds_t *p_Var1;
  int iVar2;
  int iVar3;
  timeval local_a0;
  fd_set local_98;
  
  iVar3 = *(int *)(param_1 + 0x264);
  local_a0.tv_sec = param_2 & ~((int)param_2 >> 0x1f);
  p_Var1 = &local_a0.tv_usec;
  do {
    p_Var1 = p_Var1 + 1;
    *p_Var1 = 0;
  } while (p_Var1 != local_98.fds_bits + 0x1f);
  iVar2 = iVar3 + 0x1f;
  if (-1 < iVar3) {
    iVar2 = iVar3;
  }
  local_a0.tv_usec = 0;
  local_98.fds_bits[iVar2 >> 5] = local_98.fds_bits[iVar2 >> 5] | 1 << (iVar3 % 0x20 & 0xffU);
  iVar3 = select(iVar3 + 1,&local_98,(fd_set *)0x0,(fd_set *)0x0,&local_a0);
  return 0 < iVar3;
}

