//==================== FUN_00011580 @ 00011580 ====================

void FUN_00011580(uint param_1,uint param_2,char *param_3,size_t param_4,int param_5)

{
  uint uVar1;
  byte bVar2;
  bool bVar3;
  uint in_fpscr;
  double dVar4;
  int iVar5;
  double dVar6;
  double dVar7;
  char local_34 [4];
  
  bVar3 = param_2 < 0xde0b6b4;
  if (param_2 == 0xde0b6b3) {
    bVar3 = param_1 < 0xa7640000;
  }
  local_34[0] = '\0';
  local_34[1] = '\0';
  if (bVar3) {
    bVar3 = param_2 < 0x38d7f;
    if (param_2 == 0x38d7e) {
      bVar3 = param_1 < 0xa4c68000;
    }
    if (bVar3) {
      bVar3 = param_2 < 0xe9;
      if (param_2 == 0xe8) {
        bVar3 = param_1 < 0xd4a51000;
      }
      if (bVar3) {
        bVar3 = param_2 == 0;
        if (param_2 == 0) {
          bVar3 = param_1 < 1000000000;
        }
        if (bVar3) {
          bVar3 = param_2 == 0;
          if (param_2 == 0) {
            bVar3 = param_1 < 1000000;
          }
          if (bVar3) {
            bVar3 = param_2 == 0;
            if (param_2 == 0) {
              bVar3 = param_1 < 1000;
            }
            if (bVar3) {
              dVar7 = (double)FUN_0005cb50();
              if (param_5 == 0) {
                iVar5 = VectorFloatToUnsigned(dVar7,3);
                snprintf(param_3,param_4,"%d%s",iVar5,local_34);
                return;
              }
              goto LAB_00011660;
            }
            dVar7 = (double)FUN_0005cb50();
            local_34[0] = 'K';
            local_34[1] = '\0';
            dVar7 = dVar7 / 1000.0;
          }
          else {
            FUN_0005cbc4(param_1,param_2,1000,0);
            dVar7 = (double)FUN_0005cb50();
            local_34[0] = 'M';
            local_34[1] = '\0';
            dVar7 = dVar7 / 1000.0;
          }
        }
        else {
          FUN_0005cbc4(param_1,param_2,1000000,0);
          dVar7 = (double)FUN_0005cb50();
          local_34[0] = 'G';
          local_34[1] = '\0';
          dVar7 = dVar7 / 1000.0;
        }
      }
      else {
        FUN_0005cbc4(param_1,param_2,1000000000,0);
        dVar7 = (double)FUN_0005cb50();
        local_34[0] = 'T';
        local_34[1] = '\0';
        dVar7 = dVar7 / 1000.0;
      }
    }
    else {
      FUN_0005cbc4(param_1,param_2,0xd4a51000,0xe8);
      dVar7 = (double)FUN_0005cb50();
      local_34[0] = 'P';
      local_34[1] = '\0';
      dVar7 = dVar7 / 1000.0;
    }
  }
  else {
    FUN_0005cbc4(param_1,param_2,0xa4c68000,0x38d7e);
    dVar7 = (double)FUN_0005cb50();
    local_34[0] = 'E';
    local_34[1] = '\0';
    dVar7 = dVar7 / 1000.0;
  }
  if (param_5 == 0) {
    snprintf(param_3,param_4,"%.3g%s",dVar7,local_34);
    return;
  }
LAB_00011660:
  uVar1 = in_fpscr & 0xfffffff | (uint)(dVar7 < 0.0) << 0x1f | (uint)(dVar7 == 0.0) << 0x1e;
  bVar2 = (byte)(uVar1 >> 0x18);
  dVar6 = (double)VectorSignedToFloat(param_5 + -1,(byte)(uVar1 >> 0x16) & 3);
  if ((bool)(bVar2 >> 6 & 1) || (bool)(bVar2 >> 7) != NAN(dVar7)) {
    dVar4 = 0.0;
  }
  else {
    dVar4 = log10(dVar7);
    dVar4 = floor(dVar4);
  }
  iVar5 = VectorFloatToSigned(dVar6 - dVar4,3);
  snprintf(param_3,param_4,"%*.*f%s",param_5 + 1,iVar5,dVar7,local_34);
  return;
}

