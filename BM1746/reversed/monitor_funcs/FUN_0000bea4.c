//==================== FUN_0000bea4 @ 0000bea4 ====================

char * FUN_0000bea4(char *param_1,char *param_2,int param_3)

{
  ushort uVar1;
  char *param2;
  hostent *phVar2;
  int __fd;
  int iVar3;
  size_t sVar4;
  ssize_t sVar5;
  int *piVar6;
  char *pcVar7;
  bool bVar8;
  sockaddr local_30;
  
  param2 = malloc(0xffdd);
  if (param2 == (char *)0x0) {
                    /* WARNING: Subroutine does not return */
    __assert_fail("buf","cgminer_api.c",0x8b,"callapi");
  }
  phVar2 = gethostbyname(param_2);
  if (phVar2 == (hostent *)0x0) {
    printf("Failed to resolve host %s\n",param_2);
    param2 = (char *)0x0;
  }
  else {
    __fd = socket(2,1,0);
    if (__fd == -1) {
      piVar6 = __errno_location();
      param2 = (char *)0x0;
      pcVar7 = strerror(*piVar6);
      printf("Socket initialisation failed: %s\n",pcVar7);
    }
    else {
      local_30.sa_data[6] = '\0';
      local_30.sa_data[7] = '\0';
      local_30.sa_data[8] = '\0';
      local_30.sa_data[9] = '\0';
      bVar8 = false;
      local_30.sa_data[10] = '\0';
      local_30.sa_data[0xb] = '\0';
      local_30.sa_data[0xc] = '\0';
      local_30.sa_data[0xd] = '\0';
      uVar1 = (ushort)((uint)param_3 >> 8) & 0xff | (ushort)(param_3 << 8);
      local_30.sa_data._2_4_ = *(undefined4 *)*phVar2->h_addr_list;
      local_30.sa_data[0] = (char)uVar1;
      local_30.sa_data[1] = (char)(uVar1 >> 8);
      local_30.sa_family = 2;
      while (iVar3 = connect(__fd,&local_30,0x10), iVar3 < 0) {
        if (!bVar8) {
          puts("Call API error.Disconnect with server!");
          raise(10);
        }
        bVar8 = true;
        puts("Call API Failed wait 5s !!");
        sleep(5);
      }
      if (bVar8) {
        param2 = "";
        close(__fd);
        raise(0xc);
      }
      else {
        sVar4 = strlen(param_1);
        sVar5 = send(__fd,param_1,sVar4,0);
        if (sVar5 < 0) {
          piVar6 = __errno_location();
          pcVar7 = strerror(*piVar6);
          printf("Send failed: %s\n",pcVar7);
        }
        else {
          sVar4 = 0xffdc;
          puts("Send succed");
          *param2 = '\0';
          iVar3 = 0;
          while( true ) {
            puts("begin to Recv");
            sVar5 = recv(__fd,param2 + iVar3,0xffdc,0);
            printf("Recved n: %d\n",sVar5);
            if (sVar5 < 0) break;
            if (sVar5 == 0) goto LAB_0000c054;
            iVar3 = iVar3 + sVar5;
            param2[iVar3] = '\0';
            if (sVar4 < iVar3 + 0xffdcU) {
              sVar4 = sVar4 << 1;
              param2 = realloc(param2,sVar4);
              if (param2 == (char *)0x0) {
                    /* WARNING: Subroutine does not return */
                __assert_fail("buf","cgminer_api.c",0xbb,"callapi");
              }
            }
          }
          piVar6 = __errno_location();
          pcVar7 = strerror(*piVar6);
          printf("Recv failed: %s\n",pcVar7);
        }
LAB_0000c054:
        close(__fd);
        printf("the result of callapi about %s: %s\n\n",param_1,param2);
      }
    }
  }
  return param2;
}

