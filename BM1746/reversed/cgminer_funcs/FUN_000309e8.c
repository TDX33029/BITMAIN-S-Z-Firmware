//==================== FUN_000309e8 @ 000309e8 ====================

bool FUN_000309e8(sockaddr *param_1,undefined4 *param_2,byte *param_3)

{
  byte *pbVar1;
  char *__host;
  __int32_t **pp_Var2;
  byte *pbVar3;
  byte *pbVar4;
  byte *pbVar5;
  int iVar6;
  bool bVar7;
  byte bStack_51;
  int local_50;
  int local_4c;
  int local_48;
  undefined4 local_44;
  char acStack_40 [36];
  
  __host = malloc(0x2e);
  *param_2 = __host;
  getnameinfo(param_1,0x80,__host,0x2e,(char *)0x0,0,1);
  if (param_1->sa_family == 2) {
    sprintf(acStack_40,"::ffff:%s",(char *)*param_2);
    inet_pton(10,acStack_40,&local_50);
  }
  else {
    inet_pton(10,(char *)*param_2,&local_50);
  }
  pp_Var2 = __ctype_toupper_loc();
  *param_3 = (byte)(*pp_Var2)[0x52];
  if (DAT_00078c98 == 0) {
    if ((DAT_0007b7c4 == '\0') && (iVar6 = strcmp((char *)*param_2,"127.0.0.1"), iVar6 != 0)) {
      if (local_50 == 0) {
        bVar7 = false;
        if (local_4c == 0) {
          if (local_48 == 0) {
            bVar7 = local_44 == 0x1000000;
          }
          else {
            bVar7 = false;
          }
        }
      }
      else {
        bVar7 = false;
      }
    }
    else {
      bVar7 = true;
    }
  }
  else {
    if (0 < DAT_00078cb0) {
      iVar6 = 0;
      pbVar4 = (byte *)(DAT_00078cb4 + -1);
      do {
        pbVar3 = &bStack_51;
        pbVar5 = pbVar4;
        while( true ) {
          pbVar1 = pbVar5 + 0x11;
          pbVar3 = pbVar3 + 1;
          pbVar5 = pbVar5 + 1;
          if (*pbVar5 != (*pbVar1 & *pbVar3)) break;
          if (pbVar3 == (byte *)((int)&local_44 + 3)) {
            *param_3 = pbVar4[0x21];
            return true;
          }
        }
        iVar6 = iVar6 + 1;
        pbVar4 = pbVar4 + 0x24;
      } while (iVar6 != DAT_00078cb0);
    }
    bVar7 = false;
  }
  return bVar7;
}

