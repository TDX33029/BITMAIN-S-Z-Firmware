//==================== FUN_0000a8a0 @ 0000a8a0 ====================

void FUN_0000a8a0(SSL *param_1)

{
  X509 *a;
  X509_NAME *pXVar1;
  char *pcVar2;
  
  a = SSL_get_peer_certificate(param_1);
  if (a == (X509 *)0x0) {
    puts(&DAT_0001129c);
    return;
  }
  puts("A digital certificate information:");
  pXVar1 = X509_get_subject_name(a);
  pcVar2 = X509_NAME_oneline(pXVar1,(char *)0x0,0);
  printf("certificate: %s\n",pcVar2);
  free(pcVar2);
  pXVar1 = X509_get_issuer_name(a);
  pcVar2 = X509_NAME_oneline(pXVar1,(char *)0x0,0);
  printf("Issued by the: %s\n",pcVar2);
  free(pcVar2);
  X509_free(a);
  return;
}

