//==================== FUN_00010710 @ 00010710 ====================

int FUN_00010710(int param_1)

{
  char *pcVar1;
  
  pcVar1 = (char *)FUN_0000ba78("pool1url");
  strcpy((char *)(param_1 + 4),pcVar1);
  pcVar1 = (char *)FUN_0000ba78("pool1user");
  strcpy((char *)(param_1 + 0x54),pcVar1);
  pcVar1 = (char *)FUN_0000ba78("pool1pw");
  strcpy((char *)(param_1 + 0x7c),pcVar1);
  pcVar1 = (char *)FUN_0000ba78("pool2url");
  strcpy((char *)(param_1 + 0xa8),pcVar1);
  pcVar1 = (char *)FUN_0000ba78("pool2user");
  strcpy((char *)(param_1 + 0xf8),pcVar1);
  pcVar1 = (char *)FUN_0000ba78("pool2pw");
  strcpy((char *)(param_1 + 0x120),pcVar1);
  pcVar1 = (char *)FUN_0000ba78("pool3url");
  strcpy((char *)(param_1 + 0x14c),pcVar1);
  pcVar1 = (char *)FUN_0000ba78("pool3user");
  strcpy((char *)(param_1 + 0x19c),pcVar1);
  pcVar1 = (char *)FUN_0000ba78("pool3pw");
  strcpy((char *)(param_1 + 0x1c4),pcVar1);
  pcVar1 = (char *)FUN_0000ba78("api_allow");
  strcpy((char *)(param_1 + 0x1ec),pcVar1);
  pcVar1 = (char *)FUN_0000ba78("more_options");
  strcpy((char *)(param_1 + 0x200),pcVar1);
  pcVar1 = (char *)FUN_0000ba78("freq");
  strcpy((char *)(param_1 + 0x20a),pcVar1);
  pcVar1 = (char *)FUN_0000ba78("voltage");
  strcpy((char *)(param_1 + 0x219),pcVar1);
  pcVar1 = (char *)FUN_0000ba78("pool_balance");
  strcpy((char *)(param_1 + 0x223),pcVar1);
  pcVar1 = (char *)FUN_0000ba78("bitmain_nobeeper");
  strcpy((char *)(param_1 + 0x22d),pcVar1);
  pcVar1 = (char *)FUN_0000ba78("bitmain_notempoverctrl");
  strcpy((char *)(param_1 + 0x237),pcVar1);
  return param_1;
}

