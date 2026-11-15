//==================== FUN_0004a754 @ 0004a754 ====================

undefined4 FUN_0004a754(int param_1,int param_2,int param_3,undefined4 param_4,undefined1 param_5)

{
  uint uVar1;
  int iVar2;
  byte *pbVar3;
  byte *pbVar5;
  byte *pbVar4;
  uint uVar6;
  void *__src;
  void *__dest;
  uint uVar7;
  int iVar8;
  undefined2 uStack_102a;
  char acStack_1028 [4100];
  
  uVar1 = FUN_0005c648(*(undefined1 *)(param_3 + 3),DAT_0009d6f3);
  uVar1 = uVar1 & 0xff;
  if (2 < uVar1) {
    if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 4)) {
      return 0xffffffff;
    }
    snprintf(acStack_1028,0x1000,"%s chipid %d is not exist","nonce_handle",uVar1);
    goto LAB_0004a7d4;
  }
  uVar7 = *(byte *)(param_3 + 2) & 0xf;
  param_2 = uVar1 * 0x5b0 + param_2;
  memcpy((void *)(uVar7 * 0x5b + param_2),(void *)(param_3 + 2),0x5b);
  if (uVar7 != 0xf) {
    return 1;
  }
  iVar8 = 0;
  do {
    pbVar3 = (byte *)(iVar8 * 0x5b + param_2);
    uVar7 = 0xffff;
    pbVar5 = pbVar3;
    do {
      pbVar4 = pbVar5 + 1;
      uVar6 = (uint)*(ushort *)(&DAT_00068aec + ((uint)*pbVar5 ^ uVar7 >> 8) * 2) ^ uVar7 << 8;
      uVar7 = uVar6 & 0xffff;
      pbVar5 = pbVar4;
    } while (pbVar4 != pbVar3 + 0x59);
    uVar7 = uVar7 >> 8 | (uVar6 & 0xff) << 8;
    uStack_102a = (undefined2)uVar7;
    iVar2 = memcmp(&uStack_102a,(void *)(iVar8 * 0x5b + param_2 + 0x59),2);
    if (iVar2 != 0) {
      if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 3))
      goto LAB_0004a97c;
      param_2 = iVar8 * 0x5b + param_2;
      snprintf(acStack_1028,0x1000,"%s CRC error. cal-crc=%x, chip-crc=%02x%02x\n",
               "bm1740_verify_nonce_integrality",uVar7,(uint)*(byte *)(param_2 + 0x59),
               (uint)*(byte *)(param_2 + 0x5a));
      goto LAB_0004a95c;
    }
    iVar8 = iVar8 + 1;
  } while (iVar8 != 0x10);
  iVar8 = 0;
  uVar7 = 0;
  do {
    pbVar5 = (byte *)(param_2 + iVar8);
    iVar8 = iVar8 + 0x5b;
    if (uVar7 != (*pbVar5 & 0xf)) {
      if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 3))
      goto LAB_0004a98c;
      snprintf(acStack_1028,0x1000,"nonce is not continous i=%d\n",uVar7);
      goto LAB_0004a95c;
    }
    uVar7 = uVar7 + 1;
  } while (uVar7 != 0x10);
  iVar2 = 0xf;
  iVar8 = param_2;
  do {
    pbVar5 = (byte *)(iVar8 + 0x5c);
    iVar8 = iVar8 + 0x5b;
    if ((uint)*pbVar5 != (uint)*(byte *)(param_2 + 1)) {
      if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 3))
      goto LAB_0004a98c;
      snprintf(acStack_1028,0x1000,"chip_addr is different %02x != %02x\n",
               (uint)*(byte *)(param_2 + 1),(uint)*pbVar5);
      goto LAB_0004a95c;
    }
    iVar2 = iVar2 + -1;
  } while (iVar2 != 0);
  iVar8 = 0;
  iVar2 = param_2;
  do {
    pbVar5 = (byte *)(iVar2 + 0x5d);
    iVar8 = iVar8 + 1;
    iVar2 = iVar2 + 0x5b;
    if ((uint)*pbVar5 != (uint)*(byte *)(param_2 + 2)) {
      if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 3))
      goto LAB_0004a98c;
      snprintf(acStack_1028,0x1000,"workID is different %02x != %02x\n",(uint)*(byte *)(param_2 + 2)
               ,(uint)*pbVar5);
      goto LAB_0004a95c;
    }
  } while (iVar8 != 0xf);
  iVar8 = 0xf;
  iVar2 = param_2;
  while( true ) {
    pbVar5 = (byte *)(iVar2 + 0x5e);
    iVar2 = iVar2 + 0x5b;
    if ((uint)*pbVar5 != (uint)*(byte *)(param_2 + 3)) break;
    iVar8 = iVar8 + -1;
    if (iVar8 == 0) {
      __src = (void *)(uVar1 * 0x54d + param_1);
      iVar8 = 0;
      __dest = __src;
      do {
        iVar2 = iVar8 * 0x5b;
        iVar8 = iVar8 + 1;
        memcpy(__dest,(void *)(iVar2 + param_2 + 4),0x55);
        __dest = (void *)((int)__dest + 0x55);
      } while (iVar8 != 0xf);
      memcpy((void *)((int)__src + 0x4fb),(void *)(param_2 + 0x559),0x45);
      *(undefined4 *)((int)__src + 0x548) = *(undefined4 *)(param_2 + 0x5aa);
      *(undefined1 *)((int)__src + 0x540) = *(undefined1 *)(param_2 + 0x556);
      *(undefined1 *)((int)__src + 0x541) = *(undefined1 *)(param_2 + 0x557);
      *(undefined1 *)((int)__src + 0x542) = *(undefined1 *)(param_2 + 0x558);
      *(undefined1 *)((int)__src + 0x543) = *(undefined1 *)(param_2 + 0x5a1);
      *(undefined4 *)((int)__src + 0x544) = *(undefined4 *)(param_2 + 0x5a2);
      pthread_mutex_lock((pthread_mutex_t *)&DAT_00078e88);
      uVar1 = DAT_0007c28c;
      iVar8 = DAT_0007c28c * 0x54d;
      (&DAT_0007c7e4)[iVar8] = param_5;
      (&DAT_0007c7d8)[iVar8] = *(undefined1 *)((int)__src + 0x540);
      (&DAT_0007c7d9)[iVar8] = *(byte *)((int)__src + 0x541) & 0x7f;
      (&DAT_0007c7da)[iVar8] = *(undefined1 *)((int)__src + 0x542);
      (&DAT_0007c7db)[iVar8] = *(undefined1 *)((int)__src + 0x543);
      *(undefined4 *)(&DAT_0007c7dc + iVar8) = *(undefined4 *)((int)__src + 0x544);
      *(undefined4 *)(&DAT_0007c7e0 + iVar8) = *(undefined4 *)((int)__src + 0x548);
      memcpy((void *)(iVar8 + 0x7c298),__src,0x540);
      if (uVar1 < 99) {
        DAT_0007c28c = uVar1 + 1;
      }
      else {
        DAT_0007c28c = 0;
      }
      if (DAT_0007c294 < 100) {
        DAT_0007c294 = DAT_0007c294 + 1;
      }
      else {
        DAT_0007c294 = 100;
        if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (3 < DAT_000766c4)) {
          snprintf(acStack_1028,0x1000,"%s: nonce fifo full!!!","nonce_handle");
          FUN_00038730(4,acStack_1028,0);
        }
      }
      pthread_mutex_unlock((pthread_mutex_t *)&DAT_00078e88);
      return 0;
    }
  }
  if (((DAT_0007ad48 == '\0') && (DAT_00078e08 == '\0')) && (DAT_000766c4 < 3)) {
LAB_0004a98c:
    if (DAT_000766c4 < 4) {
      return 0xffffffff;
    }
  }
  else {
    snprintf(acStack_1028,0x1000,"nonceID is different %02x != %02x chipid=%d\n",
             (uint)*(byte *)(param_2 + 3),(uint)*pbVar5,uVar1);
LAB_0004a95c:
    FUN_00038730(3,acStack_1028,0);
    if (DAT_0007ad48 == '\0') {
LAB_0004a97c:
      if (DAT_00078e08 == '\0') goto LAB_0004a98c;
    }
  }
  uVar1 = DAT_00079bfc;
  DAT_00079bfc = DAT_00079bfc + 1;
  snprintf(acStack_1028,0x1000,"receive a error nonce. total = %u",uVar1);
LAB_0004a7d4:
  FUN_00038730(4,acStack_1028,0);
  return 0xffffffff;
}

