//==================== FUN_00007758 @ 00007758 ====================

uint FUN_00007758(int param_1)

{
  char cVar1;
  byte bVar2;
  ushort uVar3;
  int iVar4;
  uint uVar5;
  undefined1 uVar6;
  undefined4 uVar7;
  bool bVar8;
  undefined1 auStack_50 [13];
  byte local_43;
  
  FUN_000027e0(auStack_50,0,0x40);
  if (param_1 == 0) {
    FUN_000014e4(&DAT_0001725c,0x245);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 0x24) != 0x11111111) {
    FUN_000014e4(&DAT_0001725c,0x246);
    DAT_00028e84 = 1;
    return 0;
  }
  *(undefined1 *)(param_1 + 0x3c) = 0;
  *(char **)(param_1 + 0x38) = s__00061a80;
  *(undefined1 *)(param_1 + 0x37) = 1;
  *(undefined1 *)(param_1 + 0x34) = 1;
  if ((*(char *)(param_1 + 0x36) == '\x02') &&
     ((*(uint *)(param_1 + 0x28) & 0xc0000000) == 0x40000000)) {
    *(undefined1 *)(param_1 + 0x34) = 5;
LAB_0000783c:
    iVar4 = FUN_00007574(param_1);
    if (iVar4 != 0) {
      return 1;
    }
    *(undefined4 *)(param_1 + 0x38) = 26000000;
    iVar4 = FUN_000082b0(param_1);
  }
  else {
    func_0xffffe2ac(10000);
    iVar4 = FUN_00006bcc(param_1,0,0);
    if (iVar4 != 0) {
      return 1;
    }
    iVar4 = FUN_00006bcc(param_1,0x100,0x40ff8000);
    if (iVar4 == 0) {
      uVar6 = 2;
    }
    else {
      uVar6 = 1;
    }
    *(undefined1 *)(param_1 + 0x34) = uVar6;
    iVar4 = *(int *)(param_1 + 4);
    *(undefined2 *)(iVar4 + 0x30) = 0xffff;
    *(undefined2 *)(iVar4 + 0x32) = 0xf3ff;
    *(undefined1 *)(iVar4 + 0x2f) = 2;
    bVar2 = *(byte *)(*(int *)(param_1 + 4) + 0x2f);
    while ((bVar2 & 2) != 0) {
      bVar2 = *(byte *)(*(int *)(param_1 + 4) + 0x2f);
    }
    bVar2 = *(byte *)(param_1 + 0x34);
    bVar8 = 4 < bVar2;
    if (bVar2 != 5) {
      bVar8 = bVar2 != 1;
    }
    if (bVar8 && (bVar2 != 5 && bVar2 != 2)) {
      return 1;
    }
    if (bVar2 != 1) {
      if (bVar2 != 2 && bVar2 != 5) {
        return 1;
      }
      goto LAB_0000783c;
    }
    iVar4 = FUN_00006d40(param_1);
    if (iVar4 != 0) {
      return 1;
    }
    if (*(char *)(param_1 + 0x36) == '\x02') {
      uVar7 = 19000000;
      *(undefined4 *)(param_1 + 0x38) = 19000000;
    }
    else {
      uVar7 = 25000000;
      *(undefined4 *)(param_1 + 0x38) = 25000000;
    }
    iVar4 = FUN_000082b0(param_1,uVar7);
  }
  if (iVar4 != 0) {
    return 1;
  }
  iVar4 = FUN_00006bcc(param_1,0x700,*(undefined4 *)(param_1 + 0x50),0);
  if (iVar4 != 0) {
    return 1;
  }
  cVar1 = *(char *)(param_1 + 0x34);
  if (cVar1 == '\x01') {
    iVar4 = FUN_00008798(param_1);
    if (iVar4 != 0) {
      return 1;
    }
    iVar4 = FUN_00007dfc(param_1,&DAT_000219c0);
    if (iVar4 != 0) {
      return 1;
    }
    if (((DAT_000219c1 & 4) != 0) && (iVar4 = FUN_00007f5c(param_1), iVar4 != 0)) {
      return 1;
    }
    uVar5 = FUN_00008168(param_1,auStack_50);
    if (uVar5 != 0) {
      return uVar5;
    }
    if ((((DAT_000219c2 < '\0') && (3 < local_43)) && (*(int *)(param_1 + 0x14) == 8)) &&
       (*(char *)(param_1 + 0x3c) == '\0')) {
      iVar4 = *(int *)(param_1 + 4);
      *(ushort *)(iVar4 + 0x2c) = *(ushort *)(iVar4 + 0x2c) & 0xfffa;
      *(ushort *)(iVar4 + 0x3e) = *(ushort *)(iVar4 + 0x3e) | 8;
      func_0xffffe2ac(&LAB_00001388);
      iVar4 = *(int *)(param_1 + 4);
      if ((*(ushort *)(iVar4 + 0x3e) & 8) == 0) {
        return 1;
      }
      *(ushort *)(iVar4 + 0x2c) = *(ushort *)(iVar4 + 0x2c) | 1;
      uVar3 = *(ushort *)(iVar4 + 0x2c);
      while ((uVar3 & 2) == 0) {
        uVar3 = *(ushort *)(iVar4 + 0x2c);
      }
      *(ushort *)(iVar4 + 0x2c) = *(ushort *)(iVar4 + 0x2c) | 4;
      func_0xffffe2ac(1000);
      *(undefined1 *)(param_1 + 0x3c) = 1;
    }
    if (((DAT_000219c0 != '\0') && ((local_43 & 2) != 0)) && (1 < *(byte *)(param_1 + 0x37))) {
      *(undefined4 *)(param_1 + 0x6c) = 5;
      iVar4 = FUN_00008428(param_1);
joined_r0x00007ae8:
      if (iVar4 != 0) {
        return 1;
      }
    }
LAB_00007a2c:
    if ((*(int *)(param_1 + 0x6c) == 4) && (*(char *)(param_1 + 0x34) != '\x01')) {
      return 0;
    }
  }
  else {
    if (cVar1 == '\x02') {
      if ((3 < *(byte *)(param_1 + 0x35)) && (*(char *)(param_1 + 0x36) == '\x01')) {
        iVar4 = FUN_00007f5c(param_1);
        if (iVar4 != 0) {
          return 1;
        }
        iVar4 = FUN_0000884c(param_1,&DAT_000219e0);
        if (iVar4 != 0) {
          return 1;
        }
        *(undefined4 *)(param_1 + 100) = DAT_00021ab4;
        if (((DAT_00021aa4 & 2) != 0) && (1 < *(byte *)(param_1 + 0x37))) {
          *(undefined4 *)(param_1 + 0x6c) = 5;
          iVar4 = FUN_00008428(param_1);
          if (iVar4 != 0) {
            return 1;
          }
          iVar4 = FUN_0000884c(param_1,&DAT_000219e0);
          if (iVar4 != 0) {
            return 1;
          }
          if (DAT_00021a99 != '\x01') {
            return 1;
          }
        }
        goto LAB_00007a2c;
      }
    }
    else if (cVar1 == '\x05') {
      iVar4 = FUN_00007f5c(param_1);
      if (iVar4 != 0) {
        return 1;
      }
      iVar4 = FUN_0000884c(param_1,&DAT_000219e0);
      if (iVar4 != 0) {
        return 1;
      }
      *(undefined4 *)(param_1 + 100) = DAT_00021ab4;
      if ((DAT_00021aa4 & 0x30) == 0) {
        if ((DAT_00021aa4 & 0xc) == 0) {
LAB_00007bc0:
          if (((DAT_00021aa4 & 2) != 0) && (1 < *(byte *)(param_1 + 0x37))) {
            *(undefined4 *)(param_1 + 0x6c) = 5;
            goto LAB_00007b4c;
          }
        }
        else if (1 < *(byte *)(param_1 + 0x37)) {
          *(undefined4 *)(param_1 + 0x6c) = 4;
          goto LAB_00007b4c;
        }
LAB_00007c80:
        *(undefined4 *)(param_1 + 0x6c) = 6;
joined_r0x00007cf0:
        if (DAT_00021a82 != '\0') goto LAB_000078ac;
      }
      else {
        if (*(byte *)(param_1 + 0x37) < 2) {
          if ((DAT_00021aa4 & 0xc) != 0) goto LAB_00007c80;
          goto LAB_00007bc0;
        }
        *(undefined4 *)(param_1 + 0x6c) = 7;
LAB_00007b4c:
        iVar4 = FUN_00008428(param_1);
        if (iVar4 != 0) {
          return 1;
        }
        iVar4 = FUN_0000884c(param_1,DAT_00007d3c);
        if (iVar4 != 0) {
          return 1;
        }
        iVar4 = *(int *)(param_1 + 0x6c);
        if (iVar4 == 7) {
          if (DAT_00021a99 != '\x02') {
            return 1;
          }
        }
        else if (iVar4 - 4U < 2) {
          if (DAT_00021a99 != '\x01') {
            return 1;
          }
          if (iVar4 != 4) goto joined_r0x00007cf0;
          iVar4 = FUN_00007f5c(param_1);
          if (iVar4 != 0) {
            return 1;
          }
        }
        if (DAT_00021a82 != '\0') goto LAB_00007a2c;
      }
      iVar4 = FUN_00008994(param_1,0x3a20100);
      goto joined_r0x00007ae8;
    }
    if (*(int *)(param_1 + 0x6c) == 4) {
      return 0;
    }
  }
LAB_000078ac:
  iVar4 = FUN_00007d40(param_1,0x200);
  return (uint)(iVar4 != 0);
}

