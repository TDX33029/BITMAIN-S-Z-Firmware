//==================== FUN_00012c40 @ 00012c40 ====================

int FUN_00012c40(undefined4 param_1,undefined4 param_2,undefined4 param_3,char *param_4,char param_5
                ,char param_6,char param_7)

{
  undefined4 uVar1;
  undefined4 extraout_r1;
  int iVar2;
  undefined4 local_bfc;
  undefined1 auStack_bf8 [256];
  undefined1 auStack_af8 [112];
  undefined4 local_a88;
  undefined4 local_a84;
  undefined4 local_a80;
  undefined4 local_a7c;
  undefined4 local_a78;
  undefined4 local_a74;
  undefined4 local_a70;
  undefined4 local_a6c;
  undefined4 local_a68;
  undefined1 auStack_a64 [252];
  undefined1 auStack_968 [112];
  undefined1 auStack_8f8 [1056];
  undefined4 local_4d8;
  undefined1 auStack_4d4 [1020];
  int local_d8;
  undefined4 uStack_d4;
  undefined1 auStack_d0 [48];
  int local_a0;
  uint local_68;
  uint local_64;
  int local_60;
  int local_5c;
  int local_58;
  int local_54;
  int local_50;
  int local_4c;
  uint local_48;
  int local_44;
  uint local_40;
  ushort local_3a;
  undefined8 local_38;
  int local_30;
  int local_2c;
  int local_28;
  int local_24;
  int local_20;
  int local_1c;
  uint local_18;
  byte local_11;
  
  FUN_00013c2c(param_1,auStack_d0);
  local_2c = local_a0;
  if (local_a0 < 0x800) {
    FUN_000107bc("File \'%s\' Not Enough %d, Something Wrong!\n",param_1,0x800);
    local_4c = 6;
  }
  else {
    local_30 = FUN_00010798(param_1,&DAT_00013f00);
    if (local_30 == 0) {
      FUN_000107bc("Read File \'%s\' Failed!\n",param_1);
      local_4c = 7;
    }
    else {
      FUN_0001082c(param_4,0x800,1,local_30);
      if (*param_4 == '&') {
        FUN_000107bc("miner_type %s\n",param_2);
        uVar1 = FUN_00010780(param_2);
        local_38 = FUN_00012180(param_2,uVar1);
        FUN_0001078c(&local_d8,param_4 + 2,8);
        iVar2 = local_d8;
        uVar1 = uStack_d4;
        FUN_000107bc("miner_type_hash %16llx, bmu_hash %16llx\n",extraout_r1,(undefined4)local_38,
                     local_38._4_4_,local_d8,uStack_d4);
        if (local_38 == CONCAT44(uStack_d4,local_d8)) {
          local_11 = 0;
          local_3a = CONCAT11(param_4[0xb],param_4[0xc]);
          if (param_7 != '\0') {
            FUN_000107bc("content:%x\n",local_3a,(int)(short)((ushort)(byte)param_4[0xb] << 8),
                         local_3a,iVar2,uVar1);
            if ((local_3a & 0xfe00) != 0xfe00) {
              FUN_000107f0("This Package Was Not Full Package!");
              return 0x14;
            }
          }
          for (local_18 = 0; (int)local_18 < 0x10; local_18 = local_18 + 1) {
            if (((int)(uint)local_3a >> (local_18 & 0xff) & 1U) != 0) {
              local_11 = local_11 + 1;
            }
          }
          if (local_11 == param_4[0x518]) {
            local_1c = ((byte)param_4[0x518] + 9) * 0x100;
            local_40 = (uint)(byte)param_4[0x519] << 0x18 | (uint)(byte)param_4[0x51a] << 0x10 |
                       (uint)(byte)param_4[0x51b] << 8 | (uint)(byte)param_4[0x51c];
            for (local_20 = 0; local_20 < (int)(uint)local_11; local_20 = local_20 + 1) {
              local_68 = (uint)(byte)param_4[local_20 * 5 + 0x51e] << 0x18 |
                         (uint)(byte)param_4[local_20 * 5 + 0x51f] << 0x10 |
                         (uint)(byte)param_4[local_20 * 5 + 0x520] << 8 |
                         (uint)(byte)param_4[local_20 * 5 + 0x521];
              local_1c = local_1c + local_68;
              FUN_000107bc("file[%d] size:[%d]\n",local_20,local_68,local_1c,iVar2,uVar1);
            }
            if (local_1c == local_2c) {
              local_44 = FUN_00010798(param_3,&DAT_00014290);
              if (local_44 == 0) {
                FUN_000107bc("Cannot Open Root PublicKey \'%s\'!\n",param_3);
                if (local_30 != 0) {
                  thunk_EXT_FUN_706d742e(local_30);
                }
                local_4c = 0xb;
              }
              else {
                local_4d8 = 0;
                thunk_EXT_FUN_756e752e(auStack_4d4,0,0x3fc);
                FUN_0001082c(&local_4d8,0x400,1,local_44);
                thunk_EXT_FUN_706d742e(local_44);
                local_44 = 0;
                local_48 = (uint)CONCAT11(param_4[0x16],param_4[0x17]);
                local_4c = FUN_00010c70(param_4 + 0x18,local_48,param_4 + 0x418,&local_4d8);
                if (local_4c == 0) {
                  local_50 = FUN_00010798("/usr/bin/fw_version",&DAT_000142dc);
                  if (local_50 != 0) {
                    FUN_000107bc("fw version data: %02X %02X %02X %02X %02X %02X %02X %02X\n",
                                 param_4[0xd],param_4[0xe],param_4[0xf],param_4[0x10],param_4[0x11],
                                 param_4[0x12],param_4[0x13],param_4[0x14]);
                    FUN_000107d4(param_4 + 0xd,1,8,local_50);
                    thunk_EXT_FUN_706d742e(local_50);
                  }
                  if (param_5 != '\0') {
                    local_54 = FUN_00010798("/tmp/tmpfw/miner.pem",&DAT_000142dc);
                    if (local_54 == 0) {
                      FUN_000107f0("Dump Miner.pem Failed!\r");
                      return 0xf;
                    }
                    FUN_000107d4(param_4 + 0x18,1,local_48,local_54);
                    thunk_EXT_FUN_706d742e(local_54);
                    local_54 = FUN_00010798("/tmp/tmpfw/miner.pem.sig",&DAT_000142dc);
                    if (local_54 == 0) {
                      FUN_000107f0("Dump Miner.pem.sig Failed!\r");
                      return 0x10;
                    }
                    FUN_000107d4(param_4 + 0x418,1,0x100,local_54);
                    thunk_EXT_FUN_706d742e(local_54);
                  }
                  thunk_EXT_FUN_756e752e(auStack_8f8,0,0x420);
                  FUN_00010820(auStack_968);
                  FUN_00010808(auStack_968,param_4,0x800);
                  FUN_00010774(auStack_8f8,auStack_968);
                  for (local_24 = 0; local_24 < (int)(uint)local_11; local_24 = local_24 + 1) {
                    local_64 = (uint)(byte)param_4[local_24 * 5 + 0x51e] << 0x18 |
                               (uint)(byte)param_4[local_24 * 5 + 0x51f] << 0x10 |
                               (uint)(byte)param_4[local_24 * 5 + 0x520] << 8 |
                               (uint)(byte)param_4[local_24 * 5 + 0x521];
                    FUN_00010de8(local_30,local_64,auStack_8f8 + (local_24 + 1) * 0x20,
                                 param_4[local_24 * 5 + 0x51d],0,param_5);
                  }
                  for (local_28 = 0; local_28 < (int)(uint)local_11; local_28 = local_28 + 1) {
                    FUN_00010de8(local_30,0x100,auStack_8f8 + (local_11 + 1 + local_28) * 0x20,
                                 param_4[local_28 * 5 + 0x51d],1,param_5);
                  }
                  local_a68 = 0;
                  thunk_EXT_FUN_756e752e(auStack_a64,0,0xfc);
                  FUN_0001075c(local_30,0xffffff00,2);
                  FUN_0001082c(&local_a68,0x100,1,local_30);
                  local_a88 = 0;
                  local_a84 = 0;
                  local_a80 = 0;
                  local_a7c = 0;
                  local_a78 = 0;
                  local_a74 = 0;
                  local_a70 = 0;
                  local_a6c = 0;
                  FUN_00010820(auStack_af8);
                  FUN_00010808(auStack_af8,auStack_8f8,(uint)local_11 * 0x40 + 0x20);
                  FUN_00010774(&local_a88,auStack_af8);
                  local_58 = 0;
                  local_5c = 0;
                  local_58 = FUN_000107a4(param_4 + 0x18,0x400);
                  local_5c = FUN_00010768(local_58,0,0,0);
                  if (local_5c == 0) {
                    uVar1 = FUN_00010714();
                    uVar1 = FUN_000107c8(uVar1,0);
                    FUN_000107bc("OpenSSL error: %s\n",uVar1);
                    FUN_000107f0("Load Pem Failed!");
                    local_4c = 0x11;
                  }
                  else {
                    local_60 = thunk_EXT_FUN_00646572
                                         (0x2a0,&local_a88,0x20,&local_a68,0x100,local_5c);
                    if (local_5c != 0) {
                      FUN_000107fc(local_5c);
                    }
                    if (local_58 != 0) {
                      FUN_00010814(local_58);
                    }
                    if (local_60 == 1) {
                      if (local_30 != 0) {
                        thunk_EXT_FUN_706d742e(local_30);
                      }
                      local_30 = 0;
                      FUN_000107f0("All Done!\r");
                      if (param_6 != '\0') {
                        FUN_000107f0("This Comment Of This Package:");
                        local_bfc = 0;
                        thunk_EXT_FUN_756e752e(auStack_bf8,0,0xfd);
                        FUN_0001078c(&local_bfc,param_4 + 0x550,0x100);
                        FUN_000107f0(&local_bfc);
                      }
                      local_4c = 0;
                    }
                    else {
                      FUN_000107f0("Check File Sig failed!\r");
                      uVar1 = FUN_00010714();
                      uVar1 = FUN_000107c8(uVar1,0);
                      FUN_000107bc("OpenSSL error: %s\n",uVar1);
                      local_4c = 0x12;
                    }
                  }
                }
                else {
                  FUN_000107bc("Check pem payload failed! ret:[%d]\n",local_4c);
                  if (local_30 != 0) {
                    thunk_EXT_FUN_706d742e(local_30);
                  }
                }
              }
            }
            else {
              FUN_000107bc("Check FileSize Failed, FileSize Should Be [%d]Bytes, But It Was [%d] Bytes, And Total Says[%d]\n"
                           ,local_40,local_2c,local_1c,iVar2,uVar1);
              if (local_30 != 0) {
                thunk_EXT_FUN_706d742e(local_30);
              }
              local_4c = 10;
            }
          }
          else {
            FUN_000107bc("Content Doesn\'t Match![%d][%d]\n",param_4[0x518],local_11,local_11,iVar2,
                         uVar1);
            if (local_30 != 0) {
              thunk_EXT_FUN_706d742e(local_30);
            }
            local_4c = 9;
          }
        }
        else {
          FUN_000107f0("input miner_type and bmu miner type donot match!");
          if (local_30 != 0) {
            thunk_EXT_FUN_706d742e(local_30);
          }
          local_4c = 0x1e;
        }
      }
      else {
        FUN_000107bc("\'%s\' Not A Btmu File!\n",param_1);
        if (local_30 != 0) {
          thunk_EXT_FUN_706d742e(local_30);
        }
        local_4c = 8;
      }
    }
  }
  return local_4c;
}

