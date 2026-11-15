//==================== FUN_00054dcc @ 00054dcc ====================

undefined4 FUN_00054dcc(ulonglong *param_1,byte *param_2)

{
  byte *pbVar1;
  byte bVar2;
  undefined4 uVar3;
  ulonglong *puVar4;
  char *pcVar6;
  char *pcVar5;
  byte *pbVar7;
  ulonglong uVar8;
  ulonglong uVar9;
  ulonglong uVar10;
  ulonglong uVar11;
  ulonglong uVar12;
  ulonglong uVar13;
  ulonglong uVar14;
  
  memset(param_1,0,0x168);
  puVar4 = param_1 + -1;
  pcVar5 = "hPoW";
  do {
    pcVar6 = pcVar5 + 8;
    uVar3 = *(undefined4 *)(pcVar5 + 0xc);
    *(undefined4 *)(puVar4 + 1) = *(undefined4 *)pcVar6;
    *(undefined4 *)((int)puVar4 + 0xc) = uVar3;
    puVar4 = puVar4 + 1;
    pcVar5 = pcVar6;
  } while (pcVar6 != &UNK_0006c578);
  pbVar7 = param_2 + 0x40;
  do {
    uVar9 = VectorShiftLeft((ulonglong)param_2[1],8,0x40,0);
    bVar2 = *param_2;
    uVar8 = VectorShiftLeft((ulonglong)param_2[2],0x10,0x40,0);
    uVar10 = VectorShiftLeft((ulonglong)param_2[3],0x18,0x40,0);
    uVar13 = VectorShiftLeft((ulonglong)param_2[4],0x20,0x40,0);
    uVar11 = VectorShiftLeft((ulonglong)param_2[5],0x28,0x40,0);
    pbVar1 = param_2 + 7;
    uVar14 = VectorShiftLeft((ulonglong)param_2[6],0x30,0x40,0);
    param_2 = param_2 + 8;
    uVar12 = VectorShiftLeft((ulonglong)*pbVar1,0x38,0x40,0);
    *param_1 = (CONCAT44((int)((uVar9 | uVar8) >> 0x20),(uint)bVar2 | (uint)(uVar9 | uVar8)) |
                uVar10 | uVar13 | uVar11 | uVar14 | uVar12) ^ *param_1;
    param_1 = param_1 + 1;
  } while (param_2 != pbVar7);
  return 0;
}

