//==================== FUN_0002f934 @ 0002f934 ====================

void FUN_0002f934(void *param_1,undefined1 *param_2,int param_3)

{
  undefined1 uVar1;
  uint uVar2;
  uint uVar3;
  longlong lVar4;
  char *param4;
  char *param5;
  char *param6;
  char *param4_00;
  char *param7;
  char *param8;
  size_t sVar5;
  uint *puVar6;
  int iVar7;
  undefined4 *puVar8;
  longlong param9;
  undefined4 local_14d8 [4];
  undefined4 local_14c8;
  undefined4 uStack_14c4;
  undefined4 uStack_14c0;
  undefined4 uStack_14bc;
  undefined4 local_14b8 [2];
  undefined4 uStack_14b0;
  undefined1 uStack_14ad;
  undefined4 uStack_14ac;
  undefined4 uStack_14a8;
  undefined4 uStack_14a4;
  undefined4 uStack_14a0;
  undefined4 local_149c;
  undefined4 local_1498 [4];
  undefined4 local_1488;
  undefined4 uStack_1484;
  undefined4 uStack_1480;
  undefined4 uStack_147c;
  uint local_1478 [19];
  char local_1429 [1025];
  char acStack_1028 [4100];
  
  if (DAT_00076be8 != (char *)0x0) {
    memset(local_1429 + 1,0,0x400);
    uStack_14b0 = 0;
    uStack_14ac = 0;
    uStack_14a8 = 0;
    uStack_14a4 = 0;
    uStack_14a0 = 0;
    local_149c = 0;
    memset(local_1478,0,0x50);
    iVar7 = 0;
    local_14d8[0] = *(undefined4 *)((int)param_1 + 0x80);
    local_14d8[1] = *(undefined4 *)((int)param_1 + 0x84);
    local_14d8[2] = *(undefined4 *)((int)param_1 + 0x88);
    local_14d8[3] = *(undefined4 *)((int)param_1 + 0x8c);
    local_14c8 = *(undefined4 *)((int)param_1 + 0x90);
    uStack_14c4 = *(undefined4 *)((int)param_1 + 0x94);
    uStack_14c0 = *(undefined4 *)((int)param_1 + 0x98);
    uStack_14bc = *(undefined4 *)((int)param_1 + 0x9c);
    uStack_14b0 = *(undefined4 *)((int)param_1 + 0x48);
    local_14b8[0] = *(undefined4 *)((int)param_1 + 0x40);
    local_14b8[1] = *(undefined4 *)((int)param_1 + 0x44);
    local_1498[0] = *(undefined4 *)((int)param_1 + 0xc0);
    local_1498[1] = *(undefined4 *)((int)param_1 + 0xc4);
    local_1498[2] = *(undefined4 *)((int)param_1 + 200);
    local_1498[3] = *(undefined4 *)((int)param_1 + 0xcc);
    local_1488 = *(undefined4 *)((int)param_1 + 0xd0);
    uStack_1484 = *(undefined4 *)((int)param_1 + 0xd4);
    uStack_1480 = *(undefined4 *)((int)param_1 + 0xd8);
    uStack_147c = *(undefined4 *)((int)param_1 + 0xdc);
    puVar8 = local_14d8;
    do {
      uVar1 = *(undefined1 *)((int)local_14d8 + iVar7);
      *(undefined1 *)((int)local_14d8 + iVar7) = *(undefined1 *)((int)puVar8 + 0x1f);
      iVar7 = iVar7 + 1;
      *(undefined1 *)((int)puVar8 + 0x1f) = uVar1;
      puVar8 = (undefined4 *)((int)puVar8 + -1);
    } while (iVar7 != 0x10);
    iVar7 = 0;
    puVar8 = local_14b8;
    do {
      uVar1 = *(undefined1 *)((int)local_14b8 + iVar7);
      *(undefined1 *)((int)local_14b8 + iVar7) = *(undefined1 *)((int)puVar8 + 0xb);
      iVar7 = iVar7 + 1;
      *(undefined1 *)((int)puVar8 + 0xb) = uVar1;
      puVar8 = (undefined4 *)((int)puVar8 + -1);
    } while (iVar7 != 6);
    iVar7 = 0;
    puVar8 = local_1498;
    do {
      uVar1 = *(undefined1 *)((int)local_1498 + iVar7);
      *(undefined1 *)((int)local_1498 + iVar7) = *(undefined1 *)((int)puVar8 + 0x1f);
      iVar7 = iVar7 + 1;
      *(undefined1 *)((int)puVar8 + 0x1f) = uVar1;
      puVar8 = (undefined4 *)((int)puVar8 + -1);
    } while (iVar7 != 0x10);
    if (DAT_0007b859 == '\0') {
      param4 = (char *)FUN_00029730(param_1,0x80);
    }
    else {
      memcpy(local_1478,param_1,0x50);
      iVar7 = 0;
      puVar6 = local_1478;
      do {
        uVar1 = *(undefined1 *)((int)local_1478 + iVar7);
        *(undefined1 *)((int)local_1478 + iVar7) = *(undefined1 *)((int)puVar6 + 0x4f);
        iVar7 = iVar7 + 1;
        *(undefined1 *)((int)puVar6 + 0x4f) = uVar1;
        puVar6 = (uint *)((int)puVar6 + -1);
      } while (iVar7 != 0x28);
      uVar2 = (local_1478[0] >> 8 & 0xff) << 0x10;
      uVar3 = (local_1478[0] >> 0x10 & 0xff) << 8;
      local_1478[0] =
           (local_1478[0] >> 0x18) << 0x1c | ((uVar3 | local_1478[0] >> 0x18) >> 4 & 0xff) << 0x10 |
           ((uVar2 | uVar3) >> 0xc & 0xff) << 8 | ((local_1478[0] & 0xf) << 0x18 | uVar2) >> 0x14;
      param4 = (char *)FUN_00029730(local_1478,0x50);
    }
    param5 = (char *)FUN_00029730(local_14d8,0x20);
    param6 = (char *)FUN_00029730(local_14b8,0xc);
    param4_00 = (char *)FUN_00029730(param_2,4);
    param7 = (char *)FUN_00029730(param_2,5);
    param8 = (char *)FUN_00029730(local_1498,0x20);
    param9 = FUN_000158e4(param_1);
    iVar7 = strcmp(DAT_00076be8,"screen");
    if (iVar7 == 0) {
      if (param_3 != 0) {
        sprintf(local_1429 + 1,"work %s nonce %s",param4,param4_00);
        if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
          snprintf(acStack_1028,0x1000,local_1429 + 1);
          FUN_00038730(3,acStack_1028,0);
        }
        sVar5 = strlen(local_1429 + 1);
        fwrite(local_1429 + 1,sVar5,1,DAT_00078954);
        fwrite("\n",1,1,DAT_00078954);
        fflush(DAT_00078954);
      }
    }
    else {
      if (((DAT_0007ad48 != '\0') || (DAT_00078e08 != '\0')) || (2 < DAT_000766c4)) {
        snprintf(acStack_1028,0x1000,local_1429 + 1);
        FUN_00038730(3,acStack_1028,0);
      }
      if (DAT_00078954 != (FILE *)0x0) {
        if (param_3 == 0) {
          sprintf(local_1429 + 1,"%s %08x work %s midstate %s data %s nonce %s hash %s diff %lld",
                  "x",*(uint *)((int)param_1 + 0x724),param4,param5,param6,param7,param8,param9);
          sVar5 = strlen(local_1429 + 1);
          fwrite(local_1429 + 1,sVar5,1,DAT_00078954);
          fwrite("\n",1,1,DAT_00078954);
          fflush(DAT_00078954);
        }
        else {
          sprintf(local_1429 + 1,"%s %08x work %s midstate %s data %s nonce %s hash %s diff %lld",
                  "o",*(uint *)((int)param_1 + 0x724),param4,param5,param6,param7,param8,param9);
          sVar5 = strlen(local_1429 + 1);
          fwrite(local_1429 + 1,sVar5,1,DAT_00078954);
          fwrite("\n",1,1,DAT_00078954);
          fflush(DAT_00078954);
          if (DAT_00078950 == 1) {
            sprintf(local_1429 + 1,"midstate %s data %s nonce %s hash %s",param5,param6,param4_00,
                    param8);
            sVar5 = strlen(local_1429 + 1);
            fwrite(local_1429 + 1,sVar5,1,DAT_00078958);
            fwrite("\n",1,1,DAT_00078958);
            fflush(DAT_00078958);
          }
          else if (DAT_00078950 == 0x20 || DAT_00078950 == 0x40) {
            sprintf(local_1429 + 1,"midstate %s data %s nonce %s hash %s",param5,param6,param4_00,
                    param8);
            iVar7 = FUN_0002f64c(DAT_00078950,*param_2);
            sVar5 = strlen(local_1429 + 1);
            fwrite(local_1429 + 1,sVar5,1,(&DAT_00078958)[iVar7]);
            fwrite("\n",1,1,(&DAT_00078958)[iVar7]);
            fflush((&DAT_00078958)[iVar7]);
          }
          if (DAT_00078a5c != '\0') {
            iVar7 = 0;
            lVar4 = param9;
            do {
              uVar2 = (uint)((ulonglong)lVar4 >> 0x21);
              uVar3 = (uint)((byte)((ulonglong)lVar4 >> 0x20) & 1) << 0x1f | (uint)lVar4 >> 1;
              lVar4 = CONCAT44(uVar2,uVar3);
              if (uVar3 == 0 && uVar2 == 0) break;
              iVar7 = iVar7 + 1;
            } while (iVar7 != 0x40);
            if ((DAT_00078e09 != '\0') &&
               (((DAT_0007ad48 != '\0' || (DAT_00078e08 != '\0')) || (6 < DAT_000766c4)))) {
              snprintf(acStack_1028,0x1000,"work diff %lld diffnum %d",param9,iVar7);
              FUN_00038730(7,acStack_1028,0);
            }
            sprintf(local_1429 + 1,"midstate %s data %s nonce %s hash %s",param5,param6,param4_00,
                    param8);
            sVar5 = strlen(local_1429 + 1);
            fwrite(local_1429 + 1,sVar5,1,(FILE *)(&DAT_00078a60)[iVar7]);
            fwrite("\n",1,1,(FILE *)(&DAT_00078a60)[iVar7]);
            fflush((FILE *)(&DAT_00078a60)[iVar7]);
          }
        }
      }
    }
    if (param4 != (char *)0x0) {
      free(param4);
    }
    if (param5 != (char *)0x0) {
      free(param5);
    }
    if (param6 != (char *)0x0) {
      free(param6);
    }
    if (param4_00 != (char *)0x0) {
      free(param4_00);
    }
    if (param7 != (char *)0x0) {
      free(param7);
    }
    if (param8 != (char *)0x0) {
      free(param8);
    }
  }
  return;
}

