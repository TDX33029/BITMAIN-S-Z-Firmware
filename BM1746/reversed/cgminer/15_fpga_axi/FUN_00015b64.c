//==================== FUN_00015b64 @ 00015b64 ====================

void FUN_00015b64(FILE *param_1)

{
  int *piVar1;
  undefined4 uVar2;
  int iVar3;
  char *param6;
  char *pcVar4;
  char *pcVar5;
  char *pcVar6;
  uint uVar7;
  undefined1 *puVar8;
  code *pcVar9;
  int iVar10;
  undefined4 *puVar11;
  undefined4 *puVar12;
  int param6_00;
  char *pcVar13;
  char *local_2c;
  
  fwrite(&DAT_00060700,1,0xd,param_1);
  if (0 < DAT_0007b198) {
    iVar10 = 0;
    do {
      iVar3 = FUN_00012038(iVar10);
      pcVar5 = "";
      param6_00 = *(int *)(iVar3 + 0x38);
      if (param6_00 == 1) {
        pcVar4 = ",";
        if (iVar10 == 0) {
          pcVar4 = pcVar5;
        }
        if (*(int *)(iVar3 + 0xd8) == 0) {
LAB_00015f94:
          pcVar6 = "";
LAB_00015f98:
          pcVar13 = "";
        }
        else {
          FUN_00029690(*(undefined4 *)(iVar3 + 0xd4));
          pcVar5 = (char *)FUN_0000f8a4();
          if (*(int *)(iVar3 + 0xd8) == 0) goto LAB_00015f94;
          pcVar6 = (char *)FUN_0000f8a4();
          if (*(int *)(iVar3 + 0xd8) == 0) goto LAB_00015f98;
          pcVar13 = "|";
        }
        param6 = (char *)FUN_0000f8a4(*(undefined4 *)(iVar3 + 0xc4));
        fprintf(param_1,"%s\n\t{\n\t\t\"url\" : \"%s%s%s%s\",",pcVar4,pcVar5,pcVar6,pcVar13,param6);
      }
      else {
        pcVar4 = ",";
        if (iVar10 == 0) {
          pcVar4 = pcVar5;
        }
        if (*(int *)(iVar3 + 0xd8) == 0) {
LAB_00015cf4:
          pcVar6 = "";
LAB_00015cf8:
          local_2c = "";
        }
        else {
          FUN_00029690(*(undefined4 *)(iVar3 + 0xd4));
          pcVar5 = (char *)FUN_0000f8a4();
          if (*(int *)(iVar3 + 0xd8) == 0) {
            param6_00 = *(int *)(iVar3 + 0x38);
            goto LAB_00015cf4;
          }
          pcVar6 = (char *)FUN_0000f8a4();
          param6_00 = *(int *)(iVar3 + 0x38);
          if (*(int *)(iVar3 + 0xd8) == 0) goto LAB_00015cf8;
          local_2c = "|";
        }
        pcVar13 = (char *)FUN_0000f8a4(*(undefined4 *)(iVar3 + 0xc4));
        fprintf(param_1,"%s\n\t{\n\t\t\"quota\" : \"%s%s%s%d;%s\",",pcVar4,pcVar5,pcVar6,local_2c,
                param6_00,pcVar13);
      }
      iVar10 = iVar10 + 1;
      uVar2 = FUN_0000f8a4(*(undefined4 *)(iVar3 + 0xcc));
      fprintf(param_1,"\n\t\t\"user\" : \"%s\",",uVar2);
      pcVar5 = (char *)FUN_0000f8a4(*(undefined4 *)(iVar3 + 0xd0));
      fprintf(param_1,"\n\t\t\"pass\" : \"%s\"\n\t}",pcVar5);
    } while (iVar10 < DAT_0007b198);
  }
  fwrite(&DAT_00060710,1,3,param_1);
  if (DAT_00075604 != 0x10) {
    puVar11 = &DAT_0007561c;
    do {
      pcVar4 = (char *)__strdup(puVar11[-7]);
      pcVar5 = strtok(pcVar4,"|");
      while (pcVar5 != (char *)0x0) {
        if ((pcVar5[1] == '-') && ((undefined4 *)puVar11[-1] != &DAT_0009d784)) {
          uVar7 = puVar11[-6];
          if (((uVar7 & 1) == 0) ||
             (((puVar8 = (undefined1 *)puVar11[-5], puVar8 != &LAB_0005b0fc &&
               (puVar8 != &UNK_0005b10c)) ||
              ((bool)*(char *)puVar11[-2] != (puVar8 == &LAB_0005b0fc))))) {
            if ((uVar7 & 2) == 0) {
LAB_00015e5c:
              if (((uVar7 & 6) != 0) &&
                 (((int *)puVar11[-2] != (int *)&DAT_00076f14 && (*(int *)puVar11[-2] != 0)))) {
                pcVar6 = (char *)FUN_0000f8a4();
                fprintf(param_1,",\n\"%s\" : \"%s\"",pcVar5 + 2,pcVar6);
              }
            }
            else {
              pcVar9 = (code *)puVar11[-4];
              if (((((pcVar9 == FUN_0005b2e8) || (pcVar9 == (code *)&LAB_000154c8)) ||
                   ((pcVar9 == (code *)&LAB_000154e0 ||
                    ((pcVar9 == (code *)&UNK_00015540 || (pcVar9 == (code *)&UNK_00015534)))))) ||
                  (pcVar9 == (code *)&LAB_000154d4)) ||
                 (((((pcVar9 == (code *)&UNK_00015528 || (pcVar9 == (code *)&UNK_0001551c)) ||
                    (pcVar9 == (code *)&UNK_00015510)) ||
                   ((pcVar9 == (code *)&UNK_00015504 || (pcVar9 == (code *)&UNK_000154f8)))) ||
                  (pcVar9 == (code *)&UNK_000154ec)))) {
                fprintf(param_1,",\n\"%s\" : \"%d\"",pcVar5 + 2,*(int *)puVar11[-2]);
              }
              else {
                if ((pcVar9 != FUN_00012200) && (pcVar9 != FUN_000121b0)) goto LAB_00015e5c;
                fprintf(param_1,",\n\"%s\" : \"%.1f\"",pcVar5 + 2,(double)*(float *)puVar11[-2]);
              }
            }
          }
          else {
            fprintf(param_1,",\n\"%s\" : true",pcVar5 + 2);
          }
        }
        pcVar5 = strtok((char *)0x0,"|");
      }
      free(pcVar4);
      piVar1 = puVar11 + 1;
      puVar11 = puVar11 + 7;
    } while (*piVar1 != 0x10);
  }
  if (DAT_00076efc == 4) {
    fwrite(",\n\"balance\" : true",1,0x12,param_1);
  }
  if (DAT_00076efc == 3) {
    fwrite(",\n\"load-balance\" : true",1,0x17,param_1);
  }
  if (DAT_00076efc == 1) {
    fwrite(",\n\"round-robin\" : true",1,0x16,param_1);
  }
  if (DAT_00076efc == 2) {
    fprintf(param_1,",\n\"rotate\" : \"%d\"",DAT_0007b558);
  }
  fwrite(&DAT_00060814,1,3,param_1);
  puVar11 = DAT_00076bd8;
  DAT_00076bd8 = (undefined4 *)0x0;
  while (puVar11 != (undefined4 *)0x0) {
    puVar12 = (undefined4 *)puVar11[1];
    free((void *)*puVar11);
    free(puVar11);
    puVar11 = puVar12;
  }
  return;
}

