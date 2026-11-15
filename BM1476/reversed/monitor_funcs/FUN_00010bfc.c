//==================== FUN_00010bfc @ 00010bfc ====================

undefined4 FUN_00010bfc(char *param_1)

{
  FILE *__s;
  
  __s = fopen("/etc/config/auth_tmp.txt","w+");
  if (__s == (FILE *)0x0) {
    return 0xffffffff;
  }
  if (((*param_1 == 'f') && (param_1[1] == '5')) && (param_1[2] == '\0')) {
    fputs(param_1,__s);
  }
  else {
    fwrite(&DAT_0001217c,1,2,__s);
  }
  fclose(__s);
  return 1;
}

