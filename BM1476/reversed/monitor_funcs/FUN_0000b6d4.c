//==================== FUN_0000b6d4 @ 0000b6d4 ====================

/* WARNING: Restarted to delay deadcode elimination for space: ram */

int FUN_0000b6d4(char *param_1)

{
  char *pcVar1;
  FILE *__stream;
  char *pcVar2;
  size_t sVar3;
  ushort **ppuVar4;
  int iVar5;
  int *piVar6;
  byte *__src;
  int param2;
  int iVar7;
  byte *local_338;
  char *local_334;
  char *local_330;
  byte abStack_32c [776];
  
  __stream = fopen(param_1,"r");
  if (__stream == (FILE *)0x0) {
    piVar6 = __errno_location();
    pcVar2 = strerror(*piVar6);
    printf("can\'t open \'%s\' as config file: %s\n",param_1,pcVar2);
  }
  else {
    param2 = 0;
LAB_0000b6fc:
    pcVar2 = fgets((char *)abStack_32c,0x302,__stream);
    if (pcVar2 != (char *)0x0) {
      param2 = param2 + 1;
      sVar3 = strlen((char *)abStack_32c);
      if (sVar3 < 0x301) {
        if (sVar3 != 0) {
          ppuVar4 = __ctype_b_loc();
          __src = abStack_32c;
          do {
            if (((*ppuVar4)[*__src] & 0x2000) == 0) {
              if (*__src != 0x23) {
                memcpy(abStack_32c,__src,sVar3);
                abStack_32c[sVar3] = 0;
                local_338 = abStack_32c;
                iVar5 = FUN_0000b250(&local_338,&local_334,&local_330);
                pcVar2 = local_334;
                iVar7 = DAT_0001b3a0;
                if (iVar5 == 0) {
                  if (DAT_0001b3a0 < 0x28) {
                    sVar3 = strlen(local_334);
                    pcVar1 = local_330;
                    if (sVar3 < 0x81) {
                      sVar3 = strlen(local_330);
                      if (sVar3 < 0x201) {
                        strncpy(&DAT_00024490 + iVar7 * 0x81,pcVar2,0x81);
                        FUN_0000b208(pcVar1,0x2f);
                        iVar7 = DAT_0001b3a0 + 1;
                        strncpy(&DAT_0001f468 + DAT_0001b3a0 * 0x201,local_330,0x201);
                        DAT_0001b3a0 = iVar7;
                      }
                      else {
                        printf("value to long %s, line %d:%d\n",param_1,param2,
                               (int)local_338 - (int)abStack_32c);
                      }
                    }
                    else {
                      printf("var name to long %s, line %d:%d\n",param_1,param2,
                             (int)local_338 - (int)abStack_32c);
                    }
                  }
                  else {
                    printf("too many vars in  %s, line %d:%d\n",param_1,param2,
                           (int)local_338 - (int)abStack_32c);
                  }
                }
                else {
                  printf("error parsing %s, line %d:%d\n",param_1,param2,
                         (int)local_338 - (int)abStack_32c);
                }
              }
              break;
            }
            sVar3 = sVar3 - 1;
            __src = __src + 1;
          } while (sVar3 != 0);
        }
      }
      else {
        printf("line too long, conf line skipped %s, line %d\n",param_1,param2);
      }
      goto LAB_0000b6fc;
    }
  }
  fclose(__stream);
  return DAT_0001b3a0;
}

