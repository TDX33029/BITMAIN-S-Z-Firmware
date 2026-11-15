//==================== FUN_000118d4 @ 000118d4 ====================

undefined4 FUN_000118d4(void)

{
  tm *ptVar1;
  undefined4 uVar2;
  int iVar3;
  time_t local_1c;
  time_t local_18 [2];
  
  if ((DAT_0007b874 == '\0') && (DAT_0007b5f0 == '\0')) {
LAB_00011980:
    uVar2 = 1;
  }
  else {
    FUN_0002a84c(local_18);
    local_1c = local_18[0];
    ptVar1 = localtime(&local_1c);
    if (DAT_0007b874 == '\0') {
      if ((ptVar1->tm_hour < DAT_0007b5fc) ||
         ((ptVar1->tm_hour == DAT_0007b5fc && (ptVar1->tm_min < DAT_0007b5f8)))) goto LAB_00011980;
    }
    else if (DAT_0007b5f0 == '\0') {
      if ((DAT_0007b880 <= ptVar1->tm_hour) &&
         ((ptVar1->tm_hour != DAT_0007b880 || (DAT_0007b87c <= ptVar1->tm_min)))) {
        DAT_0007b874 = 0;
        return 1;
      }
    }
    else {
      if ((DAT_0007b5fc <= DAT_0007b880) &&
         ((DAT_0007b880 != DAT_0007b5fc || (DAT_0007b5f8 <= DAT_0007b87c)))) {
        iVar3 = ptVar1->tm_hour;
        if (iVar3 < DAT_0007b880) {
          if (DAT_0007b5fc <= iVar3) {
LAB_00011a24:
            if ((DAT_0007b5fc != iVar3) || (DAT_0007b5f8 <= ptVar1->tm_min)) goto LAB_000119a4;
          }
        }
        else if ((DAT_0007b880 == iVar3) && (ptVar1->tm_min < DAT_0007b87c)) goto LAB_00011a24;
        goto LAB_00011980;
      }
      iVar3 = ptVar1->tm_hour;
      if ((((iVar3 < DAT_0007b5fc) || ((DAT_0007b5fc == iVar3 && (ptVar1->tm_min < DAT_0007b5f8))))
          && (DAT_0007b880 <= iVar3)) &&
         ((DAT_0007b880 != iVar3 || (DAT_0007b87c <= ptVar1->tm_min)))) goto LAB_00011980;
    }
LAB_000119a4:
    uVar2 = 0;
  }
  return uVar2;
}

