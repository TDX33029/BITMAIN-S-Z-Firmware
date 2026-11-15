//==================== FUN_00039f2c @ 00039f2c ====================

uint FUN_00039f2c(int param_1,int param_2,uint param_3)

{
  byte bVar1;
  int iVar2;
  int *piVar3;
  uint uVar4;
  uint uVar5;
  undefined4 *puVar6;
  pthread_mutex_t *ppVar7;
  int iVar8;
  uint in_fpscr;
  undefined4 uVar9;
  undefined4 extraout_s1;
  double dVar10;
  double dVar11;
  undefined4 local_1030;
  undefined4 uStack_102c;
  char acStack_1028 [4100];
  
  puVar6 = *(undefined4 **)(param_1 + 0x18);
  if (puVar6 == (undefined4 *)0x0) {
    uVar4 = 0;
  }
  else {
    FUN_0002a84c(&local_1030);
    uVar4 = puVar6[4];
    puVar6[4] = uVar4 + 1;
    puVar6[5] = puVar6[5] + (uint)(0xfffffffe < uVar4);
    ppVar7 = *(pthread_mutex_t **)(puVar6[1] + 8);
    iVar2 = pthread_mutex_lock(ppVar7);
    if (iVar2 != 0) {
      piVar3 = __errno_location();
      snprintf(acStack_1028,0x1000,"WTF MUTEX ERROR ON LOCK! errno=%d in %s %s():%d",*piVar3,
               "noncedup.c","isdupnonce",0x46);
      FUN_00038730(3,acStack_1028,1);
      FUN_00016ca8(1);
    }
    iVar2 = pthread_rwlock_wrlock((pthread_rwlock_t *)(ppVar7 + 1));
    if (iVar2 != 0) {
      piVar3 = __errno_location();
      snprintf(acStack_1028,0x1000,"WTF WRLOCK ERROR ON LOCK! errno=%d in %s %s():%d",*piVar3,
               "noncedup.c","isdupnonce",0x46);
      FUN_00038730(3,acStack_1028,1);
      FUN_00016ca8(1);
    }
    uVar4 = 1;
    iVar2 = *(int *)(puVar6[2] + 0x10);
    while( true ) {
      if (iVar2 == 0) {
        uVar5 = 0;
      }
      else {
        uVar5 = uVar4;
      }
      if (uVar5 == 0) break;
      if ((**(int **)(iVar2 + 0xc) == *(int *)(param_2 + 0x724)) &&
         ((*(int **)(iVar2 + 0xc))[1] == param_3)) {
        uVar4 = 0;
        if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 4)) {
          uVar4 = 0;
        }
        else {
          snprintf(acStack_1028,0x1000,"%s%d: Duplicate nonce %08x",
                   *(char **)(*(int *)(param_1 + 4) + 8),*(int *)(param_1 + 8),param_3);
          FUN_00038730(4,acStack_1028,0);
        }
      }
      else {
        iVar2 = *(int *)(iVar2 + 4);
      }
    }
    if (uVar4 != 0) {
      iVar2 = FUN_000391fc(puVar6[1],"noncedup.c","isdupnonce",0x51);
      **(undefined4 **)(iVar2 + 0xc) = *(undefined4 *)(param_2 + 0x724);
      *(uint *)(*(int *)(iVar2 + 0xc) + 4) = param_3;
      iVar8 = *(int *)(iVar2 + 0xc);
      *(undefined4 *)(iVar8 + 8) = local_1030;
      *(undefined4 *)(iVar8 + 0xc) = uStack_102c;
      FUN_00039370(puVar6[2],iVar2,"noncedup.c","isdupnonce",0x55);
    }
    iVar2 = *(int *)(puVar6[2] + 0x10);
    while (iVar2 != 0) {
      uVar9 = FUN_0002aa24(*(int *)(iVar2 + 0xc) + 8,&local_1030);
      dVar10 = (double)CONCAT44(extraout_s1,uVar9);
      dVar11 = (double)VectorSignedToFloat(*puVar6,(byte)(in_fpscr >> 0x16) & 3);
      uVar5 = in_fpscr & 0xfffffff | (uint)(dVar10 < dVar11) << 0x1f |
              (uint)(dVar10 == dVar11) << 0x1e;
      in_fpscr = uVar5 | (uint)(NAN(dVar10) || NAN(dVar11)) << 0x1c;
      bVar1 = (byte)(uVar5 >> 0x18);
      if ((bool)(bVar1 >> 6 & 1) || bVar1 >> 7 != ((byte)(in_fpscr >> 0x1c) & 1)) break;
      uVar9 = FUN_000392a8(puVar6[2],"noncedup.c","isdupnonce",0x59);
      FUN_00039370(puVar6[1],uVar9,"noncedup.c","isdupnonce",0x5a);
      iVar2 = *(int *)(puVar6[2] + 0x10);
    }
    ppVar7 = *(pthread_mutex_t **)(puVar6[1] + 8);
    iVar2 = pthread_rwlock_unlock((pthread_rwlock_t *)(ppVar7 + 1));
    if (iVar2 != 0) {
      piVar3 = __errno_location();
      snprintf(acStack_1028,0x1000,"WTF RWLOCK ERROR ON UNLOCK! errno=%d in %s %s():%d",*piVar3,
               "noncedup.c","isdupnonce",0x5d);
      FUN_00038730(3,acStack_1028,1);
      FUN_00016ca8(1);
    }
    iVar2 = pthread_mutex_unlock(ppVar7);
    if (iVar2 != 0) {
      piVar3 = __errno_location();
      snprintf(acStack_1028,0x1000,"WTF MUTEX ERROR ON UNLOCK! errno=%d in %s %s():%d",*piVar3,
               "noncedup.c","isdupnonce",0x5d);
      FUN_00038730(3,acStack_1028,1);
      FUN_00016ca8(1);
    }
    (*DAT_00075ed8)();
    if (uVar4 == 0) {
      uVar5 = puVar6[6];
      puVar6[6] = uVar5 + 1;
      puVar6[7] = puVar6[7] + (uint)(0xfffffffe < uVar5);
    }
    uVar4 = uVar4 ^ 1;
  }
  return uVar4;
}

