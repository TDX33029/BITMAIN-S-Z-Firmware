//==================== FUN_0003c150 @ 0003c150 ====================

uint FUN_0003c150(int param_1,void *param_2,uint param_3)

{
  byte *pbVar1;
  byte *pbVar2;
  byte *pbVar3;
  byte bVar4;
  char *__format;
  byte *pbVar5;
  uint uVar6;
  uint uVar7;
  int iVar8;
  pthread_mutex_t *__mutex;
  int iVar9;
  int iVar10;
  byte *pbVar11;
  int in_stack_ffffedb0;
  int local_123c;
  int local_1238;
  byte local_1228 [512];
  char acStack_1028 [4100];
  
  memset(local_1228,0,0x200);
  __mutex = (pthread_mutex_t *)(&DAT_00078ebc + param_1 * 0x18);
  pthread_mutex_lock(__mutex);
  switch(param_1) {
  case 0:
    iVar10 = 0x180;
    iVar8 = 0x18;
    local_1238 = 0x65;
    local_123c = 100;
    break;
  case 1:
    iVar10 = 0x180;
    iVar8 = 0x10;
    local_1238 = 0x67;
    local_123c = 0x66;
    break;
  case 2:
    iVar10 = 0x180;
    iVar8 = 8;
    local_1238 = 0x69;
    local_123c = 0x68;
    break;
  case 3:
    iVar10 = 0x180;
    iVar8 = 0;
    local_1238 = 0x6b;
    local_123c = 0x6a;
    break;
  case 4:
    iVar10 = 0x184;
    iVar8 = 0x18;
    local_1238 = 0x6d;
    local_123c = 0x6c;
    break;
  case 5:
    iVar10 = 0x184;
    iVar8 = 0x10;
    local_1238 = 0x6f;
    local_123c = 0x6e;
    break;
  case 6:
    iVar10 = 0x184;
    iVar8 = 8;
    local_1238 = 0x71;
    local_123c = 0x70;
    break;
  case 7:
    iVar10 = 0x184;
    iVar8 = 0;
    local_1238 = 0x73;
    local_123c = 0x72;
    break;
  case 8:
    iVar10 = 0x188;
    iVar8 = 0x18;
    local_1238 = 0x75;
    local_123c = 0x74;
    break;
  case 9:
    iVar10 = 0x188;
    iVar8 = 0x10;
    local_1238 = 0x77;
    local_123c = 0x76;
    break;
  default:
    if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 5)) {
      return 0;
    }
    snprintf(acStack_1028,0x1000,"%s: The uart%d is not supported!!!\n","uart_send",
             in_stack_ffffedb0);
    FUN_00038730(5,acStack_1028,0);
    return 0;
  }
  iVar9 = 0x15;
  while ((*(uint *)(DAT_00078e10 + iVar10) >> iVar8 & 0xff) < param_3) {
    usleep(3000);
    iVar9 = iVar9 + -1;
    if (iVar9 == 0) goto code_r0x0003c238;
  }
  if (-1 < *(int *)(DAT_00078e10 + local_123c * 4)) goto LAB_0003c470;
  iVar8 = 0x15;
  do {
    usleep(3000);
    if (-1 < *(int *)(DAT_00078e10 + local_123c * 4)) {
LAB_0003c470:
      iVar8 = DAT_00078e10;
      memcpy(local_1228,param_2,param_3);
      uVar7 = param_3 >> 2;
      if (uVar7 != 0) {
        pbVar5 = local_1228;
        pbVar11 = pbVar5 + uVar7 * 4;
        do {
          pbVar1 = pbVar5 + 1;
          bVar4 = *pbVar5;
          pbVar2 = pbVar5 + 3;
          pbVar3 = pbVar5 + 2;
          pbVar5 = pbVar5 + 4;
          *(uint *)(iVar8 + local_1238 * 4) =
               (uint)*pbVar1 << 0x10 | (uint)bVar4 << 0x18 | (uint)*pbVar2 | (uint)*pbVar3 << 8;
          iVar8 = DAT_00078e10;
        } while (pbVar5 != pbVar11);
      }
      uVar6 = param_3 & 3;
      if (uVar6 != 0) {
        if (uVar6 == 2) {
          uVar7 = (uint)local_1228[uVar7 * 4] << 0x18 | (uint)local_1228[uVar7 * 4 + 1] << 0x10;
        }
        else if (uVar6 == 3) {
          uVar7 = (uint)local_1228[uVar7 * 4 + 2] << 8 | (uint)local_1228[uVar7 * 4 + 1] << 0x10 |
                  (uint)local_1228[uVar7 * 4] << 0x18;
        }
        else {
          uVar7 = (uint)local_1228[uVar7 * 4] << 0x18;
        }
        *(uint *)(iVar8 + local_1238 * 4) = uVar7;
        iVar8 = DAT_00078e10;
      }
      *(uint *)(iVar8 + local_123c * 4) = param_3 | 0x80000000;
      pthread_mutex_unlock(__mutex);
      return param_3;
    }
    iVar8 = iVar8 + -1;
  } while (iVar8 != 0);
  pthread_mutex_unlock(__mutex);
  if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 5)) {
    return 0;
  }
  __format = "%s: uart%d always busy, break\n";
LAB_0003c280:
  snprintf(acStack_1028,0x1000,__format,"uart_send",param_1);
  FUN_00038730(5,acStack_1028,0);
  return 0;
code_r0x0003c238:
  pthread_mutex_unlock(__mutex);
  if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 5)) {
    return 0;
  }
  __format = "%s: uart%d always dose not has enough send fifo space, break\n";
  goto LAB_0003c280;
}

