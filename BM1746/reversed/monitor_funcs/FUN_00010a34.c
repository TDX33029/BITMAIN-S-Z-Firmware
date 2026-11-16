//==================== FUN_00010a34 @ 00010a34 ====================

void FUN_00010a34(int param_1)

{
  FILE *__s;
  
  __s = fopen(DAT_0001c3d0,"rb+");
  if (__s == (FILE *)0x0) {
    puts("open error");
  }
  fwrite("config cgminer \'default\'\n",1,0x19,__s);
  fprintf(__s,"  option pool1url  \'%s\'\n",(char *)(param_1 + 4));
  fprintf(__s,"\toption pool1user \'%s\'\n",(char *)(param_1 + 0x54));
  fprintf(__s,"\toption pool1pw   \'%s\'\n",(char *)(param_1 + 0x7c));
  fprintf(__s,"\toption pool2url  \'%s\'\n",(char *)(param_1 + 0xa8));
  fprintf(__s,"\toption pool2user \'%s\'\n",(char *)(param_1 + 0xf8));
  fprintf(__s,"\toption pool2pw   \'%s\'\n",(char *)(param_1 + 0x120));
  fprintf(__s,"\toption pool3url  \'%s\'\n",(char *)(param_1 + 0x14c));
  fprintf(__s,"\toption pool3user \'%s\'\n",(char *)(param_1 + 0x19c));
  fprintf(__s,"\toption pool3pw   \'%s\'\n",(char *)(param_1 + 0x1c4));
  fprintf(__s,"\toption api_allow  \'%s\'\n",(char *)(param_1 + 0x1ec));
  fprintf(__s,"\toption more_options \'%s\'\n",(char *)(param_1 + 0x200));
  fprintf(__s,"\toption freq   \'%s\'\n",(char *)(param_1 + 0x20a));
  fprintf(__s,"\toption voltage   \'%s\'\n",(char *)(param_1 + 0x219));
  fprintf(__s,"\toption pool_balance \'%s\'\n",(char *)(param_1 + 0x223));
  fprintf(__s,"\toption bitmain_nobeeper   \'%s\'\n",(char *)(param_1 + 0x22d));
  fprintf(__s,"\toption bitmain_notempoverctrl \'%s\'\n",(char *)(param_1 + 0x237));
  fwrite("\t           ",1,0xc,__s);
  fclose(__s);
  return;
}

