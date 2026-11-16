//==================== FUN_04009398 @ 04009398 ====================

int FUN_04009398(undefined4 param_1,int *param_2)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int *piVar3;
  int iVar4;
  int *piVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int *local_24;
  
  local_24 = param_2;
  FUN_040096d4();
  func_0x03fd1098(0x13,&local_24);
  if (local_24 == (int *)0x0) {
    FUN_04007548(DAT_04009534);
    func_0x03fbf43c(0xffffffc0);
    iVar7 = 0;
  }
  else {
    iVar4 = func_0x03fbefb0(DAT_04009538);
    if ((iVar4 == 0) || (piVar5 = (int *)FUN_04009008(), piVar5 == (int *)0x0)) {
      piVar5 = (int *)0x0;
      FUN_04008fa0(0);
    }
    else {
      FUN_04008fa0();
      FUN_04009754();
    }
    uVar2 = DAT_04009540;
    uVar1 = DAT_0400953c;
    iVar7 = 0;
    func_0x03fbf43c(0x41);
    while( true ) {
      FUN_04007548(uVar1,local_24[0x14],local_24[1]);
      if ((iVar4 != 0) && (local_24 == piVar5)) {
        FUN_04007548(uVar2);
      }
      piVar3 = local_24;
      if (((local_24 != (int *)0x0) && ((local_24[0x12] & 1U) != 0)) &&
         (*(int *)(*(int *)(*local_24 + 0x3c) + 0x14) != 0)) {
        iVar8 = local_24[2];
        iVar6 = FUN_040096ec(local_24[0x14]);
        if (iVar6 == 0) {
          if (((*(byte *)(iVar8 + 4) & 1) == 0) &&
             (((((*(byte *)(iVar8 + 4) != 0 || *(char *)(iVar8 + 5) != '\0') ||
                *(char *)(iVar8 + 6) != '\0') || *(char *)(iVar8 + 7) != '\0') ||
              *(char *)(iVar8 + 8) != '\0') || *(char *)(iVar8 + 9) != '\0')) {
            iVar6 = (**(code **)(*(int *)(*piVar3 + 0x3c) + 0x14))(piVar3);
            if (iVar6 != 0 && iVar6 != -0x26) {
              FUN_04007548(DAT_04009544,piVar3[1]);
            }
          }
          else {
            FUN_04007548(DAT_0400954c,piVar3[1],iVar8 + 4);
          }
        }
      }
      iVar7 = iVar7 + 1;
      func_0x03fd10d0(&local_24);
      if (local_24 == (int *)0x0) break;
      FUN_04007548(DAT_04009548);
    }
    func_0x03fc7f78(10);
  }
  return iVar7;
}

