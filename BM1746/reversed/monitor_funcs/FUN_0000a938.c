//==================== FUN_0000a938 @ 0000a938 ====================

undefined4 FUN_0000a938(void)

{
  SSL_METHOD *meth;
  int iVar1;
  SSL_CIPHER *pSVar2;
  char *pcVar3;
  
  SSL_library_init();
  OPENSSL_add_all_algorithms_noconf();
  SSL_load_error_strings();
  meth = TLSv1_client_method();
  DAT_00026f8c = SSL_CTX_new(meth);
  if (DAT_00026f8c == (SSL_CTX *)0x0) {
    ERR_print_errors_fp(stdout);
                    /* WARNING: Subroutine does not return */
    exit(1);
  }
  DAT_00026f98 = SSL_new(DAT_00026f8c);
  SSL_set_fd(DAT_00026f98,DAT_000258bc);
  iVar1 = SSL_connect(DAT_00026f98);
  if (iVar1 != -1) {
    pSVar2 = SSL_get_current_cipher(DAT_00026f98);
    pcVar3 = SSL_CIPHER_get_name(pSVar2);
    iVar1 = strcmp("(NONE)",pcVar3);
    if (iVar1 != 0) {
      pSVar2 = SSL_get_current_cipher(DAT_00026f98);
      pcVar3 = SSL_CIPHER_get_name(pSVar2);
      printf("Connected with %s encryption\n",pcVar3);
      FUN_0000a8a0(DAT_00026f98);
      return 1;
    }
  }
  ERR_print_errors_fp(stderr);
  return 0xffffffff;
}

