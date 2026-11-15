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

