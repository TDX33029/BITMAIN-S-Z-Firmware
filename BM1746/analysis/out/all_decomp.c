//==================== Reset @ 00000000 ====================

/* WARNING: Control flow encountered bad instruction data */

void Reset(void)

{
  int unaff_r7;
  int in_stack_00000008;
  
  FUN_00002200(in_stack_00000008 + unaff_r7);
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



//==================== UndefinedInstruction @ 00000004 ====================

/* WARNING: Control flow encountered bad instruction data */

void UndefinedInstruction(void)

{
  int in_r3;
  int unaff_r7;
  
  FUN_00002200(in_r3 + unaff_r7);
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



//==================== SupervisorCall @ 00000008 ====================

/* WARNING: Control flow encountered bad instruction data */

void SupervisorCall(void)

{
  int in_r3;
  int unaff_r7;
  
  FUN_00002200(in_r3 + unaff_r7);
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



//==================== PrefetchAbort @ 0000000c ====================

/* WARNING: Control flow encountered bad instruction data */

void PrefetchAbort(void)

{
  int in_r3;
  int unaff_r7;
  
  FUN_00002200(in_r3 + unaff_r7);
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



//==================== DataAbort @ 00000010 ====================

/* WARNING: Control flow encountered bad instruction data */

void DataAbort(void)

{
  FUN_00002200();
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



//==================== IRQ @ 00000018 ====================

void IRQ(void)

{
  undefined4 *unaff_r11;
  
  FUN_000014e4(&DAT_00017208,0x3b7);
  *unaff_r11 = 1;
  return;
}



//==================== FIQ @ 0000001c ====================

void FIQ(uint param_1)

{
  undefined4 *unaff_r11;
  
  FUN_000014e4(param_1 & 0xffff | 0x10000,0x3b7);
  *unaff_r11 = 1;
  return;
}



//==================== FUN_0000022c @ 0000022c ====================

bool FUN_0000022c(int param_1,undefined4 param_2,undefined4 *param_3)

{
  ushort uVar1;
  uint uVar2;
  int iVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  uint uVar6;
  
  if (param_3 == (undefined4 *)0x0) {
    FUN_000014e4(&DAT_00017208,0x506);
    DAT_00028e84 = 1;
    return false;
  }
  DAT_00028e84 = 0;
  if (*(char *)(param_1 + 0x6ecc) == '\x02') {
    iVar3 = *(int *)(param_1 + 8);
    do {
    } while ((*(uint *)(iVar3 + 0x400) & 0x40) != 0);
    *(uint *)(iVar3 + 0x404) = *(uint *)(iVar3 + 0x404) & 0xfffffff3;
  }
  uVar1 = *(ushort *)(param_1 + 0x1c);
  func_0xfffff540(param_1,DAT_0000034c,param_2,*(undefined4 *)(param_1 + 0x18));
  uVar6 = uVar1 - 4;
  uVar2 = uVar6 >> 2;
  if (uVar2 != 0) {
    puVar5 = param_3;
    do {
      puVar4 = puVar5 + 1;
      **(undefined4 **)(param_1 + 100) = *puVar5;
      puVar5 = puVar4;
    } while (param_3 + uVar2 != puVar4);
  }
  puVar5 = (undefined4 *)(*(uint *)(param_1 + 100) | 0x200000);
  *(undefined4 **)(param_1 + 100) = puVar5;
  *puVar5 = *(undefined4 *)((int)param_3 + uVar6);
  do {
  } while ((**(uint **)(param_1 + 8) & 0x40) == 0);
  (*(uint **)(param_1 + 8))[3] = 0x10;
  uVar2 = FUN_00001554(param_1);
  if ((uVar2 & 1) != 0) {
    return true;
  }
  if (*(char *)(param_1 + 0x6ecc) == '\x02') {
    iVar3 = func_0xffffee18(param_1);
    return iVar3 != 0;
  }
  return false;
}



//==================== FUN_0000060c @ 0000060c ====================

void FUN_0000060c(int param_1,uint param_2)

{
  if (param_1 == 0) {
    FUN_000014e4(&DAT_00017214,0xff);
    DAT_00028e84 = 1;
    return;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) == 0x11111111) {
    **(uint **)(param_1 + 4) = param_2 | **(uint **)(param_1 + 4);
    return;
  }
  FUN_000014e4(&DAT_00017214,0x100);
  DAT_00028e84 = 1;
  return;
}



//==================== FUN_00000714 @ 00000714 ====================

undefined4 FUN_00000714(int param_1)

{
  if (param_1 == 0) {
    FUN_000014e4(&DAT_00017214,0x13e);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) == 0x11111111) {
    return **(undefined4 **)(param_1 + 4);
  }
  FUN_000014e4(&DAT_00017214,0x13f);
  DAT_00028e84 = 1;
  return 0;
}



//==================== FUN_00000814 @ 00000814 ====================

undefined4 FUN_00000814(int param_1)

{
  if (param_1 == 0) {
    FUN_000014e4(&DAT_00017214,0x175);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) == 0x11111111) {
    return *(undefined4 *)(*(int *)(param_1 + 4) + 4);
  }
  FUN_000014e4(&DAT_00017214,0x176);
  DAT_00028e84 = 1;
  return 0;
}



//==================== FUN_00000914 @ 00000914 ====================

undefined4 FUN_00000914(int param_1)

{
  if (param_1 == 0) {
    FUN_000014e4(&DAT_00017214,0x1ad);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) == 0x11111111) {
    return *(undefined4 *)(*(int *)(param_1 + 4) + 8);
  }
  FUN_000014e4(&DAT_00017214,0x1ae);
  DAT_00028e84 = 1;
  return 0;
}



//==================== FUN_00000a14 @ 00000a14 ====================

undefined4 FUN_00000a14(int param_1)

{
  if (param_1 == 0) {
    FUN_000014e4(&DAT_00017214,0x1e1);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) == 0x11111111) {
    return *(undefined4 *)(*(int *)(param_1 + 4) + 0x14);
  }
  FUN_000014e4(&DAT_00017214,0x1e2);
  DAT_00028e84 = 1;
  return 0;
}



//==================== FUN_00000b14 @ 00000b14 ====================

undefined4 FUN_00000b14(int param_1)

{
  if (param_1 == 0) {
    FUN_000014e4(&DAT_00017214,0x217);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) == 0x11111111) {
    return *(undefined4 *)(*(int *)(param_1 + 4) + 0x30);
  }
  FUN_000014e4(&DAT_00017214,0x218);
  DAT_00028e84 = 1;
  return 0;
}



//==================== FUN_00000c1c @ 00000c1c ====================

undefined4 FUN_00000c1c(int param_1)

{
  if (param_1 == 0) {
    FUN_000014e4(&DAT_00017214,0x252);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) == 0x11111111) {
    return *(undefined4 *)(*(int *)(param_1 + 4) + 0x80);
  }
  FUN_000014e4(&DAT_00017214,0x253);
  DAT_00028e84 = 1;
  return 0;
}



//==================== FUN_00000ca0 @ 00000ca0 ====================

uint FUN_00000ca0(int param_1)

{
  if (param_1 == 0) {
    FUN_000014e4(&DAT_00017214,0x26d);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) == 0x11111111) {
    return ~*(uint *)(*(int *)(param_1 + 4) + 0x14) >> 0x1f;
  }
  FUN_000014e4(&DAT_00017214,0x26e);
  DAT_00028e84 = 1;
  return 0;
}



//==================== FUN_00001140 @ 00001140 ====================

uint FUN_00001140(int param_1)

{
  if (param_1 == 0) {
    FUN_000014e4(&DAT_00017220,0x9e);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) == 0x11111111) {
    return ~*(uint *)(*(int *)(param_1 + 4) + 0x10);
  }
  FUN_000014e4(&DAT_00017220,0x9f);
  DAT_00028e84 = 1;
  return 0;
}



//==================== FUN_000011c8 @ 000011c8 ====================

undefined4 FUN_000011c8(int param_1)

{
  if (param_1 == 0) {
    FUN_000014e4(&DAT_00017220,0xbc);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) == 0x11111111) {
    return *(undefined4 *)(*(int *)(param_1 + 4) + 0xc);
  }
  FUN_000014e4(&DAT_00017220,0xbd);
  DAT_00028e84 = 1;
  return 0;
}



//==================== FUN_00001458 @ 00001458 ====================

undefined4 FUN_00001458(uint param_1)

{
  longlong lVar1;
  uint uVar2;
  bool bVar3;
  uint local_18;
  uint uStack_14;
  
  func_0xffffe2d4(&local_18);
  lVar1 = (ulonglong)param_1 * 0x13de435f + CONCAT44(uStack_14,local_18);
  uVar2 = (uint)((ulonglong)lVar1 >> 0x20);
  do {
    func_0xffffe2d4(&local_18);
    bVar3 = uVar2 <= uStack_14;
    if (uStack_14 == uVar2) {
      bVar3 = (uint)lVar1 <= local_18;
    }
  } while (!bVar3);
  return 0;
}



//==================== FUN_000014a0 @ 000014a0 ====================

undefined4 FUN_000014a0(uint param_1)

{
  longlong lVar1;
  uint uVar2;
  bool bVar3;
  uint local_18;
  uint uStack_14;
  
  func_0xffffe2d4(&local_18);
  lVar1 = (ulonglong)param_1 * 0x14d + CONCAT44(uStack_14,local_18);
  uVar2 = (uint)((ulonglong)lVar1 >> 0x20);
  do {
    func_0xffffe2d4(&local_18);
    bVar3 = uVar2 <= uStack_14;
    if (uStack_14 == uVar2) {
      bVar3 = (uint)lVar1 <= local_18;
    }
  } while (!bVar3);
  return 0;
}



//==================== FUN_000014e4 @ 000014e4 ====================

void FUN_000014e4(void)

{
  if (DAT_00021540 == Reset) {
    do {
      if (DAT_00019704 == 0) {
        return;
      }
    } while (DAT_00019704 != 0);
    return;
  }
  (*DAT_00021540)();
  do {
    if (DAT_00019704 == 0) {
      return;
    }
  } while (DAT_00019704 != 0);
  return;
}



//==================== FUN_00001554 @ 00001554 ====================

ushort FUN_00001554(int param_1)

{
  ushort uVar1;
  
  func_0xfffff540(param_1,DAT_00001588,0xffffffff);
  if (*(int *)(param_1 + 0x10) == 0x10) {
    uVar1 = **(ushort **)(param_1 + 100);
  }
  else {
    uVar1 = (ushort)(byte)**(ushort **)(param_1 + 100);
  }
  return uVar1 & 0xff;
}



//==================== FUN_0000158c @ 0000158c ====================

/* WARNING: Type propagation algorithm not settling */

ushort FUN_0000158c(int param_1)

{
  longlong lVar1;
  ushort uVar2;
  uint uVar3;
  int iVar4;
  ushort uVar5;
  char *pcVar6;
  byte bVar7;
  ushort uVar8;
  int iVar9;
  byte *pbVar10;
  ushort *puVar11;
  int iVar12;
  byte *pbVar13;
  char cVar14;
  byte *pbVar15;
  char local_134 [4];
  undefined4 local_130;
  byte local_12c [4];
  byte local_128 [6];
  ushort local_122;
  ushort local_120;
  uint local_d8;
  undefined2 local_d4;
  uint local_cc;
  int local_c8;
  byte local_c4;
  byte local_c3;
  byte bStack_2b;
  ushort local_2a;
  byte abStack_28 [4];
  
  local_130 = 8;
  if (param_1 == 0) {
    FUN_000014e4(&DAT_00017230,0x1ee);
    DAT_00028e84 = 1;
    uVar5 = 0;
  }
  else {
    DAT_00028e84 = 0;
    func_0xfffff540(param_1,DAT_000019b4,0xffffffff);
    do {
      uVar3 = FUN_00001554(param_1);
    } while ((uVar3 & 0x40) == 0);
    func_0xfffff540(param_1,DAT_000019b8,0xffffffff,0x20);
    iVar4 = *(int *)(param_1 + 0x10);
    iVar9 = 0;
    pcVar6 = *(char **)(param_1 + 100);
    do {
      if (iVar4 == 0x10) {
        cVar14 = (char)*(undefined2 *)pcVar6;
      }
      else {
        cVar14 = *pcVar6;
      }
      local_134[iVar9] = cVar14;
      iVar9 = iVar9 + 1;
    } while (iVar9 != 4);
    if ((((local_134[0] == 'O') && (local_134[1] == 'N')) && (local_134[2] == 'F')) &&
       (local_134[3] == 'I')) {
      func_0xfffff540(param_1,DAT_000019bc,0xffffffff,0);
      do {
        uVar3 = FUN_00001554(param_1);
      } while ((uVar3 & 0x40) == 0);
      iVar9 = 3;
      **(undefined4 **)(param_1 + 0xc) = 0;
      iVar4 = *(int *)(param_1 + 0x10);
      pbVar13 = *(byte **)(param_1 + 100);
      do {
        pbVar15 = local_128;
        do {
          pbVar10 = pbVar15 + 1;
          if (iVar4 == 0x10) {
            bVar7 = (byte)*(undefined2 *)pbVar13;
          }
          else {
            bVar7 = *pbVar13;
          }
          *pbVar15 = bVar7;
          pbVar15 = pbVar10;
        } while (abStack_28 != pbVar10);
        pbVar15 = local_12c + 3;
        uVar5 = 0x4f4e;
        do {
          pbVar15 = pbVar15 + 1;
          iVar12 = 8;
          uVar3 = 0x80;
          uVar8 = uVar5;
          do {
            uVar5 = uVar8 << 1;
            uVar2 = ~uVar8;
            if ((*pbVar15 & uVar3) == 0) {
              uVar2 = uVar8;
            }
            uVar3 = uVar3 >> 1;
            if ((uVar2 & 0x8000) != 0) {
              uVar5 = uVar5 ^ 0x8005;
            }
            iVar12 = iVar12 + -1;
            uVar8 = uVar5;
          } while (iVar12 != 0);
        } while (&bStack_2b != pbVar15);
        if (local_2a == uVar5) {
          *(byte *)(param_1 + 0x28) = local_c4;
          DAT_0001974c = local_c3 >> 4;
          DAT_00019728 = local_c3 & 0xf;
          *(uint *)(param_1 + 0x20) = local_cc;
          DAT_0001971c = DAT_0001974c;
          *(undefined2 *)(param_1 + 0x1c) = local_d4;
          *(uint *)(param_1 + 0x18) = local_d8;
          *(undefined4 *)(param_1 + 0x34) = 0;
          *(undefined4 *)(param_1 + 0x3c) = 0;
          *(int *)(param_1 + 0x24) = local_c8;
          uVar3 = local_c8 * (uint)local_c4;
          *(undefined4 *)(param_1 + 0x44) = 0;
          *(byte *)(param_1 + 0x50) = DAT_00019728;
          DAT_00019740 = DAT_00019728 + DAT_0001974c;
          *(byte *)(param_1 + 0x51) = DAT_0001974c;
          lVar1 = (ulonglong)uVar3 * ((ulonglong)local_d8 * (ulonglong)local_cc & 0xffffffff);
          *(uint *)(param_1 + 0x38) = uVar3;
          DAT_00019710 = DAT_00019740;
          *(uint *)(param_1 + 0x30) = uVar3 * local_cc;
          *(uint *)(param_1 + 0x40) = local_d8 * local_cc;
          *(int *)(param_1 + 0x48) = (int)lVar1;
          *(uint *)(param_1 + 0x4c) =
               uVar3 * (int)((ulonglong)local_d8 * (ulonglong)local_cc >> 0x20) +
               (int)((ulonglong)lVar1 >> 0x20);
          func_0xfffff540(param_1,&DAT_00019750,0xffffffff,0);
          puVar11 = *(ushort **)(param_1 + 100);
          if (*(int *)(param_1 + 0x10) == 0x10) {
            uVar5 = *puVar11;
            uVar8 = *puVar11;
          }
          else {
            uVar5 = (ushort)(byte)*puVar11;
            uVar8 = (ushort)(byte)*puVar11;
          }
          if (((uVar5 & 0xff) == 0x2c) &&
             (uVar5 = uVar8 & 0xef,
             uVar5 == 0xc3 ||
             ((uVar5 + 0x36 & 0xfd) == 0 ||
             ((uVar8 & 0xff) == 0xf1 || ((uVar8 & 0xed) == 0xa1 || (uVar5 + 0x56 & 0xfd) == 0))))) {
            func_0xfffff540(param_1,DAT_000019c0,0xffffffff,0x90);
            uVar5 = 8;
            pbVar13 = (byte *)((int)&local_130 + 1);
            while( true ) {
              if (*(int *)(param_1 + 0x10) == 0x10) {
                **(ushort **)(param_1 + 100) = uVar5;
              }
              else {
                *(char *)*(ushort **)(param_1 + 100) = (char)uVar5;
              }
              if (local_12c == pbVar13) break;
              uVar5 = (ushort)*pbVar13;
              pbVar13 = pbVar13 + 1;
            }
            do {
              uVar3 = FUN_00001554(param_1);
            } while ((uVar3 & 0x40) == 0);
            func_0xfffff540(param_1,DAT_000019c4,0xffffffff,0x90);
            do {
              uVar3 = FUN_00001554(param_1);
            } while ((uVar3 & 0x40) == 0);
            iVar9 = 0;
            **(undefined4 **)(param_1 + 0xc) = 0;
            iVar4 = *(int *)(param_1 + 0x10);
            pbVar13 = *(byte **)(param_1 + 100);
            do {
              if (iVar4 == 0x10) {
                bVar7 = (byte)*(undefined2 *)pbVar13;
              }
              else {
                bVar7 = *pbVar13;
              }
              local_12c[iVar9] = bVar7;
              iVar9 = iVar9 + 1;
            } while (iVar9 != 4);
            if ((local_12c[0] & 8) != 0) {
              *(undefined1 *)(param_1 + 0x6ecc) = 3;
              if ((local_122 & 1) == 0) {
                *(undefined1 *)(param_1 + 0x29) = 8;
                return local_122 & 1;
              }
              *(undefined1 *)(param_1 + 0x29) = 0x10;
              return 0;
            }
LAB_00001968:
            *(undefined1 *)(param_1 + 0x6ecc) = 2;
          }
          else {
            if (local_d8 - 0x200 < 0x601) goto LAB_00001968;
            *(undefined1 *)(param_1 + 0x6ecc) = 0;
          }
          if ((local_122 & 1) == 0) {
            *(undefined1 *)(param_1 + 0x29) = 8;
          }
          else {
            *(undefined1 *)(param_1 + 0x29) = 0x10;
          }
          *(uint *)(param_1 + 0x58) = local_120 & 1;
          *(uint *)(param_1 + 0x5c) = (local_120 & 3) >> 1;
          return 0;
        }
        iVar9 = iVar9 + -1;
      } while (iVar9 != 0);
    }
    uVar5 = 1;
  }
  return uVar5;
}



//==================== FUN_000019fc @ 000019fc ====================

void FUN_000019fc(undefined4 *param_1)

{
  param_1[3] = 0x3fff;
  *param_1 = 0x28;
  *param_1 = 3;
  param_1[5] = 0x3fff;
  param_1[1] = 0;
  param_1[8] = 0x20;
  param_1[0x11] = 0x20;
  param_1[7] = 0;
  param_1[6] = 0x28b;
  param_1[0xd] = 0xf;
  *param_1 = 0x128;
  return;
}



//==================== FUN_00001a50 @ 00001a50 ====================

uint FUN_00001a50(uint param_1,uint param_2)

{
  uint uVar1;
  int iVar2;
  bool bVar3;
  bool bVar4;
  bool bVar5;
  bool bVar6;
  bool bVar7;
  bool bVar8;
  bool bVar9;
  bool bVar10;
  bool bVar11;
  bool bVar12;
  bool bVar13;
  bool bVar14;
  bool bVar15;
  bool bVar16;
  bool bVar17;
  bool bVar18;
  bool bVar19;
  bool bVar20;
  bool bVar21;
  bool bVar22;
  bool bVar23;
  bool bVar24;
  bool bVar25;
  bool bVar26;
  bool bVar27;
  bool bVar28;
  bool bVar29;
  bool bVar30;
  bool bVar31;
  bool bVar32;
  bool bVar33;
  
  if (param_2 - 1 == 0) {
    return param_1;
  }
  if (param_2 == 0) {
    uVar1 = 0;
    if (param_1 != 0) {
      uVar1 = 0xffffffff;
    }
    return uVar1;
  }
  if (param_1 <= param_2) {
    return (uint)(param_1 == param_2);
  }
  if ((param_2 & param_2 - 1) == 0) {
    return param_1 >> (0x1fU - LZCOUNT(param_2) & 0xff);
  }
  iVar2 = 0x1f - (LZCOUNT(param_2) - LZCOUNT(param_1));
  if (iVar2 != 0) {
                    /* WARNING: Could not recover jumptable at 0x00001a84. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar1 = (*(code *)(iVar2 * 0xc + 0x1a8c))();
    return uVar1;
  }
  bVar3 = param_2 << 0x1f <= param_1;
  if (bVar3) {
    param_1 = param_1 + param_2 * -0x80000000;
  }
  bVar4 = param_2 << 0x1e <= param_1;
  if (bVar4) {
    param_1 = param_1 + param_2 * -0x40000000;
  }
  bVar5 = param_2 << 0x1d <= param_1;
  if (bVar5) {
    param_1 = param_1 + param_2 * -0x20000000;
  }
  bVar6 = param_2 << 0x1c <= param_1;
  if (bVar6) {
    param_1 = param_1 + param_2 * -0x10000000;
  }
  bVar7 = param_2 << 0x1b <= param_1;
  if (bVar7) {
    param_1 = param_1 + param_2 * -0x8000000;
  }
  bVar8 = param_2 << 0x1a <= param_1;
  if (bVar8) {
    param_1 = param_1 + param_2 * -0x4000000;
  }
  bVar9 = param_2 << 0x19 <= param_1;
  if (bVar9) {
    param_1 = param_1 + param_2 * -0x2000000;
  }
  bVar10 = param_2 << 0x18 <= param_1;
  if (bVar10) {
    param_1 = param_1 + param_2 * -0x1000000;
  }
  bVar11 = param_2 << 0x17 <= param_1;
  if (bVar11) {
    param_1 = param_1 + param_2 * -0x800000;
  }
  bVar12 = param_2 << 0x16 <= param_1;
  if (bVar12) {
    param_1 = param_1 + param_2 * -0x400000;
  }
  bVar13 = param_2 << 0x15 <= param_1;
  if (bVar13) {
    param_1 = param_1 + param_2 * -0x200000;
  }
  bVar14 = param_2 << 0x14 <= param_1;
  if (bVar14) {
    param_1 = param_1 + param_2 * -0x100000;
  }
  bVar15 = param_2 << 0x13 <= param_1;
  if (bVar15) {
    param_1 = param_1 + param_2 * -0x80000;
  }
  bVar16 = param_2 << 0x12 <= param_1;
  if (bVar16) {
    param_1 = param_1 + param_2 * -0x40000;
  }
  bVar17 = param_2 << 0x11 <= param_1;
  if (bVar17) {
    param_1 = param_1 + param_2 * -0x20000;
  }
  bVar18 = param_2 << 0x10 <= param_1;
  if (bVar18) {
    param_1 = param_1 + param_2 * -0x10000;
  }
  bVar19 = param_2 << 0xf <= param_1;
  if (bVar19) {
    param_1 = param_1 + param_2 * -0x8000;
  }
  bVar20 = param_2 << 0xe <= param_1;
  if (bVar20) {
    param_1 = param_1 + param_2 * -0x4000;
  }
  bVar21 = param_2 << 0xd <= param_1;
  if (bVar21) {
    param_1 = param_1 + param_2 * -0x2000;
  }
  bVar22 = param_2 << 0xc <= param_1;
  if (bVar22) {
    param_1 = param_1 + param_2 * -0x1000;
  }
  bVar23 = param_2 << 0xb <= param_1;
  if (bVar23) {
    param_1 = param_1 + param_2 * -0x800;
  }
  bVar24 = param_2 << 10 <= param_1;
  if (bVar24) {
    param_1 = param_1 + param_2 * -0x400;
  }
  bVar25 = param_2 << 9 <= param_1;
  if (bVar25) {
    param_1 = param_1 + param_2 * -0x200;
  }
  bVar26 = param_2 << 8 <= param_1;
  if (bVar26) {
    param_1 = param_1 + param_2 * -0x100;
  }
  bVar27 = param_2 << 7 <= param_1;
  if (bVar27) {
    param_1 = param_1 + param_2 * -0x80;
  }
  bVar28 = param_2 << 6 <= param_1;
  if (bVar28) {
    param_1 = param_1 + param_2 * -0x40;
  }
  bVar29 = param_2 << 5 <= param_1;
  if (bVar29) {
    param_1 = param_1 + param_2 * -0x20;
  }
  bVar30 = param_2 << 4 <= param_1;
  if (bVar30) {
    param_1 = param_1 + param_2 * -0x10;
  }
  bVar31 = param_2 << 3 <= param_1;
  if (bVar31) {
    param_1 = param_1 + param_2 * -8;
  }
  bVar32 = param_2 << 2 <= param_1;
  if (bVar32) {
    param_1 = param_1 + param_2 * -4;
  }
  bVar33 = param_2 << 1 <= param_1;
  if (bVar33) {
    param_1 = param_1 + param_2 * -2;
  }
  return (((((((((((((((((((((((((((((((uint)bVar3 * 2 + (uint)bVar4) * 2 + (uint)bVar5) * 2 +
                                    (uint)bVar6) * 2 + (uint)bVar7) * 2 + (uint)bVar8) * 2 +
                                 (uint)bVar9) * 2 + (uint)bVar10) * 2 + (uint)bVar11) * 2 +
                              (uint)bVar12) * 2 + (uint)bVar13) * 2 + (uint)bVar14) * 2 +
                           (uint)bVar15) * 2 + (uint)bVar16) * 2 + (uint)bVar17) * 2 + (uint)bVar18)
                        * 2 + (uint)bVar19) * 2 + (uint)bVar20) * 2 + (uint)bVar21) * 2 +
                    (uint)bVar22) * 2 + (uint)bVar23) * 2 + (uint)bVar24) * 2 + (uint)bVar25) * 2 +
                (uint)bVar26) * 2 + (uint)bVar27) * 2 + (uint)bVar28) * 2 + (uint)bVar29) * 2 +
            (uint)bVar30) * 2 + (uint)bVar31) * 2 + (uint)bVar32) * 2 + (uint)bVar33) * 2 +
         (uint)(param_2 <= param_1);
}



//==================== FUN_000020ec @ 000020ec ====================

void FUN_000020ec(undefined4 param_1)

{
  int iVar1;
  undefined4 *puVar2;
  
  FUN_000029dc(param_1,0);
  if (*(code **)(DAT_00015b74 + 0x3c) != Reset) {
    (**(code **)(DAT_00015b74 + 0x3c))();
  }
  FUN_00002b40(param_1);
  iVar1 = 1;
  puVar2 = (undefined4 *)0x20010;
  do {
    iVar1 = iVar1 + -1;
    puVar2 = puVar2 + -1;
    (*(code *)*puVar2)();
  } while (iVar1 != 0);
  return;
}



//==================== FUN_00002200 @ 00002200 ====================

void FUN_00002200(uint *param_1,uint *param_2,uint param_3)

{
  byte *pbVar1;
  byte *pbVar2;
  uint *puVar3;
  int *piVar4;
  int *piVar5;
  undefined1 uVar6;
  undefined2 uVar7;
  ushort uVar8;
  uint uVar9;
  int iVar10;
  uint uVar11;
  uint uVar12;
  uint uVar13;
  uint uVar14;
  uint uVar15;
  uint uVar16;
  uint uVar17;
  int iVar18;
  undefined2 *puVar19;
  undefined2 *puVar20;
  uint *puVar21;
  int *piVar22;
  int *piVar23;
  uint *puVar24;
  bool bVar25;
  bool bVar26;
  undefined8 uVar27;
  undefined8 uVar28;
  undefined8 uVar29;
  undefined8 uVar30;
  undefined8 uVar31;
  undefined8 uVar32;
  undefined8 uVar33;
  undefined8 uVar34;
  
  if (0x3f < (int)param_3) {
    uVar9 = (uint)param_1 & 7;
    if (uVar9 == ((uint)param_2 & 7)) {
      if (((uint)param_1 & 7) != 0) {
        uVar11 = (int)param_1 * -0x20000000;
        param_3 = param_3 - (uVar11 >> 0x1d);
        puVar24 = param_1;
        puVar21 = param_2;
        if ((int)uVar11 < 0) {
          puVar21 = param_2 + 1;
          uVar9 = *param_2;
          puVar24 = param_1 + 1;
          *param_1 = uVar9;
        }
        uVar8 = (ushort)uVar9;
        bVar26 = (bool)((byte)(uVar11 >> 0x1e) & 1);
        bVar25 = ((uint)param_1 & 1) != 0;
        puVar3 = puVar21;
        if (bVar26) {
          puVar3 = (uint *)((int)puVar21 + 2);
          uVar8 = (ushort)*puVar21;
        }
        uVar6 = 0;
        param_2 = puVar3;
        if (bVar25) {
          param_2 = (uint *)((int)puVar3 + 1);
          uVar6 = (undefined1)*puVar3;
        }
        puVar21 = puVar24;
        if (bVar26) {
          puVar21 = (uint *)((int)puVar24 + 2);
          *(ushort *)puVar24 = uVar8;
        }
        param_1 = puVar21;
        if (bVar25) {
          param_1 = (uint *)((int)puVar21 + 1);
          *(undefined1 *)puVar21 = uVar6;
        }
      }
      uVar9 = param_3 - 0x40;
      if (0x3f < (int)param_3) {
        uVar11 = uVar9;
        if (0x1ff < (int)uVar9) {
          uVar30 = *(undefined8 *)param_2;
          uVar31 = *(undefined8 *)(param_2 + 0x10);
          uVar32 = *(undefined8 *)(param_2 + 0x20);
          uVar33 = *(undefined8 *)(param_2 + 0x30);
          uVar34 = *(undefined8 *)(param_2 + 0x40);
          uVar27 = *(undefined8 *)(param_2 + 2);
          uVar28 = *(undefined8 *)(param_2 + 4);
          uVar29 = *(undefined8 *)(param_2 + 6);
          param_2 = param_2 + 8;
          iVar18 = param_3 - 0x2c0;
          iVar10 = iVar18;
          if (0x27f < (int)uVar9) {
            do {
              *(undefined8 *)param_1 = uVar30;
              uVar30 = *(undefined8 *)param_2;
              *(undefined8 *)(param_1 + 2) = uVar27;
              uVar27 = *(undefined8 *)(param_2 + 2);
              *(undefined8 *)(param_1 + 4) = uVar28;
              uVar28 = *(undefined8 *)(param_2 + 4);
              *(undefined8 *)(param_1 + 6) = uVar29;
              uVar29 = *(undefined8 *)(param_2 + 6);
              *(undefined8 *)(param_1 + 8) = uVar30;
              uVar30 = *(undefined8 *)(param_2 + 0x48);
              *(undefined8 *)(param_1 + 10) = uVar27;
              uVar27 = *(undefined8 *)(param_2 + 10);
              *(undefined8 *)(param_1 + 0xc) = uVar28;
              uVar28 = *(undefined8 *)(param_2 + 0xc);
              *(undefined8 *)(param_1 + 0xe) = uVar29;
              uVar29 = *(undefined8 *)(param_2 + 0xe);
              *(undefined8 *)(param_1 + 0x10) = uVar31;
              uVar31 = *(undefined8 *)(param_2 + 0x10);
              *(undefined8 *)(param_1 + 0x12) = uVar27;
              uVar27 = *(undefined8 *)(param_2 + 0x12);
              *(undefined8 *)(param_1 + 0x14) = uVar28;
              uVar28 = *(undefined8 *)(param_2 + 0x14);
              *(undefined8 *)(param_1 + 0x16) = uVar29;
              uVar29 = *(undefined8 *)(param_2 + 0x16);
              *(undefined8 *)(param_1 + 0x18) = uVar31;
              uVar31 = *(undefined8 *)(param_2 + 0x58);
              *(undefined8 *)(param_1 + 0x1a) = uVar27;
              uVar27 = *(undefined8 *)(param_2 + 0x1a);
              *(undefined8 *)(param_1 + 0x1c) = uVar28;
              uVar28 = *(undefined8 *)(param_2 + 0x1c);
              *(undefined8 *)(param_1 + 0x1e) = uVar29;
              uVar29 = *(undefined8 *)(param_2 + 0x1e);
              *(undefined8 *)(param_1 + 0x20) = uVar32;
              uVar32 = *(undefined8 *)(param_2 + 0x20);
              *(undefined8 *)(param_1 + 0x22) = uVar27;
              uVar27 = *(undefined8 *)(param_2 + 0x22);
              *(undefined8 *)(param_1 + 0x24) = uVar28;
              uVar28 = *(undefined8 *)(param_2 + 0x24);
              *(undefined8 *)(param_1 + 0x26) = uVar29;
              uVar29 = *(undefined8 *)(param_2 + 0x26);
              *(undefined8 *)(param_1 + 0x28) = uVar32;
              uVar32 = *(undefined8 *)(param_2 + 0x68);
              *(undefined8 *)(param_1 + 0x2a) = uVar27;
              uVar27 = *(undefined8 *)(param_2 + 0x2a);
              *(undefined8 *)(param_1 + 0x2c) = uVar28;
              uVar28 = *(undefined8 *)(param_2 + 0x2c);
              *(undefined8 *)(param_1 + 0x2e) = uVar29;
              uVar29 = *(undefined8 *)(param_2 + 0x2e);
              *(undefined8 *)(param_1 + 0x30) = uVar33;
              uVar33 = *(undefined8 *)(param_2 + 0x30);
              *(undefined8 *)(param_1 + 0x32) = uVar27;
              uVar27 = *(undefined8 *)(param_2 + 0x32);
              *(undefined8 *)(param_1 + 0x34) = uVar28;
              uVar28 = *(undefined8 *)(param_2 + 0x34);
              *(undefined8 *)(param_1 + 0x36) = uVar29;
              uVar29 = *(undefined8 *)(param_2 + 0x36);
              *(undefined8 *)(param_1 + 0x38) = uVar33;
              uVar33 = *(undefined8 *)(param_2 + 0x78);
              *(undefined8 *)(param_1 + 0x3a) = uVar27;
              uVar27 = *(undefined8 *)(param_2 + 0x3a);
              *(undefined8 *)(param_1 + 0x3c) = uVar28;
              uVar28 = *(undefined8 *)(param_2 + 0x3c);
              *(undefined8 *)(param_1 + 0x3e) = uVar29;
              uVar29 = *(undefined8 *)(param_2 + 0x3e);
              *(undefined8 *)(param_1 + 0x40) = uVar34;
              uVar34 = *(undefined8 *)(param_2 + 0x40);
              *(undefined8 *)(param_1 + 0x42) = uVar27;
              uVar27 = *(undefined8 *)(param_2 + 0x42);
              *(undefined8 *)(param_1 + 0x44) = uVar28;
              uVar28 = *(undefined8 *)(param_2 + 0x44);
              *(undefined8 *)(param_1 + 0x46) = uVar29;
              uVar29 = *(undefined8 *)(param_2 + 0x46);
              *(undefined8 *)(param_1 + 0x48) = uVar34;
              uVar34 = *(undefined8 *)(param_2 + 0x88);
              *(undefined8 *)(param_1 + 0x4a) = uVar27;
              uVar27 = *(undefined8 *)(param_2 + 0x4a);
              *(undefined8 *)(param_1 + 0x4c) = uVar28;
              uVar28 = *(undefined8 *)(param_2 + 0x4c);
              *(undefined8 *)(param_1 + 0x4e) = uVar29;
              uVar29 = *(undefined8 *)(param_2 + 0x4e);
              param_1 = param_1 + 0x50;
              param_2 = param_2 + 0x50;
              iVar18 = iVar10 + -0x140;
              bVar25 = 0x13f < iVar10;
              iVar10 = iVar18;
            } while (bVar25);
          }
          *(undefined8 *)param_1 = uVar30;
          uVar30 = *(undefined8 *)param_2;
          *(undefined8 *)(param_1 + 2) = uVar27;
          uVar27 = *(undefined8 *)(param_2 + 2);
          *(undefined8 *)(param_1 + 4) = uVar28;
          uVar28 = *(undefined8 *)(param_2 + 4);
          *(undefined8 *)(param_1 + 6) = uVar29;
          uVar29 = *(undefined8 *)(param_2 + 6);
          *(undefined8 *)(param_1 + 8) = uVar30;
          *(undefined8 *)(param_1 + 10) = uVar27;
          uVar27 = *(undefined8 *)(param_2 + 10);
          *(undefined8 *)(param_1 + 0xc) = uVar28;
          uVar28 = *(undefined8 *)(param_2 + 0xc);
          *(undefined8 *)(param_1 + 0xe) = uVar29;
          uVar29 = *(undefined8 *)(param_2 + 0xe);
          *(undefined8 *)(param_1 + 0x10) = uVar31;
          uVar30 = *(undefined8 *)(param_2 + 0x10);
          *(undefined8 *)(param_1 + 0x12) = uVar27;
          uVar27 = *(undefined8 *)(param_2 + 0x12);
          *(undefined8 *)(param_1 + 0x14) = uVar28;
          uVar28 = *(undefined8 *)(param_2 + 0x14);
          *(undefined8 *)(param_1 + 0x16) = uVar29;
          uVar29 = *(undefined8 *)(param_2 + 0x16);
          *(undefined8 *)(param_1 + 0x18) = uVar30;
          *(undefined8 *)(param_1 + 0x1a) = uVar27;
          uVar27 = *(undefined8 *)(param_2 + 0x1a);
          *(undefined8 *)(param_1 + 0x1c) = uVar28;
          uVar28 = *(undefined8 *)(param_2 + 0x1c);
          *(undefined8 *)(param_1 + 0x1e) = uVar29;
          uVar29 = *(undefined8 *)(param_2 + 0x1e);
          *(undefined8 *)(param_1 + 0x20) = uVar32;
          uVar30 = *(undefined8 *)(param_2 + 0x20);
          *(undefined8 *)(param_1 + 0x22) = uVar27;
          uVar27 = *(undefined8 *)(param_2 + 0x22);
          *(undefined8 *)(param_1 + 0x24) = uVar28;
          uVar28 = *(undefined8 *)(param_2 + 0x24);
          *(undefined8 *)(param_1 + 0x26) = uVar29;
          uVar29 = *(undefined8 *)(param_2 + 0x26);
          *(undefined8 *)(param_1 + 0x28) = uVar30;
          *(undefined8 *)(param_1 + 0x2a) = uVar27;
          uVar27 = *(undefined8 *)(param_2 + 0x2a);
          *(undefined8 *)(param_1 + 0x2c) = uVar28;
          uVar28 = *(undefined8 *)(param_2 + 0x2c);
          *(undefined8 *)(param_1 + 0x2e) = uVar29;
          uVar29 = *(undefined8 *)(param_2 + 0x2e);
          *(undefined8 *)(param_1 + 0x30) = uVar33;
          uVar30 = *(undefined8 *)(param_2 + 0x30);
          *(undefined8 *)(param_1 + 0x32) = uVar27;
          uVar27 = *(undefined8 *)(param_2 + 0x32);
          *(undefined8 *)(param_1 + 0x34) = uVar28;
          uVar28 = *(undefined8 *)(param_2 + 0x34);
          *(undefined8 *)(param_1 + 0x36) = uVar29;
          uVar29 = *(undefined8 *)(param_2 + 0x36);
          *(undefined8 *)(param_1 + 0x38) = uVar30;
          *(undefined8 *)(param_1 + 0x3a) = uVar27;
          uVar27 = *(undefined8 *)(param_2 + 0x3a);
          *(undefined8 *)(param_1 + 0x3c) = uVar28;
          uVar28 = *(undefined8 *)(param_2 + 0x3c);
          *(undefined8 *)(param_1 + 0x3e) = uVar29;
          uVar29 = *(undefined8 *)(param_2 + 0x3e);
          *(undefined8 *)(param_1 + 0x40) = uVar34;
          uVar30 = *(undefined8 *)(param_2 + 0x40);
          *(undefined8 *)(param_1 + 0x42) = uVar27;
          uVar27 = *(undefined8 *)(param_2 + 0x42);
          *(undefined8 *)(param_1 + 0x44) = uVar28;
          uVar28 = *(undefined8 *)(param_2 + 0x44);
          *(undefined8 *)(param_1 + 0x46) = uVar29;
          uVar29 = *(undefined8 *)(param_2 + 0x46);
          *(undefined8 *)(param_1 + 0x48) = uVar30;
          param_2 = param_2 + 0x48;
          *(undefined8 *)(param_1 + 0x4a) = uVar27;
          *(undefined8 *)(param_1 + 0x4c) = uVar28;
          *(undefined8 *)(param_1 + 0x4e) = uVar29;
          param_1 = param_1 + 0x50;
          uVar11 = iVar18 + 0x140;
        }
        do {
          uVar9 = uVar11 - 0x40;
          uVar28 = *(undefined8 *)(param_2 + 2);
          *(undefined8 *)param_1 = *(undefined8 *)param_2;
          uVar27 = *(undefined8 *)(param_2 + 4);
          *(undefined8 *)(param_1 + 2) = uVar28;
          uVar28 = *(undefined8 *)(param_2 + 6);
          *(undefined8 *)(param_1 + 4) = uVar27;
          uVar27 = *(undefined8 *)(param_2 + 8);
          *(undefined8 *)(param_1 + 6) = uVar28;
          uVar28 = *(undefined8 *)(param_2 + 10);
          *(undefined8 *)(param_1 + 8) = uVar27;
          uVar27 = *(undefined8 *)(param_2 + 0xc);
          *(undefined8 *)(param_1 + 10) = uVar28;
          uVar28 = *(undefined8 *)(param_2 + 0xe);
          *(undefined8 *)(param_1 + 0xc) = uVar27;
          param_2 = param_2 + 0x10;
          *(undefined8 *)(param_1 + 0xe) = uVar28;
          param_1 = param_1 + 0x10;
          bVar25 = 0x3f < (int)uVar11;
          uVar11 = uVar9;
        } while (bVar25);
        if ((uVar9 & 0x3f) == 0) {
          return;
        }
      }
      uVar11 = uVar9 & 0x38;
      piVar22 = (int *)((int)param_1 + uVar11);
      piVar4 = (int *)((int)param_2 + uVar11);
      iVar10 = 0x34 - uVar11;
      switch(uVar11) {
      case 0x38:
        *(undefined8 *)(piVar22 + -0xe) = *(undefined8 *)(piVar4 + -0xe);
      case 0x30:
        *(undefined8 *)(piVar22 + -0xc) = *(undefined8 *)(piVar4 + -0xc);
      case 0x28:
        *(undefined8 *)(piVar22 + -10) = *(undefined8 *)(piVar4 + -10);
      case 0x20:
        *(undefined8 *)(piVar22 + -8) = *(undefined8 *)(piVar4 + -8);
      case 0x18:
        *(undefined8 *)(piVar22 + -6) = *(undefined8 *)(piVar4 + -6);
      case 0x10:
        *(undefined8 *)(piVar22 + -4) = *(undefined8 *)(piVar4 + -4);
      case 8:
        *(undefined8 *)(piVar22 + -2) = *(undefined8 *)(piVar4 + -2);
      case 0:
        piVar23 = piVar22;
        piVar5 = piVar4;
        if ((uVar9 & 4) != 0) {
          piVar5 = piVar4 + 1;
          iVar10 = *piVar4;
          piVar23 = piVar22 + 1;
          *piVar22 = iVar10;
        }
        uVar7 = (undefined2)iVar10;
        bVar26 = (bool)((byte)(uVar9 >> 1) & 1);
        bVar25 = (uVar9 & 1) != 0;
        piVar4 = piVar5;
        if (bVar26) {
          piVar4 = (int *)((int)piVar5 + 2);
          uVar7 = (undefined2)*piVar5;
        }
        uVar6 = 0;
        if (bVar25) {
          uVar6 = (undefined1)*piVar4;
        }
        piVar4 = piVar23;
        if (bVar26) {
          piVar4 = (int *)((int)piVar23 + 2);
          *(undefined2 *)piVar23 = uVar7;
        }
        if (bVar25) {
          *(undefined1 *)piVar4 = uVar6;
        }
        return;
      }
    }
    HintPreloadData(param_2);
    HintPreloadData(param_2 + 0x10);
    HintPreloadData(param_2 + 0x20);
    if (((uint)param_1 & 7) != 0) {
      uVar11 = (int)param_1 * -0x20000000;
      param_3 = param_3 - (uVar11 >> 0x1d);
      puVar24 = param_1;
      puVar21 = param_2;
      if ((int)uVar11 < 0) {
        puVar21 = param_2 + 1;
        uVar9 = *param_2;
        puVar24 = param_1 + 1;
        *param_1 = uVar9;
      }
      uVar6 = (undefined1)uVar9;
      bVar26 = (bool)((byte)(uVar11 >> 0x1e) & 1);
      uVar9 = (int)param_1 * -0x80000000;
      bVar25 = uVar9 != 0;
      puVar3 = puVar21;
      if (bVar25) {
        puVar3 = (uint *)((int)puVar21 + 1);
        uVar6 = (undefined1)*puVar21;
      }
      param_2 = puVar3;
      if (bVar26) {
        param_2 = (uint *)((int)puVar3 + 2);
        uVar9 = (uint)(ushort)*puVar3;
      }
      puVar21 = puVar24;
      if (bVar25) {
        puVar21 = (uint *)((int)puVar24 + 1);
        *(undefined1 *)puVar24 = uVar6;
      }
      param_1 = puVar21;
      if (bVar26) {
        param_1 = (uint *)((int)puVar21 + 2);
        *(ushort *)puVar21 = (ushort)uVar9;
      }
    }
    HintPreloadData(param_2 + 0x30);
    param_3 = param_3 - 0x40;
    if (-1 < (int)param_3) {
      HintPreloadData(param_2 + 0x40);
      uVar9 = *param_2;
      uVar11 = param_2[1];
      uVar12 = param_2[2];
      uVar13 = param_2[3];
      uVar14 = param_2[4];
      uVar15 = param_2[5];
      uVar16 = param_2[6];
      param_2 = param_2 + 7;
      uVar17 = *param_2;
      param_1 = param_1 + -2;
      while( true ) {
        bVar25 = param_3 < 0x40;
        param_3 = param_3 - 0x40;
        param_1[2] = uVar9;
        param_1[3] = uVar11;
        uVar9 = param_2[1];
        uVar11 = param_2[2];
        param_1[4] = uVar12;
        param_1[5] = uVar13;
        uVar12 = param_2[3];
        uVar13 = param_2[4];
        param_1[6] = uVar14;
        param_1[7] = uVar15;
        uVar14 = param_2[5];
        uVar15 = param_2[6];
        param_1[8] = uVar16;
        param_1[9] = uVar17;
        uVar16 = param_2[7];
        uVar17 = param_2[8];
        if (bVar25) break;
        HintPreloadData(param_2 + 0x49);
        param_1[10] = uVar9;
        param_1[0xb] = uVar11;
        uVar9 = param_2[9];
        uVar11 = param_2[10];
        param_1[0xc] = uVar12;
        param_1[0xd] = uVar13;
        uVar12 = param_2[0xb];
        uVar13 = param_2[0xc];
        param_1[0xe] = uVar14;
        param_1[0xf] = uVar15;
        uVar14 = param_2[0xd];
        uVar15 = param_2[0xe];
        param_1[0x10] = uVar16;
        param_1[0x11] = uVar17;
        uVar16 = param_2[0xf];
        param_2 = param_2 + 0x10;
        uVar17 = *param_2;
        param_1 = param_1 + 0x10;
      }
      param_1[10] = uVar9;
      param_1[0xb] = uVar11;
      param_2 = param_2 + 9;
      param_1[0xc] = uVar12;
      param_1[0xd] = uVar13;
      param_1[0xe] = uVar14;
      param_1[0xf] = uVar15;
      param_1[0x10] = uVar16;
      param_1[0x11] = uVar17;
      param_1 = param_1 + 0x12;
      param_3 = param_3 & 0x3f;
      if (param_3 == 0) {
        return;
      }
    }
  }
  uVar9 = param_3 & 0x3c;
  puVar19 = (undefined2 *)((int)param_1 + uVar9);
  pbVar1 = (byte *)((int)param_2 + uVar9);
  iVar10 = 0x3a - uVar9;
  switch(uVar9) {
  case 0x3c:
    *(undefined4 *)(puVar19 + -0x1e) = *(undefined4 *)(pbVar1 + -0x3c);
  case 0x38:
    *(undefined4 *)(puVar19 + -0x1c) = *(undefined4 *)(pbVar1 + -0x38);
  case 0x34:
    *(undefined4 *)(puVar19 + -0x1a) = *(undefined4 *)(pbVar1 + -0x34);
  case 0x30:
    *(undefined4 *)(puVar19 + -0x18) = *(undefined4 *)(pbVar1 + -0x30);
  case 0x2c:
    *(undefined4 *)(puVar19 + -0x16) = *(undefined4 *)(pbVar1 + -0x2c);
  case 0x28:
    *(undefined4 *)(puVar19 + -0x14) = *(undefined4 *)(pbVar1 + -0x28);
  case 0x24:
    *(undefined4 *)(puVar19 + -0x12) = *(undefined4 *)(pbVar1 + -0x24);
  case 0x20:
    *(undefined4 *)(puVar19 + -0x10) = *(undefined4 *)(pbVar1 + -0x20);
  case 0x1c:
    *(undefined4 *)(puVar19 + -0xe) = *(undefined4 *)(pbVar1 + -0x1c);
  case 0x18:
    *(undefined4 *)(puVar19 + -0xc) = *(undefined4 *)(pbVar1 + -0x18);
  case 0x14:
    *(undefined4 *)(puVar19 + -10) = *(undefined4 *)(pbVar1 + -0x14);
  case 0x10:
    *(undefined4 *)(puVar19 + -8) = *(undefined4 *)(pbVar1 + -0x10);
  case 0xc:
    *(undefined4 *)(puVar19 + -6) = *(undefined4 *)(pbVar1 + -0xc);
  case 8:
    *(undefined4 *)(puVar19 + -4) = *(undefined4 *)(pbVar1 + -8);
  case 4:
    iVar10 = *(int *)(pbVar1 + -4);
    *(int *)(puVar19 + -2) = iVar10;
  case 0:
    uVar7 = (undefined2)iVar10;
    bVar26 = (bool)((byte)(param_3 >> 1) & 1);
    bVar25 = (param_3 & 1) != 0;
    pbVar2 = pbVar1;
    if (bVar26) {
      pbVar2 = pbVar1 + 2;
      uVar7 = *(undefined2 *)pbVar1;
    }
    if (bVar25) {
      pbVar2 = (byte *)(uint)*pbVar2;
    }
    puVar20 = puVar19;
    if (bVar26) {
      puVar20 = puVar19 + 1;
      *puVar19 = uVar7;
    }
    if (bVar25) {
      *(char *)puVar20 = (char)pbVar2;
    }
    return;
  }
}



//==================== FUN_000027e0 @ 000027e0 ====================

void FUN_000027e0(undefined4 *param_1,undefined1 param_2,uint param_3)

{
  uint uVar1;
  undefined4 *puVar3;
  undefined4 *puVar2;
  undefined4 uVar4;
  
  if (((uint)param_1 & 3) != 0) {
    if (param_3 == 0) {
      return;
    }
    puVar3 = param_1;
    while( true ) {
      param_3 = param_3 - 1;
      param_1 = (undefined4 *)((int)puVar3 + 1);
      *(undefined1 *)puVar3 = param_2;
      if (((uint)param_1 & 3) == 0) break;
      puVar3 = param_1;
      if (param_3 == 0) {
        return;
      }
    }
  }
  if (param_3 < 4) {
    if (param_3 == 0) {
      return;
    }
    puVar3 = param_1;
    do {
      puVar2 = (undefined4 *)((int)puVar3 + 1);
      *(undefined1 *)puVar3 = param_2;
      puVar3 = puVar2;
    } while ((undefined4 *)((int)param_1 + param_3) != puVar2);
    return;
  }
  uVar4 = CONCAT22(CONCAT11(param_2,param_2),CONCAT11(param_2,param_2));
  if (0xf < param_3) {
    uVar1 = param_3 - 0x10 >> 4;
    puVar3 = param_1 + 4;
    do {
      puVar3[-4] = uVar4;
      puVar2 = puVar3 + 4;
      puVar3[-3] = uVar4;
      puVar3[-2] = uVar4;
      puVar3[-1] = uVar4;
      puVar3 = puVar2;
    } while (puVar2 != param_1 + uVar1 * 4 + 8);
    param_3 = param_3 & 0xf;
    param_1 = param_1 + (uVar1 + 1) * 4;
    if (param_3 < 4) goto LAB_000028a8;
  }
  uVar1 = param_3 - 4 >> 2;
  puVar3 = param_1;
  do {
    puVar2 = puVar3 + 1;
    *puVar3 = uVar4;
    puVar3 = puVar2;
  } while (puVar2 != param_1 + uVar1 + 1);
  param_3 = param_3 & 3;
  param_1 = param_1 + uVar1 + 1;
LAB_000028a8:
  if (param_3 == 0) {
    return;
  }
  puVar3 = param_1;
  do {
    puVar2 = (undefined4 *)((int)puVar3 + 1);
    *(undefined1 *)puVar3 = param_2;
    puVar3 = puVar2;
  } while ((undefined4 *)((int)param_1 + param_3) != puVar2);
  return;
}



//==================== FUN_00002900 @ 00002900 ====================

int FUN_00002900(uint param_1)

{
  char cVar1;
  char cVar2;
  char cVar3;
  char cVar4;
  int iVar5;
  int iVar6;
  uint *puVar7;
  uint uVar8;
  uint uVar9;
  uint uVar10;
  bool bVar11;
  bool bVar12;
  bool bVar13;
  bool bVar14;
  
  HintPreloadData(param_1);
  puVar7 = (uint *)(param_1 & 0xfffffff8);
  HintPreloadData(puVar7 + 8);
  if ((param_1 & 7) != 0) {
    uVar9 = puVar7[1];
    iVar6 = -(param_1 & 7);
    HintPreloadData(puVar7 + 0x10);
    uVar10 = -1 << ((param_1 & 3) << 3);
    uVar8 = *puVar7 | ~uVar10;
    if ((param_1 & 4) == 0) goto LAB_00002930;
    uVar9 = uVar9 | ~uVar10;
    uVar8 = 0xffffffff;
    goto LAB_00002930;
  }
  iVar5 = -8;
  do {
    uVar8 = *puVar7;
    uVar9 = puVar7[1];
    HintPreloadData(puVar7 + 0x10);
    iVar6 = iVar5 + 8;
LAB_00002930:
    cVar1 = -((char)uVar8 == '\0');
    cVar2 = -((char)(uVar8 >> 8) == '\0');
    cVar3 = -((char)(uVar8 >> 0x10) == '\0');
    cVar4 = -((char)(uVar8 >> 0x18) == '\0');
    uVar8 = CONCAT13(cVar4,CONCAT12(cVar3,CONCAT11(cVar2,cVar1)));
    bVar11 = (char)uVar9 != '\0';
    bVar12 = (char)(uVar9 >> 8) != '\0';
    bVar13 = (char)(uVar9 >> 0x10) != '\0';
    bVar14 = (char)(uVar9 >> 0x18) != '\0';
    uVar9 = CONCAT13(bVar14 * cVar4 - !bVar14,
                     CONCAT12(bVar13 * cVar3 - !bVar13,
                              CONCAT11(bVar12 * cVar2 - !bVar12,bVar11 * cVar1 - !bVar11)));
    iVar5 = iVar6;
    if (uVar9 != 0) break;
    uVar8 = puVar7[2];
    uVar9 = puVar7[3];
    iVar5 = iVar6 + 8;
    cVar1 = -((char)uVar8 == '\0');
    cVar2 = -((char)(uVar8 >> 8) == '\0');
    cVar3 = -((char)(uVar8 >> 0x10) == '\0');
    cVar4 = -((char)(uVar8 >> 0x18) == '\0');
    uVar8 = CONCAT13(cVar4,CONCAT12(cVar3,CONCAT11(cVar2,cVar1)));
    bVar11 = (char)uVar9 != '\0';
    bVar12 = (char)(uVar9 >> 8) != '\0';
    bVar13 = (char)(uVar9 >> 0x10) != '\0';
    bVar14 = (char)(uVar9 >> 0x18) != '\0';
    uVar9 = CONCAT13(bVar14 * cVar4 - !bVar14,
                     CONCAT12(bVar13 * cVar3 - !bVar13,
                              CONCAT11(bVar12 * cVar2 - !bVar12,bVar11 * cVar1 - !bVar11)));
    if (uVar9 != 0) break;
    uVar8 = puVar7[4];
    uVar9 = puVar7[5];
    iVar5 = iVar6 + 0x10;
    cVar1 = -((char)uVar8 == '\0');
    cVar2 = -((char)(uVar8 >> 8) == '\0');
    cVar3 = -((char)(uVar8 >> 0x10) == '\0');
    cVar4 = -((char)(uVar8 >> 0x18) == '\0');
    uVar8 = CONCAT13(cVar4,CONCAT12(cVar3,CONCAT11(cVar2,cVar1)));
    bVar11 = (char)uVar9 != '\0';
    bVar12 = (char)(uVar9 >> 8) != '\0';
    bVar13 = (char)(uVar9 >> 0x10) != '\0';
    bVar14 = (char)(uVar9 >> 0x18) != '\0';
    uVar9 = CONCAT13(bVar14 * cVar4 - !bVar14,
                     CONCAT12(bVar13 * cVar3 - !bVar13,
                              CONCAT11(bVar12 * cVar2 - !bVar12,bVar11 * cVar1 - !bVar11)));
    if (uVar9 != 0) break;
    uVar8 = puVar7[6];
    uVar9 = puVar7[7];
    puVar7 = puVar7 + 8;
    iVar5 = iVar6 + 0x18;
    cVar1 = -((char)uVar8 == '\0');
    cVar2 = -((char)(uVar8 >> 8) == '\0');
    cVar3 = -((char)(uVar8 >> 0x10) == '\0');
    cVar4 = -((char)(uVar8 >> 0x18) == '\0');
    uVar8 = CONCAT13(cVar4,CONCAT12(cVar3,CONCAT11(cVar2,cVar1)));
    bVar11 = (char)uVar9 != '\0';
    bVar12 = (char)(uVar9 >> 8) != '\0';
    bVar13 = (char)(uVar9 >> 0x10) != '\0';
    bVar14 = (char)(uVar9 >> 0x18) != '\0';
    uVar9 = CONCAT13(bVar14 * cVar4 - !bVar14,
                     CONCAT12(bVar13 * cVar3 - !bVar13,
                              CONCAT11(bVar12 * cVar2 - !bVar12,bVar11 * cVar1 - !bVar11)));
  } while (uVar9 == 0);
  if (uVar8 == 0) {
    iVar5 = iVar5 + 4;
    uVar8 = uVar9;
  }
  return iVar5 + ((uint)LZCOUNT(uVar8 << 0x18 | (uVar8 >> 8 & 0xff) << 0x10 |
                                (uVar8 >> 0x10 & 0xff) << 8 | uVar8 >> 0x18) >> 3);
}



//==================== FUN_000029dc @ 000029dc ====================

/* WARNING: Removing unreachable block (ram,0x00002ad4) */
/* WARNING: Removing unreachable block (ram,0x00002ae4) */
/* WARNING: Removing unreachable block (ram,0x00002b28) */
/* WARNING: Removing unreachable block (ram,0x00002aec) */
/* WARNING: Removing unreachable block (ram,0x00002afc) */

void FUN_000029dc(undefined4 param_1,int param_2)

{
  uint uVar1;
  bool bVar2;
  int iVar3;
  undefined4 uVar4;
  code *pcVar5;
  int *piVar6;
  undefined4 *puVar7;
  uint uVar8;
  int iVar9;
  int iVar10;
  
  iVar3 = DAT_00015b74;
  piVar6 = (int *)(DAT_00015b74 + 0x148);
  uVar4 = param_1;
LAB_00002a10:
  iVar9 = *(int *)(iVar3 + 0x148);
  if (iVar9 != 0) {
    uVar8 = *(int *)(iVar9 + 4) - 1;
    if (-1 < (int)uVar8) {
      puVar7 = (undefined4 *)(iVar9 + (*(int *)(iVar9 + 4) + 1) * 4);
      do {
        if ((param_2 == 0) || (puVar7[0x40] == param_2)) {
          pcVar5 = (code *)*puVar7;
          if (*(int *)(iVar9 + 4) - 1U == uVar8) {
            *(uint *)(iVar9 + 4) = uVar8;
          }
          else {
            *puVar7 = 0;
          }
          if (pcVar5 != Reset) {
            uVar1 = 1 << (uVar8 & 0xff);
            iVar10 = *(int *)(iVar9 + 4);
            if ((uVar1 & *(uint *)(iVar9 + 0x188)) == 0) {
              uVar4 = (*pcVar5)(uVar4);
            }
            else if ((uVar1 & *(uint *)(iVar9 + 0x18c)) == 0) {
              uVar4 = (*pcVar5)(param_1);
            }
            else {
              uVar4 = (*pcVar5)(puVar7[0x20]);
            }
            if ((*(int *)(iVar9 + 4) != iVar10) || (*piVar6 != iVar9)) goto LAB_00002a10;
          }
        }
        bVar2 = uVar8 == 0;
        puVar7 = puVar7 + -1;
        uVar8 = uVar8 - 1;
        if (bVar2) {
          return;
        }
      } while( true );
    }
  }
  return;
}



//==================== FUN_00002b40 @ 00002b40 ====================

void FUN_00002b40(void)

{
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}



//==================== FUN_00002b44 @ 00002b44 ====================

uint FUN_00002b44(undefined4 *param_1,uint param_2)

{
  byte bVar1;
  uint uVar2;
  byte *pbVar4;
  byte *pbVar3;
  byte *pbVar5;
  bool bVar6;
  bool bVar7;
  bool bVar8;
  
  pbVar5 = (byte *)*param_1;
  if (pbVar5 != (byte *)0x0) {
    pbVar4 = pbVar5;
    do {
      pbVar3 = pbVar4 + 1;
      bVar1 = *pbVar4;
      bVar8 = 0x39 < bVar1;
      bVar6 = bVar1 != 0x3a;
      if (bVar6) {
        bVar8 = 0x1f < bVar1;
      }
      bVar7 = bVar1 != 0x20;
      if (bVar8 && (bVar6 && bVar7)) {
        param_2 = 1;
      }
      if (!bVar8 || (!bVar6 || !bVar7)) {
        param_2 = 0;
      }
      pbVar4 = pbVar3;
    } while (bVar8 && (bVar6 && bVar7));
    if (bVar1 != 0x3a) {
      return param_2;
    }
    uVar2 = *pbVar5 - 0x30;
    if ((((uVar2 & 0xff) < 10) && (pbVar3 == pbVar5 + 2)) && ((int)uVar2 < 2)) {
      *param_1 = pbVar3;
      return uVar2;
    }
  }
  return 0xffffffff;
}



//==================== FUN_00002bbc @ 00002bbc ====================

undefined4 FUN_00002bbc(undefined4 *param_1,undefined4 *param_2)

{
  uint uVar1;
  char *pcVar2;
  
  if ((((param_1 != (undefined4 *)0x0) && (pcVar2 = (char *)*param_1, pcVar2 != (char *)0x0)) &&
      (*pcVar2 != '\0')) && (*(short *)(param_1 + 1) == *(short *)(pcVar2 + 6))) {
    uVar1 = FUN_000064b8(pcVar2[1]);
    if ((uVar1 & 1) == 0) {
      *param_2 = *param_1;
      return 0;
    }
  }
  *param_2 = 0;
  return 9;
}



//==================== FUN_00002c30 @ 00002c30 ====================

void FUN_00002c30(int *param_1,undefined4 *param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  char cVar5;
  int iVar6;
  char *pcVar7;
  
  iVar6 = *param_1;
  iVar3 = 0;
  iVar4 = 0;
  pcVar7 = (char *)(iVar6 + -1);
  do {
    pcVar7 = pcVar7 + 1;
    cVar5 = *pcVar7;
    iVar4 = iVar4 + 1;
    if (cVar5 != ' ') {
      if (cVar5 == '\x05') {
        cVar5 = -0x1b;
      }
      iVar2 = iVar3 + 1;
      iVar1 = iVar3;
      if (iVar4 == 9) {
        *(undefined1 *)((int)param_2 + iVar3 + 9) = 0x2e;
        iVar2 = iVar3 + 2;
        iVar1 = iVar3 + 1;
      }
      iVar3 = iVar2;
      *(char *)((int)param_2 + iVar1 + 9) = cVar5;
    }
  } while (iVar4 != 0xb);
  *(undefined1 *)((int)param_2 + iVar3 + 9) = 0;
  *(undefined1 *)(param_2 + 2) = *(undefined1 *)(iVar6 + 0xb);
  *param_2 = *(undefined4 *)(iVar6 + 0x1c);
  *(undefined2 *)((int)param_2 + 6) = *(undefined2 *)(iVar6 + 0x16);
  *(undefined2 *)(param_2 + 1) = *(undefined2 *)(iVar6 + 0x18);
  return;
}



//==================== FUN_00002ce0 @ 00002ce0 ====================

undefined4 FUN_00002ce0(int param_1)

{
  int iVar1;
  
  iVar1 = FUN_0000676c(*(undefined1 *)(param_1 + 1),param_1 + 0x40,*(undefined4 *)(param_1 + 0x2c),1
                      );
  if (iVar1 != 0) {
    return 1;
  }
  *(undefined1 *)(param_1 + 3) = 0;
  if (((uint)(*(int *)(param_1 + 0x2c) - *(int *)(param_1 + 0x20)) < *(uint *)(param_1 + 0x18)) &&
     (*(char *)(param_1 + 2) == '\x02')) {
    FUN_0000676c(*(undefined1 *)(param_1 + 1),param_1 + 0x40,
                 *(int *)(param_1 + 0x2c) + *(uint *)(param_1 + 0x18),1);
    return 0;
  }
  return 0;
}



//==================== FUN_00002d5c @ 00002d5c ====================

bool FUN_00002d5c(int param_1,int param_2)

{
  int iVar1;
  uint uVar2;
  undefined1 *puVar4;
  undefined1 *puVar3;
  uint uVar5;
  int iVar6;
  
  if ((*(char *)(param_1 + 3) != '\0') && (iVar6 = FUN_00002ce0(), iVar6 != 0)) {
    return true;
  }
  if (param_2 - 2U < *(int *)(param_1 + 0x14) - 2U) {
    iVar6 = (param_2 - 2U) * (uint)*(ushort *)(param_1 + 10) + *(int *)(param_1 + 0x28);
  }
  else {
    iVar6 = 0;
  }
  *(int *)(param_1 + 0x2c) = iVar6;
  puVar4 = (undefined1 *)(param_1 + 0x40);
  do {
    puVar3 = puVar4 + 1;
    *puVar4 = 0;
    puVar4 = puVar3;
  } while (puVar3 != (undefined1 *)(param_1 + 0x240));
  if (*(short *)(param_1 + 10) == 0) {
    uVar2 = 0;
    uVar5 = 0;
  }
  else {
    uVar5 = 0;
    do {
      iVar1 = FUN_0000676c(*(undefined1 *)(param_1 + 1),(undefined1 *)(param_1 + 0x40),iVar6 + uVar5
                           ,1);
      uVar2 = (uint)*(ushort *)(param_1 + 10);
      if (iVar1 != 0) break;
      uVar5 = uVar5 + 1;
    } while (uVar5 < uVar2);
  }
  return uVar5 != uVar2;
}



//==================== FUN_00002e20 @ 00002e20 ====================

uint FUN_00002e20(char *param_1)

{
  char cVar1;
  int iVar2;
  uint uVar3;
  undefined4 uVar4;
  char *pcVar6;
  char *pcVar5;
  undefined4 uVar7;
  
  if (param_1[3] == '\0') {
    cVar1 = *param_1;
  }
  else {
    uVar3 = FUN_00002ce0();
    if (uVar3 != 0) {
      return uVar3;
    }
    cVar1 = *param_1;
  }
  if ((cVar1 == '\x03') && (param_1[4] == '\x01')) {
    pcVar6 = param_1 + 0x40;
    do {
      pcVar5 = pcVar6 + 1;
      *pcVar6 = '\0';
      pcVar6 = pcVar5;
    } while (param_1 + 0x240 != pcVar5);
    uVar4 = *(undefined4 *)(param_1 + 0x10);
    uVar7 = *(undefined4 *)(param_1 + 0xc);
    param_1[0x228] = (char)uVar4;
    param_1[0x23e] = 'U';
    param_1[0x22c] = (char)uVar7;
    param_1[0x229] = (char)((uint)uVar4 >> 8);
    param_1[0x22a] = (char)((uint)uVar4 >> 0x10);
    param_1[0x22b] = (char)((uint)uVar4 >> 0x18);
    param_1[0x22d] = (char)((uint)uVar7 >> 8);
    param_1[0x22e] = (char)((uint)uVar7 >> 0x10);
    param_1[0x23f] = -0x56;
    param_1[0x22f] = (char)((uint)uVar7 >> 0x18);
    param_1[0x224] = 'r';
    param_1[0x225] = 'r';
    *(int *)(param_1 + 0x2c) = *(int *)(param_1 + 0x1c) + 1;
    param_1[0x40] = 'R';
    param_1[0x41] = 'R';
    param_1[0x42] = 'a';
    param_1[0x227] = 'a';
    param_1[0x43] = 'A';
    param_1[0x226] = 'A';
    FUN_0000676c(param_1[1],param_1 + 0x40,*(int *)(param_1 + 0x1c) + 1,1);
    param_1[4] = '\0';
  }
  iVar2 = FUN_000066f8(param_1[1],0);
  return (uint)(iVar2 != 0);
}



//==================== FUN_00002f3c @ 00002f3c ====================

uint FUN_00002f3c(int param_1,undefined4 param_2)

{
  uint uVar1;
  int iVar2;
  
  if ((*(char *)(param_1 + 3) != '\0') && (uVar1 = FUN_00002ce0(), uVar1 != 0)) {
    return uVar1;
  }
  iVar2 = FUN_0000667c(*(undefined1 *)(param_1 + 1),param_1 + 0x40,param_2,1);
  if (iVar2 != 0) {
    param_2 = 0xffffffff;
  }
  *(undefined4 *)(param_1 + 0x2c) = param_2;
  return (uint)(iVar2 != 0);
}



//==================== FUN_00002f8c @ 00002f8c ====================

int FUN_00002f8c(int *param_1)

{
  int iVar1;
  int iVar2;
  
  iVar2 = *param_1;
  if ((param_1[6] == *(int *)(iVar2 + 0x2c)) || (iVar1 = FUN_00002f3c(iVar2), iVar1 == 0)) {
    *(undefined1 *)param_1[7] = 0xe5;
    *(undefined1 *)(iVar2 + 3) = 1;
    return 0;
  }
  return iVar1;
}



//==================== FUN_00002fd8 @ 00002fd8 ====================

undefined4 FUN_00002fd8(int param_1,int param_2)

{
  byte bVar1;
  undefined4 uVar2;
  byte *pbVar3;
  byte *pbVar4;
  byte *pbVar5;
  int iVar6;
  bool bVar7;
  
  *(undefined1 *)(param_1 + 3) = 0;
  *(undefined4 *)(param_1 + 0x2c) = 0xffffffff;
  if ((param_2 != -1) && (iVar6 = FUN_00002f3c(), iVar6 != 0)) {
    return 4;
  }
  if (*(short *)(param_1 + 0x23e) != -0x55ab) {
    return 3;
  }
  bVar1 = *(byte *)(param_1 + 0x40);
  bVar7 = 0xea < bVar1;
  if (bVar1 != 0xeb) {
    bVar7 = bVar1 != 0xe8;
  }
  if (!bVar7 || (bVar1 == 0xeb || bVar1 == 0xe9)) {
    if (((*(char *)(param_1 + 0x76) == 'F') && (*(char *)(param_1 + 0x77) == 'A')) &&
       (*(char *)(param_1 + 0x78) == 'T')) {
      return 0;
    }
    pbVar3 = (byte *)0x17244;
    iVar6 = *(byte *)(param_1 + 0x92) - 0x46;
    pbVar4 = (byte *)(param_1 + 0x93);
    while (iVar6 == 0) {
      pbVar5 = pbVar4 + 1;
      pbVar3 = pbVar3 + 1;
      iVar6 = (uint)*pbVar4 - (uint)*pbVar3;
      pbVar4 = pbVar5;
      if (pbVar5 == (byte *)(param_1 + 0x97)) {
        if (iVar6 == 0) {
          uVar2 = 0;
        }
        else {
          uVar2 = 2;
        }
        return uVar2;
      }
    }
  }
  return 2;
}



//==================== FUN_000030bc @ 000030bc ====================

undefined4 FUN_000030bc(undefined4 param_1,undefined4 *param_2,uint param_3)

{
  byte bVar1;
  byte bVar2;
  ushort uVar3;
  int iVar4;
  undefined4 uVar5;
  uint uVar6;
  char *pcVar7;
  int *piVar8;
  char cVar9;
  uint uVar10;
  char *pcVar11;
  int iVar12;
  char *pcVar13;
  int *piVar14;
  uint uVar15;
  int iVar16;
  int iVar17;
  uint extraout_r12;
  uint uVar18;
  uint extraout_r12_00;
  bool bVar19;
  int aiStack_38 [4];
  int iStack_28;
  
  *param_2 = 0;
  iVar4 = FUN_00002b44();
  if (iVar4 < 0) {
    uVar5 = 0xb;
  }
  else {
    pcVar13 = *(char **)(&DAT_00021544 + iVar4 * 4);
    if (pcVar13 == (char *)0x0) {
      uVar5 = 0xc;
    }
    else {
      *param_2 = pcVar13;
      if ((*pcVar13 == '\0') || (uVar6 = FUN_000064b8(pcVar13[1]), (uVar6 & 1) != 0)) {
        *pcVar13 = '\0';
        pcVar13[1] = (char)iVar4;
        uVar6 = FUN_000065cc();
        if ((uVar6 & 1) == 0) {
          if (((param_3 & 0xfe) != 0) && ((uVar6 & 4) != 0)) {
            return 10;
          }
          uVar6 = FUN_00002fd8(pcVar13,0);
          if (uVar6 == 2) {
            piVar8 = aiStack_38;
            pcVar7 = pcVar13 + 0x208;
            do {
              pcVar11 = pcVar7 + 0x10;
              uVar5 = 0;
              if (pcVar7[-6] != '\0') {
                uVar5 = CONCAT31(CONCAT21(*(undefined2 *)pcVar7,pcVar7[-1]),pcVar7[-2]);
              }
              *piVar8 = uVar5;
              piVar8 = piVar8 + 1;
              pcVar7 = pcVar11;
            } while (pcVar11 != pcVar13 + 0x248);
            piVar8 = aiStack_38;
            do {
              piVar14 = piVar8 + 1;
              iVar4 = *piVar8;
              uVar6 = 3;
              if ((iVar4 != 0) &&
                 (uVar6 = FUN_00002fd8(pcVar13,iVar4), uVar18 = extraout_r12_00, uVar6 < 2))
              goto LAB_00003194;
              piVar8 = piVar14;
            } while (&iStack_28 != piVar14);
            if (uVar6 != 4) {
              return 0xd;
            }
          }
          else if (uVar6 != 4) {
            if (uVar6 < 2) {
              iVar4 = 0;
              uVar18 = extraout_r12;
LAB_00003194:
              if (*(short *)(pcVar13 + 0x4b) == 0x200) {
                uVar6 = (uint)*(ushort *)(pcVar13 + 0x56);
                if (uVar6 == 0) {
                  uVar6 = *(uint *)(pcVar13 + 100);
                }
                bVar1 = pcVar13[0x50];
                *(uint *)(pcVar13 + 0x18) = uVar6;
                pcVar13[2] = bVar1;
                if (bVar1 - 1 < 2) {
                  bVar2 = pcVar13[0x4d];
                  *(ushort *)(pcVar13 + 10) = (ushort)bVar2;
                  if ((bVar2 != 0) && ((bVar2 - 1 & bVar2) == 0)) {
                    uVar3 = *(ushort *)(pcVar13 + 0x51);
                    *(ushort *)(pcVar13 + 8) = uVar3;
                    if ((uVar3 & 0xf) == 0) {
                      bVar2 = pcVar13[0x54];
                      uVar10 = (uint)*(ushort *)(pcVar13 + 0x53);
                      bVar19 = uVar10 == 0;
                      if (bVar19) {
                        uVar10 = (uint)(byte)pcVar13[0x61];
                        uVar18 = (uint)*(ushort *)(pcVar13 + 0x62);
                        bVar2 = pcVar13[0x60];
                      }
                      if (bVar19) {
                        uVar10 = (uint)bVar2 | (uVar10 | uVar18 << 8) << 8;
                      }
                      uVar18 = (uint)CONCAT11(pcVar13[0x4f],pcVar13[0x4e]);
                      if (uVar18 != 0) {
                        iVar17 = uVar6 * bVar1;
                        uVar15 = uVar18 + (uVar3 >> 4) + iVar17;
                        if ((uVar15 <= uVar10) &&
                           (pcVar7 = (char *)FUN_00001a50(uVar10 - uVar15),
                           pcVar7 + -1 < (char *)0xffffff4 || pcVar7 == (char *)0xffffff5)) {
                          iVar12 = uVar18 + iVar4;
                          iVar16 = uVar15 + iVar4;
                          pcVar11 = pcVar7 + 2;
                          if (s_PS7_init_Data_Corrupted_0000fff0 + 5 < pcVar7) {
                            *(int *)(pcVar13 + 0x20) = iVar12;
                            *(char **)(pcVar13 + 0x14) = pcVar11;
                            *(int *)(pcVar13 + 0x1c) = iVar4;
                            *(int *)(pcVar13 + 0x28) = iVar16;
                            if (uVar3 != 0 || *(short *)(pcVar13 + 0x6a) != 0) {
                              return 0xd;
                            }
                            *(undefined4 *)(pcVar13 + 0x24) = *(undefined4 *)(pcVar13 + 0x6c);
                            if (uVar6 < (int)pcVar11 * 4 + 0x1ffU >> 9) {
                              return 0xd;
                            }
                            pcVar13[0x10] = -1;
                            pcVar13[0x11] = -1;
                            pcVar13[0x12] = -1;
                            pcVar13[0x13] = -1;
                            pcVar13[0xc] = -1;
                            pcVar13[0xd] = -1;
                            pcVar13[0xe] = -1;
                            pcVar13[0xf] = -1;
                            pcVar13[4] = -0x80;
                            if (((*(short *)(pcVar13 + 0x70) == 1) &&
                                (((iVar4 + 1 == *(int *)(pcVar13 + 0x2c) ||
                                  (iVar4 = FUN_00002f3c(pcVar13), iVar4 == 0)) &&
                                 (pcVar13[4] = '\0', *(short *)(pcVar13 + 0x23e) == -0x55ab)))) &&
                               ((*(int *)(pcVar13 + 0x40) == 0x41615252 &&
                                (*(int *)(pcVar13 + 0x224) == 0x61417272)))) {
                              cVar9 = '\x03';
                              *(undefined4 *)(pcVar13 + 0x10) = *(undefined4 *)(pcVar13 + 0x228);
                              *(uint *)(pcVar13 + 0xc) =
                                   CONCAT31(CONCAT21(*(undefined2 *)(pcVar13 + 0x22e),pcVar13[0x22d]
                                                    ),pcVar13[0x22c]);
                            }
                            else {
                              cVar9 = '\x03';
                            }
                          }
                          else {
                            if (pcVar7 < (char *)0xff6) {
                              *(char **)(pcVar13 + 0x14) = pcVar11;
                              *(int *)(pcVar13 + 0x1c) = iVar4;
                              *(int *)(pcVar13 + 0x20) = iVar12;
                              *(int *)(pcVar13 + 0x28) = iVar16;
                              if (uVar3 == 0) {
                                return 0xd;
                              }
                              *(int *)(pcVar13 + 0x24) = iVar17 + iVar12;
                              iVar4 = ((uint)pcVar11 & 1) + ((uint)((int)pcVar11 * 3) >> 1);
                              cVar9 = '\x01';
                            }
                            else {
                              *(char **)(pcVar13 + 0x14) = pcVar11;
                              *(int *)(pcVar13 + 0x1c) = iVar4;
                              *(int *)(pcVar13 + 0x20) = iVar12;
                              *(int *)(pcVar13 + 0x28) = iVar16;
                              if (uVar3 == 0) {
                                return 0xd;
                              }
                              iVar4 = (int)pcVar11 * 2;
                              *(int *)(pcVar13 + 0x24) = iVar17 + iVar12;
                              cVar9 = '\x02';
                            }
                            if (uVar6 < iVar4 + 0x1ffU >> 9) {
                              return 0xd;
                            }
                            pcVar13[0x10] = -1;
                            pcVar13[0x11] = -1;
                            pcVar13[0x12] = -1;
                            pcVar13[0x13] = -1;
                            pcVar13[0xc] = -1;
                            pcVar13[0xd] = -1;
                            pcVar13[0xe] = -1;
                            pcVar13[0xf] = -1;
                            pcVar13[4] = -0x80;
                          }
                          *pcVar13 = cVar9;
                          DAT_0002154c = DAT_0002154c + 1;
                          *(short *)(pcVar13 + 6) = DAT_0002154c;
                          return 0;
                        }
                      }
                    }
                  }
                }
              }
            }
            return 0xd;
          }
          uVar5 = 1;
        }
        else {
          uVar5 = 3;
        }
      }
      else if ((param_3 & 0xfe) == 0) {
        uVar5 = 0;
      }
      else {
        uVar5 = 0;
        if ((uVar6 & 4) != 0) {
          return 10;
        }
      }
    }
  }
  return uVar5;
}



//==================== FUN_0000351c @ 0000351c ====================

int FUN_0000351c(char *param_1,uint param_2,uint param_3)

{
  char cVar1;
  undefined2 uVar2;
  int iVar3;
  uint uVar4;
  byte *pbVar5;
  byte bVar6;
  uint uVar7;
  
  cVar1 = *param_1;
  bVar6 = (byte)(param_3 >> 8);
  if (cVar1 == '\x02') {
    if ((*(int *)(param_1 + 0x20) + (param_2 >> 8) == *(int *)(param_1 + 0x2c)) ||
       (iVar3 = FUN_00002f3c(), iVar3 == 0)) {
      iVar3 = (param_2 & 0xff) * 2;
      param_1[iVar3 + 0x40] = (char)param_3;
      param_1[iVar3 + 0x41] = bVar6;
      param_1[3] = '\x01';
      return 0;
    }
    return iVar3;
  }
  if (cVar1 == '\x03') {
    if ((*(int *)(param_1 + 0x20) + (param_2 >> 7) == *(int *)(param_1 + 0x2c)) ||
       (iVar3 = FUN_00002f3c(), iVar3 == 0)) {
      param_3 = param_3 & 0xfffffff;
      iVar3 = (param_2 & 0x7f) * 4;
      uVar2 = *(undefined2 *)(param_1 + iVar3 + 0x42);
      param_1[iVar3 + 0x40] = (char)param_3;
      param_1[iVar3 + 0x41] = (char)(param_3 >> 8);
      param_1[iVar3 + 0x42] = (char)(param_3 >> 0x10);
      param_1[iVar3 + 0x43] = (byte)((ushort)uVar2 >> 8) & 0xf0 | (byte)(param_3 >> 0x18);
      param_1[3] = '\x01';
      return 0;
    }
    return iVar3;
  }
  if (cVar1 != '\x01') {
    return 2;
  }
  uVar7 = param_2 + (param_2 >> 1);
  if ((*(int *)(param_1 + 0x20) + (uVar7 >> 9) != *(int *)(param_1 + 0x2c)) &&
     (iVar3 = FUN_00002f3c(), iVar3 != 0)) {
    return iVar3;
  }
  uVar4 = uVar7 + 1;
  uVar7 = uVar7 & 0x1ff;
  if ((param_2 & 1) == 0) {
    param_1[uVar7 + 0x40] = (char)param_3;
    param_1[3] = '\x01';
    if (((uVar4 >> 9) + *(int *)(param_1 + 0x20) != *(int *)(param_1 + 0x2c)) &&
       (iVar3 = FUN_00002f3c(param_1), iVar3 != 0)) {
      return iVar3;
    }
    pbVar5 = (byte *)(param_1 + (uVar4 & 0x1ff) + 0x40);
    bVar6 = *pbVar5 & 0xf0 | bVar6 & 0xf;
  }
  else {
    param_1[uVar7 + 0x40] = param_1[uVar7 + 0x40] & 0xfU | (byte)(param_3 << 4);
    param_1[3] = '\x01';
    if (((uVar4 >> 9) + *(int *)(param_1 + 0x20) != *(int *)(param_1 + 0x2c)) &&
       (iVar3 = FUN_00002f3c(param_1), iVar3 != 0)) {
      return iVar3;
    }
    bVar6 = (byte)((param_3 & 0xfff) >> 4);
    pbVar5 = (byte *)(param_1 + (uVar4 & 0x1ff) + 0x40);
  }
  *pbVar5 = bVar6;
  param_1[3] = '\x01';
  return 0;
}



//==================== FUN_000036fc @ 000036fc ====================

uint FUN_000036fc(char *param_1,uint param_2)

{
  char cVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  
  if (param_2 < 2) {
    return 1;
  }
  if (*(uint *)(param_1 + 0x14) <= param_2) {
    return 1;
  }
  cVar1 = *param_1;
  if (cVar1 == '\x02') {
    if ((*(int *)(param_1 + 0x20) + (param_2 >> 8) == *(int *)(param_1 + 0x2c)) ||
       (iVar2 = FUN_00002f3c(), iVar2 == 0)) {
      iVar2 = (param_2 & 0xff) * 2;
      return (uint)CONCAT11(param_1[iVar2 + 0x41],param_1[iVar2 + 0x40]);
    }
  }
  else if (cVar1 == '\x03') {
    if ((*(int *)(param_1 + 0x20) + (param_2 >> 7) == *(int *)(param_1 + 0x2c)) ||
       (iVar2 = FUN_00002f3c(), iVar2 == 0)) {
      iVar2 = (param_2 & 0x7f) * 4;
      return CONCAT31(*(undefined3 *)(param_1 + iVar2 + 0x41),param_1[iVar2 + 0x40]) & 0xfffffff;
    }
  }
  else {
    if (cVar1 != '\x01') {
      return 1;
    }
    iVar2 = *(int *)(param_1 + 0x20);
    uVar4 = param_2 + (param_2 >> 1);
    iVar3 = *(int *)(param_1 + 0x2c);
    if (iVar2 + (uVar4 >> 9) != iVar3) {
      iVar2 = FUN_00002f3c();
      if (iVar2 != 0) {
        return 0xffffffff;
      }
      iVar2 = *(int *)(param_1 + 0x20);
      iVar3 = *(int *)(param_1 + 0x2c);
    }
    cVar1 = param_1[(uVar4 & 0x1ff) + 0x40];
    if ((iVar2 + (uVar4 + 1 >> 9) == iVar3) || (iVar2 = FUN_00002f3c(param_1), iVar2 == 0)) {
      if ((param_2 & 1) == 0) {
        return CONCAT11(param_1[(uVar4 + 1 & 0x1ff) + 0x40],cVar1) & 0xfff;
      }
      return (uint)(ushort)(CONCAT11(param_1[(uVar4 + 1 & 0x1ff) + 0x40],cVar1) >> 4);
    }
  }
  return 0xffffffff;
}



//==================== FUN_00003868 @ 00003868 ====================

uint FUN_00003868(int *param_1,uint param_2)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  
  iVar5 = *param_1;
  if (param_2 == 0) {
    uVar4 = *(uint *)(iVar5 + 0xc);
    if (uVar4 == 0) {
      uVar4 = 1;
      iVar1 = *(int *)(iVar5 + 0x10);
    }
    else {
      iVar1 = *(int *)(iVar5 + 0x10);
      if (*(uint *)(iVar5 + 0x14) <= uVar4) {
        uVar4 = 1;
      }
    }
    if (iVar1 == 0) {
      return 0;
    }
    uVar2 = *(uint *)(iVar5 + 0x14);
  }
  else {
    uVar3 = FUN_000036fc(iVar5);
    if (uVar3 < 2) {
      return 1;
    }
    if (uVar3 == 0xffffffff) {
      return 0xffffffff;
    }
    if (uVar3 < *(uint *)(iVar5 + 0x14)) {
      return uVar3;
    }
    if (*(int *)(iVar5 + 0x10) == 0) {
      return 0;
    }
    uVar3 = param_2 + 1;
    if (*(uint *)(iVar5 + 0x14) <= uVar3) {
      uVar3 = 2;
    }
    uVar4 = FUN_000036fc(*param_1,uVar3);
    if (uVar4 == 0xffffffff || uVar4 == 1) {
      return uVar4;
    }
    if (uVar4 == 0) {
      if (uVar3 != 0) goto LAB_00003908;
      uVar2 = *(uint *)(iVar5 + 0x14);
      uVar4 = param_2;
    }
    else {
      uVar4 = *(uint *)(iVar5 + 0xc);
      uVar2 = *(uint *)(iVar5 + 0x14);
      if (uVar4 < 2) {
        uVar4 = param_2;
      }
      else if (uVar2 <= uVar4) {
        uVar4 = param_2;
      }
    }
  }
  uVar3 = uVar4;
  while( true ) {
    uVar3 = uVar3 + 1;
    if ((uVar2 <= uVar3) && (uVar3 = 2, uVar4 < 2)) {
      return 0;
    }
    uVar2 = FUN_000036fc(*param_1,uVar3);
    if (uVar2 == 0) break;
    if (uVar2 == 0xffffffff || uVar2 == 1) {
      return uVar2;
    }
    if (uVar3 == uVar4) {
      return 0;
    }
    uVar2 = *(uint *)(iVar5 + 0x14);
  }
LAB_00003908:
  if ((1 < uVar3) && (uVar3 < *(uint *)(iVar5 + 0x14))) {
    iVar1 = FUN_0000351c(iVar5,uVar3,0xffffffff);
    if (iVar1 == 0 && param_2 != 0) {
      if (param_2 < 2) {
        return 1;
      }
      if (*(uint *)(iVar5 + 0x14) <= param_2) {
        return 1;
      }
      iVar1 = FUN_0000351c(iVar5,param_2,uVar3);
    }
    if (iVar1 == 0) {
      *(uint *)(iVar5 + 0xc) = uVar3;
      if (*(uint *)(iVar5 + 0x10) <= *(int *)(iVar5 + 0x14) - 2U) {
        *(uint *)(iVar5 + 0x10) = *(uint *)(iVar5 + 0x10) - 1;
      }
      *(byte *)(iVar5 + 4) = *(byte *)(iVar5 + 4) | 1;
      return uVar3;
    }
    if (iVar1 == 1) {
      return 0xffffffff;
    }
  }
  return 1;
}



//==================== FUN_00003a48 @ 00003a48 ====================

int FUN_00003a48(int *param_1,uint param_2,uint param_3)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  bool bVar4;
  bool bVar5;
  
  if (param_2 < 2) {
    return 2;
  }
  iVar3 = *param_1;
  uVar2 = *(uint *)(iVar3 + 0x14);
  if (param_2 < uVar2) {
    if (param_3 == 0) {
      iVar1 = iVar3;
      uVar2 = param_2;
      goto LAB_00003ae0;
    }
    bVar5 = param_3 <= uVar2;
    bVar4 = uVar2 == param_3;
    if (bVar5 && !bVar4) {
      bVar5 = param_3 != 0;
      bVar4 = param_3 == 1;
    }
    if (bVar5 && !bVar4) {
      iVar1 = FUN_0000351c(iVar3,param_3,0xffffffff);
      if (iVar1 != 0) {
        return iVar1;
      }
      do {
        iVar1 = *param_1;
        uVar2 = param_2;
LAB_00003ae0:
        param_2 = FUN_000036fc(iVar1,uVar2);
        if (param_2 == 0) {
          return 0;
        }
        if (param_2 == 1) {
          return 2;
        }
        if (param_2 == 0xffffffff) {
          return 1;
        }
        if (uVar2 < 2) {
          return 2;
        }
        if (*(uint *)(iVar3 + 0x14) <= uVar2) {
          return 2;
        }
        iVar1 = FUN_0000351c(iVar3,uVar2,0);
        if (iVar1 != 0) {
          return iVar1;
        }
        if (*(uint *)(iVar3 + 0x10) < *(uint *)(iVar3 + 0x14) - 2) {
          *(uint *)(iVar3 + 0x10) = *(uint *)(iVar3 + 0x10) + 1;
          *(byte *)(iVar3 + 4) = *(byte *)(iVar3 + 4) | 1;
        }
      } while (param_2 < *(uint *)(iVar3 + 0x14));
      return 0;
    }
  }
  return 2;
}



//==================== FUN_00003b44 @ 00003b44 ====================

undefined4 FUN_00003b44(int *param_1,int param_2)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  
  uVar4 = param_1[4] + 0x20;
  if (0x1fffff < uVar4) {
LAB_00003c54:
    param_1[6] = 0;
    return 4;
  }
  if (param_1[6] == 0) {
    return 4;
  }
  iVar5 = *param_1;
  if ((uVar4 & 0x1ff) == 0) {
    param_1[6] = param_1[6] + 1;
    if (param_1[5] == 0) {
      if ((uint)*(ushort *)(iVar5 + 8) <= uVar4 >> 5) goto LAB_00003c54;
    }
    else if ((*(ushort *)(iVar5 + 10) - 1 & uVar4 >> 9) == 0) {
      uVar1 = FUN_000036fc(iVar5);
      if (uVar1 < 2) {
        return 2;
      }
      if (uVar1 == 0xffffffff) {
        return 1;
      }
      uVar3 = *(uint *)(iVar5 + 0x14);
      if (uVar3 <= uVar1) {
        if (param_2 == 0) goto LAB_00003c54;
        uVar1 = FUN_00003868(param_1,param_1[5]);
        if (uVar1 == 0) {
          return 7;
        }
        if (uVar1 == 1) {
          return 2;
        }
        if (uVar1 == 0xffffffff) {
          return 1;
        }
        iVar2 = FUN_00002d5c(iVar5,uVar1);
        if (iVar2 != 0) {
          return 1;
        }
        uVar3 = *(uint *)(iVar5 + 0x14);
      }
      param_1[5] = uVar1;
      if (uVar1 - 2 < uVar3 - 2) {
        iVar2 = (uVar1 - 2) * (uint)*(ushort *)(iVar5 + 10) + *(int *)(iVar5 + 0x28);
      }
      else {
        iVar2 = 0;
      }
      param_1[6] = iVar2;
    }
  }
  param_1[4] = uVar4;
  param_1[7] = iVar5 + 0x40 + (uVar4 & 0x1ff);
  return 0;
}



//==================== FUN_00003c84 @ 00003c84 ====================

int FUN_00003c84(int *param_1)

{
  char cVar1;
  byte bVar2;
  byte bVar3;
  int iVar4;
  int iVar5;
  
  iVar4 = 4;
  iVar5 = *param_1;
  while( true ) {
    if (param_1[6] == 0) {
      if (iVar4 == 0) {
        return 0;
      }
      goto LAB_00003d30;
    }
    if ((*(int *)(iVar5 + 0x2c) != param_1[6]) && (iVar4 = FUN_00002f3c(iVar5), iVar4 != 0))
    goto LAB_00003d30;
    cVar1 = *(char *)param_1[7];
    if (cVar1 == '\0') break;
    bVar2 = ((char *)param_1[7])[0xb];
    bVar3 = bVar2 & 0x3f;
    *(byte *)((int)param_1 + 6) = bVar3;
    if ((bVar3 != 0xf && (cVar1 != -0x1b && cVar1 != '.')) && ((bVar2 & 0x1f) != 8)) {
      return 0;
    }
    iVar4 = FUN_00003b44(param_1);
    if (iVar4 != 0) {
LAB_00003d30:
      param_1[6] = 0;
      return iVar4;
    }
  }
  iVar4 = 4;
  goto LAB_00003d30;
}



//==================== FUN_00003d50 @ 00003d50 ====================

undefined4 FUN_00003d50(undefined4 *param_1)

{
  byte *pbVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  byte *pbVar5;
  
  uVar2 = param_1[2];
  param_1[4] = 0;
  pbVar5 = (byte *)*param_1;
  uVar3 = uVar2;
  if ((uVar2 == 0) && ((*pbVar5 < 3 || (uVar3 = *(uint *)(pbVar5 + 0x24), uVar3 == 0)))) {
    if (*(short *)(pbVar5 + 8) == 0) {
      return 2;
    }
    iVar4 = *(int *)(pbVar5 + 0x24);
    param_1[6] = iVar4;
    uVar3 = uVar2;
  }
  else {
    if (*(ushort *)(pbVar5 + 10) == 0) {
      pbVar1 = pbVar5;
      while( true ) {
        uVar3 = FUN_000036fc(pbVar1,uVar3);
        if (uVar3 == 0xffffffff) {
          return 1;
        }
        if (uVar3 < 2) break;
        if (*(uint *)(pbVar5 + 0x14) <= uVar3) {
          return 2;
        }
        pbVar1 = (byte *)*param_1;
      }
      return 2;
    }
    if (*(int *)(pbVar5 + 0x14) - 2U <= uVar3 - 2) {
      param_1[5] = uVar3;
      param_1[6] = 0;
      return 2;
    }
    iVar4 = (uVar3 - 2) * (uint)*(ushort *)(pbVar5 + 10) + *(int *)(pbVar5 + 0x28);
    param_1[6] = iVar4;
  }
  param_1[5] = uVar3;
  if (iVar4 == 0) {
    return 2;
  }
  param_1[7] = pbVar5 + 0x40;
  return 0;
}



//==================== FUN_00003e38 @ 00003e38 ====================

int FUN_00003e38(int *param_1)

{
  int iVar1;
  char *pcVar2;
  char *pcVar4;
  char *pcVar3;
  int *piVar5;
  int iVar6;
  
  iVar6 = *param_1;
  iVar1 = FUN_00003d50();
  if (iVar1 == 0) {
    do {
      if ((param_1[6] != *(int *)(iVar6 + 0x2c)) &&
         (iVar1 = FUN_00002f3c(iVar6,param_1[6]), iVar1 != 0)) break;
      pcVar2 = (char *)param_1[7];
      if (*pcVar2 == '\0' || *pcVar2 == -0x1b) {
        if (param_1[6] != *(int *)(iVar6 + 0x2c)) {
          iVar1 = FUN_00002f3c(iVar6);
          if (iVar1 != 0) {
            return iVar1;
          }
          pcVar2 = (char *)param_1[7];
        }
        pcVar4 = pcVar2;
        do {
          pcVar3 = pcVar4 + 1;
          *pcVar4 = '\0';
          pcVar4 = pcVar3;
        } while (pcVar2 + 0x20 != pcVar3);
        piVar5 = (int *)param_1[7];
        *piVar5 = param_1[8];
        piVar5[1] = param_1[9];
        *(char *)(piVar5 + 2) = (char)param_1[10];
        *(undefined1 *)((int)piVar5 + 9) = *(undefined1 *)((int)param_1 + 0x29);
        *(undefined1 *)((int)piVar5 + 10) = *(undefined1 *)((int)param_1 + 0x2a);
        *(undefined1 *)(iVar6 + 3) = 1;
        return 0;
      }
      iVar1 = FUN_00003b44(param_1,1);
    } while (iVar1 == 0);
  }
  if (iVar1 == 4) {
    iVar1 = 7;
  }
  return iVar1;
}



//==================== FUN_00003f2c @ 00003f2c ====================

int FUN_00003f2c(int *param_1,byte *param_2)

{
  byte bVar1;
  uint uVar2;
  int iVar3;
  uint uVar4;
  byte *pbVar5;
  uint uVar6;
  undefined1 uVar7;
  byte bVar8;
  byte *pbVar9;
  byte *pbVar10;
  byte *pbVar11;
  byte *pbVar12;
  int iVar13;
  byte *pbVar14;
  char *pcVar15;
  uint uVar16;
  bool bVar17;
  
  pcVar15 = (char *)*param_1;
  if (*param_2 == 0x2f || *param_2 == 0x5c) {
    do {
      param_2 = param_2 + 1;
    } while (*param_2 == 0x5c || *param_2 == 0x2f);
  }
  param_1[2] = 0;
  if (*param_2 < 0x20) {
    *(undefined1 *)((int)param_1 + 0x2b) = 0x80;
    iVar13 = FUN_00003d50(param_1);
    return iVar13;
  }
  pbVar14 = (byte *)(param_1 + 8);
LAB_00003f78:
  pbVar12 = pbVar14;
  do {
    pbVar9 = pbVar12 + 1;
    *pbVar12 = 0x20;
    pbVar12 = pbVar9;
  } while (pbVar9 != (byte *)((int)param_1 + 0x2b));
  uVar4 = 0;
  uVar16 = 8;
  bVar8 = *param_2;
  iVar13 = 0;
  iVar3 = 1;
  while (uVar2 = (uint)bVar8, 0x20 < uVar2) {
    if (uVar2 == 0x5c || uVar2 == 0x2f) {
      pbVar12 = param_2 + iVar3;
      if (param_2[iVar3] == 0x2f || param_2[iVar3] == 0x5c) {
        pbVar9 = param_2 + iVar13 + 2;
        do {
          pbVar12 = pbVar9;
          pbVar9 = pbVar12 + 1;
        } while (*pbVar12 == 0x5c || *pbVar12 == 0x2f);
      }
      goto LAB_00003ffc;
    }
    bVar17 = 0x2d < uVar2;
    if (uVar2 != 0x2e) {
      bVar17 = uVar4 <= uVar16;
    }
    if (bVar17 && (uVar2 != 0x2e && uVar16 != uVar4)) {
      pbVar12 = DAT_00004250;
      if (uVar2 < 0x81) {
        if (uVar2 == 0x22) {
          return 6;
        }
        uVar6 = 0x2a;
      }
      else {
        if ((uVar2 < 0xa0) || ((uVar2 + 0x20 & 0xff) < 0x1d)) {
          bVar1 = param_2[iVar3];
          iVar3 = iVar13 + 2;
          if (bVar1 < 0x40) {
            return 6;
          }
          if ((0x7e < bVar1) && (0x7c < (bVar1 ^ 0x80))) {
            return 6;
          }
          if (uVar16 - 1 <= uVar4) {
            return 6;
          }
          pbVar14[uVar4] = bVar8;
          pbVar14[uVar4 + 1] = bVar1;
          uVar4 = uVar4 + 2;
          goto LAB_00003fe4;
        }
        uVar6 = 0x2a;
      }
      do {
        if (uVar2 == uVar6) {
          return 6;
        }
        uVar6 = (uint)pbVar12[1];
        pbVar12 = pbVar12 + 1;
      } while (uVar6 != 0);
      if (uVar2 - 0x61 < 0x1a) {
        bVar8 = bVar8 - 0x20;
      }
      pbVar14[uVar4] = bVar8;
      uVar4 = uVar4 + 1;
    }
    else {
      if (uVar16 == 0xb || uVar2 != 0x2e) {
        return 6;
      }
      uVar4 = 8;
      uVar16 = 0xb;
    }
LAB_00003fe4:
    bVar8 = param_2[iVar3];
    iVar13 = iVar3;
    iVar3 = iVar3 + 1;
  }
  pbVar12 = param_2 + iVar3;
LAB_00003ffc:
  if (uVar4 == 0) {
    return 6;
  }
  iVar13 = *param_1;
  if ((char)param_1[8] == -0x1b) {
    *(undefined1 *)(param_1 + 8) = 5;
  }
  if (uVar2 < 0x21) {
    uVar7 = 4;
  }
  else {
    uVar7 = 0;
  }
  *(undefined1 *)((int)param_1 + 0x2b) = uVar7;
  iVar3 = FUN_00003d50(param_1);
  if (iVar3 != 0) {
LAB_000040d4:
    if (iVar3 != 4) {
      return iVar3;
    }
LAB_000040dc:
    if ((*(byte *)((int)param_1 + 0x2b) & 4) == 0) {
      return 5;
    }
    return 4;
  }
  do {
    if ((param_1[6] != *(int *)(iVar13 + 0x2c)) && (iVar3 = FUN_00002f3c(iVar13), iVar3 != 0))
    goto LAB_000040d4;
    pbVar9 = (byte *)param_1[7];
    if (*pbVar9 == 0) goto LAB_000040dc;
    bVar8 = pbVar9[0xb];
    *(byte *)((int)param_1 + 6) = bVar8 & 0x3f;
    if ((pbVar9[0xb] & 8) == 0) {
      iVar3 = (uint)*pbVar9 - (uint)*(byte *)(param_1 + 8);
      pbVar5 = pbVar14;
      pbVar10 = pbVar9 + 1;
      do {
        if (iVar3 != 0) goto LAB_000040bc;
        pbVar11 = pbVar10 + 1;
        pbVar5 = pbVar5 + 1;
        iVar3 = (uint)*pbVar10 - (uint)*pbVar5;
        pbVar10 = pbVar11;
      } while (pbVar11 != pbVar9 + 0xb);
      if (iVar3 == 0) break;
    }
LAB_000040bc:
    iVar3 = FUN_00003b44(param_1,0);
    if (iVar3 != 0) goto LAB_000040d4;
  } while( true );
  if ((*(byte *)((int)param_1 + 0x2b) & 4) != 0) {
    return 0;
  }
  if ((bVar8 & 0x10) == 0) {
    return 5;
  }
  uVar4 = (uint)*(ushort *)(pcVar15 + (param_1[4] & 0x1ffU) + 0x5a);
  if (*pcVar15 == '\x03') {
    uVar4 = CONCAT22(*(undefined2 *)(pcVar15 + (param_1[4] & 0x1ffU) + 0x54),
                     *(ushort *)(pcVar15 + (param_1[4] & 0x1ffU) + 0x5a));
  }
  param_1[2] = uVar4;
  param_2 = pbVar12;
  goto LAB_00003f78;
}



//==================== FUN_000042d4 @ 000042d4 ====================

int FUN_000042d4(undefined4 *param_1,undefined4 param_2,byte param_3)

{
  byte bVar1;
  undefined2 uVar2;
  ushort uVar3;
  int iVar4;
  undefined4 uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  undefined4 *puVar10;
  undefined4 *puVar9;
  byte bVar11;
  int iVar12;
  bool bVar13;
  undefined4 local_54;
  byte *local_50;
  byte *local_4c;
  byte local_46;
  int local_30;
  char local_21;
  
  local_54 = param_2;
  FUN_000027e0(&local_4c,0,0x2c);
  if (param_1 == (undefined4 *)0x0) {
    return 9;
  }
  bVar11 = param_3 & 0x3f;
  iVar4 = FUN_000030bc(&local_54,&local_50,bVar11);
  if (iVar4 != 0) goto LAB_00004320;
  local_4c = local_50;
  iVar4 = FUN_00003f2c(&local_4c,local_54);
  if (iVar4 == 0) {
    if (local_21 < '\0') {
      iVar4 = 6;
      goto LAB_00004320;
    }
    if ((param_3 & 0x1c) == 0) {
      if ((local_46 & 0x10) != 0) {
        iVar4 = 4;
        goto LAB_00004320;
      }
      if (((param_3 & 2) == 0) || ((local_46 & 1) == 0)) {
        iVar12 = *(int *)(local_50 + 0x2c);
        if ((param_3 & 8) == 0) goto LAB_000044dc;
        goto LAB_000044d8;
      }
    }
    else if ((local_46 & 0x11) == 0) {
      if ((param_3 & 4) != 0) {
        iVar4 = 8;
        goto LAB_00004320;
      }
      if ((param_3 & 8) != 0) goto LAB_000043e8;
      iVar12 = *(int *)(local_50 + 0x2c);
      goto LAB_000044dc;
    }
    iVar4 = 7;
  }
  else {
    if (((param_3 & 0x1c) == 0) || (iVar4 != 4)) goto LAB_00004394;
    bVar11 = bVar11 | 8;
    iVar4 = FUN_00003e38(&local_4c);
    if (iVar4 != 0) goto LAB_00004394;
LAB_000043e8:
    iVar4 = local_30;
    uVar6 = (uint)*local_50;
    bVar1 = *(byte *)(local_30 + 0x1b);
    bVar13 = uVar6 == 3;
    if (bVar13) {
      uVar6 = (uint)*(byte *)(local_30 + 0x15);
    }
    uVar8 = (uint)*(ushort *)(local_30 + 0x1a);
    if (bVar13) {
      bVar1 = *(byte *)(local_30 + 0x14);
    }
    if (bVar13) {
      uVar8 = uVar8 | ((uint)bVar1 | uVar6 << 8) << 0x10;
    }
    uVar5 = FUN_00006760();
    *(char *)(iVar4 + 0xe) = (char)uVar5;
    *(char *)(iVar4 + 0xf) = (char)((uint)uVar5 >> 8);
    *(char *)(iVar4 + 0x10) = (char)((uint)uVar5 >> 0x10);
    *(char *)(iVar4 + 0x11) = (char)((uint)uVar5 >> 0x18);
    *(undefined1 *)(local_30 + 0xb) = 0x20;
    *(undefined1 *)(local_30 + 0x1a) = 0;
    *(undefined1 *)(local_30 + 0x1b) = 0;
    if (*local_50 == 3) {
      *(undefined1 *)(local_30 + 0x14) = 0;
      *(undefined1 *)(local_30 + 0x15) = 0;
    }
    *(undefined1 *)(local_30 + 0x1c) = 0;
    *(undefined1 *)(local_30 + 0x1d) = 0;
    *(undefined1 *)(local_30 + 0x1e) = 0;
    *(undefined1 *)(local_30 + 0x1f) = 0;
    local_50[3] = 1;
    if (uVar8 == 0) {
LAB_000044d0:
      iVar12 = *(int *)(local_50 + 0x2c);
    }
    else {
      iVar12 = *(int *)(local_50 + 0x2c);
      iVar4 = FUN_00003a48(&local_4c,uVar8);
      if (iVar4 != 0) {
LAB_00004394:
        *param_1 = 0;
        return iVar4;
      }
      if (iVar12 != *(int *)(local_50 + 0x2c)) {
        iVar4 = FUN_00002f3c(local_50,iVar12);
        *(uint *)(local_50 + 0xc) = uVar8 - 1;
        if (iVar4 != 0) goto LAB_00004394;
        goto LAB_000044d0;
      }
      *(uint *)(local_50 + 0xc) = uVar8 - 1;
    }
LAB_000044d8:
    bVar11 = bVar11 | 0x40;
LAB_000044dc:
    param_1[8] = iVar12;
    param_1[9] = local_30;
    bVar1 = *(byte *)(local_30 + 0x1b);
    uVar6 = (uint)*local_50;
    bVar13 = uVar6 == 3;
    if (bVar13) {
      uVar6 = (uint)*(byte *)(local_30 + 0x15);
    }
    uVar8 = (uint)*(ushort *)(local_30 + 0x1a);
    if (bVar13) {
      bVar1 = *(byte *)(local_30 + 0x14);
    }
    if (bVar13) {
      uVar8 = uVar8 | ((uint)bVar1 | uVar6 << 8) << 0x10;
    }
    param_1[2] = uVar8;
    uVar5 = *(undefined4 *)(local_30 + 0x1c);
    *param_1 = local_50;
    uVar2 = *(undefined2 *)(local_50 + 6);
    *(byte *)(param_1 + 4) = bVar11;
    *(undefined2 *)(param_1 + 1) = uVar2;
    param_1[3] = uVar5;
    *(undefined1 *)((int)param_1 + 0x11) = 0;
    param_1[7] = 0;
    param_1[5] = 0;
    puVar10 = param_1 + 0x10;
    do {
      puVar9 = (undefined4 *)((int)puVar10 + 1);
      *(undefined1 *)puVar10 = 0;
      puVar10 = puVar9;
    } while (puVar9 != param_1 + 0x90);
    if ((bVar11 & 0x20) == 0) {
      return 0;
    }
    uVar6 = param_1[3];
    if (uVar6 == 0) {
      return 0;
    }
    uVar3 = *(ushort *)(local_50 + 10);
    uVar7 = param_1[2];
    param_1[5] = uVar6;
    uVar8 = (uint)uVar3 * 0x200;
    if (uVar8 < uVar6) {
      do {
        uVar6 = uVar6 + (uint)uVar3 * -0x200;
        uVar7 = FUN_000036fc(*param_1);
        if (uVar7 < 2) {
          param_1[6] = uVar7;
          goto LAB_000045e8;
        }
        bVar13 = 0xfffffffe < uVar7;
        if (uVar7 != 0xffffffff) {
          bVar13 = uVar8 <= uVar6;
        }
      } while (bVar13 && (uVar7 != 0xffffffff && uVar6 != uVar8));
      param_1[6] = uVar7;
      if (uVar7 == 0xffffffff) {
        iVar4 = 1;
        goto LAB_00004320;
      }
    }
    else {
      param_1[6] = uVar7;
    }
    if ((uVar6 & 0x1ff) == 0) {
      return 0;
    }
    if ((uVar7 - 2 < *(int *)(local_50 + 0x14) - 2U) &&
       (iVar4 = (uint)*(ushort *)(local_50 + 10) * (uVar7 - 2) + *(int *)(local_50 + 0x28),
       iVar4 != 0)) {
      iVar4 = iVar4 + (uVar6 >> 9);
      param_1[7] = iVar4;
      iVar4 = FUN_0000667c(local_50[1],param_1 + 0x10,iVar4,1);
      if (iVar4 == 0) {
        return 0;
      }
      iVar4 = 1;
    }
    else {
LAB_000045e8:
      iVar4 = 2;
    }
  }
LAB_00004320:
  *param_1 = 0;
  return iVar4;
}



//==================== FUN_00004670 @ 00004670 ====================

uint FUN_00004670(undefined4 *param_1,undefined4 *param_2,uint param_3,int *param_4)

{
  undefined1 uVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  undefined4 *puVar6;
  undefined4 *puVar7;
  undefined1 *puVar8;
  uint uVar9;
  undefined4 *puVar10;
  uint uVar11;
  uint uVar12;
  undefined4 *puVar13;
  undefined4 *puVar14;
  uint uVar15;
  int local_2c [2];
  
  *param_4 = 0;
  uVar2 = FUN_00002bbc(param_1,local_2c);
  if ((uVar2 == 0) && (uVar2 = (uint)*(byte *)((int)param_1 + 0x11), uVar2 == 0)) {
    if ((*(byte *)(param_1 + 4) & 1) == 0) {
      uVar2 = 7;
    }
    else {
      uVar15 = param_1[5];
      uVar9 = param_1[3] - uVar15;
      if (param_3 <= param_1[3] - uVar15) {
        uVar9 = param_3;
      }
      if (uVar9 != 0) {
        puVar10 = param_1 + 0x10;
        do {
          uVar12 = uVar15 & 0x1ff;
          if (uVar12 == 0) {
            uVar12 = *(ushort *)(local_2c[0] + 10) - 1 & uVar15 >> 9;
            if (uVar12 == 0) {
              if (uVar15 == 0) {
                uVar15 = param_1[2];
              }
              else {
                uVar15 = FUN_000036fc(*param_1,param_1[6]);
              }
              if (uVar15 < 2) goto LAB_00004904;
              if (uVar15 == 0xffffffff) goto LAB_00004758;
              param_1[6] = uVar15;
            }
            else {
              uVar15 = param_1[6];
            }
            if (*(int *)(local_2c[0] + 0x14) - 2U <= uVar15 - 2) {
LAB_00004904:
              *(undefined1 *)((int)param_1 + 0x11) = 2;
              return 2;
            }
            uVar5 = (uint)*(ushort *)(local_2c[0] + 10);
            iVar3 = uVar5 * (uVar15 - 2) + *(int *)(local_2c[0] + 0x28);
            if (iVar3 == 0) goto LAB_00004904;
            uVar11 = uVar9 >> 9;
            iVar3 = uVar12 + iVar3;
            if (uVar11 == 0) {
              if (param_1[7] != iVar3) {
                uVar1 = *(undefined1 *)(local_2c[0] + 1);
                if (*(char *)(param_1 + 4) < '\0') {
                  iVar4 = FUN_0000676c(uVar1,puVar10,param_1[7],1);
                  if (iVar4 != 0) goto LAB_00004758;
                  *(byte *)(param_1 + 4) = *(byte *)(param_1 + 4) & 0x7f;
                  uVar1 = *(undefined1 *)(local_2c[0] + 1);
                }
                iVar4 = FUN_0000667c(uVar1,puVar10,iVar3,1);
                if (iVar4 != 0) goto LAB_00004758;
              }
              uVar15 = param_1[5];
              param_1[7] = iVar3;
              uVar12 = uVar15 & 0x1ff;
              goto LAB_000047ac;
            }
            if (uVar5 < uVar12 + uVar11) {
              uVar11 = uVar5 - uVar12;
            }
            iVar4 = FUN_0000667c(*(undefined1 *)(local_2c[0] + 1),param_2,iVar3,uVar11);
            if (iVar4 != 0) {
LAB_00004758:
              *(undefined1 *)((int)param_1 + 0x11) = 1;
              return 1;
            }
            if ((*(char *)(param_1 + 4) < '\0') &&
               (iVar4 = param_1[7], (uint)(iVar4 - iVar3) < uVar11)) {
              puVar6 = param_1 + 0xf;
              puVar13 = param_2 + (iVar4 - iVar3) * 0x80;
              do {
                puVar6 = puVar6 + 1;
                puVar14 = puVar13 + 1;
                *puVar13 = *puVar6;
                puVar13 = puVar14;
              } while (puVar14 != param_2 + (iVar4 - iVar3) * 0x80 + 0x80);
            }
            uVar11 = uVar11 << 9;
            uVar15 = param_1[5];
          }
          else {
LAB_000047ac:
            puVar13 = (undefined4 *)((int)puVar10 + uVar12);
            uVar11 = 0x200 - uVar12;
            if (uVar9 <= 0x200 - uVar12) {
              uVar11 = uVar9;
            }
            if (uVar11 < 4) {
              uVar12 = uVar11;
              puVar6 = param_2;
            }
            else {
              uVar12 = uVar11 - 4 >> 2;
              puVar6 = puVar13;
              puVar14 = param_2;
              do {
                puVar7 = puVar6 + 1;
                *puVar14 = *puVar6;
                puVar6 = puVar7;
                puVar14 = puVar14 + 1;
              } while (puVar13 + uVar12 + 1 != puVar7);
              iVar3 = uVar12 + 1;
              puVar13 = puVar13 + iVar3;
              uVar12 = uVar11 & 3;
              puVar6 = param_2 + iVar3;
              if ((uVar11 & 3) == 0) goto LAB_00004804;
            }
            puVar8 = (undefined1 *)((int)puVar6 + -1);
            puVar6 = puVar13;
            do {
              puVar14 = (undefined4 *)((int)puVar6 + 1);
              puVar8 = puVar8 + 1;
              *puVar8 = *(undefined1 *)puVar6;
              puVar6 = puVar14;
            } while (puVar14 != (undefined4 *)((int)puVar13 + uVar12));
            uVar15 = param_1[5];
          }
LAB_00004804:
          uVar9 = uVar9 - uVar11;
          uVar15 = uVar15 + uVar11;
          param_2 = (undefined4 *)((int)param_2 + uVar11);
          *param_4 = *param_4 + uVar11;
          param_1[5] = uVar15;
        } while (uVar9 != 0);
      }
    }
  }
  return uVar2;
}



//==================== FUN_00004910 @ 00004910 ====================

uint FUN_00004910(int param_1,undefined4 *param_2,uint param_3,int *param_4)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  undefined4 *puVar6;
  byte bVar7;
  int iVar8;
  undefined4 *puVar9;
  undefined4 *puVar10;
  uint uVar11;
  uint uVar12;
  undefined4 *puVar13;
  undefined1 *puVar14;
  int local_2c [2];
  
  *param_4 = 0;
  uVar1 = FUN_00002bbc(param_1,local_2c);
  if ((uVar1 == 0) && (uVar1 = (uint)*(byte *)(param_1 + 0x11), uVar1 == 0)) {
    bVar7 = *(byte *)(param_1 + 0x10);
    if ((bVar7 & 2) == 0) {
      uVar1 = 7;
    }
    else {
      uVar4 = *(uint *)(param_1 + 0x14);
      if (CARRY4(uVar4,param_3)) {
        param_3 = ~uVar4;
      }
      if (param_3 != 0) {
        iVar8 = param_1 + 0x40;
        do {
          uVar12 = uVar4 & 0x1ff;
          if (uVar12 == 0) {
            uVar12 = *(ushort *)(local_2c[0] + 10) - 1 & uVar4 >> 9;
            if (uVar12 == 0) {
              if (uVar4 == 0) {
                iVar2 = *(int *)(param_1 + 8);
                if (iVar2 == 0) {
                  iVar2 = FUN_00003868(param_1,0);
                  goto LAB_00004ba0;
                }
              }
              else {
                iVar2 = FUN_00003868(param_1,*(undefined4 *)(param_1 + 0x18));
LAB_00004ba0:
                if (iVar2 == 0) break;
              }
              if (iVar2 == 1) goto LAB_00004c10;
              if (iVar2 == -1) goto LAB_00004a3c;
              *(int *)(param_1 + 0x18) = iVar2;
              if (*(int *)(param_1 + 8) == 0) {
                *(int *)(param_1 + 8) = iVar2;
              }
            }
            if (*(char *)(param_1 + 0x10) < '\0') {
              iVar2 = FUN_0000676c(*(undefined1 *)(local_2c[0] + 1),iVar8,
                                   *(undefined4 *)(param_1 + 0x1c),1);
              if (iVar2 != 0) goto LAB_00004a3c;
              *(byte *)(param_1 + 0x10) = *(byte *)(param_1 + 0x10) & 0x7f;
            }
            uVar4 = *(int *)(param_1 + 0x18) - 2;
            if (*(int *)(local_2c[0] + 0x14) - 2U <= uVar4) {
LAB_00004c10:
              *(undefined1 *)(param_1 + 0x11) = 2;
              return 2;
            }
            uVar5 = (uint)*(ushort *)(local_2c[0] + 10);
            iVar2 = uVar5 * uVar4 + *(int *)(local_2c[0] + 0x28);
            if (iVar2 == 0) goto LAB_00004c10;
            uVar11 = param_3 >> 9;
            iVar2 = uVar12 + iVar2;
            if (uVar11 == 0) {
              uVar4 = *(uint *)(param_1 + 0x14);
              if ((*(int *)(param_1 + 0x1c) != iVar2) && (uVar4 < *(uint *)(param_1 + 0xc))) {
                iVar3 = FUN_0000667c(*(undefined1 *)(local_2c[0] + 1),iVar8,iVar2,1);
                if (iVar3 != 0) goto LAB_00004a3c;
                uVar4 = *(uint *)(param_1 + 0x14);
              }
              uVar12 = uVar4 & 0x1ff;
              *(int *)(param_1 + 0x1c) = iVar2;
              goto LAB_00004a74;
            }
            if (uVar5 < uVar12 + uVar11) {
              uVar11 = uVar5 - uVar12;
            }
            iVar3 = FUN_0000676c(*(undefined1 *)(local_2c[0] + 1),param_2,iVar2,uVar11);
            if (iVar3 != 0) {
LAB_00004a3c:
              *(undefined1 *)(param_1 + 0x11) = 1;
              return 1;
            }
            uVar4 = *(int *)(param_1 + 0x1c) - iVar2;
            if (uVar4 < uVar11) {
              puVar6 = param_2 + uVar4 * 0x80 + -1;
              puVar13 = (undefined4 *)(param_1 + 0x40);
              do {
                puVar6 = puVar6 + 1;
                puVar10 = puVar13 + 1;
                *puVar13 = *puVar6;
                puVar13 = puVar10;
              } while (puVar10 != (undefined4 *)(param_1 + 0x240));
              *(byte *)(param_1 + 0x10) = *(byte *)(param_1 + 0x10) & 0x7f;
            }
            uVar11 = uVar11 << 9;
            uVar4 = *(uint *)(param_1 + 0x14);
          }
          else {
LAB_00004a74:
            puVar13 = (undefined4 *)(iVar8 + uVar12);
            uVar11 = 0x200 - uVar12;
            if (param_3 <= 0x200 - uVar12) {
              uVar11 = param_3;
            }
            if (uVar11 < 4) {
              uVar12 = uVar11;
              puVar6 = param_2;
LAB_00004b28:
              puVar14 = (undefined1 *)((int)puVar13 + -1);
              puVar13 = puVar6;
              do {
                puVar10 = (undefined4 *)((int)puVar13 + 1);
                puVar14 = puVar14 + 1;
                *puVar14 = *(undefined1 *)puVar13;
                puVar13 = puVar10;
              } while (puVar10 != (undefined4 *)((int)puVar6 + uVar12));
              uVar4 = *(uint *)(param_1 + 0x14);
            }
            else {
              uVar12 = uVar11 - 4 >> 2;
              puVar6 = param_2;
              puVar10 = puVar13;
              do {
                puVar9 = puVar6 + 1;
                *puVar10 = *puVar6;
                puVar6 = puVar9;
                puVar10 = puVar10 + 1;
              } while (param_2 + uVar12 + 1 != puVar9);
              iVar2 = uVar12 + 1;
              puVar13 = puVar13 + iVar2;
              uVar12 = uVar11 & 3;
              puVar6 = param_2 + iVar2;
              if ((uVar11 & 3) != 0) goto LAB_00004b28;
            }
            *(byte *)(param_1 + 0x10) =
                 ~((byte)~(byte)(((uint)*(byte *)(param_1 + 0x10) << 0x19) >> 0x18) >> 1);
          }
          uVar4 = uVar4 + uVar11;
          param_2 = (undefined4 *)((int)param_2 + uVar11);
          uVar12 = *(uint *)(param_1 + 0xc);
          if (*(uint *)(param_1 + 0xc) < uVar4) {
            uVar12 = uVar4;
          }
          param_3 = param_3 - uVar11;
          *param_4 = *param_4 + uVar11;
          *(uint *)(param_1 + 0xc) = uVar12;
          *(uint *)(param_1 + 0x14) = uVar4;
        } while (param_3 != 0);
        bVar7 = *(byte *)(param_1 + 0x10);
      }
      *(byte *)(param_1 + 0x10) = bVar7 | 0x40;
    }
  }
  return uVar1;
}



//==================== FUN_00004c1c @ 00004c1c ====================

void FUN_00004c1c(undefined4 *param_1)

{
  int iVar1;
  undefined4 uVar2;
  char *pcVar3;
  undefined4 uVar4;
  int local_14 [2];
  
  iVar1 = FUN_00002bbc(param_1,local_14);
  if ((iVar1 == 0) && ((*(byte *)(param_1 + 4) & 0x40) != 0)) {
    if ((*(byte *)(param_1 + 4) & 0x80) != 0) {
      iVar1 = FUN_0000676c(*(undefined1 *)(local_14[0] + 1),param_1 + 0x10,param_1[7],1);
      if (iVar1 != 0) {
        return;
      }
      *(byte *)(param_1 + 4) = *(byte *)(param_1 + 4) & 0x7f;
    }
    uVar2 = FUN_00006760(0);
    if ((param_1[8] != *(int *)(local_14[0] + 0x2c)) &&
       (iVar1 = FUN_00002f3c(local_14[0]), iVar1 != 0)) {
      return;
    }
    iVar1 = param_1[9];
    *(byte *)(iVar1 + 0xb) = *(byte *)(iVar1 + 0xb) | 0x20;
    uVar4 = param_1[2];
    pcVar3 = (char *)*param_1;
    *(char *)(iVar1 + 0x1a) = (char)uVar4;
    *(char *)(iVar1 + 0x1b) = (char)((uint)uVar4 >> 8);
    if (*pcVar3 == '\x03') {
      *(char *)(iVar1 + 0x14) = (char)((uint)uVar4 >> 0x10);
      *(char *)(iVar1 + 0x15) = (char)((uint)uVar4 >> 0x18);
    }
    uVar4 = param_1[3];
    *(char *)(iVar1 + 0x17) = (char)((uint)uVar2 >> 8);
    *(char *)(iVar1 + 0x16) = (char)uVar2;
    *(char *)(iVar1 + 0x1c) = (char)uVar4;
    *(char *)(iVar1 + 0x18) = (char)((uint)uVar2 >> 0x10);
    *(char *)(iVar1 + 0x1d) = (char)((uint)uVar4 >> 8);
    *(char *)(iVar1 + 0x1e) = (char)((uint)uVar4 >> 0x10);
    *(char *)(iVar1 + 0x1f) = (char)((uint)uVar4 >> 0x18);
    *(char *)(iVar1 + 0x19) = (char)((uint)uVar2 >> 0x18);
    *(undefined1 *)(iVar1 + 0x12) = 0;
    *(undefined1 *)(iVar1 + 0x13) = 0;
    *(undefined1 *)(local_14[0] + 3) = 1;
    FUN_00002e20(local_14[0]);
    *(byte *)(param_1 + 4) = *(byte *)(param_1 + 4) & 0xbf;
  }
  return;
}



//==================== FUN_00004d5c @ 00004d5c ====================

void FUN_00004d5c(undefined4 *param_1)

{
  int iVar1;
  undefined1 auStack_c [4];
  
  iVar1 = FUN_00004c1c();
  if ((iVar1 == 0) && (iVar1 = FUN_00002bbc(param_1,auStack_c), iVar1 == 0)) {
    *param_1 = 0;
  }
  return;
}



//==================== FUN_00004d90 @ 00004d90 ====================

uint FUN_00004d90(undefined4 *param_1,uint param_2)

{
  undefined1 uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  uint uVar7;
  uint uVar8;
  int local_24 [2];
  
  uVar2 = FUN_00002bbc(param_1,local_24);
  if (uVar2 != 0) {
    return uVar2;
  }
  if (*(byte *)((int)param_1 + 0x11) != 0) {
    return (uint)*(byte *)((int)param_1 + 0x11);
  }
  if (((uint)param_1[3] < param_2) && ((*(byte *)(param_1 + 4) & 2) == 0)) {
    param_2 = param_1[3];
  }
  iVar6 = param_1[5];
  param_1[5] = 0;
  if (param_2 == 0) {
    return 0;
  }
  uVar7 = (uint)*(ushort *)(local_24[0] + 10);
  uVar2 = uVar7 * 0x200;
  if (iVar6 == 0) {
LAB_00004eec:
    uVar3 = param_1[2];
    if (uVar3 != 0) {
      uVar8 = 0;
LAB_00004efc:
      param_1[6] = uVar3;
      goto LAB_00004f00;
    }
    uVar3 = FUN_00003868(param_1,0);
    if (uVar3 != 1) {
      if (uVar3 == 0xffffffff) goto LAB_00004fc0;
      param_1[2] = uVar3;
      uVar8 = param_1[5];
      goto LAB_00004efc;
    }
LAB_00004fd4:
    uVar2 = 2;
    *(undefined1 *)((int)param_1 + 0x11) = 2;
  }
  else {
    uVar8 = iVar6 - 1;
    uVar3 = FUN_00001a50(param_2 - 1,uVar2);
    uVar4 = FUN_00001a50(uVar8,uVar2);
    if (uVar3 < uVar4) goto LAB_00004eec;
    uVar3 = param_1[6];
    uVar8 = uVar7 * -0x200 & uVar8;
    param_1[5] = uVar8;
    param_2 = param_2 - uVar8;
LAB_00004f00:
    iVar6 = 0;
    if (uVar3 != 0) {
      while (uVar2 < param_2) {
        param_1[5] = uVar2 + uVar8;
        param_2 = param_2 + uVar7 * -0x200;
        if ((*(byte *)(param_1 + 4) & 2) == 0) {
          uVar3 = FUN_000036fc(*param_1);
        }
        else {
          uVar3 = FUN_00003868(param_1,uVar3);
          if (uVar3 == 0) {
            uVar8 = param_1[5];
            iVar6 = 0;
            goto LAB_00004e7c;
          }
        }
        if (uVar3 == 0xffffffff) goto LAB_00004fc0;
        if ((uVar3 < 2) || (*(uint *)(local_24[0] + 0x14) <= uVar3)) goto LAB_00004fd4;
        param_1[6] = uVar3;
        uVar8 = param_1[5];
      }
      uVar8 = uVar8 + param_2;
      param_1[5] = uVar8;
      if ((param_2 & 0x1ff) == 0) {
        iVar6 = 0;
      }
      else {
        if ((*(int *)(local_24[0] + 0x14) - 2U <= uVar3 - 2) ||
           (iVar6 = (uint)*(ushort *)(local_24[0] + 10) * (uVar3 - 2) + *(int *)(local_24[0] + 0x28)
           , iVar6 == 0)) goto LAB_00004fd4;
        iVar6 = iVar6 + (param_2 >> 9);
      }
    }
LAB_00004e7c:
    if ((uint)param_1[3] < uVar8) {
      param_1[3] = uVar8;
      *(byte *)(param_1 + 4) = *(byte *)(param_1 + 4) | 0x40;
    }
    if ((uVar8 & 0x1ff) == 0) {
      return 0;
    }
    if (param_1[7] == iVar6) {
      return 0;
    }
    uVar1 = *(undefined1 *)(local_24[0] + 1);
    if (*(char *)(param_1 + 4) < '\0') {
      iVar5 = FUN_0000676c(uVar1,param_1 + 0x10,param_1[7],1);
      if (iVar5 != 0) goto LAB_00004fc0;
      *(byte *)(param_1 + 4) = *(byte *)(param_1 + 4) & 0x7f;
      uVar1 = *(undefined1 *)(local_24[0] + 1);
    }
    iVar5 = FUN_0000667c(uVar1,param_1 + 0x10,iVar6,1);
    if (iVar5 != 0) {
LAB_00004fc0:
      *(undefined1 *)((int)param_1 + 0x11) = 1;
      return 1;
    }
    param_1[7] = iVar6;
    uVar2 = 0;
  }
  return uVar2;
}



//==================== FUN_00005008 @ 00005008 ====================

int FUN_00005008(undefined4 *param_1,undefined4 param_2)

{
  ushort uVar1;
  int iVar2;
  uint uVar3;
  undefined4 local_14 [2];
  char *local_c;
  
  if (param_1 == (undefined4 *)0x0) {
    return 9;
  }
  local_14[0] = param_2;
  iVar2 = FUN_000030bc(local_14,&local_c,0);
  if (iVar2 == 0) {
    *param_1 = local_c;
    iVar2 = FUN_00003f2c(param_1,local_14[0]);
    if (iVar2 == 0) {
      if (-1 < *(char *)((int)param_1 + 0x2b)) {
        if ((*(byte *)((int)param_1 + 6) & 0x10) == 0) goto LAB_000050e0;
        uVar1 = *(ushort *)(param_1[7] + 0x1a);
        uVar3 = (uint)uVar1;
        if (*local_c == '\x03') {
          uVar3 = CONCAT22(*(undefined2 *)(param_1[7] + 0x14),uVar1);
        }
        param_1[2] = uVar3;
      }
      *(undefined2 *)(param_1 + 1) = *(undefined2 *)(local_c + 6);
      iVar2 = FUN_00003d50(param_1);
      if (iVar2 == 4) {
LAB_000050e0:
        *param_1 = 0;
        return 5;
      }
      if (iVar2 == 0) {
        return 0;
      }
    }
    else if (iVar2 == 4) goto LAB_000050e0;
  }
  *param_1 = 0;
  return iVar2;
}



//==================== FUN_000050f0 @ 000050f0 ====================

void FUN_000050f0(undefined4 *param_1)

{
  int iVar1;
  undefined1 auStack_c [4];
  
  iVar1 = FUN_00002bbc(param_1,auStack_c);
  if (iVar1 == 0) {
    *param_1 = 0;
  }
  return;
}



//==================== FUN_00005114 @ 00005114 ====================

uint FUN_00005114(int param_1,int param_2)

{
  uint uVar1;
  int iVar2;
  undefined1 auStack_14 [8];
  
  uVar1 = FUN_00002bbc(param_1,auStack_14);
  if (uVar1 == 0) {
    if (param_2 == 0) {
      uVar1 = FUN_00003d50(param_1);
      return uVar1;
    }
    uVar1 = FUN_00003c84();
    if ((uVar1 & 0xfb) == 0) {
      iVar2 = *(int *)(param_1 + 0x18);
      *(undefined1 *)(param_2 + 9) = 0;
      if (iVar2 != 0) {
        FUN_00002c30(param_1 + 0x1c,param_2);
      }
      uVar1 = FUN_00003b44(param_1,0);
      if (uVar1 == 4) {
        uVar1 = 0;
      }
    }
  }
  return uVar1;
}



//==================== FUN_00005194 @ 00005194 ====================

int FUN_00005194(undefined4 param_1,int param_2)

{
  int iVar1;
  undefined4 local_44 [2];
  undefined1 auStack_3c [24];
  int local_24;
  undefined1 auStack_20 [15];
  char local_11;
  
  local_44[0] = param_1;
  FUN_000027e0(auStack_3c,0,0x2c);
  iVar1 = FUN_000030bc(local_44,auStack_3c,0);
  if ((iVar1 == 0) && (iVar1 = FUN_00003f2c(auStack_3c,local_44[0]), iVar1 == 0)) {
    if (local_11 < '\0') {
      iVar1 = 6;
    }
    else if ((param_2 != 0) && (*(undefined1 *)(param_2 + 9) = 0, local_24 != 0)) {
      FUN_00002c30(auStack_20,param_2);
    }
  }
  return iVar1;
}



//==================== FUN_00005224 @ 00005224 ====================

int FUN_00005224(undefined4 param_1,uint *param_2,undefined4 *param_3)

{
  int iVar1;
  int iVar2;
  char *pcVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  char *pcVar8;
  int iVar9;
  int iVar10;
  undefined4 local_2c [2];
  char *local_24;
  
  local_2c[0] = param_1;
  iVar4 = FUN_000030bc(local_2c,&local_24,0);
  pcVar3 = local_24;
  if (iVar4 == 0) {
    iVar9 = *(int *)(local_24 + 0x14);
    uVar6 = *(uint *)(local_24 + 0x10);
    *param_3 = local_24;
    if (iVar9 - 2U < uVar6) {
      if (*local_24 == '\x01') {
        uVar7 = 2;
        uVar6 = 0;
        do {
          iVar9 = FUN_000036fc(pcVar3,uVar7);
          uVar7 = uVar7 + 1;
          pcVar8 = local_24;
          if (iVar9 == -1) {
            iVar4 = 1;
            break;
          }
          if (iVar9 == 1) {
            iVar4 = 2;
            break;
          }
          if (iVar9 == 0) {
            uVar6 = uVar6 + 1;
          }
        } while (uVar7 < *(uint *)(local_24 + 0x14));
      }
      else {
        uVar7 = 0;
        uVar6 = 0;
        iVar5 = *(int *)(local_24 + 0x20);
        pcVar8 = local_24;
        do {
          while( true ) {
            iVar10 = iVar5;
            if (uVar7 == 0) {
              iVar10 = iVar5 + 1;
              if (iVar5 == *(int *)(pcVar8 + 0x2c)) {
              }
              else {
                iVar5 = FUN_00002f3c(pcVar8);
                pcVar8 = local_24;
                if (iVar5 != 0) {
                  iVar4 = iVar5;
                  goto LAB_00005318;
                }
              }
            }
            iVar1 = uVar7 + 0x40;
            iVar2 = uVar7 + 0x41;
            iVar5 = iVar10;
            if (*pcVar8 != '\x02') break;
            uVar7 = uVar7 + 2 & 0x1ff;
            if (CONCAT11(pcVar8[iVar2],pcVar8[iVar1]) == 0) {
              uVar6 = uVar6 + 1;
            }
            iVar9 = iVar9 + -1;
            if (iVar9 == 0) goto LAB_00005318;
          }
          iVar10 = uVar7 + 0x42;
          uVar7 = uVar7 + 4 & 0x1ff;
          if ((CONCAT31(CONCAT21(*(undefined2 *)(pcVar8 + iVar10),pcVar8[iVar2]),pcVar8[iVar1]) &
              0xfffffff) == 0) {
            uVar6 = uVar6 + 1;
          }
          iVar9 = iVar9 + -1;
        } while (iVar9 != 0);
      }
LAB_00005318:
      *param_2 = uVar6;
      *(uint *)(pcVar8 + 0x10) = uVar6;
      pcVar8[4] = pcVar8[4] | 1;
    }
    else {
      *param_2 = uVar6;
    }
  }
  return iVar4;
}



//==================== FUN_000053a8 @ 000053a8 ====================

uint FUN_000053a8(undefined4 *param_1)

{
  uint uVar1;
  int iVar2;
  byte bVar3;
  int local_14 [2];
  
  uVar1 = FUN_00002bbc(param_1,local_14);
  if (uVar1 != 0) {
    return uVar1;
  }
  if (*(byte *)((int)param_1 + 0x11) != 0) {
    return (uint)*(byte *)((int)param_1 + 0x11);
  }
  if ((*(byte *)(param_1 + 4) & 2) == 0) {
    return 7;
  }
  if ((uint)param_1[3] <= (uint)param_1[5]) {
    return 0;
  }
  if (param_1[5] == 0) {
    uVar1 = FUN_00003a48(param_1,param_1[2],0);
    param_1[2] = 0;
LAB_0000540c:
    bVar3 = *(byte *)(param_1 + 4) | 0x40;
    *(byte *)(param_1 + 4) = bVar3;
    param_1[3] = param_1[5];
    if (uVar1 != 0) goto LAB_00005484;
LAB_00005428:
    if ((bVar3 & 0x80) == 0) {
      return 0;
    }
    iVar2 = FUN_0000676c(*(undefined1 *)(local_14[0] + 1),param_1 + 0x10,param_1[7],1);
    if (iVar2 == 0) {
      *(byte *)(param_1 + 4) = *(byte *)(param_1 + 4) & 0x7f;
      return 0;
    }
    uVar1 = 1;
  }
  else {
    uVar1 = FUN_000036fc(*param_1,param_1[6]);
    if (uVar1 == 0xffffffff) {
      uVar1 = 1;
    }
    else {
      if (uVar1 != 1) {
        if (uVar1 < *(uint *)(local_14[0] + 0x14)) {
          uVar1 = FUN_00003a48(param_1,uVar1,param_1[6]);
          goto LAB_0000540c;
        }
        bVar3 = *(byte *)(param_1 + 4) | 0x40;
        *(byte *)(param_1 + 4) = bVar3;
        param_1[3] = param_1[5];
        goto LAB_00005428;
      }
      uVar1 = 2;
    }
    *(byte *)(param_1 + 4) = *(byte *)(param_1 + 4) | 0x40;
    param_1[3] = param_1[5];
  }
LAB_00005484:
  *(char *)((int)param_1 + 0x11) = (char)uVar1;
  return uVar1;
}



//==================== FUN_000054fc @ 000054fc ====================

int FUN_000054fc(undefined4 param_1)

{
  int iVar1;
  uint uVar2;
  undefined4 local_6c [2];
  char *local_64;
  char *local_60;
  byte local_5a;
  int local_44;
  char local_35;
  char *local_34 [2];
  uint local_2c;
  
  local_6c[0] = param_1;
  iVar1 = FUN_000030bc(local_6c,&local_64,2);
  if (iVar1 != 0) {
    return iVar1;
  }
  local_60 = local_64;
  iVar1 = FUN_00003f2c(&local_60,local_6c[0]);
  if (iVar1 != 0) {
    return iVar1;
  }
  if (local_35 < '\0') {
    return 6;
  }
  if ((local_5a & 1) == 0) {
    uVar2 = (uint)*(ushort *)(local_44 + 0x1a);
    if (*local_64 == '\x03') {
      uVar2 = CONCAT22(*(undefined2 *)(local_44 + 0x14),*(ushort *)(local_44 + 0x1a));
    }
    if ((local_5a & 0x10) != 0) {
      local_34[0] = local_64;
      local_2c = uVar2;
      iVar1 = FUN_00003d50(local_34);
      if (iVar1 != 0) {
        return iVar1;
      }
      iVar1 = FUN_00003c84(local_34);
      if (iVar1 == 0) goto LAB_00005560;
      if (iVar1 != 4) {
        return iVar1;
      }
    }
    iVar1 = FUN_00002f8c(&local_60);
    if (iVar1 == 0 && uVar2 != 0) {
      iVar1 = FUN_00003a48(&local_60,uVar2,0);
    }
    if (iVar1 == 0) {
      iVar1 = FUN_00002e20(local_64);
    }
  }
  else {
LAB_00005560:
    iVar1 = 7;
  }
  return iVar1;
}



//==================== FUN_00005618 @ 00005618 ====================

int FUN_00005618(undefined4 param_1)

{
  char cVar1;
  char cVar2;
  char cVar3;
  char cVar4;
  char cVar5;
  char cVar6;
  int iVar7;
  int iVar8;
  undefined4 uVar9;
  char *pcVar10;
  char *pcVar11;
  char *pcVar12;
  undefined4 local_6c;
  char *local_68;
  char *local_64 [4];
  char *local_54 [7];
  int local_38;
  
  local_6c = param_1;
  iVar7 = FUN_000030bc(&local_6c,&local_68,2);
  if (iVar7 == 0) {
    local_54[0] = local_68;
    iVar7 = FUN_00003f2c(local_54,local_6c);
    if (iVar7 == 0) {
      iVar7 = 8;
    }
    else if (iVar7 == 4) {
      local_64[0] = local_68;
      iVar8 = FUN_00003868(local_64,0);
      if (iVar8 == 0) {
        iVar7 = 7;
        FUN_00006760();
      }
      else if (iVar8 == 1) {
        FUN_00006760();
        iVar7 = 2;
      }
      else if (iVar8 == -1) {
        FUN_00006760();
        iVar7 = 1;
      }
      else {
        uVar9 = FUN_00006760();
        iVar7 = FUN_00002d5c(local_68,iVar8);
        if (iVar7 == 0) {
          pcVar11 = local_68 + 0x40;
          do {
            pcVar10 = pcVar11 + 1;
            *pcVar11 = ' ';
            pcVar11 = pcVar10;
          } while (local_68 + 0x4b != pcVar10);
          local_68[0x56] = (char)uVar9;
          cVar4 = (char)((uint)uVar9 >> 8);
          cVar6 = (char)((uint)iVar8 >> 8);
          cVar5 = (char)((uint)uVar9 >> 0x10);
          local_68[0x40] = '.';
          cVar2 = (char)((uint)uVar9 >> 0x18);
          local_68[0x59] = cVar2;
          local_68[0x5a] = (char)iVar8;
          local_68[0x4b] = '\x10';
          local_68[0x57] = cVar4;
          local_68[0x58] = cVar5;
          local_68[0x5b] = cVar6;
          cVar3 = (char)((uint)iVar8 >> 0x10);
          cVar1 = (char)((uint)iVar8 >> 0x18);
          if (*local_68 == '\x03') {
            local_68[0x54] = cVar3;
            local_68[0x55] = cVar1;
          }
          pcVar10 = local_68 + 0x3c;
          pcVar11 = local_68 + 0x60;
          do {
            pcVar10 = pcVar10 + 4;
            pcVar12 = pcVar11 + 4;
            *(undefined4 *)pcVar11 = *(undefined4 *)pcVar10;
            pcVar11 = pcVar12;
          } while (pcVar12 != local_68 + 0x80);
          local_68[0x61] = '.';
          local_68[0x7a] = (char)local_54[2];
          local_68[0x7b] = (char)((uint)local_54[2] >> 8);
          if (*local_68 == '\x03') {
            local_68[0x74] = (char)((uint)local_54[2] >> 0x10);
            local_68[0x75] = (char)((uint)local_54[2] >> 0x18);
          }
          local_68[3] = '\x01';
          iVar7 = FUN_00003e38(local_54);
          if (iVar7 == 0) {
            *(char *)(local_38 + 0x16) = (char)uVar9;
            *(char *)(local_38 + 0x17) = cVar4;
            *(char *)(local_38 + 0x18) = cVar5;
            *(char *)(local_38 + 0x19) = cVar2;
            *(char *)(local_38 + 0x1a) = (char)iVar8;
            *(char *)(local_38 + 0x1b) = cVar6;
            cVar2 = *local_68;
            *(undefined1 *)(local_38 + 0xb) = 0x10;
            if (cVar2 == '\x03') {
              *(char *)(local_38 + 0x14) = cVar3;
              *(char *)(local_38 + 0x15) = cVar1;
            }
            local_68[3] = '\x01';
            iVar7 = FUN_00002e20(local_68);
            return iVar7;
          }
        }
      }
      FUN_00003a48(local_54,iVar8,0);
    }
  }
  return iVar7;
}



//==================== FUN_00005824 @ 00005824 ====================

int FUN_00005824(undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  undefined4 *puVar2;
  char **ppcVar3;
  uint uVar4;
  undefined4 *puVar5;
  int *piVar6;
  int *piVar7;
  undefined4 local_90;
  undefined4 local_8c [2];
  char *local_84;
  undefined4 auStack_80 [2];
  byte local_75;
  undefined4 local_73;
  undefined4 local_6f;
  undefined4 local_6b;
  undefined4 local_67;
  undefined1 local_63;
  undefined1 local_62;
  undefined1 local_61;
  char *local_60 [2];
  int local_58;
  int local_50;
  int local_44;
  byte local_35;
  int aiStack_34 [11];
  int iStack_8;
  
  local_90 = param_2;
  local_8c[0] = param_1;
  FUN_00002b44(&local_90);
  iVar1 = FUN_000030bc(local_8c,&local_84,2);
  if (iVar1 == 0) {
    local_60[0] = local_84;
    iVar1 = FUN_00003f2c(local_60,local_8c[0]);
    if (iVar1 == 0) {
      if ((local_35 & 0xa0) == 0) {
        puVar5 = (undefined4 *)(local_44 + -4);
        puVar2 = auStack_80;
        do {
          puVar5 = puVar5 + 1;
          *puVar2 = *puVar5;
          puVar2 = puVar2 + 1;
        } while ((undefined4 *)(local_44 + 0x1c) != puVar5);
        ppcVar3 = local_60;
        piVar6 = aiStack_34;
        do {
          piVar7 = piVar6 + 1;
          *piVar6 = (int)*ppcVar3;
          ppcVar3 = ppcVar3 + 1;
          piVar6 = piVar7;
        } while (piVar7 != &iStack_8);
        iVar1 = FUN_00003f2c(aiStack_34,local_90);
        if (iVar1 == 0) {
          if ((aiStack_34[2] != local_58) || (aiStack_34[4] != local_50)) {
            return 8;
          }
        }
        else if (iVar1 != 4) {
          return iVar1;
        }
        iVar1 = FUN_00003e38(aiStack_34);
        if (iVar1 == 0) {
          *(undefined4 *)(aiStack_34[7] + 0x11) = local_6f;
          if ((local_75 & 0x10) == 0) {
            local_75 = local_75 | 0x20;
          }
          *(undefined4 *)(aiStack_34[7] + 0xd) = local_73;
          *(undefined4 *)(aiStack_34[7] + 0x15) = local_6b;
          *(undefined1 *)(aiStack_34[7] + 0x1e) = local_62;
          *(byte *)(aiStack_34[7] + 0xb) = local_75;
          *(undefined4 *)(aiStack_34[7] + 0x19) = local_67;
          *(undefined1 *)(aiStack_34[7] + 0x1d) = local_63;
          *(undefined1 *)(aiStack_34[7] + 0x1f) = local_61;
          local_84[3] = '\x01';
          if (((*(byte *)(aiStack_34[7] + 0xb) & 0x10) != 0) && (local_58 != aiStack_34[2])) {
            uVar4 = (uint)*(ushort *)(aiStack_34[7] + 0x1a);
            if (*local_84 == '\x03') {
              aiStack_34[7] = (int)*(ushort *)(aiStack_34[7] + 0x14);
            }
            if (*local_84 == '\x03') {
              uVar4 = uVar4 | aiStack_34[7] << 0x10;
            }
            if ((*(int *)(local_84 + 0x14) - 2U <= uVar4 - 2) ||
               (iVar1 = (uVar4 - 2) * (uint)*(ushort *)(local_84 + 10) + *(int *)(local_84 + 0x28),
               iVar1 == 0)) {
              return 2;
            }
            if ((iVar1 != *(int *)(local_84 + 0x2c)) &&
               (iVar1 = FUN_00002f3c(local_84,iVar1), iVar1 != 0)) {
              return iVar1;
            }
            if (local_84[0x61] == '.') {
              local_84[0x7a] = (char)aiStack_34[2];
              local_84[0x7b] = (char)((uint)aiStack_34[2] >> 8);
              if (*local_84 == '\x03') {
                local_84[0x74] = (char)((uint)aiStack_34[2] >> 0x10);
                local_84[0x75] = (char)((uint)aiStack_34[2] >> 0x18);
              }
              local_84[3] = '\x01';
            }
          }
          iVar1 = FUN_00002f8c(local_60);
          if (iVar1 == 0) {
            iVar1 = FUN_00002e20(local_84);
          }
        }
      }
      else {
        iVar1 = 6;
      }
    }
  }
  return iVar1;
}



//==================== FUN_00005a60 @ 00005a60 ====================

undefined1
FUN_00005a60(undefined4 param_1,uint param_2,uint param_3,undefined4 *param_4,uint param_5)

{
  undefined1 uVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  char *pcVar5;
  undefined4 uVar6;
  uint uVar7;
  ushort *puVar8;
  uint uVar9;
  undefined1 uVar10;
  undefined4 *puVar12;
  undefined4 *puVar11;
  undefined1 uVar13;
  uint uVar14;
  uint uVar15;
  uint uVar16;
  uint uVar17;
  uint uVar18;
  undefined4 *puVar19;
  undefined1 uVar20;
  int iVar21;
  bool bVar22;
  int local_58;
  uint local_50;
  int local_4c;
  int local_48;
  uint local_44;
  undefined4 local_34;
  uint local_30;
  uint local_2c [2];
  
  local_34 = param_1;
  uVar3 = FUN_00002b44(&local_34);
  if ((int)uVar3 < 0) {
    return 0xb;
  }
  uVar15 = uVar3 & 0xff;
  if (*(undefined1 **)(&DAT_00021544 + uVar3 * 4) != (undefined1 *)0x0) {
    **(undefined1 **)(&DAT_00021544 + uVar3 * 4) = 0;
  }
  uVar3 = FUN_000065cc(uVar15);
  if ((uVar3 & 1) != 0) {
    return 3;
  }
  if ((uVar3 & 4) != 0) {
    return 10;
  }
  iVar4 = FUN_000066f8(uVar15,3,&local_30);
  if (((iVar4 != 0) || (0x7fff < local_30 - 1)) || ((local_30 & local_30 - 1) != 0)) {
    local_30 = 1;
  }
  if (param_3 != 0) {
    if (0xfffe00 < param_3 - 0x200) {
      return 0x13;
    }
    if ((param_3 - 1 & param_3) != 0) {
      return 0x13;
    }
  }
  uVar3 = param_5 >> 9;
  if (param_4 == (undefined4 *)0x0 || uVar3 == 0) {
    return 0x11;
  }
  iVar4 = FUN_000066f8(uVar15,1,local_2c);
  if (iVar4 != 0) {
    return true;
  }
  if ((param_2 & 8) == 0) {
    if (local_2c[0] < 0x3f) {
      return 0xe;
    }
    local_58 = 0x3f;
    uVar16 = local_2c[0] - 0x3f;
  }
  else {
    local_58 = 0;
    uVar16 = local_2c[0];
  }
  if (0x7f < uVar16) {
    param_3 = param_3 >> 9;
    if (0x80 < param_3) {
      return 0x13;
    }
    uVar14 = param_2 & 2;
    local_2c[0] = uVar16;
    if (uVar14 == 0) {
      if ((param_2 & 1) == 0) {
        return 0x13;
      }
    }
    else if (((param_2 & 7) == 2) || ((param_2 & 1) == 0)) {
      if (param_3 != 0) goto LAB_00005bac;
LAB_00005f58:
      if (uVar16 >> 0x11 == 0) {
        uVar18 = 1;
        param_3 = 0;
      }
      else {
        uVar18 = 2;
        uVar14 = 2;
        puVar8 = DAT_000064b0;
        do {
          if (uVar16 >> 0x11 < uVar14) {
            param_3 = 0;
            break;
          }
          puVar8 = puVar8 + 1;
          uVar14 = (uint)*puVar8;
          uVar18 = uVar18 << 1;
          param_3 = 0;
        } while (uVar14 != 0);
      }
      while( true ) {
        iVar4 = FUN_00001a50(uVar16,uVar18);
        local_50 = iVar4 * 4 + 0x207U >> 9;
        if (0xffeffff < iVar4 - 0xfff6U) {
          return 0xe;
        }
        iVar21 = local_50 + local_58 + 0x20;
        iVar4 = ((local_30 - 1) + iVar21 & -local_30) - iVar21;
        local_4c = local_58 + 0x20 + iVar4;
        iVar4 = iVar4 + 0x20;
        if (uVar16 < (uVar18 * 0x10 - local_58) + iVar21) {
          return 0xe;
        }
        pcVar5 = (char *)FUN_00001a50((uVar16 - iVar4) - local_50,uVar18);
        if (s_PS7_init_Data_Corrupted_0000fff0 + 6 <= pcVar5) break;
        if (param_3 != 0) {
          return 0xe;
        }
        param_3 = uVar18 >> 1;
        uVar14 = param_3;
joined_r0x00005ba8:
        if (uVar14 == 0) {
          return 0xe;
        }
LAB_00005bac:
        uVar18 = param_3;
      }
      local_48 = 3;
      local_44 = 0;
LAB_00005c58:
      puVar12 = param_4;
      do {
        puVar19 = (undefined4 *)((int)puVar12 + 1);
        *(undefined1 *)puVar12 = 0;
        puVar12 = puVar19;
      } while (puVar19 != param_4 + 0x80);
      *(char *)((int)param_4 + 0xf) = (char)((uint)iVar4 >> 8);
      param_4[2] = 0x302e35;
      if (local_48 == 3) {
        uVar20 = 0;
      }
      else {
        uVar20 = 2;
      }
      bVar22 = 0xffff < uVar16;
      uVar1 = (undefined1)(uVar16 >> 8);
      if (bVar22) {
        uVar13 = (char)(uVar16 >> 0x18);
      }
      else {
        uVar13 = uVar1;
      }
      *(undefined1 *)((int)param_4 + 0x11) = 0;
      uVar10 = (undefined1)uVar16;
      if (bVar22) {
        *(undefined1 *)((int)param_4 + 0x23) = uVar13;
      }
      else {
        *(undefined1 *)(param_4 + 5) = uVar13;
      }
      param_4[1] = 0x534f4453;
      if (bVar22) {
        *(undefined1 *)(param_4 + 8) = uVar10;
        uVar10 = (undefined1)(uVar16 >> 0x10);
      }
      *(undefined1 *)(param_4 + 4) = 1;
      if (bVar22) {
        *(undefined1 *)((int)param_4 + 0x21) = uVar1;
        *(undefined1 *)((int)param_4 + 0x22) = uVar10;
      }
      else {
        *(undefined1 *)((int)param_4 + 0x13) = uVar10;
      }
      *param_4 = 0x4d90feeb;
      *(char *)((int)param_4 + 0xe) = (char)iVar4;
      *(char *)((int)param_4 + 0xd) = (char)uVar18;
      *(undefined1 *)(param_4 + 6) = 0x3f;
      *(undefined1 *)((int)param_4 + 0x12) = uVar20;
      *(undefined1 *)(param_4 + 3) = 2;
      *(undefined1 *)(param_4 + 7) = (undefined1)local_58;
      *(undefined1 *)((int)param_4 + 0x15) = 0xf8;
      *(undefined1 *)((int)param_4 + 0x19) = 0;
      *(undefined1 *)((int)param_4 + 0x1b) = 0;
      *(undefined1 *)((int)param_4 + 0x1d) = 0;
      *(undefined1 *)((int)param_4 + 0x1e) = 0;
      *(undefined1 *)((int)param_4 + 0x1f) = 0;
      *(undefined1 *)((int)param_4 + 0x1a) = 0xff;
      uVar20 = (undefined1)(local_50 >> 8);
      if (local_48 == 3) {
        uVar6 = FUN_00006760();
        *(undefined1 *)(param_4 + 9) = (undefined1)local_50;
        *(char *)((int)param_4 + 0x43) = (char)uVar6;
        *(char *)((int)param_4 + 0x45) = (char)((uint)uVar6 >> 0x10);
        *(char *)(param_4 + 0x11) = (char)((uint)uVar6 >> 8);
        *(undefined4 *)((int)param_4 + 0x47) = 0x4e204f4e;
        *(char *)((int)param_4 + 0x46) = (char)((uint)uVar6 >> 0x18);
        *(undefined4 *)((int)param_4 + 0x4b) = 0x20454d41;
        *(undefined4 *)((int)param_4 + 0x53) = 0x32335441;
        *(char *)((int)param_4 + 0x26) = (char)(local_50 >> 0x10);
        *(undefined1 *)((int)param_4 + 0x2d) = 0;
        *(undefined1 *)((int)param_4 + 0x2e) = 0;
        *(undefined1 *)((int)param_4 + 0x2f) = 0;
        *(undefined1 *)((int)param_4 + 0x31) = 0;
        *(undefined1 *)((int)param_4 + 0x33) = 0;
        *(undefined4 *)((int)param_4 + 0x4f) = 0x46202020;
        *(undefined1 *)((int)param_4 + 0x25) = uVar20;
        *(char *)((int)param_4 + 0x27) = (char)(local_50 >> 0x18);
        *(undefined1 *)(param_4 + 0x10) = 0x80;
        *(undefined1 *)((int)param_4 + 0x42) = 0x29;
        *(undefined1 *)(param_4 + 0xb) = 2;
        *(undefined1 *)((int)param_4 + 0x32) = 6;
        *(undefined1 *)((int)param_4 + 0x59) = 0x20;
        *(undefined1 *)((int)param_4 + 0x1fe) = 0x55;
        *(undefined1 *)(param_4 + 0xc) = 1;
        *(undefined2 *)((int)param_4 + 0x57) = 0x2020;
        *(undefined1 *)((int)param_4 + 0x1ff) = 0xaa;
        iVar4 = FUN_0000676c(uVar15,param_4,local_58,1);
        if (iVar4 != 0) {
          return true;
        }
        FUN_0000676c(uVar15,param_4,local_58 + 6,1);
        puVar12 = param_4;
        do {
          puVar11 = (undefined4 *)((int)puVar12 + 1);
          *(undefined1 *)puVar12 = 0;
          puVar12 = puVar11;
        } while (puVar19 != puVar11);
        pcVar5 = pcVar5 + -1;
        *(char *)(param_4 + 0x7a) = (char)pcVar5;
        *(char *)((int)param_4 + 0x1e9) = (char)((uint)pcVar5 >> 8);
        *(char *)((int)param_4 + 0x1ea) = (char)((uint)pcVar5 >> 0x10);
        *(char *)((int)param_4 + 0x1eb) = (char)((uint)pcVar5 >> 0x18);
        *(undefined1 *)param_4 = 0x52;
        *(undefined1 *)((int)param_4 + 1) = 0x52;
        *(undefined1 *)((int)param_4 + 3) = 0x41;
        *(undefined1 *)((int)param_4 + 0x1e6) = 0x41;
        *(undefined1 *)(param_4 + 0x79) = 0x72;
        *(undefined1 *)((int)param_4 + 0x1e5) = 0x72;
        *(undefined1 *)(param_4 + 0x7b) = 2;
        *(undefined1 *)((int)param_4 + 0x1fe) = 0x55;
        *(undefined1 *)((int)param_4 + 0x1ed) = 0;
        *(undefined1 *)((int)param_4 + 0x1ee) = 0;
        *(undefined1 *)((int)param_4 + 0x1ef) = 0;
        *(undefined1 *)((int)param_4 + 2) = 0x61;
        *(undefined1 *)((int)param_4 + 0x1e7) = 0x61;
        *(undefined1 *)((int)param_4 + 0x1ff) = 0xaa;
        FUN_0000676c(uVar15,param_4,local_58 + 7,1);
        FUN_0000676c(uVar15,param_4,local_58 + 1);
      }
      else {
        uVar6 = FUN_00006760();
        *(char *)((int)param_4 + 0x27) = (char)uVar6;
        *(char *)(param_4 + 10) = (char)((uint)uVar6 >> 8);
        *(char *)((int)param_4 + 0x2a) = (char)((uint)uVar6 >> 0x18);
        *(char *)((int)param_4 + 0x29) = (char)((uint)uVar6 >> 0x10);
        *(undefined4 *)((int)param_4 + 0x2f) = 0x20454d41;
        *(undefined4 *)((int)param_4 + 0x33) = 0x46202020;
        *(undefined4 *)((int)param_4 + 0x2b) = 0x4e204f4e;
        *(undefined4 *)((int)param_4 + 0x37) = 0x20205441;
        *(undefined1 *)(param_4 + 9) = 0x80;
        *(undefined1 *)((int)param_4 + 0x26) = 0x29;
        *(undefined1 *)((int)param_4 + 0x17) = uVar20;
        *(undefined1 *)((int)param_4 + 0x3d) = 0x20;
        *(undefined1 *)((int)param_4 + 0x1fe) = 0x55;
        *(undefined1 *)((int)param_4 + 0x16) = (undefined1)local_50;
        *(undefined2 *)((int)param_4 + 0x3b) = 0x2020;
        *(undefined1 *)((int)param_4 + 0x1ff) = 0xaa;
        iVar4 = FUN_0000676c(uVar15,param_4,local_58,1);
        if (iVar4 != 0) {
          return true;
        }
      }
      puVar12 = param_4;
      do {
        puVar11 = (undefined4 *)((int)puVar12 + 1);
        *(undefined1 *)puVar12 = 0;
        puVar12 = puVar11;
      } while ((undefined4 *)((int)param_4 + (param_5 & 0xfffffe00)) != puVar11);
      if (local_48 == 3) {
        *(undefined1 *)param_4 = 0xf8;
        *(undefined1 *)((int)param_4 + 1) = 0xff;
        *(undefined1 *)((int)param_4 + 2) = 0xff;
        *(undefined1 *)((int)param_4 + 3) = 0xff;
        *(undefined1 *)(param_4 + 1) = 0xff;
        *(undefined1 *)((int)param_4 + 5) = 0xff;
        *(undefined1 *)((int)param_4 + 6) = 0xff;
        *(undefined1 *)((int)param_4 + 7) = 0xff;
        *(undefined1 *)(param_4 + 2) = 0xff;
        *(undefined1 *)((int)param_4 + 9) = 0xff;
        *(undefined1 *)((int)param_4 + 10) = 0xff;
        *(undefined1 *)((int)param_4 + 0xb) = 0xf;
      }
      else {
        if (local_48 == 1) {
          uVar20 = 0;
        }
        else {
          uVar20 = 0xff;
        }
        *(undefined1 *)param_4 = 0xf8;
        *(undefined1 *)((int)param_4 + 3) = uVar20;
        *(undefined1 *)((int)param_4 + 1) = 0xff;
        *(undefined1 *)((int)param_4 + 2) = 0xff;
      }
      while( true ) {
        if (local_50 < uVar3) {
          uVar16 = local_50;
        }
        else {
          uVar16 = uVar3;
        }
        iVar4 = FUN_0000676c(uVar15,param_4,local_4c,uVar16);
        if (iVar4 != 0) break;
        puVar12 = param_4;
        do {
          puVar11 = (undefined4 *)((int)puVar12 + 1);
          *(undefined1 *)puVar12 = 0;
          puVar12 = puVar11;
        } while (puVar19 != puVar11);
        local_50 = local_50 - uVar16;
        local_4c = local_4c + uVar16;
        if (local_50 == 0) {
          if (local_48 == 3) {
            local_44 = uVar18;
          }
          while( true ) {
            if (local_44 < uVar3) {
              uVar16 = local_44;
            }
            else {
              uVar16 = uVar3;
            }
            iVar4 = FUN_0000676c(uVar15,param_4,local_4c,uVar16);
            uVar14 = local_2c[0];
            if (iVar4 != 0) break;
            local_44 = local_44 - uVar16;
            local_4c = local_4c + uVar16;
            if (local_44 == 0) {
              if (local_48 == 3) {
                uVar20 = 0xc;
              }
              else if (local_2c[0] < 0x10000) {
                if (local_48 == 2) {
                  uVar20 = 4;
                }
                else {
                  uVar20 = 1;
                }
              }
              else {
                uVar20 = 6;
              }
              puVar12 = param_4;
              if ((param_2 & 8) == 0) {
                do {
                  puVar19 = (undefined4 *)((int)puVar12 + 1);
                  *(undefined1 *)puVar12 = 0;
                  puVar12 = puVar19;
                } while (puVar11 != puVar19);
                *(undefined1 *)((int)param_4 + 0x1c2) = uVar20;
                *(undefined1 *)((int)param_4 + 0x1fe) = 0x55;
                *(undefined1 *)((int)param_4 + 0x1ff) = 0xaa;
                *(undefined1 *)((int)param_4 + 0x1be) = 0;
                *(undefined1 *)((int)param_4 + 0x1c1) = 0;
                *(undefined1 *)((int)param_4 + 0x1bf) = 1;
                *(undefined1 *)(param_4 + 0x70) = 1;
                uVar3 = FUN_00001a50(local_2c[0] + local_58,0x3ec1);
                *(char *)((int)param_4 + 0x1c5) = (char)uVar3;
                *(char *)((int)param_4 + 0x1ca) = (char)uVar14;
                *(undefined1 *)((int)param_4 + 0x1c6) = (undefined1)local_58;
                *(byte *)(param_4 + 0x71) = (byte)(uVar3 >> 2) | 0x3f;
                *(char *)((int)param_4 + 0x1cb) = (char)(uVar14 >> 8);
                *(undefined1 *)((int)param_4 + 0x1c7) = 0;
                *(undefined1 *)(param_4 + 0x72) = 0;
                *(undefined1 *)((int)param_4 + 0x1c9) = 0;
                *(char *)(param_4 + 0x73) = (char)(uVar14 >> 0x10);
                *(char *)((int)param_4 + 0x1cd) = (char)(uVar14 >> 0x18);
                *(undefined1 *)((int)param_4 + 0x1c3) = 0xfe;
                iVar4 = FUN_0000676c(uVar15,param_4,0,1);
                if (iVar4 != 0) {
                  return true;
                }
              }
              iVar4 = FUN_000066f8(uVar15,0);
              return iVar4 != 0;
            }
          }
          return true;
        }
      }
      return true;
    }
    uVar2 = -local_30;
    iVar4 = local_30 - 1;
    local_4c = local_58 + 1;
    uVar17 = uVar15;
LAB_00005fdc:
    do {
      uVar18 = param_3;
      if (param_3 == 0) {
        if (uVar16 >> 0xc == 0) {
          uVar18 = 1;
        }
        else {
          uVar18 = 2;
          uVar7 = 4;
          puVar8 = DAT_000064b4;
          do {
            if (uVar16 >> 0xc < uVar7) break;
            puVar8 = puVar8 + 1;
            uVar7 = (uint)*puVar8;
            uVar18 = uVar18 << 1;
          } while (uVar7 != 0);
        }
      }
      uVar7 = FUN_00001a50(uVar16,uVar18);
      if (uVar7 < 0xff6) {
        uVar17 = 1;
        uVar9 = uVar7 * 3 + 1 >> 1;
      }
      else {
        uVar9 = (uVar7 + 2) * 2;
      }
      if (uVar7 < 0xff6) {
        uVar9 = uVar9 + 3;
      }
      else {
        uVar17 = 2;
      }
      local_50 = uVar9 + 0x1ff >> 9;
      iVar21 = local_50 + 0x20 + local_4c;
      local_50 = ((iVar4 + iVar21 & uVar2) - iVar21) + local_50;
      if (uVar16 < (uVar18 * 0x10 - local_58) + iVar21) {
        return 0xe;
      }
      pcVar5 = (char *)FUN_00001a50((uVar16 - local_50) + -0x21,uVar18);
      if (uVar17 != 2) {
        if ((char *)0xff5 < pcVar5) {
          return 0xe;
        }
        local_48 = 1;
        iVar4 = 1;
        local_44 = 0x20;
        goto LAB_00005c58;
      }
      if (pcVar5 < s_PS7_init_Data_Corrupted_0000fff0 + 6) {
        if ((char *)0xff5 < pcVar5) {
          iVar4 = 1;
          local_44 = 0x20;
          local_48 = 2;
          goto LAB_00005c58;
        }
        if (param_3 != 0) {
          return 0xe;
        }
        param_3 = uVar18 << 1;
      }
      else {
        if (param_3 != 0) goto joined_r0x00005ba8;
        param_3 = uVar18 * 2;
        if (param_3 < 0x41) goto LAB_00005fdc;
        if (uVar14 != 0) goto LAB_00005f58;
      }
    } while (param_3 < 0x81);
  }
  return 0xe;
}



//==================== FUN_000064b8 @ 000064b8 ====================

void FUN_000064b8(int param_1)

{
  byte bVar1;
  int iVar2;
  uint uVar3;
  
  bVar1 = (&DAT_00019be8)[param_1];
  iVar2 = *(int *)(&DAT_00021564 + param_1 * 0x220);
  if (iVar2 == 0) {
    DAT_000219a0 = -0x1ff00000;
    DAT_000219a4 = 1;
    DAT_000219a8 = iVar2;
    if ((uRame01000fe & 0xff) == 2) {
      uVar3 = uRame0100040 & 0xc0000000;
      *(uint *)(&DAT_000219ac + param_1 * 4) = uVar3;
      if (uVar3 == 0x40000000) goto LAB_00006568;
    }
    else {
      *(undefined4 *)(&DAT_000219ac + param_1 * 4) = 0;
    }
    if ((uRame0100024 & 0x10000) == 0) {
LAB_00006524:
      iVar2 = 500;
      do {
        func_0xffffe2ac(10000);
        uVar3 = *(uint *)(DAT_000219a0 + 0x24);
        if ((uVar3 & 0x10000) != 0) goto LAB_0000654c;
        iVar2 = iVar2 + -1;
      } while (iVar2 != 0);
      bVar1 = 3;
      goto LAB_0000656c;
    }
  }
  else {
    uVar3 = *(uint *)(DAT_000219a0 + 0x24);
    if (*(int *)(&DAT_000219ac + param_1 * 4) != 0x40000000) {
      if ((DAT_000219a4 != 0) && ((uVar3 & 0x10000) == 0)) goto LAB_00006524;
LAB_0000654c:
      if ((DAT_000219a8 != 0) && ((uVar3 & 0x80000) == 0)) {
        bVar1 = bVar1 & 0xfd | 4;
        goto LAB_0000656c;
      }
    }
  }
LAB_00006568:
  bVar1 = bVar1 & 0xf9;
LAB_0000656c:
  (&DAT_00019be8)[param_1] = bVar1;
  return;
}



//==================== FUN_000065cc @ 000065cc ====================

uint FUN_000065cc(int param_1)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  
  uVar1 = FUN_000064b8();
  if (((uVar1 & 2) != 0) || ((uVar1 & 1) == 0)) {
    return uVar1;
  }
  if (DAT_000219a4 != 0) {
    do {
    } while ((*(uint *)(DAT_000219a0 + 0x24) & 0x70000) != 0x70000);
  }
  iVar2 = FUN_000067e8(param_1);
  if (iVar2 != 0) {
    iVar3 = param_1 * 0x220 + 0x21560;
    iVar2 = FUN_00006804(iVar3,iVar2,*(undefined4 *)(iVar2 + 4));
    if ((iVar2 == 0) && (iVar2 = FUN_00007758(iVar3), iVar2 == 0)) {
      (&DAT_00019be8)[param_1] = (char)(uVar1 & 0xfe);
      return uVar1 & 0xfe;
    }
  }
  return uVar1 & 0xff | 1;
}



//==================== FUN_0000667c @ 0000667c ====================

bool FUN_0000667c(int param_1,undefined4 param_2,int param_3,int param_4)

{
  uint uVar1;
  int iVar2;
  
  uVar1 = FUN_000064b8();
  if ((uVar1 & 1) != 0) {
    return (bool)3;
  }
  if (param_4 == 0) {
    return (bool)4;
  }
  if (*(int *)(&DAT_00021590 + param_1 * 0x220) == 0) {
    param_3 = param_3 << 9;
  }
  iVar2 = FUN_000072c8(param_1 * 0x220 + 0x21560,param_3,param_4,param_2);
  return iVar2 != 0;
}



//==================== FUN_000066f8 @ 000066f8 ====================

undefined4 FUN_000066f8(int param_1,int param_2,undefined4 *param_3)

{
  uint uVar1;
  
  uVar1 = FUN_000064b8();
  if ((uVar1 & 1) != 0) {
    return 3;
  }
  if (param_2 == 1) {
    *param_3 = *(undefined4 *)(&DAT_000215c4 + param_1 * 0x220);
    return 0;
  }
  if (param_2 == 0) {
    return 0;
  }
  if (param_2 == 3) {
    *param_3 = 0x80;
    return 0;
  }
  return 4;
}



//==================== FUN_00006760 @ 00006760 ====================

undefined4 FUN_00006760(void)

{
  return 0x3c210000;
}



//==================== FUN_0000676c @ 0000676c ====================

bool FUN_0000676c(int param_1,undefined4 param_2,int param_3,int param_4)

{
  uint uVar1;
  int iVar2;
  
  uVar1 = FUN_000064b8();
  if ((uVar1 & 1) != 0) {
    return (bool)3;
  }
  if (param_4 == 0) {
    return (bool)4;
  }
  if (*(int *)(&DAT_00021590 + param_1 * 0x220) == 0) {
    param_3 = param_3 << 9;
  }
  iVar2 = FUN_00007430(param_1 * 0x220 + 0x21560,param_3,param_4,param_2);
  return iVar2 != 0;
}



//==================== FUN_000067e8 @ 000067e8 ====================

undefined2 * FUN_000067e8(uint param_1)

{
  undefined2 *puVar1;
  
  if (DAT_00019bec == param_1) {
    puVar1 = &DAT_00019bec;
  }
  else {
    puVar1 = (undefined2 *)0x0;
  }
  return puVar1;
}



//==================== FUN_00006804 @ 00006804 ====================

undefined4 FUN_00006804(undefined2 *param_1,undefined2 *param_2,int param_3)

{
  byte bVar1;
  ushort uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int iVar5;
  undefined1 uVar6;
  uint uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  
  if (param_1 == (undefined2 *)0x0) {
    FUN_000014e4(&DAT_0001725c,0xb9);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (param_2 == (undefined2 *)0x0) {
    FUN_000014e4(&DAT_0001725c,0xba);
    DAT_00028e84 = 1;
    return 0;
  }
  uVar3 = *(undefined4 *)(param_2 + 4);
  uVar8 = *(undefined4 *)(param_2 + 6);
  uVar10 = *(undefined4 *)(param_2 + 8);
  *param_1 = *param_2;
  *(undefined4 *)(param_1 + 0x12) = 0x11111111;
  uVar9 = *(undefined4 *)(param_2 + 10);
  uVar4 = *(undefined4 *)(param_2 + 0xe);
  *(undefined4 *)(param_1 + 4) = uVar3;
  uVar3 = *(undefined4 *)(param_2 + 0xc);
  *(int *)(param_1 + 2) = param_3;
  *(undefined4 *)(param_1 + 6) = uVar8;
  *(undefined4 *)(param_1 + 10) = uVar9;
  *(undefined4 *)(param_1 + 0xc) = uVar3;
  *(undefined4 *)(param_1 + 8) = uVar10;
  *(undefined4 *)(param_1 + 0xe) = uVar4;
  uVar6 = *(undefined1 *)(param_2 + 0x10);
  *(undefined4 *)(param_1 + 0x32) = 0;
  *(undefined4 *)(param_1 + 0x38) = 0;
  *(undefined1 *)(param_1 + 0x10) = uVar6;
  uVar2 = *(ushort *)(param_3 + 0xfe);
  *(undefined4 *)(param_1 + 0x36) = 6;
  *(undefined4 *)(param_1 + 0x100) = 0;
  *(undefined4 *)(param_1 + 0x102) = 0;
  if ((uVar2 & 0xff) == 2) {
    *(undefined1 *)(param_3 + 0x29) = 0x10;
  }
  else {
    *(undefined1 *)(param_3 + 0x29) = 0;
  }
  func_0xffffe2ac(1000);
  *(undefined1 *)(*(int *)(param_1 + 2) + 0x2f) = 1;
  iVar5 = *(int *)(param_1 + 2);
  bVar1 = *(byte *)(iVar5 + 0x2f);
  while ((bVar1 & 1) != 0) {
    bVar1 = *(byte *)(iVar5 + 0x2f);
  }
  *(char *)(param_1 + 0x1b) = (char)*(undefined2 *)(iVar5 + 0xfe);
  *(undefined4 *)(param_1 + 0x14) = *(undefined4 *)(iVar5 + 0x40);
  *(undefined1 *)(iVar5 + 0x29) = 0xf;
  if ((*(char *)(param_1 + 0x1b) == '\x02') &&
     ((*(uint *)(param_1 + 0x14) & 0xc0000000) == 0x40000000)) {
    func_0xffffe2ac(200);
  }
  iVar5 = FUN_000082b0(param_1,&DAT_00061a80);
  if (iVar5 != 0) {
    return 1;
  }
  uVar7 = *(uint *)(param_1 + 0x14);
  if ((uVar7 & 0x1000000) == 0) {
    if ((uVar7 & 0x2000000) == 0) {
      if ((uVar7 & 0x4000000) == 0) {
        uVar6 = 1;
      }
      else {
        uVar6 = 0xb;
      }
    }
    else {
      uVar6 = 0xd;
    }
  }
  else {
    uVar6 = 0xf;
  }
  *(undefined1 *)(*(int *)(param_1 + 2) + 0x29) = uVar6;
  if (*(char *)(param_1 + 0x1b) == '\x02') {
    uVar6 = 0x18;
  }
  else {
    uVar6 = 0x10;
  }
  *(undefined1 *)(*(int *)(param_1 + 2) + 0x28) = uVar6;
  iVar5 = *(int *)(param_1 + 2);
  *(undefined2 *)(iVar5 + 0x34) = 0xfeff;
  *(undefined2 *)(iVar5 + 0x36) = 0xf3ff;
  *(undefined2 *)(iVar5 + 0x38) = 0;
  *(undefined2 *)(iVar5 + 0x3a) = 0;
  DAT_00028e88 = 0x13;
  *(undefined2 *)(iVar5 + 4) = 0x200;
  return 0;
}



//==================== FUN_00006a24 @ 00006a24 ====================

code * FUN_00006a24(int param_1,code *param_2)

{
  code *pcVar1;
  
  if (param_2 == (code *)0x1200) goto LAB_00006b0c;
  if (param_2 < (code *)0x1201) {
    if (param_2 == (code *)0x800) {
      pcVar1 = (code *)0x83a;
      if (*(char *)(param_1 + 0x34) == '\x01') {
        pcVar1 = FUN_00000814;
      }
      return pcVar1;
    }
    if (param_2 < (code *)0x801) {
      if (param_2 == (code *)0x300) {
        pcVar1 = (code *)0x31a;
        if (*(char *)(param_1 + 0x34) == '\x01') {
          pcVar1 = (code *)0x31b;
        }
        return pcVar1;
      }
      if (param_2 < (code *)0x301) {
        if (param_2 == (code *)0x100) {
          return (code *)0x102;
        }
        if (param_2 == (code *)0x200) {
          return (code *)0x209;
        }
      }
      else {
        if (param_2 == (code *)0x600) {
          pcVar1 = (code *)0x61b;
          if (*(char *)(param_1 + 0x34) == '\x01') {
            pcVar1 = (code *)0x63a;
          }
          return pcVar1;
        }
        if (param_2 == (code *)0x700) {
          return FUN_00000714;
        }
        if (param_2 == (code *)0x500) {
          return (code *)0x51b;
        }
      }
      return param_2;
    }
    if (param_2 != (code *)0xb00) {
      if (param_2 < (code *)0xb01) {
        if (param_2 == (code *)0x900) {
          return (code *)0x909;
        }
        if (param_2 != (code *)0xa00) {
          return param_2;
        }
      }
      else if (param_2 != (code *)0x1000) {
        if (param_2 == (code *)&DAT_00001100) goto LAB_00006b0c;
        if (param_2 != (code *)0xc00) {
          return param_2;
        }
      }
    }
  }
  else if (param_2 != FUN_000036fc + 4) {
    if (param_2 < FUN_000036fc + 5) {
      if (param_2 != (code *)0x1700) {
        if (param_2 < (code *)0x1701) {
          if ((param_2 != (code *)&DAT_00001300) && (param_2 != (code *)0x1500)) {
            return param_2;
          }
        }
        else if (param_2 != (code *)0x1900) {
          if (param_2 == (code *)0x3400) goto LAB_00006af4;
          if (param_2 != (code *)0x1800) {
            return param_2;
          }
        }
      }
    }
    else if (param_2 != (code *)0x9700) {
      if ((code *)0x9700 < param_2) {
        if (param_2 == (code *)0xaa00) {
          return (code *)0xaa1a;
        }
        if (param_2 == (code *)0xb300) {
          return (code *)0xb33a;
        }
        if (param_2 == (code *)0xa900) {
          return (code *)0xa902;
        }
        return param_2;
      }
      if (param_2 == (code *)0x8600) {
        return (code *)0x861a;
      }
      if (param_2 != (code *)0x8d00) {
        return param_2;
      }
      goto LAB_00006af4;
    }
LAB_00006b0c:
    return (code *)((uint)param_2 | 0x3a);
  }
LAB_00006af4:
  return (code *)((uint)param_2 | 0x1a);
}



//==================== FUN_00006bcc @ 00006bcc ====================

ushort FUN_00006bcc(int param_1,int param_2,undefined4 param_3,undefined2 param_4)

{
  ushort uVar1;
  uint uVar2;
  uint uVar3;
  int extraout_r2;
  int iVar4;
  
  if (param_1 == 0) {
    FUN_000014e4(&DAT_0001725c,0x451);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 0x24) != 0x11111111) {
    FUN_000014e4(&DAT_0001725c,0x452);
    DAT_00028e84 = 1;
    return 0;
  }
  iVar4 = *(int *)(param_1 + 4);
  if ((*(uint *)(iVar4 + 0x24) & 1) == 0) {
    *(undefined2 *)(iVar4 + 6) = param_4;
    *(undefined1 *)(iVar4 + 0x2e) = 0xe;
    *(undefined4 *)(*(int *)(param_1 + 4) + 8) = param_3;
    iVar4 = *(int *)(param_1 + 4);
    *(undefined2 *)(iVar4 + 0x30) = 0xffff;
    *(undefined2 *)(iVar4 + 0x32) = 0xf3ff;
    uVar2 = FUN_00006a24();
    uVar3 = param_2 - 0x1300U & 0xfffffdff;
    if (((uVar3 == 0) || ((*(uint *)(extraout_r2 + 0x24) & 2) == 0)) || ((uVar2 & 0x20) == 0)) {
      *(uint *)(extraout_r2 + 0xc) = (uVar2 & 0x3fff) << 0x10 | (uint)DAT_00028e88;
      iVar4 = *(int *)(param_1 + 4);
      do {
        uVar1 = *(ushort *)(iVar4 + 0x30);
        if ((uVar3 == 0) && ((*(ushort *)(iVar4 + 0x30) & 0x20) != 0)) {
          *(undefined2 *)(iVar4 + 0x30) = 0x20;
          break;
        }
        if ((uVar1 & 0x8000) != 0) {
          uVar1 = *(ushort *)(iVar4 + 0x32);
          *(undefined2 *)(iVar4 + 0x32) = 0xf3ff;
          if ((uVar1 & 0xfffe) == 0) {
            uVar1 = 2;
          }
          return uVar1;
        }
      } while ((uVar1 & 1) == 0);
      *(undefined2 *)(iVar4 + 0x30) = 1;
      return 0;
    }
  }
  return 1;
}



//==================== FUN_00006d40 @ 00006d40 ====================

undefined4 FUN_00006d40(int param_1)

{
  byte bVar1;
  ushort uVar2;
  ushort uVar3;
  ushort uVar4;
  int iVar5;
  uint uVar6;
  int iVar7;
  undefined4 uVar8;
  uint uVar9;
  undefined1 uVar10;
  
  if (param_1 == 0) {
    FUN_000014e4(&DAT_0001725c,0x167);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 0x24) != 0x11111111) {
    FUN_000014e4(&DAT_0001725c,0x168);
    DAT_00028e84 = 1;
    return 0;
  }
  *(undefined4 *)(param_1 + 0x14) = 4;
  if ((((*(char *)(param_1 + 0x36) != '\x02') ||
       ((*(uint *)(param_1 + 0x28) & 0xc0000000) != 0x40000000)) && (*(int *)(param_1 + 0xc) != 0))
     && ((*(uint *)(*(int *)(param_1 + 4) + 0x24) & 0x10000) == 0)) {
    return 1;
  }
  iVar5 = FUN_00006bcc(param_1,0,0);
  if (iVar5 != 0) {
    return 1;
  }
  uVar6 = FUN_00006bcc(param_1,0x800,0x1aa);
  if ((uVar6 & 0xfffffffd) != 0) {
    return 1;
  }
  iVar5 = *(int *)(param_1 + 4);
  if (uVar6 == 2) {
    *(undefined1 *)(iVar5 + 0x2f) = 2;
    iVar5 = *(int *)(param_1 + 4);
    bVar1 = *(byte *)(iVar5 + 0x2f);
    while ((bVar1 & 2) != 0) {
      bVar1 = *(byte *)(iVar5 + 0x2f);
    }
  }
  if (*(int *)(iVar5 + 0x10) == 0x1aa) {
    uVar10 = 2;
  }
  else {
    uVar10 = 1;
  }
  *(undefined1 *)(param_1 + 0x35) = uVar10;
  do {
    iVar5 = FUN_00006bcc(param_1,0x3700,0);
    uVar8 = 0x40ff8000;
    if (iVar5 != 0) {
      return 1;
    }
    if (*(char *)(param_1 + 0x36) == '\x02') {
      if (*(int *)(param_1 + 0x14) == 8) {
        uVar8 = 0x41ff8000;
      }
      else {
        uVar8 = 0x40ff8000;
      }
    }
    iVar5 = FUN_00006bcc(param_1,0xa900,uVar8,0);
    if (iVar5 != 0) {
      return 1;
    }
    uVar6 = *(uint *)(*(int *)(param_1 + 4) + 0x10);
  } while (-1 < (int)uVar6);
  if ((uVar6 & 0x40000000) != 0) {
    *(undefined4 *)(param_1 + 0x30) = 1;
  }
  if ((uVar6 & 0x1000000) != 0) {
    *(undefined1 *)(param_1 + 0x3c) = 1;
    iVar5 = FUN_00006bcc(param_1,0xb00,0);
    iVar7 = *(int *)(param_1 + 4);
    uVar6 = *(uint *)(iVar7 + 0x24);
    while ((uVar6 & 0x1f00000) != 0) {
      uVar6 = *(uint *)(iVar7 + 0x24);
    }
    *(ushort *)(iVar7 + 0x2c) = *(ushort *)(iVar7 + 0x2c) & 0xfffa;
    *(ushort *)(iVar7 + 0x3e) = *(ushort *)(iVar7 + 0x3e) | 8;
    func_0xffffe2ac(&LAB_00001388);
    iVar7 = *(int *)(param_1 + 4);
    if ((*(ushort *)(iVar7 + 0x3e) & 8) == 0) {
      return 1;
    }
    *(ushort *)(iVar7 + 0x2c) = *(ushort *)(iVar7 + 0x2c) | 1;
    uVar2 = *(ushort *)(iVar7 + 0x2c);
    while ((uVar2 & 2) == 0) {
      uVar2 = *(ushort *)(iVar7 + 0x2c);
    }
    *(ushort *)(iVar7 + 0x2c) = *(ushort *)(iVar7 + 0x2c) | 4;
    func_0xffffe2ac(1000);
    uVar6 = *(uint *)(*(int *)(param_1 + 4) + 0x24);
    while ((uVar6 & 0x1f00000) != 0x1f00000) {
      uVar6 = *(uint *)(*(int *)(param_1 + 4) + 0x24);
    }
    if (iVar5 != 0) {
      return 1;
    }
  }
  iVar5 = FUN_00006bcc(param_1,0x200,0);
  if (iVar5 == 0) {
    iVar5 = *(int *)(param_1 + 4);
    uVar2 = *(ushort *)(iVar5 + 0x14);
    uVar3 = *(ushort *)(iVar5 + 0x18);
    uVar4 = *(ushort *)(iVar5 + 0x1c);
    *(uint *)(param_1 + 0x40) = (uint)*(ushort *)(iVar5 + 0x10);
    *(uint *)(param_1 + 0x44) = (uint)uVar2;
    *(uint *)(param_1 + 0x48) = (uint)uVar3;
    *(uint *)(param_1 + 0x4c) = (uint)uVar4;
    do {
      iVar5 = FUN_00006bcc(param_1,0x300,0);
      if (iVar5 != 0) {
        return 1;
      }
      uVar6 = *(uint *)(*(int *)(param_1 + 4) + 0x10) & 0xffff0000;
      *(uint *)(param_1 + 0x50) = uVar6;
    } while (uVar6 == 0);
    iVar5 = FUN_00006bcc(param_1,0x900);
    if (iVar5 == 0) {
      iVar5 = *(int *)(param_1 + 4);
      uVar9 = *(uint *)(iVar5 + 0x14);
      uVar6 = (*(uint *)(iVar5 + 0x1c) & 0xffffff) >> 0x16;
      if (uVar6 == 0) {
        *(uint *)(param_1 + 100) =
             ((((*(uint *)(iVar5 + 0x18) & 3) << 10 | uVar9 >> 0x16) + 1 <<
              ((uVar9 & 0x3ff) >> 7) + 2) << ((*(uint *)(iVar5 + 0x18) & 0xfff) >> 8)) >> 9;
        return 0;
      }
      if (uVar6 == 1) {
        *(uint *)(param_1 + 100) = (((uVar9 & 0x3fffffff) >> 8) + 1) * 0x400;
      }
      return 0;
    }
  }
  return 1;
}



//==================== FUN_000070e0 @ 000070e0 ====================

bool FUN_000070e0(int param_1)

{
  int iVar1;
  
  iVar1 = FUN_00006bcc(param_1,0x700,*(undefined4 *)(param_1 + 0x50),0);
  return iVar1 != 0;
}



//==================== FUN_00007100 @ 00007100 ====================

void FUN_00007100(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  int iVar9;
  int iVar10;
  bool bVar11;
  
  iVar2 = *(int *)(param_1 + 4);
  uVar8 = *(uint *)(param_1 + 0x200);
  iVar9 = *(int *)(param_1 + 0x204);
  uVar7 = *(ushort *)(iVar2 + 4) & 0xfff;
  uVar5 = param_2 * uVar7;
  if (uVar5 < 0x10000) {
    iVar1 = 0;
  }
  else {
    uVar6 = uVar5 >> 0x10;
    if ((uVar5 & 0xffff) != 0) {
      uVar6 = uVar6 + 1;
    }
    if (uVar6 == 1) {
      iVar1 = 0;
    }
    else {
      iVar3 = 0;
      iVar4 = iVar9;
      uVar5 = uVar8;
      iVar10 = param_1;
      do {
        iVar1 = iVar3 + 1;
        iVar3 = param_1 + iVar3 * 0xc;
        *(uint *)(iVar3 + 0x84) = uVar5;
        bVar11 = 0xfffeffff < uVar5;
        uVar5 = uVar5 + 0x10000;
        *(int *)(iVar3 + 0x88) = iVar4;
        iVar4 = iVar4 + (uint)bVar11;
        *(undefined2 *)(iVar10 + 0x80) = 0x21;
        *(undefined2 *)(iVar10 + 0x82) = 0;
        iVar3 = iVar1;
        iVar10 = iVar10 + 0xc;
      } while (iVar1 != uVar6 - 1);
      bVar11 = CARRY4(uVar8,iVar1 * 0x10000);
      uVar8 = uVar8 + iVar1 * 0x10000;
      iVar9 = iVar9 + (uint)bVar11;
    }
  }
  iVar1 = param_1 + iVar1 * 0xc;
  *(uint *)(iVar1 + 0x84) = uVar8;
  *(int *)(iVar1 + 0x88) = iVar9;
  *(short *)(iVar1 + 0x82) = (short)param_2 * (short)uVar7;
  *(undefined2 *)(iVar1 + 0x80) = 0x23;
  *(int *)(iVar2 + 0x58) = param_1 + 0x80;
  if (*(char *)(param_1 + 0x20) == '\0') {
    func_0xffffd370(param_1 + 0x80,0x180);
  }
  *(uint *)(param_1 + 0x200) = 0;
  *(undefined4 *)(param_1 + 0x204) = 0;
  return;
}



//==================== FUN_000071f8 @ 000071f8 ====================

/* WARNING: Control flow encountered bad instruction data */

void FUN_000071f8(int param_1,int param_2,int param_3)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  uint uVar6;
  int iVar7;
  int iVar8;
  
  iVar5 = *(int *)(param_1 + 4);
  uVar6 = *(ushort *)(iVar5 + 4) & 0xfff;
  uVar3 = param_2 * uVar6;
  if (uVar3 < 0x10000) {
    iVar7 = 0;
  }
  else {
    uVar4 = uVar3 >> 0x10;
    if ((uVar3 & 0xffff) != 0) {
      uVar4 = uVar4 + 1;
    }
    if (uVar4 == 1) {
      iVar7 = 0;
    }
    else {
      iVar2 = param_3;
      iVar1 = 0;
      iVar8 = param_1;
      do {
        iVar7 = iVar1 + 1;
        iVar1 = param_1 + iVar1 * 0xc;
        *(int *)(iVar1 + 0x84) = iVar2;
        iVar2 = iVar2 + 0x10000;
        *(undefined4 *)(iVar1 + 0x88) = 0;
        *(undefined2 *)(iVar8 + 0x80) = 0x21;
        *(undefined2 *)(iVar8 + 0x82) = 0;
        iVar1 = iVar7;
        iVar8 = iVar8 + 0xc;
      } while (iVar7 != uVar4 - 1);
      param_3 = param_3 + iVar7 * 0x10000;
    }
  }
  iVar7 = param_1 + iVar7 * 0xc;
  *(int *)(iVar7 + 0x84) = param_3;
  *(undefined4 *)(iVar7 + 0x88) = 0;
  *(short *)(iVar7 + 0x82) = (short)param_2 * (short)uVar6;
  *(undefined2 *)(iVar7 + 0x80) = 0x23;
  *(int *)(iVar5 + 0x58) = param_1 + 0x80;
  if (*(char *)(param_1 + 0x20) == '\0') {
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  return;
}



//==================== FUN_000072c8 @ 000072c8 ====================

undefined4 FUN_000072c8(int param_1,undefined4 param_2,int param_3,undefined4 param_4)

{
  int iVar1;
  undefined *puVar2;
  
  if (((((*(char *)(param_1 + 0x36) == '\x02') &&
        ((*(uint *)(param_1 + 0x28) & 0xc0000000) == 0x40000000)) || (*(int *)(param_1 + 0xc) == 0))
      || ((*(uint *)(*(int *)(param_1 + 4) + 0x24) & 0x10000) != 0)) &&
     ((*(int *)(*(int *)(param_1 + 4) + 4) == 0x200 ||
      (iVar1 = FUN_00007d40(param_1,0x200), iVar1 == 0)))) {
    if (*(int *)(param_1 + 0x204) == 0) {
      FUN_000071f8(param_1,param_3,param_4);
      if (*(char *)(param_1 + 0x20) == '\0') {
        func_0xffffd23c(param_4,param_3 << 9);
      }
    }
    else {
      FUN_00007100(param_1,param_3);
    }
    if (param_3 == 1) {
      DAT_00028e88 = 0x13;
      puVar2 = &DAT_00001100;
    }
    else {
      DAT_00028e88 = 0x37;
      puVar2 = (undefined *)0x1200;
    }
    iVar1 = FUN_00006bcc(param_1,puVar2,param_2,param_3);
    if (iVar1 == 0) {
      iVar1 = *(int *)(param_1 + 4);
      do {
        if ((*(ushort *)(iVar1 + 0x30) & 0x8000) != 0) {
          *(undefined2 *)(iVar1 + 0x32) = 0xf3ff;
          return 1;
        }
      } while ((*(ushort *)(iVar1 + 0x30) & 2) == 0);
      *(undefined2 *)(iVar1 + 0x30) = 2;
      if (*(char *)(param_1 + 0x20) == '\0') {
        func_0xffffd23c(param_4,param_3 << 9);
        return 0;
      }
      return 0;
    }
  }
  return 1;
}



//==================== FUN_00007430 @ 00007430 ====================

undefined4 FUN_00007430(int param_1,undefined4 param_2,int param_3,undefined4 param_4)

{
  int iVar1;
  undefined4 uVar2;
  
  if (((((*(char *)(param_1 + 0x36) == '\x02') &&
        ((*(uint *)(param_1 + 0x28) & 0xc0000000) == 0x40000000)) || (*(int *)(param_1 + 0xc) == 0))
      || ((*(uint *)(*(int *)(param_1 + 4) + 0x24) & 0x10000) != 0)) &&
     ((*(int *)(*(int *)(param_1 + 4) + 4) == 0x200 ||
      (iVar1 = FUN_00007d40(param_1,0x200), iVar1 == 0)))) {
    if (*(int *)(param_1 + 0x204) == 0) {
      FUN_000071f8(param_1,param_3,param_4);
      if (*(char *)(param_1 + 0x20) == '\0') {
        func_0xffffd370(param_4,param_3 << 9);
      }
    }
    else {
      FUN_00007100(param_1,param_3);
    }
    if (param_3 == 1) {
      DAT_00028e88 = 3;
      uVar2 = 0x1800;
    }
    else {
      DAT_00028e88 = 0x27;
      uVar2 = 0x1900;
    }
    iVar1 = FUN_00006bcc(param_1,uVar2,param_2,param_3);
    if (iVar1 == 0) {
      iVar1 = *(int *)(param_1 + 4);
      do {
        if ((*(ushort *)(iVar1 + 0x30) & 0x8000) != 0) {
          *(undefined2 *)(iVar1 + 0x32) = 0xf3ff;
          return 1;
        }
      } while ((*(ushort *)(iVar1 + 0x30) & 2) == 0);
      *(undefined2 *)(iVar1 + 0x30) = 2;
      return 0;
    }
  }
  return 1;
}



//==================== FUN_00007574 @ 00007574 ====================

undefined4 FUN_00007574(int param_1)

{
  ushort uVar1;
  ushort uVar2;
  ushort uVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  
  if (param_1 == 0) {
    FUN_000014e4(&DAT_0001725c,0x6ce);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 0x24) != 0x11111111) {
    FUN_000014e4(&DAT_0001725c,0x6cf);
    DAT_00028e84 = 1;
    return 0;
  }
  if (((((*(char *)(param_1 + 0x36) == '\x02') &&
        ((*(uint *)(param_1 + 0x28) & 0xc0000000) == 0x40000000)) || (*(int *)(param_1 + 0xc) == 0))
      || ((*(uint *)(*(int *)(param_1 + 4) + 0x24) & 0x10000) != 0)) &&
     (iVar4 = FUN_00006bcc(param_1,0,0), iVar4 == 0)) {
    do {
      iVar4 = FUN_00006bcc(param_1,0x100,0x40ff8000,0);
      if (iVar4 != 0) {
        return 1;
      }
      uVar5 = *(uint *)(*(int *)(param_1 + 4) + 0x10);
    } while (-1 < (int)uVar5);
    if ((uVar5 & 0x40000000) != 0) {
      *(undefined4 *)(param_1 + 0x30) = 1;
    }
    iVar4 = FUN_00006bcc(param_1,0x200,0);
    if (iVar4 == 0) {
      iVar4 = *(int *)(param_1 + 4);
      uVar1 = *(ushort *)(iVar4 + 0x14);
      uVar2 = *(ushort *)(iVar4 + 0x18);
      uVar3 = *(ushort *)(iVar4 + 0x1c);
      *(uint *)(param_1 + 0x40) = (uint)*(ushort *)(iVar4 + 0x10);
      *(uint *)(param_1 + 0x44) = (uint)uVar1;
      *(uint *)(param_1 + 0x4c) = (uint)uVar3;
      *(uint *)(param_1 + 0x48) = (uint)uVar2;
      *(undefined4 *)(param_1 + 0x50) = 0x12340000;
      iVar4 = FUN_00006bcc(param_1,0x300,0x12340000);
      if ((iVar4 == 0) &&
         (iVar4 = FUN_00006bcc(param_1,0x900,*(undefined4 *)(param_1 + 0x50)), iVar4 == 0)) {
        iVar4 = *(int *)(param_1 + 4);
        uVar6 = *(uint *)(iVar4 + 0x14);
        uVar5 = *(uint *)(iVar4 + 0x18);
        *(char *)(param_1 + 0x35) = (char)((*(uint *)(iVar4 + 0x1c) & 0x3fffff) >> 0x12);
        *(uint *)(param_1 + 100) =
             ((((uVar5 & 3) << 10 | uVar6 >> 0x16) + 1 << ((uVar6 & 0x3ff) >> 7) + 2) <<
             ((uVar5 & 0xfff) >> 8)) >> 9;
        return 0;
      }
    }
  }
  return 1;
}



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
  *(undefined **)(param_1 + 0x38) = &DAT_00061a80;
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



//==================== FUN_00007d40 @ 00007d40 ====================

undefined4 FUN_00007d40(int param_1,undefined4 param_2)

{
  int iVar1;
  
  if (param_1 == 0) {
    FUN_000014e4(&DAT_00017264,0x7b);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 0x24) != 0x11111111) {
    FUN_000014e4(&DAT_00017264,0x7c);
    DAT_00028e84 = 1;
    return 0;
  }
  if (((*(uint *)(*(int *)(param_1 + 4) + 0x24) & 0x303) == 0) &&
     (iVar1 = FUN_00006bcc(param_1,0x1000,param_2), iVar1 == 0)) {
    *(ushort *)(*(int *)(param_1 + 4) + 4) = (ushort)param_2 & 0xfff;
    return 0;
  }
  return 1;
}



//==================== FUN_00007dfc @ 00007dfc ====================

undefined4 FUN_00007dfc(int param_1,int param_2)

{
  int iVar1;
  undefined1 *puVar2;
  
  if (param_1 == 0) {
    FUN_000014e4(&DAT_00017264,0xb3);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 0x24) != 0x11111111) {
    FUN_000014e4(&DAT_00017264,0xb4);
    DAT_00028e84 = 1;
    return 0;
  }
  puVar2 = (undefined1 *)(param_2 + -1);
  do {
    puVar2 = puVar2 + 1;
    *puVar2 = 0;
  } while (puVar2 != (undefined1 *)(param_2 + 7));
  iVar1 = FUN_00006bcc(param_1,0x3700,*(undefined4 *)(param_1 + 0x50),0);
  if (iVar1 == 0) {
    *(undefined2 *)(*(int *)(param_1 + 4) + 4) = 8;
    FUN_000071f8(param_1,1,param_2);
    DAT_00028e88 = 0x11;
    if (*(char *)(param_1 + 0x20) == '\0') {
      func_0xffffd23c(param_2,8);
    }
    iVar1 = FUN_00006bcc(param_1,0xb300,0,1);
    if (iVar1 == 0) {
      iVar1 = *(int *)(param_1 + 4);
      do {
        if ((*(ushort *)(iVar1 + 0x30) & 0x8000) != 0) {
          *(undefined2 *)(iVar1 + 0x32) = 0xf3ff;
          return 1;
        }
      } while ((*(ushort *)(iVar1 + 0x30) & 2) == 0);
      *(undefined2 *)(iVar1 + 0x30) = 2;
      return 0;
    }
  }
  return 1;
}



//==================== FUN_00007f5c @ 00007f5c ====================

undefined4 FUN_00007f5c(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  byte bVar3;
  
  if (param_1 == 0) {
    FUN_000014e4(&DAT_00017264,0x109);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 0x24) != 0x11111111) {
    FUN_000014e4(&DAT_00017264,0x10a);
    DAT_00028e84 = 1;
    return 0;
  }
  if ((*(char *)(param_1 + 0x36) != '\x02') || (3 < *(uint *)(param_1 + 0x14))) {
    if (*(char *)(param_1 + 0x34) == '\x01') {
      iVar1 = FUN_00006bcc(param_1,0x3700,*(undefined4 *)(param_1 + 0x50),0);
      if (iVar1 != 0) {
        return 1;
      }
      *(undefined1 *)(param_1 + 0x37) = 2;
      iVar1 = FUN_00006bcc(param_1,0x8600);
      if (iVar1 != 0) {
        return 1;
      }
    }
    else {
      if (((*(uint *)(param_1 + 0x34) & 0xff00ff) == 0x20005) && (*(int *)(param_1 + 0x14) == 8)) {
        if (*(int *)(param_1 + 0x6c) == 4) {
          uVar2 = 0x3b70600;
        }
        else {
          uVar2 = 0x3b70200;
        }
        *(undefined1 *)(param_1 + 0x37) = 3;
      }
      else {
        if (*(int *)(param_1 + 0x6c) == 4) {
          uVar2 = 0x3b70500;
        }
        else {
          uVar2 = 0x3b70100;
        }
        *(undefined1 *)(param_1 + 0x37) = 2;
      }
      iVar1 = FUN_00006bcc(param_1,0x600,uVar2,0);
      if (iVar1 != 0) {
        return 1;
      }
      iVar1 = *(int *)(param_1 + 4);
      do {
        if ((*(ushort *)(iVar1 + 0x30) & 0x8000) != 0) {
          *(undefined2 *)(iVar1 + 0x32) = 0xf3ff;
          return 1;
        }
      } while ((*(ushort *)(iVar1 + 0x30) & 2) == 0);
      *(undefined2 *)(iVar1 + 0x30) = 2;
    }
    func_0xffffe2ac(1000);
    bVar3 = *(byte *)(*(int *)(param_1 + 4) + 0x28);
    if (*(char *)(param_1 + 0x37) == '\x03') {
      bVar3 = bVar3 | 0x20;
    }
    else {
      bVar3 = bVar3 | 2;
    }
    *(byte *)(*(int *)(param_1 + 4) + 0x28) = bVar3;
    if (*(int *)(param_1 + 0x6c) == 4) {
      *(ushort *)(*(int *)(param_1 + 4) + 0x3e) =
           *(ushort *)(*(int *)(param_1 + 4) + 0x3e) & 0xfff8 | 4;
      return 0;
    }
  }
  return 0;
}



//==================== FUN_00008168 @ 00008168 ====================

undefined4 FUN_00008168(int param_1,int param_2)

{
  int iVar1;
  undefined1 *puVar2;
  
  if (param_1 == 0) {
    FUN_000014e4(&DAT_00017264,0x193);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 0x24) != 0x11111111) {
    FUN_000014e4(&DAT_00017264,0x194);
    DAT_00028e84 = 1;
    return 0;
  }
  puVar2 = (undefined1 *)(param_2 + -1);
  do {
    puVar2 = puVar2 + 1;
    *puVar2 = 0;
  } while (puVar2 != (undefined1 *)(param_2 + 0x3f));
  *(undefined2 *)(*(int *)(param_1 + 4) + 4) = 0x40;
  FUN_000071f8(param_1,1,param_2);
  DAT_00028e88 = 0x11;
  if (*(char *)(param_1 + 0x20) == '\0') {
    func_0xffffd23c(param_2,0x40);
  }
  iVar1 = FUN_00006bcc(param_1,0x600,0xfffff0,1);
  if (iVar1 != 0) {
    return 1;
  }
  iVar1 = *(int *)(param_1 + 4);
  do {
    if ((*(ushort *)(iVar1 + 0x30) & 0x8000) != 0) {
      *(undefined2 *)(iVar1 + 0x32) = 0xf3ff;
      return 1;
    }
  } while ((*(ushort *)(iVar1 + 0x30) & 2) == 0);
  *(undefined2 *)(iVar1 + 0x30) = 2;
  return 0;
}



//==================== FUN_000082b0 @ 000082b0 ====================

undefined4 FUN_000082b0(int param_1,uint param_2)

{
  ushort uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  undefined4 *puVar5;
  undefined4 uVar6;
  int iVar7;
  bool bVar8;
  
  if (param_1 == 0) {
    FUN_000014e4(&DAT_00017264,0x2a4);
    DAT_00028e84 = 1;
    return 0;
  }
  puVar5 = &DAT_00028e84;
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 0x24) != 0x11111111) {
    FUN_000014e4(&DAT_00017264,0x2a5);
    DAT_00028e84 = 1;
    return 0;
  }
  iVar7 = *(int *)(param_1 + 4);
  uVar6 = *(undefined4 *)(param_1 + 8);
  *(ushort *)(iVar7 + 0x2c) = *(ushort *)(iVar7 + 0x2c) & 0xfffa;
  bVar8 = *(char *)(param_1 + 0x36) == '\x02';
  if (!bVar8) {
    puVar5 = (undefined4 *)0x9;
  }
  uVar4 = 1;
  if (bVar8) {
    uVar4 = 1;
    do {
      uVar2 = FUN_00001a50(uVar6,uVar4);
      uVar3 = uVar4 & 0xffff;
      uVar4 = uVar4 + 1;
      if (uVar2 <= param_2) {
        *(ushort *)(iVar7 + 0x2c) =
             *(ushort *)(iVar7 + 0x2c) & 0x3f | 1 | (ushort)((uVar3 >> 9) << 6) |
             (ushort)((uVar3 >> 1) << 8);
        goto LAB_0000837c;
      }
    } while (uVar4 != 0x7ff);
  }
  else {
    do {
      uVar2 = FUN_00001a50(uVar6,uVar4);
      if (uVar2 <= param_2) {
        *(ushort *)(iVar7 + 0x2c) =
             *(ushort *)(iVar7 + 0x2c) & 0xff | 1 | (ushort)((uVar4 >> 1) << 8);
LAB_0000837c:
        uVar1 = *(ushort *)(iVar7 + 0x2c);
        while ((uVar1 & 2) == 0) {
          uVar1 = *(ushort *)(iVar7 + 0x2c);
        }
        *(ushort *)(iVar7 + 0x2c) = *(ushort *)(iVar7 + 0x2c) | 4;
        return 0;
      }
      puVar5 = (undefined4 *)((int)puVar5 + -1);
      uVar4 = (uVar4 & 0x7fff) << 1;
    } while (puVar5 != (undefined4 *)0x0);
  }
  return 1;
}



//==================== FUN_00008428 @ 00008428 ====================

undefined4 FUN_00008428(int param_1)

{
  int iVar1;
  undefined *puVar2;
  char cVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined2 uVar6;
  undefined1 auStack_58 [68];
  
  FUN_000027e0(auStack_58,0,0x40);
  if (param_1 == 0) {
    FUN_000014e4(&DAT_00017264,0x1e4);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 0x24) != 0x11111111) {
    FUN_000014e4(&DAT_00017264,0x1e5);
    DAT_00028e84 = 1;
    return 0;
  }
  if (*(char *)(param_1 + 0x34) == '\x01') {
    *(undefined2 *)(*(int *)(param_1 + 4) + 4) = 0x40;
    FUN_000071f8(param_1,1,auStack_58);
    if (*(char *)(param_1 + 0x20) == '\0') {
      func_0xffffd370(auStack_58,0x40);
    }
    DAT_00028e88 = 0x11;
    iVar1 = FUN_00006bcc(param_1,0x600,0x80fffff1,1);
    if (iVar1 != 0) {
      return 1;
    }
    iVar1 = *(int *)(param_1 + 4);
    do {
      if ((*(ushort *)(iVar1 + 0x30) & 0x8000) != 0) goto LAB_000085f8;
    } while ((*(ushort *)(iVar1 + 0x30) & 2) == 0);
    uVar5 = 50000000;
  }
  else {
    if (*(char *)(param_1 + 0x34) != '\x02') {
      if (*(int *)(param_1 + 0x6c) == 7) {
        uVar5 = 200000000;
        uVar4 = 0x3b90200;
      }
      else {
        uVar5 = 52000000;
        uVar4 = 0x3b90100;
      }
      *(undefined4 *)(param_1 + 0x38) = uVar5;
      iVar1 = FUN_00006bcc(param_1,0x600,uVar4,0);
      if (iVar1 != 0) {
        return 1;
      }
      iVar1 = *(int *)(param_1 + 4);
      do {
        if ((*(ushort *)(iVar1 + 0x30) & 0x8000) != 0) {
          *(undefined2 *)(iVar1 + 0x32) = 0xf3ff;
          return 1;
        }
      } while ((*(ushort *)(iVar1 + 0x30) & 2) == 0);
      uVar5 = *(undefined4 *)(param_1 + 0x38);
      *(undefined2 *)(iVar1 + 0x30) = 2;
      iVar1 = FUN_000082b0(param_1,uVar5);
      if (iVar1 != 0) {
        return 1;
      }
      if (*(int *)(param_1 + 0x6c) == 7) {
        DAT_00028e84 = iVar1;
        if (*(int *)(param_1 + 0x24) == 0x11111111) {
          iVar1 = *(int *)(param_1 + 4);
          cVar3 = '(';
          if (*(char *)(param_1 + 0x37) == '\x03') {
            uVar6 = 0x80;
          }
          else {
            uVar6 = 0x40;
          }
          *(undefined2 *)(iVar1 + 4) = uVar6;
          DAT_00028e88 = 0x10;
          *(ushort *)(iVar1 + 0x3e) = *(ushort *)(iVar1 + 0x3e) | 0x40;
          func_0xffffe2ac(1);
          do {
            puVar2 = (undefined *)0x1500;
            if (*(char *)(param_1 + 0x34) == '\x01') {
              puVar2 = &DAT_00001300;
            }
            iVar1 = FUN_00006bcc(param_1,puVar2,0,1);
            cVar3 = cVar3 + -1;
            if (iVar1 != 0) {
              return 1;
            }
          } while (((*(ushort *)(*(int *)(param_1 + 4) + 0x3e) & 0x40) != 0) && (cVar3 != '\0'));
          if ((*(ushort *)(*(int *)(param_1 + 4) + 0x3e) & 0x80) == 0) {
            return 1;
          }
          func_0xffffe2ac(1);
        }
        else {
          FUN_000014e4(&DAT_00017264,0x475);
          DAT_00028e84 = 1;
        }
      }
      goto LAB_00008630;
    }
    iVar1 = FUN_00006bcc(param_1,0x600,0x3b90100,0);
    if (iVar1 != 0) {
      return 1;
    }
    iVar1 = *(int *)(param_1 + 4);
    do {
      if ((*(ushort *)(iVar1 + 0x30) & 0x8000) != 0) {
LAB_000085f8:
        *(undefined2 *)(iVar1 + 0x32) = 0xf3ff;
        return 1;
      }
    } while ((*(ushort *)(iVar1 + 0x30) & 2) == 0);
    uVar5 = 52000000;
  }
  *(undefined2 *)(iVar1 + 0x30) = 2;
  *(undefined4 *)(param_1 + 0x38) = uVar5;
  iVar1 = FUN_000082b0(param_1,uVar5);
  if (iVar1 != 0) {
    return 1;
  }
LAB_00008630:
  func_0xffffe2ac(1000);
  *(byte *)(*(int *)(param_1 + 4) + 0x28) = *(byte *)(*(int *)(param_1 + 4) + 0x28) | 4;
  return 0;
}



//==================== FUN_00008798 @ 00008798 ====================

bool FUN_00008798(int param_1)

{
  int iVar1;
  
  if (param_1 == 0) {
    FUN_000014e4(&DAT_00017264,0x319);
    DAT_00028e84 = 1;
    return false;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 0x24) != 0x11111111) {
    FUN_000014e4(&DAT_00017264,0x31a);
    DAT_00028e84 = 1;
    return false;
  }
  iVar1 = FUN_00006bcc(param_1,0x3700,*(undefined4 *)(param_1 + 0x50),0);
  if (iVar1 == 0) {
    iVar1 = FUN_00006bcc(param_1,0xaa00,0);
    return iVar1 != 0;
  }
  return true;
}



//==================== FUN_0000884c @ 0000884c ====================

undefined4 FUN_0000884c(int param_1,int param_2)

{
  int iVar1;
  undefined1 *puVar2;
  
  if (param_1 == 0) {
    FUN_000014e4(&DAT_00017264,0x349);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 0x24) != 0x11111111) {
    FUN_000014e4(&DAT_00017264,0x34a);
    DAT_00028e84 = 1;
    return 0;
  }
  puVar2 = (undefined1 *)(param_2 + -1);
  do {
    puVar2 = puVar2 + 1;
    *puVar2 = 0;
  } while (puVar2 != (undefined1 *)(param_2 + 0x1ff));
  *(undefined2 *)(*(int *)(param_1 + 4) + 4) = 0x200;
  FUN_000071f8(param_1,1,param_2);
  if (*(char *)(param_1 + 0x20) == '\0') {
    func_0xffffd23c(param_2,0x200);
  }
  DAT_00028e88 = 0x11;
  iVar1 = FUN_00006bcc(param_1,0x800,0,1);
  if (iVar1 != 0) {
    return 1;
  }
  iVar1 = *(int *)(param_1 + 4);
  do {
    if ((*(ushort *)(iVar1 + 0x30) & 0x8000) != 0) {
      *(undefined2 *)(iVar1 + 0x32) = 0xf3ff;
      return 1;
    }
  } while ((*(ushort *)(iVar1 + 0x30) & 2) == 0);
  *(undefined2 *)(iVar1 + 0x30) = 2;
  return 0;
}



//==================== FUN_00008994 @ 00008994 ====================

undefined4 FUN_00008994(int param_1,undefined4 param_2)

{
  int iVar1;
  
  iVar1 = FUN_00006bcc(param_1,0x600,param_2,0);
  if (iVar1 != 0) {
    return 1;
  }
  iVar1 = *(int *)(param_1 + 4);
  do {
    if ((*(ushort *)(iVar1 + 0x30) & 0x8000) != 0) {
      *(undefined2 *)(iVar1 + 0x32) = 0xf3ff;
      return 1;
    }
  } while ((*(ushort *)(iVar1 + 0x30) & 2) == 0);
  *(undefined2 *)(iVar1 + 0x30) = 2;
  return 0;
}



//==================== FUN_00008a00 @ 00008a00 ====================

int FUN_00008a00(uint param_1)

{
  uint local_10;
  int local_c;
  
  local_10 = param_1 & 1;
  for (local_c = 2; local_c != 0; local_c = local_c * local_c) {
    local_10 = (2 - local_10 * param_1) * local_10;
  }
  return -local_10;
}



//==================== FUN_00008a78 @ 00008a78 ====================

void FUN_00008a78(undefined4 param_1,uint *param_2,uint *param_3,uint *param_4,int param_5)

{
  ulonglong uVar1;
  longlong lVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  uint *puVar6;
  uint uVar7;
  uint *local_16c;
  uint local_15c [63];
  int local_60;
  uint local_5c;
  undefined8 local_58;
  undefined4 local_4c;
  int local_48;
  uint local_44;
  uint local_40;
  uint *local_3c;
  uint *local_38;
  uint *local_34;
  int local_30;
  uint local_2c;
  
  FUN_000027e0(local_15c,0,0x104);
  local_16c = param_3;
  for (local_2c = 0; local_2c < 0x40; local_2c = local_2c + 1) {
    local_40 = *local_16c;
    local_16c = local_16c + 1;
    uVar1 = (ulonglong)local_40 * (ulonglong)*param_2 + (ulonglong)local_15c[0];
    local_34 = param_2 + 1;
    local_3c = local_15c + 1;
    local_48 = (int)uVar1;
    local_44 = (uint)(uVar1 >> 0x20);
    uVar5 = local_44;
    uVar7 = param_5 * local_48;
    lVar2 = (ulonglong)uVar7 * (ulonglong)*param_4 + (uVar1 & 0xffffffff);
    local_38 = param_4 + 1;
    local_48 = (int)lVar2;
    local_4c = local_48;
    local_44 = (uint)((ulonglong)lVar2 >> 0x20);
    local_30 = 0x3f;
    do {
      uVar4 = *local_34;
      local_34 = local_34 + 1;
      uVar1 = (ulonglong)local_40 * (ulonglong)uVar4 + (ulonglong)uVar5 + (ulonglong)local_44;
      uVar5 = (uint)(uVar1 >> 0x20);
      lVar2 = (ulonglong)uVar7 * (ulonglong)*local_38 + (ulonglong)*local_3c;
      local_38 = local_38 + 1;
      local_58 = lVar2 + (uVar1 & 0xffffffff);
      puVar6 = local_3c + -1;
      local_3c = local_3c + 1;
      *puVar6 = (uint)(lVar2 + (uVar1 & 0xffffffff));
      local_30 = local_30 + -1;
      local_44 = local_58._4_4_;
    } while (local_30 != 0);
    local_44 = (uint)CARRY4(uVar5,local_5c);
    local_60 = local_58._4_4_ + uVar5 + local_5c;
    local_5c = local_44 + CARRY4(local_58._4_4_,uVar5 + local_5c);
    _local_48 = CONCAT44(local_5c,local_60);
    local_30 = 0;
  }
  if ((local_5c != 0) || (iVar3 = FUN_00008df8(local_15c,param_4), iVar3 != 0)) {
    FUN_00008eac(local_15c,param_4);
  }
  FUN_00002200(param_1,local_15c,0x100);
  return;
}



//==================== FUN_00008df8 @ 00008df8 ====================

bool FUN_00008df8(int param_1,int param_2)

{
  int local_c;
  
  local_c = 0x40;
  do {
    local_c = local_c + -1;
    if (*(int *)(param_1 + local_c * 4) != *(int *)(param_2 + local_c * 4)) break;
  } while (0 < local_c);
  return *(uint *)(param_2 + local_c * 4) < *(uint *)(param_1 + local_c * 4);
}



//==================== FUN_00008eac @ 00008eac ====================

void FUN_00008eac(uint *param_1,uint *param_2)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint *local_28;
  uint *local_24;
  uint local_18;
  uint local_14;
  
  local_14 = 0;
  local_28 = param_2;
  local_24 = param_1;
  for (local_18 = 0; local_18 < 0x40; local_18 = local_18 + 1) {
    uVar1 = *local_24;
    uVar3 = *local_28;
    uVar2 = uVar1 - uVar3;
    local_28 = local_28 + 1;
    *local_24 = uVar2 - local_14;
    local_24 = local_24 + 1;
    local_14 = (uint)(-(uint)(uVar2 < local_14) != (uint)(uVar1 < uVar3));
  }
  return;
}



//==================== FUN_00008f80 @ 00008f80 ====================

void FUN_00008f80(undefined4 param_1,undefined4 param_2,uint param_3,undefined4 *param_4,
                 undefined4 param_5)

{
  undefined1 auStack_218 [256];
  undefined4 local_118 [64];
  uint local_18;
  undefined4 local_14;
  uint local_10;
  uint local_c;
  
  local_c = 0;
  local_10 = 0x80000000;
  FUN_000027e0(local_118,0,0x100);
  local_118[0] = 1;
  local_14 = FUN_00008a00(*param_4);
  FUN_00008a78(auStack_218,param_2,param_5,param_4,local_14);
  FUN_00008a78(param_1,param_5,local_118,param_4,local_14);
  for (; local_10 != 0; local_10 = local_10 >> 1) {
    local_18 = param_3 & local_10;
    if (local_c != 0) {
      FUN_00008a78(param_1,param_1,param_1,param_4,local_14);
    }
    if (local_c == 0) {
      local_c = local_18;
    }
    if (local_18 != 0) {
      FUN_00008a78(param_1,param_1,auStack_218,param_4,local_14);
    }
  }
  FUN_00008a78(param_1,param_1,local_118,param_4,local_14);
  return;
}



//==================== FUN_000090d8 @ 000090d8 ====================

void FUN_000090d8(undefined4 param_1,undefined4 *param_2,undefined4 param_3,int param_4,
                 undefined4 param_5)

{
  undefined1 auStack_234 [256];
  undefined4 local_134 [64];
  uint local_34;
  uint local_30;
  undefined4 local_2c;
  undefined4 local_28;
  int local_24;
  undefined4 local_20;
  undefined4 *local_1c;
  undefined4 local_18;
  int local_14;
  uint local_10;
  uint local_c;
  
  local_c = 0;
  local_28 = param_5;
  local_24 = param_4;
  local_20 = param_3;
  local_1c = param_2;
  local_18 = param_1;
  FUN_000027e0(local_134,0,0x100);
  local_134[0] = 1;
  local_2c = FUN_00008a00(*local_1c);
  FUN_00008a78(auStack_234,local_18,local_20,local_1c,local_2c);
  FUN_00008a78(local_28,local_20,local_134,local_1c,local_2c);
  for (local_14 = 0x3f; -1 < local_14; local_14 = local_14 + -1) {
    local_30 = *(uint *)(local_24 + local_14 * 4);
    for (local_10 = 0x80000000; local_10 != 0; local_10 = local_10 >> 1) {
      local_34 = local_30 & local_10;
      if (local_c != 0) {
        FUN_00008a78(local_28,local_28,local_28,local_1c,local_2c);
      }
      if (local_c == 0) {
        local_c = local_34;
      }
      if (local_34 != 0) {
        FUN_00008a78(local_28,local_28,auStack_234,local_1c,local_2c);
      }
    }
  }
  FUN_00008a78(local_28,local_28,local_134,local_1c,local_2c);
  return;
}



//==================== FUN_00009294 @ 00009294 ====================

void FUN_00009294(int param_1)

{
  FUN_00009974(param_1);
  *(undefined4 *)(param_1 + 0x60) = 0;
  *(undefined4 *)(param_1 + 100) = 0;
  return;
}



//==================== FUN_000092c8 @ 000092c8 ====================

void FUN_000092c8(int param_1,int param_2,uint param_3)

{
  uint uVar1;
  uint local_64;
  int local_60;
  uint auStack_54 [16];
  uint local_14;
  uint local_10;
  uint local_c;
  
  local_10 = *(uint *)(param_1 + 0x60) & 0x3f;
  local_14 = 0x40 - local_10;
  if (param_3 != 0) {
    uVar1 = *(uint *)(param_1 + 0x60);
    *(uint *)(param_1 + 0x60) = param_3 + uVar1;
    *(uint *)(param_1 + 100) = *(int *)(param_1 + 100) + (uint)CARRY4(param_3,uVar1);
    local_64 = param_3;
    local_60 = param_2;
    if ((local_10 != 0) && (local_14 <= param_3)) {
      FUN_00002200(param_1 + local_10 + 0x20,param_2,local_14);
      for (local_c = 0; local_c < 0x10; local_c = local_c + 1) {
        auStack_54[local_c] =
             (uint)*(byte *)(param_1 + local_c * 4 + 0x20) << 0x18 |
             (uint)*(byte *)(param_1 + local_c * 4 + 0x21) << 0x10 |
             (uint)*(byte *)(param_1 + local_c * 4 + 0x22) << 8 |
             (uint)*(byte *)(param_1 + local_c * 4 + 0x23);
      }
      FUN_000099a0(param_1,auStack_54);
      local_64 = param_3 - local_14;
      local_60 = param_2 + local_14;
      local_10 = 0;
    }
    for (; 0x3f < local_64; local_64 = local_64 - 0x40) {
      for (local_c = 0; local_c < 0x10; local_c = local_c + 1) {
        auStack_54[local_c] =
             (uint)*(byte *)(local_60 + local_c * 4) << 0x18 |
             (uint)*(byte *)(local_60 + local_c * 4 + 1) << 0x10 |
             (uint)*(byte *)(local_60 + local_c * 4 + 2) << 8 |
             (uint)*(byte *)(local_60 + local_c * 4 + 3);
      }
      FUN_000099a0(param_1,auStack_54);
      local_60 = local_60 + 0x40;
    }
    if (local_64 != 0) {
      FUN_00002200(param_1 + local_10 + 0x20,local_60,local_64);
    }
  }
  return;
}



//==================== FUN_000095a0 @ 000095a0 ====================

void FUN_000095a0(int param_1,int param_2)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  undefined1 local_34;
  undefined1 local_33;
  undefined1 local_32;
  undefined1 local_31;
  undefined1 local_30;
  undefined1 local_2f;
  undefined1 local_2e;
  undefined1 local_2d;
  int local_2c;
  uint local_28;
  uint local_24;
  
  local_34 = (undefined1)((*(uint *)(param_1 + 100) & 0x1fffffff) >> 0x15);
  uVar1 = (*(uint *)(param_1 + 100) & 0x1fffffff) >> 0xd;
  local_33 = (undefined1)uVar1;
  uVar2 = (*(uint *)(param_1 + 100) & 0x1fffffff) >> 5;
  local_32 = (undefined1)uVar2;
  uVar6 = *(uint *)(param_1 + 0x60) >> 0x1d | *(int *)(param_1 + 100) << 3;
  local_31 = (undefined1)uVar6;
  uVar7 = *(uint *)(param_1 + 0x60);
  uVar3 = (uVar7 & 0x1fffffff) >> 0x15;
  local_30 = (undefined1)uVar3;
  uVar8 = *(uint *)(param_1 + 0x60);
  uVar4 = (uVar8 & 0x1fffffff) >> 0xd;
  local_2f = (undefined1)uVar4;
  uVar9 = *(uint *)(param_1 + 0x60);
  uVar5 = (uVar9 & 0x1fffffff) >> 5;
  local_2e = (undefined1)uVar5;
  local_2d = (undefined1)((*(uint *)(param_1 + 0x60) & 0xff) << 3);
  local_28 = *(uint *)(param_1 + 0x60) & 0x3f;
  if (local_28 < 0x38) {
    local_2c = 0x38 - local_28;
  }
  else {
    local_2c = 0x78 - local_28;
  }
  FUN_000092c8(param_1,&DAT_00019c10,local_2c,local_2c,uVar5,0,uVar9 << 3,
               uVar9 >> 0x1d | *(int *)(param_1 + 100) << 3,uVar4,0,uVar8 << 3,
               uVar8 >> 0x1d | *(int *)(param_1 + 100) << 3,uVar3,0,uVar7 << 3,
               uVar7 >> 0x1d | *(int *)(param_1 + 100) << 3,uVar6,0,*(uint *)(param_1 + 0x60) << 3,
               uVar6,uVar2,0,*(uint *)(param_1 + 0x60) << 3,
               *(uint *)(param_1 + 0x60) >> 0x1d | *(uint *)(param_1 + 100) << 3,uVar1,0);
  FUN_000092c8(param_1,&local_34,8);
  for (local_24 = 0; local_24 < 8; local_24 = local_24 + 1) {
    *(char *)(param_2 + local_24 * 4) =
         (char)((uint)*(undefined4 *)(param_1 + local_24 * 4) >> 0x18);
    *(char *)(param_2 + local_24 * 4 + 1) =
         (char)((uint)*(undefined4 *)(param_1 + local_24 * 4) >> 0x10);
    *(char *)(param_2 + local_24 * 4 + 2) =
         (char)((uint)*(undefined4 *)(param_1 + local_24 * 4) >> 8);
    *(char *)(param_2 + local_24 * 4 + 3) = (char)*(undefined4 *)(param_1 + local_24 * 4);
  }
  return;
}



//==================== FUN_00009924 @ 00009924 ====================

void FUN_00009924(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  undefined1 auStack_70 [104];
  
  FUN_00009294(auStack_70);
  FUN_000092c8(auStack_70,param_1,param_2);
  FUN_000095a0(auStack_70,param_3);
  return;
}



//==================== FUN_00009974 @ 00009974 ====================

void FUN_00009974(undefined4 param_1)

{
  FUN_00002200(param_1,&DAT_00015b98,0x20);
  return;
}



//==================== FUN_000099a0 @ 000099a0 ====================

void FUN_000099a0(int param_1,uint *param_2)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  uint local_74;
  uint local_70;
  uint local_6c;
  uint local_68;
  uint local_64;
  uint local_60;
  uint local_5c;
  uint local_58;
  uint local_54;
  uint local_50;
  uint local_4c;
  uint local_48;
  uint local_44;
  uint local_40;
  uint local_3c;
  uint local_38;
  uint local_34 [8];
  uint local_14;
  
  FUN_00002200(local_34,param_1,0x20);
  for (local_14 = 0; local_14 < 0x40; local_14 = local_14 + 0x10) {
    if (local_14 == 0) {
      local_74 = *param_2;
    }
    else {
      local_74 = local_74 +
                 ((local_3c >> 0x11 | local_3c << 0xf) ^ (local_3c >> 0x13 | local_3c << 0xd) ^
                 local_3c >> 10) + local_50 +
                 ((local_70 >> 7 | local_70 << 0x19) ^ (local_70 >> 0x12 | local_70 << 0xe) ^
                 local_70 >> 3);
    }
    iVar1 = local_34[7] +
            ((local_34[4] >> 6 | local_34[4] << 0x1a) ^ (local_34[4] >> 0xb | local_34[4] << 0x15) ^
            (local_34[4] >> 0x19 | local_34[4] << 7)) +
            (local_34[6] ^ local_34[4] & (local_34[5] ^ local_34[6])) +
            *(int *)(&DAT_00015bb8 + local_14 * 4) + local_74;
    local_34[3] = local_34[3] + iVar1;
    uVar2 = iVar1 + ((local_34[0] >> 2 | local_34[0] << 0x1e) ^
                     (local_34[0] >> 0xd | local_34[0] << 0x13) ^
                    (local_34[0] >> 0x16 | local_34[0] << 10)) +
                    (local_34[1] ^ (local_34[0] ^ local_34[1]) & (local_34[1] ^ local_34[2]));
    if (local_14 == 0) {
      local_70 = param_2[1];
    }
    else {
      local_70 = local_70 +
                 ((local_38 >> 0x11 | local_38 << 0xf) ^ (local_38 >> 0x13 | local_38 << 0xd) ^
                 local_38 >> 10) + local_4c +
                 ((local_6c >> 7 | local_6c << 0x19) ^ (local_6c >> 0x12 | local_6c << 0xe) ^
                 local_6c >> 3);
    }
    iVar1 = local_34[6] +
            ((local_34[3] >> 6 | local_34[3] * 0x4000000) ^
             (local_34[3] >> 0xb | local_34[3] * 0x200000) ^
            (local_34[3] >> 0x19 | local_34[3] * 0x80)) +
            (local_34[5] ^ local_34[3] & (local_34[4] ^ local_34[5])) +
            *(int *)(&DAT_00015bb8 + (local_14 + 1) * 4) + local_70;
    local_34[2] = local_34[2] + iVar1;
    uVar3 = iVar1 + ((uVar2 >> 2 | uVar2 * 0x40000000) ^ (uVar2 >> 0xd | uVar2 * 0x80000) ^
                    (uVar2 >> 0x16 | uVar2 * 0x400)) +
                    (local_34[0] ^ (uVar2 ^ local_34[0]) & (local_34[0] ^ local_34[1]));
    if (local_14 == 0) {
      local_6c = param_2[2];
    }
    else {
      local_6c = local_6c +
                 ((local_74 >> 0x11 | local_74 << 0xf) ^ (local_74 >> 0x13 | local_74 << 0xd) ^
                 local_74 >> 10) + local_48 +
                 ((local_68 >> 7 | local_68 << 0x19) ^ (local_68 >> 0x12 | local_68 << 0xe) ^
                 local_68 >> 3);
    }
    iVar1 = local_34[5] +
            ((local_34[2] >> 6 | local_34[2] * 0x4000000) ^
             (local_34[2] >> 0xb | local_34[2] * 0x200000) ^
            (local_34[2] >> 0x19 | local_34[2] * 0x80)) +
            (local_34[4] ^ local_34[2] & (local_34[3] ^ local_34[4])) +
            *(int *)(&DAT_00015bb8 + (local_14 + 2) * 4) + local_6c;
    local_34[1] = local_34[1] + iVar1;
    uVar4 = iVar1 + ((uVar3 >> 2 | uVar3 * 0x40000000) ^ (uVar3 >> 0xd | uVar3 * 0x80000) ^
                    (uVar3 >> 0x16 | uVar3 * 0x400)) +
                    (uVar2 ^ (uVar3 ^ uVar2) & (uVar2 ^ local_34[0]));
    if (local_14 == 0) {
      local_68 = param_2[3];
    }
    else {
      local_68 = local_68 +
                 ((local_70 >> 0x11 | local_70 << 0xf) ^ (local_70 >> 0x13 | local_70 << 0xd) ^
                 local_70 >> 10) + local_44 +
                 ((local_64 >> 7 | local_64 << 0x19) ^ (local_64 >> 0x12 | local_64 << 0xe) ^
                 local_64 >> 3);
    }
    iVar1 = local_34[4] +
            ((local_34[1] >> 6 | local_34[1] * 0x4000000) ^
             (local_34[1] >> 0xb | local_34[1] * 0x200000) ^
            (local_34[1] >> 0x19 | local_34[1] * 0x80)) +
            (local_34[3] ^ local_34[1] & (local_34[2] ^ local_34[3])) +
            *(int *)(&DAT_00015bb8 + (local_14 + 3) * 4) + local_68;
    local_34[0] = local_34[0] + iVar1;
    uVar5 = iVar1 + ((uVar4 >> 2 | uVar4 * 0x40000000) ^ (uVar4 >> 0xd | uVar4 * 0x80000) ^
                    (uVar4 >> 0x16 | uVar4 * 0x400)) + (uVar3 ^ (uVar4 ^ uVar3) & (uVar3 ^ uVar2));
    if (local_14 == 0) {
      local_64 = param_2[4];
    }
    else {
      local_64 = local_64 +
                 ((local_6c >> 0x11 | local_6c << 0xf) ^ (local_6c >> 0x13 | local_6c << 0xd) ^
                 local_6c >> 10) + local_40 +
                 ((local_60 >> 7 | local_60 << 0x19) ^ (local_60 >> 0x12 | local_60 << 0xe) ^
                 local_60 >> 3);
    }
    iVar1 = local_34[3] +
            ((local_34[0] >> 6 | local_34[0] * 0x4000000) ^
             (local_34[0] >> 0xb | local_34[0] * 0x200000) ^
            (local_34[0] >> 0x19 | local_34[0] * 0x80)) +
            (local_34[2] ^ local_34[0] & (local_34[1] ^ local_34[2])) +
            *(int *)(&DAT_00015bb8 + (local_14 + 4) * 4) + local_64;
    uVar2 = uVar2 + iVar1;
    uVar6 = iVar1 + ((uVar5 >> 2 | uVar5 * 0x40000000) ^ (uVar5 >> 0xd | uVar5 * 0x80000) ^
                    (uVar5 >> 0x16 | uVar5 * 0x400)) + (uVar4 ^ (uVar5 ^ uVar4) & (uVar4 ^ uVar3));
    if (local_14 == 0) {
      local_60 = param_2[5];
    }
    else {
      local_60 = local_60 +
                 ((local_68 >> 0x11 | local_68 << 0xf) ^ (local_68 >> 0x13 | local_68 << 0xd) ^
                 local_68 >> 10) + local_3c +
                 ((local_5c >> 7 | local_5c << 0x19) ^ (local_5c >> 0x12 | local_5c << 0xe) ^
                 local_5c >> 3);
    }
    iVar1 = local_34[2] +
            ((uVar2 >> 6 | uVar2 * 0x4000000) ^ (uVar2 >> 0xb | uVar2 * 0x200000) ^
            (uVar2 >> 0x19 | uVar2 * 0x80)) + (local_34[1] ^ uVar2 & (local_34[0] ^ local_34[1])) +
            *(int *)(&DAT_00015bb8 + (local_14 + 5) * 4) + local_60;
    uVar3 = uVar3 + iVar1;
    uVar7 = iVar1 + ((uVar6 >> 2 | uVar6 * 0x40000000) ^ (uVar6 >> 0xd | uVar6 * 0x80000) ^
                    (uVar6 >> 0x16 | uVar6 * 0x400)) + (uVar5 ^ (uVar6 ^ uVar5) & (uVar5 ^ uVar4));
    if (local_14 == 0) {
      local_5c = param_2[6];
    }
    else {
      local_5c = local_5c +
                 ((local_64 >> 0x11 | local_64 << 0xf) ^ (local_64 >> 0x13 | local_64 << 0xd) ^
                 local_64 >> 10) + local_38 +
                 ((local_58 >> 7 | local_58 << 0x19) ^ (local_58 >> 0x12 | local_58 << 0xe) ^
                 local_58 >> 3);
    }
    iVar1 = local_34[1] +
            ((uVar3 >> 6 | uVar3 * 0x4000000) ^ (uVar3 >> 0xb | uVar3 * 0x200000) ^
            (uVar3 >> 0x19 | uVar3 * 0x80)) + (local_34[0] ^ uVar3 & (uVar2 ^ local_34[0])) +
            *(int *)(&DAT_00015bb8 + (local_14 + 6) * 4) + local_5c;
    uVar4 = uVar4 + iVar1;
    uVar8 = iVar1 + ((uVar7 >> 2 | uVar7 * 0x40000000) ^ (uVar7 >> 0xd | uVar7 * 0x80000) ^
                    (uVar7 >> 0x16 | uVar7 * 0x400)) + (uVar6 ^ (uVar7 ^ uVar6) & (uVar6 ^ uVar5));
    if (local_14 == 0) {
      local_58 = param_2[7];
    }
    else {
      local_58 = local_58 +
                 ((local_60 >> 0x11 | local_60 << 0xf) ^ (local_60 >> 0x13 | local_60 << 0xd) ^
                 local_60 >> 10) + local_74 +
                 ((local_54 >> 7 | local_54 << 0x19) ^ (local_54 >> 0x12 | local_54 << 0xe) ^
                 local_54 >> 3);
    }
    iVar1 = local_34[0] +
            ((uVar4 >> 6 | uVar4 * 0x4000000) ^ (uVar4 >> 0xb | uVar4 * 0x200000) ^
            (uVar4 >> 0x19 | uVar4 * 0x80)) + (uVar2 ^ uVar4 & (uVar3 ^ uVar2)) +
            *(int *)(&DAT_00015bb8 + (local_14 + 7) * 4) + local_58;
    uVar5 = uVar5 + iVar1;
    uVar9 = iVar1 + ((uVar8 >> 2 | uVar8 * 0x40000000) ^ (uVar8 >> 0xd | uVar8 * 0x80000) ^
                    (uVar8 >> 0x16 | uVar8 * 0x400)) + (uVar7 ^ (uVar8 ^ uVar7) & (uVar7 ^ uVar6));
    if (local_14 == 0) {
      local_54 = param_2[8];
    }
    else {
      local_54 = local_54 +
                 ((local_5c >> 0x11 | local_5c << 0xf) ^ (local_5c >> 0x13 | local_5c << 0xd) ^
                 local_5c >> 10) + local_70 +
                 ((local_50 >> 7 | local_50 << 0x19) ^ (local_50 >> 0x12 | local_50 << 0xe) ^
                 local_50 >> 3);
    }
    iVar1 = uVar2 + ((uVar5 >> 6 | uVar5 * 0x4000000) ^ (uVar5 >> 0xb | uVar5 * 0x200000) ^
                    (uVar5 >> 0x19 | uVar5 * 0x80)) + (uVar3 ^ uVar5 & (uVar4 ^ uVar3)) +
                    *(int *)(&DAT_00015bb8 + (local_14 + 8) * 4) + local_54;
    uVar6 = uVar6 + iVar1;
    local_34[7] = iVar1 + ((uVar9 >> 2 | uVar9 * 0x40000000) ^ (uVar9 >> 0xd | uVar9 * 0x80000) ^
                          (uVar9 >> 0x16 | uVar9 * 0x400)) +
                          (uVar8 ^ (uVar9 ^ uVar8) & (uVar8 ^ uVar7));
    if (local_14 == 0) {
      local_50 = param_2[9];
    }
    else {
      local_50 = local_50 +
                 ((local_58 >> 0x11 | local_58 << 0xf) ^ (local_58 >> 0x13 | local_58 << 0xd) ^
                 local_58 >> 10) + local_6c +
                 ((local_4c >> 7 | local_4c << 0x19) ^ (local_4c >> 0x12 | local_4c << 0xe) ^
                 local_4c >> 3);
    }
    iVar1 = uVar3 + ((uVar6 >> 6 | uVar6 * 0x4000000) ^ (uVar6 >> 0xb | uVar6 * 0x200000) ^
                    (uVar6 >> 0x19 | uVar6 * 0x80)) + (uVar4 ^ uVar6 & (uVar5 ^ uVar4)) +
                    *(int *)(&DAT_00015bb8 + (local_14 + 9) * 4) + local_50;
    uVar7 = uVar7 + iVar1;
    local_34[6] = iVar1 + ((local_34[7] >> 2 | local_34[7] * 0x40000000) ^
                           (local_34[7] >> 0xd | local_34[7] * 0x80000) ^
                          (local_34[7] >> 0x16 | local_34[7] * 0x400)) +
                          (uVar9 ^ (local_34[7] ^ uVar9) & (uVar9 ^ uVar8));
    if (local_14 == 0) {
      local_4c = param_2[10];
    }
    else {
      local_4c = local_4c +
                 ((local_54 >> 0x11 | local_54 << 0xf) ^ (local_54 >> 0x13 | local_54 << 0xd) ^
                 local_54 >> 10) + local_68 +
                 ((local_48 >> 7 | local_48 << 0x19) ^ (local_48 >> 0x12 | local_48 << 0xe) ^
                 local_48 >> 3);
    }
    iVar1 = uVar4 + ((uVar7 >> 6 | uVar7 * 0x4000000) ^ (uVar7 >> 0xb | uVar7 * 0x200000) ^
                    (uVar7 >> 0x19 | uVar7 * 0x80)) + (uVar5 ^ uVar7 & (uVar6 ^ uVar5)) +
                    *(int *)(&DAT_00015bb8 + (local_14 + 10) * 4) + local_4c;
    uVar8 = uVar8 + iVar1;
    local_34[5] = iVar1 + ((local_34[6] >> 2 | local_34[6] * 0x40000000) ^
                           (local_34[6] >> 0xd | local_34[6] * 0x80000) ^
                          (local_34[6] >> 0x16 | local_34[6] * 0x400)) +
                          (local_34[7] ^ (local_34[6] ^ local_34[7]) & (local_34[7] ^ uVar9));
    if (local_14 == 0) {
      local_48 = param_2[0xb];
    }
    else {
      local_48 = local_48 +
                 ((local_50 >> 0x11 | local_50 << 0xf) ^ (local_50 >> 0x13 | local_50 << 0xd) ^
                 local_50 >> 10) + local_64 +
                 ((local_44 >> 7 | local_44 << 0x19) ^ (local_44 >> 0x12 | local_44 << 0xe) ^
                 local_44 >> 3);
    }
    iVar1 = uVar5 + ((uVar8 >> 6 | uVar8 * 0x4000000) ^ (uVar8 >> 0xb | uVar8 * 0x200000) ^
                    (uVar8 >> 0x19 | uVar8 * 0x80)) + (uVar6 ^ uVar8 & (uVar7 ^ uVar6)) +
                    *(int *)(&DAT_00015bb8 + (local_14 + 0xb) * 4) + local_48;
    uVar9 = uVar9 + iVar1;
    uVar2 = iVar1 + ((local_34[5] >> 2 | local_34[5] * 0x40000000) ^
                     (local_34[5] >> 0xd | local_34[5] * 0x80000) ^
                    (local_34[5] >> 0x16 | local_34[5] * 0x400)) +
                    (local_34[6] ^ (local_34[5] ^ local_34[6]) & (local_34[6] ^ local_34[7]));
    if (local_14 == 0) {
      local_44 = param_2[0xc];
    }
    else {
      local_44 = local_44 +
                 ((local_4c >> 0x11 | local_4c << 0xf) ^ (local_4c >> 0x13 | local_4c << 0xd) ^
                 local_4c >> 10) + local_60 +
                 ((local_40 >> 7 | local_40 << 0x19) ^ (local_40 >> 0x12 | local_40 << 0xe) ^
                 local_40 >> 3);
    }
    iVar1 = uVar6 + ((uVar9 >> 6 | uVar9 * 0x4000000) ^ (uVar9 >> 0xb | uVar9 * 0x200000) ^
                    (uVar9 >> 0x19 | uVar9 * 0x80)) + (uVar7 ^ uVar9 & (uVar8 ^ uVar7)) +
                    *(int *)(&DAT_00015bb8 + (local_14 + 0xc) * 4) + local_44;
    local_34[7] = local_34[7] + iVar1;
    local_34[3] = iVar1 + ((uVar2 >> 2 | uVar2 * 0x40000000) ^ (uVar2 >> 0xd | uVar2 * 0x80000) ^
                          (uVar2 >> 0x16 | uVar2 * 0x400)) +
                          (local_34[5] ^ (uVar2 ^ local_34[5]) & (local_34[5] ^ local_34[6]));
    if (local_14 == 0) {
      local_40 = param_2[0xd];
    }
    else {
      local_40 = local_40 +
                 ((local_48 >> 0x11 | local_48 << 0xf) ^ (local_48 >> 0x13 | local_48 << 0xd) ^
                 local_48 >> 10) + local_5c +
                 ((local_3c >> 7 | local_3c << 0x19) ^ (local_3c >> 0x12 | local_3c << 0xe) ^
                 local_3c >> 3);
    }
    iVar1 = uVar7 + ((local_34[7] >> 6 | local_34[7] * 0x4000000) ^
                     (local_34[7] >> 0xb | local_34[7] * 0x200000) ^
                    (local_34[7] >> 0x19 | local_34[7] * 0x80)) +
                    (uVar8 ^ local_34[7] & (uVar9 ^ uVar8)) +
                    *(int *)(&DAT_00015bb8 + (local_14 + 0xd) * 4) + local_40;
    local_34[6] = local_34[6] + iVar1;
    local_34[2] = iVar1 + ((local_34[3] >> 2 | local_34[3] * 0x40000000) ^
                           (local_34[3] >> 0xd | local_34[3] * 0x80000) ^
                          (local_34[3] >> 0x16 | local_34[3] * 0x400)) +
                          (uVar2 ^ (local_34[3] ^ uVar2) & (uVar2 ^ local_34[5]));
    if (local_14 == 0) {
      local_3c = param_2[0xe];
    }
    else {
      local_3c = local_3c +
                 ((local_44 >> 0x11 | local_44 << 0xf) ^ (local_44 >> 0x13 | local_44 << 0xd) ^
                 local_44 >> 10) + local_58 +
                 ((local_38 >> 7 | local_38 << 0x19) ^ (local_38 >> 0x12 | local_38 << 0xe) ^
                 local_38 >> 3);
    }
    iVar1 = uVar8 + ((local_34[6] >> 6 | local_34[6] * 0x4000000) ^
                     (local_34[6] >> 0xb | local_34[6] * 0x200000) ^
                    (local_34[6] >> 0x19 | local_34[6] * 0x80)) +
                    (uVar9 ^ local_34[6] & (local_34[7] ^ uVar9)) +
                    *(int *)(&DAT_00015bb8 + (local_14 + 0xe) * 4) + local_3c;
    local_34[5] = local_34[5] + iVar1;
    local_34[1] = iVar1 + ((local_34[2] >> 2 | local_34[2] * 0x40000000) ^
                           (local_34[2] >> 0xd | local_34[2] * 0x80000) ^
                          (local_34[2] >> 0x16 | local_34[2] * 0x400)) +
                          (local_34[3] ^ (local_34[2] ^ local_34[3]) & (local_34[3] ^ uVar2));
    if (local_14 == 0) {
      local_38 = param_2[0xf];
    }
    else {
      local_38 = local_38 +
                 ((local_40 >> 0x11 | local_40 << 0xf) ^ (local_40 >> 0x13 | local_40 << 0xd) ^
                 local_40 >> 10) + local_54 +
                 ((local_74 >> 7 | local_74 << 0x19) ^ (local_74 >> 0x12 | local_74 << 0xe) ^
                 local_74 >> 3);
    }
    iVar1 = uVar9 + ((local_34[5] >> 6 | local_34[5] * 0x4000000) ^
                     (local_34[5] >> 0xb | local_34[5] * 0x200000) ^
                    (local_34[5] >> 0x19 | local_34[5] * 0x80)) +
                    (local_34[7] ^ local_34[5] & (local_34[6] ^ local_34[7])) +
                    *(int *)(&DAT_00015bb8 + (local_14 + 0xf) * 4) + local_38;
    local_34[4] = uVar2 + iVar1;
    local_34[0] = iVar1 + ((local_34[1] >> 2 | local_34[1] * 0x40000000) ^
                           (local_34[1] >> 0xd | local_34[1] * 0x80000) ^
                          (local_34[1] >> 0x16 | local_34[1] * 0x400)) +
                          (local_34[2] ^ (local_34[1] ^ local_34[2]) & (local_34[2] ^ local_34[3]));
  }
  for (local_14 = 0; local_14 < 8; local_14 = local_14 + 1) {
    *(uint *)(param_1 + local_14 * 4) = *(int *)(param_1 + local_14 * 4) + local_34[local_14];
  }
  return;
}



//==================== FUN_0000aedc @ 0000aedc ====================

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_0000aedc(int param_1,int param_2,int param_3)

{
  if (param_1 == 1) {
    _DAT_f800d00c = _DAT_f800d00c | 0x10000;
  }
  else {
    _DAT_f800d00c = _DAT_f800d00c & 0xfffeffff;
  }
  _DAT_f800d00c = _DAT_f800d00c & 0xffffffcf;
  if (param_3 != 1) {
    if (param_3 == 2) {
      _DAT_f800d00c = _DAT_f800d00c | 0x10;
    }
    else if (param_3 == 3) {
      _DAT_f800d00c = _DAT_f800d00c | 0x20;
    }
  }
  _DAT_f800d018 = (uint)(param_2 * 0xc) / 1000000;
  _DAT_f800d01c = (uint)(param_2 * 0xf) / 100000000;
  return 0;
}



//==================== FUN_0000af74 @ 0000af74 ====================

void FUN_0000af74(void)

{
  int iVar1;
  undefined1 *puVar2;
  uint uVar3;
  char *pcVar4;
  uint uVar5;
  undefined1 *puVar7;
  undefined1 *puVar6;
  uint uVar8;
  uint uVar9;
  char cVar10;
  char cVar11;
  uint uVar12;
  bool bVar13;
  
  puVar2 = &DAT_00028eac;
  do {
    puVar7 = puVar2;
    do {
      puVar6 = puVar7 + 5;
      *puVar7 = 0;
      puVar7 = puVar6;
    } while (puVar6 != puVar2 + 0x9b);
    puVar2 = puVar2 + 1;
  } while (puVar2 != &DAT_00028eb1);
  uVar12 = 0;
  do {
    uVar5 = 2;
    uVar3 = 0;
    while( true ) {
      uVar8 = (uVar5 & 0xff) - 1;
      uVar9 = uVar8 & 0xff;
      if (uVar5 == 0x20) break;
      uVar8 = uVar8 & 0xfb;
      bVar13 = 2 < uVar8;
      if (uVar8 != 3) {
        bVar13 = uVar9 != 0;
      }
      if (bVar13 && (uVar8 != 3 && uVar9 != 1)) {
        if ((uVar5 & 0xff) != 0x10) {
          iVar1 = uVar3 * 5;
          uVar3 = uVar3 + 1 & 0xff;
          if ((int)(uVar5 & 1 << (uVar12 & 0xff)) >> (uVar12 & 0xff) == 1) {
            (&DAT_00028eac)[uVar12 + iVar1] = 1;
          }
          else {
            (&DAT_00028eac)[uVar12 + iVar1] = 0;
          }
        }
      }
      uVar5 = uVar5 + 1;
    }
    uVar12 = uVar12 + 1;
  } while (uVar12 != 5);
  uVar12 = 0;
  do {
    uVar3 = 0;
    puVar2 = &DAT_00028eac + uVar12;
    do {
      uVar5 = uVar3 & 0xff;
      uVar3 = uVar3 + 1;
      if (uVar5 == (uVar12 & 0xff)) {
        puVar2[0x82] = 1;
      }
      else {
        puVar2[0x82] = 0;
      }
      puVar2 = puVar2 + 5;
    } while (uVar3 != 5);
    uVar12 = uVar12 + 1;
  } while (uVar12 != 5);
  uVar12 = 0;
  cVar10 = '\0';
  cVar11 = '\x1a';
  pcVar4 = &DAT_00028e8d;
  DAT_00028e8c = 0xff;
  do {
    uVar3 = uVar12 & 0xff;
    if (((uVar3 - 3 & 0xfb) == 0 || uVar3 < 2) || (uVar3 == 0xf)) {
      *pcVar4 = cVar11;
      cVar11 = cVar11 + '\x01';
    }
    else {
      *pcVar4 = cVar10;
      cVar10 = cVar10 + '\x01';
    }
    uVar12 = uVar12 + 1;
    pcVar4 = pcVar4 + 1;
  } while (uVar12 != 0x1f);
  return;
}



//==================== FUN_0000b0e4 @ 0000b0e4 ====================

void FUN_0000b0e4(int param_1,int param_2)

{
  char *pcVar1;
  char *pcVar2;
  byte bVar3;
  char *pcVar4;
  byte *pbVar5;
  char *pcVar6;
  
  pcVar4 = (char *)(param_2 + -1);
  pcVar6 = (char *)(param_1 + -1);
  do {
    pcVar6 = pcVar6 + 1;
    pcVar4 = pcVar4 + 1;
    *pcVar4 = *pcVar6;
  } while (pcVar6 != (char *)(param_1 + 0x19));
  pbVar5 = (byte *)(param_2 + 0x19);
  pcVar4 = &DAT_00028eac;
  do {
    bVar3 = 0;
    pbVar5 = pbVar5 + 1;
    *pbVar5 = 0;
    pcVar1 = pcVar4;
    pcVar2 = (char *)(param_1 + -1);
    do {
      pcVar2 = pcVar2 + 1;
      bVar3 = *pcVar1 * *pcVar2 + bVar3;
      *pbVar5 = bVar3;
      pcVar1 = pcVar1 + 5;
    } while (pcVar2 != pcVar6);
    *pbVar5 = bVar3 & 1;
    pcVar4 = pcVar4 + 1;
  } while (pbVar5 != (byte *)(param_2 + 0x1e));
  return;
}



//==================== FUN_0000b160 @ 0000b160 ====================

undefined1 FUN_0000b160(int param_1,byte *param_2)

{
  char *pcVar1;
  char *pcVar2;
  byte bVar3;
  byte *pbVar4;
  char *pcVar5;
  
  pbVar4 = param_2 + -1;
  pcVar5 = &DAT_00028eac;
  do {
    bVar3 = 0;
    pbVar4 = pbVar4 + 1;
    *pbVar4 = 0;
    pcVar1 = pcVar5;
    pcVar2 = (char *)(param_1 + -1);
    do {
      pcVar2 = pcVar2 + 1;
      bVar3 = *pcVar1 * *pcVar2 + bVar3;
      *pbVar4 = bVar3;
      pcVar1 = pcVar1 + 5;
    } while (pcVar2 != (char *)(param_1 + 0x1e));
    *pbVar4 = bVar3 & 1;
    pcVar5 = pcVar5 + 1;
  } while (pbVar4 != param_2 + 4);
  return (&DAT_00028e8c)
         [(uint)param_2[3] * 8 + (uint)param_2[4] * 0x10 + (uint)*param_2 + (uint)param_2[2] * 4 +
          (uint)param_2[1] * 2 & 0xff];
}



//==================== FUN_0000b254 @ 0000b254 ====================

bool FUN_0000b254(uint param_1)

{
  uint uVar1;
  bool bVar2;
  bool bVar3;
  
  if (((((param_1 != 0xf800e104 && param_1 != 0xf800e080) &&
        (param_1 != 0xf800e20c && param_1 != 0xf800e188)) &&
       (param_1 != 0xf800e314 && param_1 != 0xf800e290)) &&
      ((((param_1 != 0xf800e41c && param_1 != 0xf800e398 &&
         (param_1 != 0xf800e524 && param_1 != 0xf800e4a0)) &&
        ((param_1 != 0xf800e978 && param_1 != 0xf800e8fc &&
         ((param_1 != 0xf800ea70 && param_1 != 0xf800e9f4 &&
          (param_1 != 0xf800eb68 && param_1 != 0xf800eaec)))))) &&
       (param_1 != 0xf800ec60 && param_1 != 0xf800ebe4)))) &&
     (param_1 != 0xf800ed58 && param_1 != 0xf800ecdc)) {
    uVar1 = param_1 + 0x7ff1fc0;
    bVar3 = 0x3b < uVar1;
    bVar2 = uVar1 == 0x3c;
    if (0x3c < uVar1) {
      bVar3 = 0x1b < param_1 + 0x7ff2000;
      bVar2 = param_1 + 0x7ff2000 == 0x1c;
    }
    if (((bVar3 && !bVar2) && (0x1fc < (param_1 & 0xfffff7ff) + 0x7ff1a00)) &&
       (0x1c < param_1 + 0x7ff17a0)) {
      return param_1 + 0x7ff1800 < 0x3d;
    }
  }
  return true;
}



//==================== FUN_0000b3d4 @ 0000b3d4 ====================

undefined4 FUN_0000b3d4(undefined4 *param_1,byte *param_2)

{
  undefined4 uVar1;
  int iVar2;
  int local_2c [3];
  undefined4 local_20;
  int local_1c;
  
  local_20 = 2;
  FUN_0000bb98(local_2c);
  if (local_2c[0] - 0x766eU < 0x53d1) {
    if (local_1c - 0x91ebU < 0x170b) {
      iVar2 = FUN_0000b254(param_1);
      if (iVar2 == 0) {
        *param_2 = (byte)*param_1 & 1;
        uVar1 = 0;
      }
      else {
        uVar1 = 1;
      }
    }
    else {
      uVar1 = 3;
    }
  }
  else {
    uVar1 = 2;
  }
  return uVar1;
}



//==================== FUN_0000b450 @ 0000b450 ====================

int FUN_0000b450(undefined4 param_1)

{
  int iVar1;
  uint uVar2;
  byte *pbVar3;
  int iVar4;
  int iVar5;
  byte *pbVar6;
  int iVar7;
  undefined1 auStack_14c [7];
  byte bStack_145;
  byte local_144 [31];
  byte abStack_125 [261];
  
  iVar4 = -0x7ff1f80;
  iVar7 = 0;
  FUN_0000af74();
  do {
    pbVar3 = local_144;
    do {
      iVar1 = FUN_0000b254(iVar4);
      iVar5 = iVar4;
      if (iVar1 == 1) {
        iVar5 = iVar4 + 4;
      }
      pbVar6 = pbVar3 + 1;
      iVar4 = iVar5 + 4;
      iVar5 = FUN_0000b3d4(iVar5,pbVar3);
      if (iVar5 != 0) {
        return iVar5;
      }
      pbVar3 = pbVar6;
    } while (pbVar6 != abStack_125);
    uVar2 = FUN_0000b160(local_144,auStack_14c);
    if (uVar2 < 0x1f) {
      local_144[uVar2] = local_144[uVar2] + 1 & 1;
    }
    pbVar6 = &bStack_145;
    pbVar3 = abStack_125 + iVar7;
    do {
      pbVar6 = pbVar6 + 1;
      pbVar3 = pbVar3 + 1;
      *pbVar3 = *pbVar6;
    } while (local_144 + 0x19 != pbVar6);
    iVar7 = iVar7 + 0x1a;
  } while (iVar7 != 0x104);
  FUN_0000bed4(abStack_125 + 1,param_1,0x100);
  return 0;
}



//==================== FUN_0000b680 @ 0000b680 ====================

bool FUN_0000b680(undefined4 *param_1)

{
  bool bVar1;
  undefined8 uVar2;
  
  uVar2 = FUN_0000b254();
  bVar1 = (int)uVar2 == 0;
  if (bVar1) {
    *(byte *)((ulonglong)uVar2 >> 0x20) = (byte)*param_1 & 1;
  }
  return !bVar1;
}



//==================== FUN_0000b6a4 @ 0000b6a4 ====================

undefined4 FUN_0000b6a4(undefined4 param_1)

{
  undefined4 uVar1;
  int iVar2;
  uint *puVar3;
  undefined8 uVar4;
  undefined4 local_10;
  undefined4 uStack_c;
  
  local_10 = 0;
  uStack_c = 0;
  uVar4 = FUN_0000b254(param_1,param_1);
  puVar3 = (uint *)((ulonglong)uVar4 >> 0x20);
  if ((int)uVar4 == 0) {
    if ((*puVar3 & 1) == 0) {
      *puVar3 = 1;
      FUN_0000bcd8(&local_10,puVar3,0x96,0);
      do {
        iVar2 = FUN_0000bd28(local_10,uStack_c);
      } while (iVar2 != 1);
    }
    uVar1 = 0;
  }
  else {
    uVar1 = 1;
  }
  return uVar1;
}



//==================== FUN_0000b710 @ 0000b710 ====================

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_0000b710(uint *param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  char local_21 [5];
  
  if (param_1 == (uint *)0x0) {
    return 0x8100;
  }
  iVar1 = FUN_0000c024();
  if (((((1 < *param_1) || (1 < param_1[1])) || (1 < param_1[2])) ||
      ((1 < param_1[5] || (1 < param_1[3])))) || (1 < param_1[4])) {
    return 0x8600;
  }
  iVar2 = FUN_0000bb18();
  if (iVar2 != 0) {
    return 0x8200;
  }
  _DAT_f800d004 = 0xdf0d;
  if ((_DAT_f800d008 & 1) != 0) {
    _DAT_f800d004 = 0xdf0d;
    return 0x8300;
  }
  if ((_DAT_f800d010 & 0x3000) != 0) {
    _DAT_f800d000 = 0x767b;
    _DAT_f800d004 = 0xdf0d;
    return 0x8400;
  }
  if (40000000 < iVar1 + 0xfeced300U) {
    iVar2 = 0x850b;
    goto LAB_0000b7d4;
  }
  iVar2 = FUN_0000aedc(0,iVar1,1);
  if (iVar2 != 0) {
    iVar2 = iVar2 + 0x8500;
    goto LAB_0000b7d4;
  }
  _DAT_f800d014 = _DAT_f800d014 & 0xffffffec | 0x10;
  FUN_0000bc50(iVar1);
  if ((param_1[2] != 0) && (iVar2 = FUN_0000e7c4(0xf800e028,iVar1), iVar2 != 0)) {
    iVar2 = iVar2 + 0x9100;
    goto LAB_0000b7d4;
  }
  if (param_1[5] == 0) {
LAB_0000b860:
    if ((param_1[1] == 0) || (iVar2 = FUN_0000e7c4(0xf800e02c,iVar1), iVar2 == 0)) {
      if ((param_1[3] == 0) || (iVar2 = FUN_0000e7c4(0xf800e030,iVar1), iVar2 == 0)) {
        if ((param_1[4] == 0) || (iVar2 = FUN_0000e7c4(0xf800e034,iVar1), iVar2 == 0)) {
          iVar2 = 0;
          if (*param_1 != 0) {
            iVar3 = FUN_0000e7c4(0xf800e020,iVar1);
            iVar1 = FUN_0000e7c4(0xf800e024,iVar1);
            if (iVar3 != 0 && iVar1 != 0) {
              iVar2 = iVar3 + 0x9600;
            }
          }
        }
        else {
          iVar2 = iVar2 + 0x9900;
        }
      }
      else {
        iVar2 = iVar2 + 0x9800;
      }
    }
    else {
      iVar2 = iVar2 + 0x9500;
    }
  }
  else {
    iVar2 = FUN_0000aedc(1,iVar1);
    if (iVar2 == 0) {
      iVar3 = 0x136;
      iVar2 = -0x7ff1f80;
      do {
        while (iVar4 = FUN_0000b254(iVar2), iVar4 == 1) {
          iVar2 = iVar2 + 4;
        }
        iVar4 = FUN_0000b3d4(iVar2,local_21);
        iVar2 = iVar2 + 4;
        if (iVar4 != 0) {
LAB_0000b9dc:
          iVar2 = FUN_0000aedc(0,iVar1,1);
          if (iVar2 == 0) {
            iVar2 = iVar4;
          }
          goto LAB_0000b9f4;
        }
        if (local_21[0] != '\0') {
          iVar4 = 9;
          goto LAB_0000b9dc;
        }
        iVar3 = iVar3 + -1;
      } while (iVar3 != 0);
      iVar2 = FUN_0000aedc(0,iVar1,1);
      if (iVar2 == 0) {
        iVar2 = FUN_0000e88c(param_1 + 6,iVar1);
        if (iVar2 != 0) {
          iVar2 = iVar2 + 0x9400;
          goto LAB_0000b7d4;
        }
        goto LAB_0000b860;
      }
    }
LAB_0000b9f4:
    iVar2 = iVar2 + 0x9700;
  }
LAB_0000b7d4:
  _DAT_f800d014 = _DAT_f800d014 & 0xffffffef | 3;
  _DAT_f800d000 = 0x767b;
  return iVar2;
}



//==================== FUN_0000ba3c @ 0000ba3c ====================

undefined4 FUN_0000ba3c(int param_1,char *param_2)

{
  undefined4 uVar1;
  
  if ((param_1 - 0x30U & 0xff) < 10) {
    *param_2 = (char)(param_1 - 0x30U);
    return 0;
  }
  if (param_1 - 0x61U < 6) {
    *param_2 = (char)param_1 + -0x57;
    return 0;
  }
  if (param_1 - 0x41U < 6) {
    uVar1 = 0;
    *param_2 = (char)param_1 + -0x37;
  }
  else {
    uVar1 = 0x11;
  }
  return uVar1;
}



//==================== FUN_0000ba90 @ 0000ba90 ====================

undefined4 FUN_0000ba90(int param_1,int param_2,int param_3)

{
  int iVar1;
  uint extraout_r2;
  uint uVar2;
  byte local_12;
  char local_11;
  
  if (param_3 == 0) {
    return 0;
  }
  uVar2 = 0;
  while( true ) {
    iVar1 = FUN_0000ba3c(*(undefined1 *)(param_1 + uVar2),&local_11);
    if ((iVar1 != 0) ||
       (iVar1 = FUN_0000ba3c(*(undefined1 *)(param_1 + uVar2 + 1),&local_12), iVar1 != 0)) break;
    *(byte *)(param_2 + (uVar2 >> 1)) = local_12 | local_11 << 4;
    uVar2 = uVar2 + 2;
    if (extraout_r2 <= uVar2) {
      return 0;
    }
  }
  return 0x11;
}



//==================== FUN_0000bb18 @ 0000bb18 ====================

undefined4 FUN_0000bb18(void)

{
  int iVar1;
  
  DAT_00028f4c = 0;
  iVar1 = FUN_0000c2f8(0);
  if (iVar1 == 0) {
    return 0xd;
  }
  iVar1 = FUN_0000c314(&DAT_00021be0,iVar1,*(undefined4 *)(iVar1 + 4));
  if (iVar1 != 0) {
    return 0xe;
  }
  iVar1 = FUN_0000c240(&DAT_00021be0);
  if (iVar1 == 0) {
    FUN_0000cdd4(&DAT_00021be0,0);
    return 0;
  }
  return 0xf;
}



//==================== FUN_0000bb98 @ 0000bb98 ====================

void FUN_0000bb98(undefined4 *param_1)

{
  undefined4 uVar1;
  undefined1 uVar2;
  int iVar3;
  undefined1 uVar4;
  undefined1 uVar5;
  
  if (param_1 == (undefined4 *)0x0) {
    return;
  }
  uVar1 = FUN_0000c6dc(&DAT_00021be0,0);
  *param_1 = uVar1;
  uVar1 = FUN_0000c888(&DAT_00021be0,4);
  param_1[1] = uVar1;
  uVar1 = FUN_0000c888(&DAT_00021be0,0);
  iVar3 = param_1[3];
  param_1[2] = uVar1;
  if (iVar3 - 2U < 4) {
    uVar2 = (&DAT_00015cb6)[iVar3];
    uVar5 = (&DAT_00015cba)[iVar3];
    uVar4 = (&DAT_00015cbe)[iVar3];
  }
  else {
    uVar2 = 0xd;
    uVar4 = 8;
    uVar5 = 0xc;
  }
  uVar1 = FUN_0000c6dc(&DAT_00021be0,uVar2);
  param_1[4] = uVar1;
  uVar1 = FUN_0000c888(&DAT_00021be0,uVar5);
  param_1[5] = uVar1;
  uVar1 = FUN_0000c888(&DAT_00021be0,uVar4);
  param_1[6] = uVar1;
  return;
}



//==================== FUN_0000bc50 @ 0000bc50 ====================

void FUN_0000bc50(void)

{
  uRamf8f00200 = 0;
  uRamf8f00204 = 0;
  uRamf8f00208 = 1;
  return;
}



//==================== FUN_0000bc74 @ 0000bc74 ====================

undefined8 FUN_0000bc74(void)

{
  return CONCAT44(uRamf8f00204,uRamf8f00200);
}



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



//==================== FUN_0000bd28 @ 0000bd28 ====================

bool FUN_0000bd28(uint param_1,uint param_2)

{
  uint uVar1;
  bool bVar2;
  undefined8 uVar3;
  
  uVar3 = FUN_0000bc74();
  uVar1 = (uint)((ulonglong)uVar3 >> 0x20);
  bVar2 = param_2 <= uVar1;
  if (uVar1 == param_2) {
    bVar2 = param_1 <= (uint)uVar3;
  }
  return bVar2;
}



//==================== FUN_0000bda8 @ 0000bda8 ====================

undefined4 FUN_0000bda8(int param_1,int param_2,uint param_3)

{
  undefined4 uVar1;
  int iVar2;
  uint extraout_r2;
  int iVar3;
  uint uVar4;
  byte *pbVar5;
  byte local_12;
  char local_11;
  
  if ((param_3 & 1) == 0) {
    pbVar5 = (byte *)(param_2 + (param_3 >> 3));
    uVar4 = 0;
    do {
      iVar2 = FUN_0000ba3c(*(undefined1 *)(param_1 + uVar4),&local_11);
      iVar3 = param_1 + uVar4;
      uVar4 = uVar4 + 2;
      if ((iVar2 != 0) || (iVar2 = FUN_0000ba3c(*(undefined1 *)(iVar3 + 1),&local_12), iVar2 != 0))
      {
        return 0x11;
      }
      pbVar5 = pbVar5 + -1;
      *pbVar5 = local_12 | local_11 << 4;
      uVar1 = 0;
    } while (uVar4 < extraout_r2);
  }
  else {
    uVar1 = 0x10;
  }
  return uVar1;
}



//==================== FUN_0000be74 @ 0000be74 ====================

void FUN_0000be74(byte *param_1,int param_2,int param_3)

{
  uint uVar1;
  undefined1 *puVar2;
  int iVar3;
  undefined1 *puVar4;
  uint uVar5;
  byte *pbVar6;
  
  if (param_3 == 0) {
    return;
  }
  puVar2 = (undefined1 *)(param_2 + -1);
  puVar4 = puVar2 + param_3;
  do {
    puVar2 = puVar2 + 1;
    *puVar2 = 0;
  } while (puVar2 != puVar4);
  iVar3 = 0;
  do {
    uVar5 = 0;
    pbVar6 = (byte *)(param_2 + iVar3);
    do {
      param_3 = param_3 + -1;
      uVar1 = uVar5 & 0xff;
      uVar5 = uVar5 + 1;
      *pbVar6 = (byte)((int)(uint)*param_1 >> uVar1) & 1;
      iVar3 = (int)(pbVar6 + 1) - param_2;
      if (param_3 == 0) {
        return;
      }
      pbVar6 = pbVar6 + 1;
    } while (uVar5 != 8);
    param_1 = param_1 + 1;
  } while( true );
}



//==================== FUN_0000bed4 @ 0000bed4 ====================

void FUN_0000bed4(int param_1,int param_2,uint param_3)

{
  byte *pbVar1;
  byte *pbVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  bool bVar8;
  
  pbVar1 = (byte *)(param_2 + -1);
  uVar6 = param_3 >> 3;
  if ((param_3 & 7) != 0) {
    uVar6 = (param_3 >> 3) + 1;
  }
  pbVar2 = pbVar1;
  while (pbVar1 + uVar6 != pbVar2) {
    pbVar2 = pbVar2 + 1;
    *pbVar2 = 0;
  }
  if (param_3 == 0) {
    return;
  }
  iVar5 = 0;
  do {
    pbVar1 = pbVar1 + 1;
    uVar3 = (uint)*pbVar1;
    uVar4 = param_3 - 1;
    uVar7 = 0;
    uVar6 = uVar4;
    pbVar2 = (byte *)(param_1 + iVar5);
    do {
      param_3 = uVar6;
      bVar8 = uVar7 == uVar4;
      iVar5 = (int)(pbVar2 + 1) - param_1;
      uVar6 = uVar7 & 0xff;
      uVar7 = uVar7 + 1;
      uVar3 = uVar3 | (*pbVar2 & 1) << uVar6 & 0xff;
      *pbVar1 = (byte)uVar3;
      if (bVar8) {
        return;
      }
      uVar6 = param_3 - 1;
      pbVar2 = pbVar2 + 1;
    } while (uVar7 != 8);
  } while( true );
}



//==================== FUN_0000bf60 @ 0000bf60 ====================

bool FUN_0000bf60(undefined1 *param_1)

{
  bool bVar1;
  int iVar2;
  undefined4 local_20;
  undefined4 uStack_1c;
  undefined4 uStack_18;
  undefined4 uStack_14;
  undefined4 uStack_10;
  undefined2 uStack_c;
  undefined1 local_a;
  
  local_20 = DAT_00017274;
  uStack_1c = DAT_00017278;
  uStack_18 = DAT_0001727c;
  uStack_14 = DAT_00017280;
  uStack_10 = DAT_00017284;
  local_a = (undefined1)((uint)DAT_00017288 >> 0x10);
  uStack_c = (undefined2)DAT_00017288;
  if (param_1 == (undefined1 *)0x0) {
    bVar1 = true;
  }
  else {
    iVar2 = FUN_0000dfb0(&local_20,*param_1);
    bVar1 = iVar2 == 0;
  }
  return bVar1;
}



//==================== FUN_0000bfbc @ 0000bfbc ====================

char * FUN_0000bfbc(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  if (param_1 == 0) {
    return s_Calculated_checksum_0000f018 + 0xb;
  }
  if (param_2 == 0) {
    return s_Calculated_checksum_0000f018 + 9;
  }
  iVar1 = FUN_00002900();
  if (iVar1 != param_2) {
    return s_Calculated_checksum_0000f018 + 8;
  }
  iVar2 = param_1;
  do {
    iVar3 = iVar2 + 1;
    iVar2 = FUN_0000bf60(iVar2);
    if (iVar2 != 0) {
      return s_Calculated_checksum_0000f018 + 10;
    }
    iVar2 = iVar3;
  } while (param_1 + iVar1 != iVar3);
  return (char *)0x0;
}



//==================== FUN_0000c024 @ 0000c024 ====================

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_0000c024(void)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  
  DAT_00028f50 = 0;
  uVar2 = (_DAT_f8000100 & 0x7ffff) >> 0xc;
  uVar3 = (_DAT_f8000120 & 0x3fff) >> 8;
  iVar1 = FUN_00001a50(uVar3 * 0x27bc86bf,uVar2);
  uVar2 = FUN_00001a50(iVar1 * uVar2,uVar3);
  DAT_00028f50 = uVar2 / 20000000;
  return iVar1;
}



//==================== FUN_0000c0dc @ 0000c0dc ====================

undefined4 FUN_0000c0dc(int param_1)

{
  uint uVar1;
  undefined4 uVar2;
  undefined1 *puVar3;
  undefined1 *puVar4;
  int iVar5;
  undefined1 auStack_54 [4];
  undefined1 auStack_50 [7];
  undefined1 uStack_49;
  undefined1 local_48 [64];
  
  FUN_000027e0(local_48,0,0x40);
  uVar1 = FUN_00002900(param_1);
  if (uVar1 < 0x41) {
    if (uVar1 == 0x40) {
      puVar3 = (undefined1 *)(param_1 + -1);
      puVar4 = &uStack_49;
      do {
        puVar3 = puVar3 + 1;
        puVar4 = puVar4 + 1;
        *puVar4 = *puVar3;
      } while (puVar4 != local_48 + 0x40);
    }
    else {
      puVar3 = local_48 + 0x40 + -uVar1;
      puVar4 = (undefined1 *)(param_1 + -1);
      do {
        puVar4 = puVar4 + 1;
        puVar3 = puVar3 + 1;
        *puVar3 = *puVar4;
      } while (puVar4 != (undefined1 *)(param_1 + uVar1));
    }
    iVar5 = 8;
    do {
      FUN_0000ba90(auStack_50,auStack_54);
      iVar5 = iVar5 + -1;
    } while (iVar5 != 0);
    uVar2 = 0;
  }
  else {
    uVar2 = 0x20;
  }
  return uVar2;
}



//==================== FUN_0000c240 @ 0000c240 ====================

bool FUN_0000c240(int param_1)

{
  int iVar1;
  
  if (param_1 == 0) {
    FUN_000014e4(&DAT_0001728c,0x69);
    DAT_00028e84 = 1;
    return false;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) == 0x11111111) {
    FUN_0000c654();
    FUN_0000d884(param_1,1,0x55);
    iVar1 = FUN_0000d938(param_1,1);
    FUN_0000c654(param_1);
    return iVar1 != 0x55;
  }
  FUN_000014e4(&DAT_0001728c,0x6a);
  DAT_00028e84 = 1;
  return false;
}



//==================== FUN_0000c2f8 @ 0000c2f8 ====================

undefined2 * FUN_0000c2f8(uint param_1)

{
  undefined2 *puVar1;
  
  if (DAT_00019c50 == param_1) {
    puVar1 = &DAT_00019c50;
  }
  else {
    puVar1 = (undefined2 *)0x0;
  }
  return puVar1;
}



//==================== FUN_0000c314 @ 0000c314 ====================

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_0000c314(undefined2 *param_1,undefined2 *param_2,undefined4 param_3)

{
  undefined2 uVar1;
  
  if (param_1 == (undefined2 *)0x0) {
    FUN_000014e4(&DAT_000172a0,0x6e);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (param_2 == (undefined2 *)0x0) {
    FUN_000014e4(&DAT_000172a0,0x6f);
    DAT_00028e84 = 1;
    return 0;
  }
  uVar1 = *param_2;
  *(undefined4 *)(param_1 + 2) = param_3;
  *param_1 = uVar1;
  _DAT_f8007034 = 0x757bdf0d;
  **(uint **)(param_1 + 2) = **(uint **)(param_1 + 2) | 0x80ff0000;
  *(undefined4 *)(*(int *)(param_1 + 2) + 0x18) = 0;
  *(undefined4 *)(param_1 + 4) = 0x11111111;
  return 0;
}



//==================== FUN_0000c44c @ 0000c44c ====================

undefined4 FUN_0000c44c(int param_1)

{
  if (param_1 == 0) {
    FUN_000014e4(&DAT_000172a0,0xc4);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) == 0x11111111) {
    return **(undefined4 **)(param_1 + 4);
  }
  FUN_000014e4(&DAT_000172a0,0xc5);
  DAT_00028e84 = 1;
  return 0;
}



//==================== FUN_0000c4d0 @ 0000c4d0 ====================

undefined4 FUN_0000c4d0(int param_1)

{
  if (param_1 == 0) {
    FUN_000014e4(&DAT_000172a0,0xe2);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) == 0x11111111) {
    return *(undefined4 *)(*(int *)(param_1 + 4) + 0xc);
  }
  FUN_000014e4(&DAT_000172a0,0xe3);
  DAT_00028e84 = 1;
  return 0;
}



//==================== FUN_0000c5d0 @ 0000c5d0 ====================

undefined4 FUN_0000c5d0(int param_1)

{
  if (param_1 == 0) {
    FUN_000014e4(&DAT_000172a0,0x11d);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) == 0x11111111) {
    return *(undefined4 *)(*(int *)(param_1 + 4) + 0x18);
  }
  FUN_000014e4(&DAT_000172a0,0x11e);
  DAT_00028e84 = 1;
  return 0;
}



//==================== FUN_0000c654 @ 0000c654 ====================

void FUN_0000c654(int param_1)

{
  if (param_1 == 0) {
    FUN_000014e4(&DAT_000172a0,0x139);
    DAT_00028e84 = 1;
    return;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) == 0x11111111) {
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x18) = 0x10;
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x18) = 0;
    return;
  }
  FUN_000014e4(&DAT_000172a0,0x13a);
  DAT_00028e84 = 1;
  return;
}



//==================== FUN_0000c6dc @ 0000c6dc ====================

uint FUN_0000c6dc(int param_1,uint param_2)

{
  bool bVar1;
  bool bVar2;
  
  if (param_1 == 0) {
    FUN_000014e4(&DAT_000172a0,0x164);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) != 0x11111111) {
    FUN_000014e4(&DAT_000172a0,0x165);
    DAT_00028e84 = 1;
    return 0;
  }
  bVar2 = 5 < param_2;
  bVar1 = param_2 == 6;
  if (6 < param_2) {
    bVar2 = 0x11 < param_2 - 0xd;
    bVar1 = param_2 - 0xd == 0x12;
  }
  if (bVar2 && !bVar1) {
    FUN_000014e4(&DAT_000172a0,0x168);
    DAT_00028e84 = 1;
    return 0;
  }
  *(uint *)(*(int *)(param_1 + 4) + 0x10) = param_2 << 0x10 | 0x4000000;
  *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = *(undefined4 *)(*(int *)(param_1 + 4) + 0x14);
  return *(uint *)(*(int *)(param_1 + 4) + 0x14) & 0xffff;
}



//==================== FUN_0000c7b8 @ 0000c7b8 ====================

uint FUN_0000c7b8(int param_1,uint param_2)

{
  if (param_1 == 0) {
    FUN_000014e4(&DAT_000172a0,0x18a);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) != 0x11111111) {
    FUN_000014e4(&DAT_000172a0,0x18b);
    DAT_00028e84 = 1;
    return 0;
  }
  if (param_2 < 3) {
    *(uint *)(*(int *)(param_1 + 4) + 0x10) = (param_2 + 8) * 0x10000 | 0x4000000;
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = *(undefined4 *)(*(int *)(param_1 + 4) + 0x14);
    return *(uint *)(*(int *)(param_1 + 4) + 0x14) & 0xffff;
  }
  FUN_000014e4(&DAT_000172a0,0x18c);
  DAT_00028e84 = 1;
  return 0;
}



//==================== FUN_0000c888 @ 0000c888 ====================

uint FUN_0000c888(int param_1,uint param_2)

{
  bool bVar1;
  bool bVar2;
  
  if (param_1 == 0) {
    FUN_000014e4(&DAT_000172a0,0x1b4);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) != 0x11111111) {
    FUN_000014e4(&DAT_000172a0,0x1b5);
    DAT_00028e84 = 1;
    return 0;
  }
  bVar2 = 9 < param_2;
  bVar1 = param_2 == 10;
  if (10 < param_2) {
    bVar2 = 1 < param_2 - 0xc;
    bVar1 = param_2 - 0xc == 2;
  }
  if (bVar2 && !bVar1) {
    FUN_000014e4(&DAT_000172a0,0x1b8);
    DAT_00028e84 = 1;
    return 0;
  }
  *(uint *)(*(int *)(param_1 + 4) + 0x10) = (param_2 + 0x20) * 0x10000 | 0x4000000;
  *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = *(undefined4 *)(*(int *)(param_1 + 4) + 0x14);
  return *(uint *)(*(int *)(param_1 + 4) + 0x14) & 0xffff;
}



//==================== FUN_0000ca3c @ 0000ca3c ====================

uint FUN_0000ca3c(int param_1)

{
  if (param_1 == 0) {
    FUN_000014e4(&DAT_000172a0,0x20a);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) == 0x11111111) {
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = 0x4400000;
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = *(undefined4 *)(*(int *)(param_1 + 4) + 0x14);
    return (*(uint *)(*(int *)(param_1 + 4) + 0x14) & 0x3fff) >> 0xc;
  }
  FUN_000014e4(&DAT_000172a0,0x20b);
  DAT_00028e84 = 1;
  return 0;
}



//==================== FUN_0000cba0 @ 0000cba0 ====================

uint FUN_0000cba0(int param_1)

{
  if (param_1 == 0) {
    FUN_000014e4(&DAT_000172a0,0x2e2);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) == 0x11111111) {
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = 0x4410000;
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = *(undefined4 *)(*(int *)(param_1 + 4) + 0x14);
    return ~*(uint *)(*(int *)(param_1 + 4) + 0x14) & 0xf0f;
  }
  FUN_000014e4(&DAT_000172a0,0x2e3);
  DAT_00028e84 = 1;
  return 0;
}



//==================== FUN_0000cd30 @ 0000cd30 ====================

uint FUN_0000cd30(int param_1)

{
  if (param_1 == 0) {
    FUN_000014e4(&DAT_000172a0,0x33f);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) == 0x11111111) {
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = 0x4410000;
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = *(undefined4 *)(*(int *)(param_1 + 4) + 0x14);
    return *(uint *)(*(int *)(param_1 + 4) + 0x14) & 0xf0;
  }
  FUN_000014e4(&DAT_000172a0,0x340);
  DAT_00028e84 = 1;
  return 0;
}



//==================== FUN_0000cdd4 @ 0000cdd4 ====================

void FUN_0000cdd4(int param_1,uint param_2)

{
  bool bVar1;
  
  if (param_1 == 0) {
    FUN_000014e4(&DAT_000172a0,0x368);
    DAT_00028e84 = 1;
    return;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) != 0x11111111) {
    FUN_000014e4(&DAT_000172a0,0x369);
    DAT_00028e84 = 1;
    return;
  }
  bVar1 = 7 < param_2;
  if (param_2 != 8) {
    bVar1 = 3 < param_2;
  }
  if (bVar1 && (param_2 != 8 && param_2 != 4)) {
    FUN_000014e4(&DAT_000172a0,0x36b);
    DAT_00028e84 = 1;
    return;
  }
  *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = 0x4410000;
  *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = *(undefined4 *)(*(int *)(param_1 + 4) + 0x14);
  *(uint *)(*(int *)(param_1 + 4) + 0x10) =
       *(uint *)(*(int *)(param_1 + 4) + 0x14) & 0xfff | (param_2 & 0xf) << 0xc | 0x8410000;
  return;
}



//==================== FUN_0000ceb8 @ 0000ceb8 ====================

uint FUN_0000ceb8(int param_1)

{
  if (param_1 == 0) {
    FUN_000014e4(&DAT_000172a0,0x393);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) == 0x11111111) {
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = 0x4410000;
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = *(undefined4 *)(*(int *)(param_1 + 4) + 0x14);
    return (*(uint *)(*(int *)(param_1 + 4) + 0x14) & 0xffff) >> 0xc;
  }
  FUN_000014e4(&DAT_000172a0,0x394);
  DAT_00028e84 = 1;
  return 0;
}



//==================== FUN_0000cf5c @ 0000cf5c ====================

undefined4 FUN_0000cf5c(int param_1,uint param_2,uint param_3,uint param_4,uint param_5)

{
  int iVar1;
  undefined4 uVar2;
  uint uVar3;
  bool bVar4;
  
  if (param_1 == 0) {
    FUN_000014e4(&DAT_000172a0,0x24a);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) != 0x11111111) {
    uVar2 = 0x24b;
LAB_0000cff8:
    FUN_000014e4(&DAT_000172a0,uVar2);
    DAT_00028e84 = 1;
    return 0;
  }
  bVar4 = 7 < param_2;
  if (param_2 != 8) {
    bVar4 = 5 < param_2;
  }
  if ((bVar4 && (param_2 != 8 && param_2 != 6)) && (0x12 < param_2 - 0xd)) {
    uVar2 = 0x24f;
    goto LAB_0000cff8;
  }
  if (1 < param_3) {
    uVar2 = 0x251;
    goto LAB_0000cff8;
  }
  if (1 < param_4) {
    uVar2 = 0x252;
    goto LAB_0000cff8;
  }
  if (1 < param_5) {
    uVar2 = 0x254;
    goto LAB_0000cff8;
  }
  iVar1 = FUN_0000ceb8(param_1);
  if (iVar1 != 3) {
    return 1;
  }
  *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = 0x4400000;
  *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = *(undefined4 *)(*(int *)(param_1 + 4) + 0x14);
  uVar3 = *(uint *)(*(int *)(param_1 + 4) + 0x14) & 0x3000;
  if (param_3 == 1) {
    bVar4 = 2 < param_2;
    if (param_2 != 3) {
      bVar4 = 0xe < param_2 - 0x10;
    }
    if (bVar4 && (param_2 != 3 && param_2 - 0x10 != 0xf)) {
      return 1;
    }
    uVar3 = uVar3 | 0x100;
    if (param_5 != 1) goto LAB_0000d09c;
  }
  else {
    if (param_5 != 1) goto LAB_0000d09c;
    bVar4 = 2 < param_2;
    if (param_2 != 3) {
      bVar4 = 0xe < param_2 - 0x10;
    }
    if (bVar4 && (param_2 != 3 && param_2 - 0x10 != 0xf)) {
      return 1;
    }
  }
  uVar3 = uVar3 | 0x400;
LAB_0000d09c:
  if (param_4 == 1) {
    uVar3 = uVar3 | 0x200;
  }
  *(uint *)(*(int *)(param_1 + 4) + 0x10) = param_2 & 0x1f | 0x8400000 | uVar3;
  return 0;
}



//==================== FUN_0000d190 @ 0000d190 ====================

uint FUN_0000d190(int param_1)

{
  if (param_1 == 0) {
    FUN_000014e4(&DAT_000172a0,0x3d8);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) == 0x11111111) {
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = 0x4420000;
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = *(undefined4 *)(*(int *)(param_1 + 4) + 0x14);
    return (*(uint *)(*(int *)(param_1 + 4) + 0x14) & 0xffff) >> 8;
  }
  FUN_000014e4(&DAT_000172a0,0x3d9);
  DAT_00028e84 = 1;
  return 0;
}



//==================== FUN_0000d234 @ 0000d234 ====================

undefined4 FUN_0000d234(int param_1,uint param_2)

{
  int iVar1;
  
  if (param_1 == 0) {
    FUN_000014e4(&DAT_000172a0,0x400);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) != 0x11111111) {
    FUN_000014e4(&DAT_000172a0,0x401);
    DAT_00028e84 = 1;
    return 0;
  }
  iVar1 = FUN_0000ceb8();
  if (iVar1 == 0) {
    *(uint *)(*(int *)(param_1 + 4) + 0x10) = param_2 & 0x7fe1 | 0x8480000;
    *(uint *)(*(int *)(param_1 + 4) + 0x10) = param_2 >> 0x10 | 0x8490000;
    return 0;
  }
  return 1;
}



//==================== FUN_0000d300 @ 0000d300 ====================

uint FUN_0000d300(int param_1)

{
  uint uVar1;
  
  if (param_1 == 0) {
    FUN_000014e4(&DAT_000172a0,0x434);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) == 0x11111111) {
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = 0x4480000;
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = *(undefined4 *)(*(int *)(param_1 + 4) + 0x14);
    uVar1 = *(uint *)(*(int *)(param_1 + 4) + 0x14);
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = 0x4490000;
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = *(undefined4 *)(*(int *)(param_1 + 4) + 0x14);
    return uVar1 & 0x7fe1 | *(int *)(*(int *)(param_1 + 4) + 0x14) << 0x10;
  }
  FUN_000014e4(&DAT_000172a0,0x435);
  DAT_00028e84 = 1;
  return 0;
}



//==================== FUN_0000d3cc @ 0000d3cc ====================

undefined4 FUN_0000d3cc(int param_1,uint param_2)

{
  int iVar1;
  
  if (param_1 == 0) {
    FUN_000014e4(&DAT_000172a0,0x463);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) != 0x11111111) {
    FUN_000014e4(&DAT_000172a0,0x464);
    DAT_00028e84 = 1;
    return 0;
  }
  iVar1 = FUN_0000ceb8();
  if (iVar1 == 0) {
    *(uint *)(*(int *)(param_1 + 4) + 0x10) = param_2 & 0x7fe0 | 0x84a0000;
    *(uint *)(*(int *)(param_1 + 4) + 0x10) = param_2 >> 0x10 | 0x84b0000;
    return 0;
  }
  return 1;
}



//==================== FUN_0000d498 @ 0000d498 ====================

uint FUN_0000d498(int param_1)

{
  uint uVar1;
  
  if (param_1 == 0) {
    FUN_000014e4(&DAT_000172a0,0x496);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) == 0x11111111) {
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = 0x44a0000;
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = *(undefined4 *)(*(int *)(param_1 + 4) + 0x14);
    uVar1 = *(uint *)(*(int *)(param_1 + 4) + 0x14);
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = 0x44b0000;
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = *(undefined4 *)(*(int *)(param_1 + 4) + 0x14);
    return uVar1 & 0x7fe0 | *(int *)(*(int *)(param_1 + 4) + 0x14) << 0x10;
  }
  FUN_000014e4(&DAT_000172a0,0x497);
  DAT_00028e84 = 1;
  return 0;
}



//==================== FUN_0000d564 @ 0000d564 ====================

undefined4 FUN_0000d564(int param_1,uint param_2)

{
  int iVar1;
  
  if (param_1 == 0) {
    FUN_000014e4(&DAT_000172a0,0x4c3);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) != 0x11111111) {
    FUN_000014e4(&DAT_000172a0,0x4c4);
    DAT_00028e84 = 1;
    return 0;
  }
  iVar1 = FUN_0000ceb8();
  if (iVar1 == 0) {
    *(uint *)(*(int *)(param_1 + 4) + 0x10) = param_2 & 0x800 | 0x84c0000;
    *(uint *)(*(int *)(param_1 + 4) + 0x10) = param_2 >> 0x10 | 0x84d0000;
    return 0;
  }
  return 1;
}



//==================== FUN_0000d62c @ 0000d62c ====================

uint FUN_0000d62c(int param_1)

{
  uint uVar1;
  
  if (param_1 == 0) {
    FUN_000014e4(&DAT_000172a0,0x4f6);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) == 0x11111111) {
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = 0x44c0000;
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = *(undefined4 *)(*(int *)(param_1 + 4) + 0x14);
    uVar1 = *(uint *)(*(int *)(param_1 + 4) + 0x14);
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = 0x44d0000;
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = *(undefined4 *)(*(int *)(param_1 + 4) + 0x14);
    return uVar1 & 0x800 | *(int *)(*(int *)(param_1 + 4) + 0x14) << 0x10;
  }
  FUN_000014e4(&DAT_000172a0,0x4f7);
  DAT_00028e84 = 1;
  return 0;
}



//==================== FUN_0000d6f4 @ 0000d6f4 ====================

undefined4 FUN_0000d6f4(int param_1,uint param_2)

{
  int iVar1;
  
  if (param_1 == 0) {
    FUN_000014e4(&DAT_000172a0,0x526);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) != 0x11111111) {
    FUN_000014e4(&DAT_000172a0,0x527);
    DAT_00028e84 = 1;
    return 0;
  }
  iVar1 = FUN_0000ceb8();
  if (iVar1 == 0) {
    *(uint *)(*(int *)(param_1 + 4) + 0x10) = param_2 & 0x800 | 0x84e0000;
    *(uint *)(*(int *)(param_1 + 4) + 0x10) = param_2 >> 0x10 | 0x84f0000;
    return 0;
  }
  return 1;
}



//==================== FUN_0000d7bc @ 0000d7bc ====================

uint FUN_0000d7bc(int param_1)

{
  uint uVar1;
  
  if (param_1 == 0) {
    FUN_000014e4(&DAT_000172a0,0x55a);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) == 0x11111111) {
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = 0x44e0000;
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = *(undefined4 *)(*(int *)(param_1 + 4) + 0x14);
    uVar1 = *(uint *)(*(int *)(param_1 + 4) + 0x14);
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = 0x44f0000;
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = *(undefined4 *)(*(int *)(param_1 + 4) + 0x14);
    return uVar1 & 0x800 | *(int *)(*(int *)(param_1 + 4) + 0x14) << 0x10;
  }
  FUN_000014e4(&DAT_000172a0,0x55b);
  DAT_00028e84 = 1;
  return 0;
}



//==================== FUN_0000d884 @ 0000d884 ====================

void FUN_0000d884(int param_1,uint param_2,uint param_3)

{
  if (param_1 == 0) {
    FUN_000014e4(&DAT_000172a0,0x583);
    DAT_00028e84 = 1;
    return;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) != 0x11111111) {
    FUN_000014e4(&DAT_000172a0,0x584);
    DAT_00028e84 = 1;
    return;
  }
  if (param_2 < 0x10) {
    *(uint *)(*(int *)(param_1 + 4) + 0x10) = param_3 | 0x8000000 | (param_2 + 0x50) * 0x10000;
    return;
  }
  FUN_000014e4(&DAT_000172a0,0x585);
  DAT_00028e84 = 1;
  return;
}



//==================== FUN_0000d938 @ 0000d938 ====================

uint FUN_0000d938(int param_1,uint param_2)

{
  if (param_1 == 0) {
    FUN_000014e4(&DAT_000172a0,0x5a5);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) != 0x11111111) {
    FUN_000014e4(&DAT_000172a0,0x5a6);
    DAT_00028e84 = 1;
    return 0;
  }
  if (param_2 < 0x10) {
    *(uint *)(*(int *)(param_1 + 4) + 0x10) = (param_2 + 0x50) * 0x10000 | 0x4000000;
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = *(undefined4 *)(*(int *)(param_1 + 4) + 0x14);
    return *(uint *)(*(int *)(param_1 + 4) + 0x14) & 0xffff;
  }
  FUN_000014e4(&DAT_000172a0,0x5a7);
  DAT_00028e84 = 1;
  return 0;
}



//==================== FUN_0000dc4c @ 0000dc4c ====================

uint FUN_0000dc4c(int param_1)

{
  if (param_1 == 0) {
    FUN_000014e4(&DAT_000172a0,0x643);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) == 0x11111111) {
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = 0x4400000;
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = *(undefined4 *)(*(int *)(param_1 + 4) + 0x14);
    return (*(uint *)(*(int *)(param_1 + 4) + 0x14) & 0x3ff) >> 9;
  }
  FUN_000014e4(&DAT_000172a0,0x644);
  DAT_00028e84 = 1;
  return 0;
}



//==================== FUN_0000deac @ 0000deac ====================

uint FUN_0000deac(int param_1)

{
  if (param_1 == 0) {
    FUN_000014e4(&DAT_000172a0,0x6d0);
    DAT_00028e84 = 1;
    return 0;
  }
  DAT_00028e84 = 0;
  if (*(int *)(param_1 + 8) == 0x11111111) {
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = 0x4420000;
    *(undefined4 *)(*(int *)(param_1 + 4) + 0x10) = *(undefined4 *)(*(int *)(param_1 + 4) + 0x14);
    return *(uint *)(*(int *)(param_1 + 4) + 0x14) >> 4 & 0xffffffc;
  }
  FUN_000014e4(&DAT_000172a0,0x6d1);
  DAT_00028e84 = 1;
  return 0;
}



//==================== FUN_0000dfb0 @ 0000dfb0 ====================

uint * FUN_0000dfb0(uint *param_1,uint param_2)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  
  param_2 = param_2 & 0xff;
  if (param_2 == 0) {
    if (((uint)param_1 & 3) != 0) {
      if ((byte)*param_1 == 0) {
        return param_1;
      }
      while (param_1 = (uint *)((int)param_1 + 1), ((uint)param_1 & 3) != 0) {
        if (*(byte *)param_1 == 0) {
          return param_1;
        }
      }
    }
    uVar1 = *param_1 + 0xfefefeff & ~*param_1;
    while ((uVar1 & 0x80808080) == 0) {
      param_1 = param_1 + 1;
      uVar1 = *param_1 + 0xfefefeff & ~*param_1;
    }
    if ((byte)*param_1 == 0) {
      return param_1;
    }
    do {
      param_1 = (uint *)((int)param_1 + 1);
    } while (*(byte *)param_1 != 0);
    return param_1;
  }
  if (((uint)param_1 & 3) != 0) {
    if ((byte)*param_1 == 0) {
      return (uint *)0x0;
    }
    if (param_2 == (byte)*param_1) {
      return param_1;
    }
    while (param_1 = (uint *)((int)param_1 + 1), ((uint)param_1 & 3) != 0) {
      if (*(byte *)param_1 == 0) {
        return (uint *)0x0;
      }
      if (*(byte *)param_1 == param_2) {
        return param_1;
      }
    }
  }
  uVar2 = param_2 | param_2 << 8;
  uVar1 = *param_1;
  uVar2 = uVar2 | uVar2 << 0x10;
  uVar3 = uVar2 ^ uVar1;
  for (uVar1 = uVar3 + 0xfefefeff & ~uVar3 | uVar1 + 0xfefefeff & ~uVar1; (uVar1 & 0x80808080) == 0;
      uVar1 = uVar1 + 0xfefefeff & ~uVar1 | uVar3 + 0xfefefeff & ~uVar3) {
    param_1 = param_1 + 1;
    uVar1 = *param_1;
    uVar3 = uVar1 ^ uVar2;
  }
  if ((byte)*param_1 != 0) {
    if (param_2 == (byte)*param_1) {
      return param_1;
    }
    while( true ) {
      param_1 = (uint *)((int)param_1 + 1);
      if (*(byte *)param_1 == 0) break;
      if (*(byte *)param_1 == param_2) {
        return param_1;
      }
    }
  }
  return (uint *)0x0;
}



//==================== FUN_0000e138 @ 0000e138 ====================

/* WARNING: Removing unreachable block (ram,0x0000e23c) */
/* WARNING: Removing unreachable block (ram,0x0000e268) */
/* WARNING: Removing unreachable block (ram,0x0000e274) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_0000e138(void)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  uint uVar4;
  
  iVar1 = func_0xffffcbec();
  if (iVar1 != 0) {
    uVar3 = func_0xffffc97c();
    func_0xffffde2c(&DAT_00016954,uVar3);
    func_0xffffde2c(&DAT_00016410,0xa012);
    do {
    } while ((_DAT_e000002c & 8) == 0);
    func_0xffff9580();
  }
  _DAT_f8000008 = 0xdf0ddf0d;
  func_0xffffd6a4();
  func_0xffffd700();
  func_0xffffdbcc();
  func_0xffffdbd0(1,0x1b28,0);
  func_0xffffdbd0(2,0x1b0c,0);
  func_0xffffdbd0(3,0x1af0,0);
  func_0xffffdbd0(4,0x1ad4,0);
  func_0xffffdbd0(5,0x1ab8,0);
  func_0xffffdbd0(6,0x1a9c,0);
  func_0xffffde2c(&DAT_0001696c);
  func_0xffffde2c(&DAT_0001699c,0x7e2,3,&DAT_00016990,&DAT_000169b4);
  _DAT_00200000 = 0xaa55aa55;
  _DAT_00100000 = 0xaa55aa55;
  iVar1 = func_0xffffc174();
  if (iVar1 == 1) {
    func_0xffffde2c(&DAT_000169d4);
    func_0xffffde2c(&DAT_00016410,0xa002);
    do {
    } while ((_DAT_e000002c & 8) == 0);
    func_0xffff9580();
  }
  func_0xffffde2c(&DAT_000169e8);
  func_0xffffa560();
  DAT_00021f94 = FUN_00000714(DAT_00028e74);
  if ((DAT_00021f94 & 0x1000) != 0) {
    func_0xffffde2c(&DAT_00016a08);
  }
  _DAT_f8000258 = _DAT_f8000258 | 0x60000000;
  uVar4 = _DAT_f800025c & 7;
  if (uVar4 == 4) {
    func_0xffffde2c(&DAT_00016a34);
    iVar1 = func_0xffffbe84();
    if (iVar1 != 0) {
      func_0xffffde2c(&DAT_00016a48);
      func_0xffffde2c(&DAT_00016410,0xa00a);
      do {
      } while ((_DAT_e000002c & 8) == 0);
      func_0xffffa790();
    }
    DAT_00021bf8 = 0x2fa8;
    func_0xffffde2c(&DAT_00016a5c);
  }
  else if (uVar4 == 2) {
    func_0xffffde2c(&DAT_00016a70);
    func_0xffffc118();
    func_0xffffde2c(&DAT_00016a84);
    DAT_00021bf8 = 0x31ac;
  }
  else if (uVar4 == 5) {
    func_0xffffde2c(&DAT_00016a98);
    iVar1 = func_0xffffcff4(&DAT_00016aac);
    if (iVar1 != 0) {
      func_0xffffde2c(&DAT_00016ab8);
      func_0xffffde2c(&DAT_00016410,0xa009);
      do {
      } while ((_DAT_e000002c & 8) == 0);
      func_0xffffa790();
    }
    DAT_00021bf8 = 0x4124;
    func_0xffffde2c(&DAT_00016ac8);
  }
  else if (uVar4 == 6) {
    func_0xffffde2c(&DAT_00016ad8);
    iVar1 = func_0xffffcff4(&DAT_00016aac);
    if (iVar1 != 0) {
      func_0xffffde2c(&DAT_00016af0);
      func_0xffffde2c(&DAT_00016410,0xa009);
      do {
      } while ((_DAT_e000002c & 8) == 0);
      func_0xffffa790();
    }
    DAT_00021bf8 = 0x4124;
    func_0xffffde2c(&DAT_00016b00);
  }
  else if (uVar4 == 0) {
    func_0xffffde2c(&DAT_00016b14);
    if ((_DAT_f800700c & 4) != 0) {
      func_0xffffcb4c();
      _DAT_f8000008 = 0xdf0ddf0d;
    }
    _DAT_f8000258 = _DAT_f8000258 & 0xfffffff;
    _DAT_f8000004 = 0x767b767b;
    FUN_0000e948();
  }
  else {
    func_0xffffde2c(&DAT_00016b28);
    func_0xffffde2c(&DAT_00016410,0xa000);
    do {
    } while ((_DAT_e000002c & 8) == 0);
    func_0xffffa790();
  }
  func_0xffffde2c(&DAT_00016b40,DAT_0002105c);
  if ((DAT_0002105c + 0x1f000000U & 0xfeffffff) != 0 &&
      (DAT_0002105c != -0x4000000 && DAT_0002105c != -0x1ff00000)) {
    func_0xffffde2c(&DAT_00016b60);
    func_0xffffde2c(&DAT_00016410,0xa006);
    do {
    } while ((_DAT_e000002c & 8) == 0);
    func_0xffffa790();
  }
  if (DAT_0002105c == -0x1e000000) {
    func_0xffffde2c(&DAT_00016b7c);
    DAT_00021060 = 1;
  }
  DAT_00021f90 = 1;
  iVar2 = func_0xffffabcc();
  if (iVar2 != 0) {
    func_0xffffde2c(&DAT_00016b94);
    func_0xffffa790();
  }
  uVar3 = func_0xffff9dc4();
  func_0xffffde2c(&DAT_00016bb4,uVar3);
  func_0xffffa8e0(uVar3);
  return iVar1;
}



//==================== FUN_0000e6c0 @ 0000e6c0 ====================

int FUN_0000e6c0(uint *param_1,int param_2)

{
  int iVar1;
  int local_2c [3];
  undefined4 local_20;
  int local_1c;
  
  local_20 = 2;
  FUN_0000bb98(local_2c);
  if (local_2c[0] - 0x766eU < 0x53d1) {
    if (local_1c - 0x91ebU < 0x170b) {
      if (param_2 + 0xfeced300U < 0x2625a01) {
        iVar1 = FUN_0000aedc(0,param_2,1);
        if (iVar1 == 0) {
          iVar1 = FUN_0000b254(param_1);
          if (iVar1 == 0) {
            if (((*param_1 & 1) == 0) ||
               ((iVar1 = FUN_0000aedc(0,param_2,2), iVar1 == 0 &&
                (((*param_1 & 1) == 0 ||
                 ((iVar1 = FUN_0000aedc(0,param_2,3), iVar1 == 0 && ((*param_1 & 1) == 0)))))))) {
              iVar1 = 8;
            }
          }
          else {
            iVar1 = 1;
          }
        }
      }
      else {
        iVar1 = 0xb;
      }
    }
    else {
      iVar1 = 3;
    }
  }
  else {
    iVar1 = 2;
  }
  return iVar1;
}



//==================== FUN_0000e7c4 @ 0000e7c4 ====================

int FUN_0000e7c4(uint param_1,undefined4 param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int local_2c [3];
  undefined4 local_20;
  int local_1c;
  
  local_20 = 2;
  FUN_0000bb98(local_2c);
  if (local_2c[0] - 0x766eU < 0x53d1) {
    if (local_1c - 0x91ebU < 0x170b) {
      iVar1 = FUN_0000b6a4(param_1);
      if (iVar1 == 0) {
        iVar1 = FUN_0000e6c0(param_1,param_2);
      }
      iVar3 = param_1 + 0x87c + (param_1 & 0x7f) * -2;
      iVar2 = FUN_0000b6a4(iVar3);
      if (iVar2 == 0) {
        iVar2 = FUN_0000e6c0(iVar3,param_2);
        if (iVar2 != 0 && iVar1 != 0) {
          return iVar2;
        }
      }
      else if (iVar1 != 0) {
        return iVar2;
      }
      iVar1 = 0;
    }
    else {
      iVar1 = 6;
    }
  }
  else {
    iVar1 = 5;
  }
  return iVar1;
}



//==================== FUN_0000e88c @ 0000e88c ====================

int FUN_0000e88c(undefined4 param_1,undefined4 param_2)

{
  byte *pbVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  byte local_144 [32];
  undefined1 auStack_124 [264];
  
  FUN_000027e0(auStack_124,0,0x104);
  iVar3 = -0x7ff1f80;
  iVar4 = 0;
  FUN_0000be74(param_1,auStack_124,0x100);
  FUN_0000af74();
  do {
    iVar5 = 0;
    FUN_0000b0e4(auStack_124 + iVar4 * 0x1a,local_144);
    do {
      iVar2 = FUN_0000b254(iVar3);
      pbVar1 = local_144 + iVar5;
      if (iVar2 == 1) {
        iVar3 = iVar3 + 4;
      }
      iVar5 = iVar5 + 1;
      if (((*pbVar1 & 1) != 0) && (iVar2 = FUN_0000e7c4(iVar3,param_2), iVar2 != 0)) {
        return iVar2;
      }
      iVar3 = iVar3 + 4;
    } while (iVar5 != 0x1f);
    iVar4 = iVar4 + 1;
    if (iVar4 == 10) {
      return 0;
    }
  } while( true );
}



//==================== FUN_0000e948 @ 0000e948 ====================

void FUN_0000e948(undefined4 param_1)

{
  coproc_moveto_Invalidate_Entire_Instruction(param_1);
  coproc_moveto_Instruction_cache(param_1,6);
  DataSynchronizationBarrier(0xf);
  InstructionSynchronizationBarrier(0xf);
  coproc_moveto_Control(0);
  InstructionSynchronizationBarrier(0xf);
  do {
    WaitForEvent();
  } while( true );
}



