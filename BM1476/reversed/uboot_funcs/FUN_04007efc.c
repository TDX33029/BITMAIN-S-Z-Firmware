//==================== FUN_04007efc @ 04007efc ====================

void FUN_04007efc(int param_1,ushort *param_2,int param_3)

{
  ushort uVar1;
  int *piVar2;
  int *piVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  code *pcVar6;
  int iVar7;
  int local_24;
  
  piVar2 = DAT_040080f4;
  if (param_3 < 0x1c) {
    FUN_04007548(DAT_040080f0,param_3,0x1c);
  }
  else if (((((ushort)(*param_2 >> 8 | *param_2 << 8) == 1) &&
            ((ushort)(param_2[1] >> 8 | param_2[1] << 8) == 0x800)) && ((char)param_2[2] == '\x06'))
          && (*(char *)((int)param_2 + 5) == '\x04')) {
    if (*DAT_040080f4 != 0) {
      FUN_04005ea4(&local_24,param_2 + 0xc,4);
      piVar3 = DAT_040080f8;
      if (*piVar2 == local_24) {
        uVar1 = param_2[3] >> 8 | param_2[3] << 8;
        if (uVar1 == 1) {
          iVar7 = FUN_0400a4b8(param_1,param_1 + 6,0x806);
          param_2[3] = 0x200;
          FUN_04005ea4(param_2 + 9,param_2 + 4,6);
          FUN_04005ea4(param_2 + 0xc,param_2 + 7,4);
          FUN_04005ea4(param_2 + 4,DAT_0400810c,6);
          FUN_04005ea4(param_2 + 7,piVar2,4);
          FUN_0400929c(param_1,iVar7 + 0x1c);
        }
        else if (uVar1 == 2) {
          if (*DAT_040080f8 != 0) {
            FUN_04005ea4(&local_24,param_2 + 7,4);
            piVar2 = DAT_04008100;
            if (local_24 == *DAT_040080fc) {
              if (*DAT_04008100 != 0) {
                FUN_04005ea4(*DAT_04008100,param_2 + 4,6);
              }
              pcVar6 = (code *)FUN_0400a02c();
              puVar5 = DAT_04008108;
              puVar4 = DAT_04008104;
              (*pcVar6)(param_2,0,local_24,0,param_3);
              FUN_04005ea4(*puVar4,param_2 + 4,6);
              FUN_0400929c(*puVar4,*puVar5);
              *piVar3 = 0;
              *puVar5 = 0;
              *piVar2 = 0;
            }
          }
        }
      }
    }
  }
  return;
}

