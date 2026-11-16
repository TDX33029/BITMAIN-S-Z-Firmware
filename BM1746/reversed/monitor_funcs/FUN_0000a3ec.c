//==================== FUN_0000a3ec @ 0000a3ec ====================

void FUN_0000a3ec(void)

{
  int iVar1;
  char *__s;
  _STACK *p_Var2;
  
  puts("oops! stop by ctrl+c or kill!!!----------------------------------------");
  iVar1 = fileno(DAT_0001c3c8);
  iVar1 = flock(iVar1,8);
  if (iVar1 == 0) {
    __s = "unlock fd_pid_conf succeed!";
  }
  else {
    __s = "unlock fd_pid_conf failed!";
  }
  puts(__s);
  fclose(DAT_0001c3c8);
  FUN_00010f48(DAT_0001d420);
  DAT_0001b2ec = 0;
  pthread_join(DAT_0001d404,(void **)0x0);
  pthread_join(DAT_0001f464,(void **)0x0);
  pthread_join(DAT_0001c3cc,(void **)0x0);
  FUN_0000e49c(&DAT_0001b3a8);
  FUN_0000e49c(&DAT_0001d410);
  SSL_shutdown(DAT_00026f98);
  SSL_free(DAT_00026f98);
  close(DAT_000258bc);
  SSL_CTX_free(DAT_00026f8c);
  CONF_modules_free();
  ERR_remove_state(0);
  ENGINE_cleanup();
  CONF_modules_unload(1);
  ERR_free_strings();
  EVP_cleanup();
  CRYPTO_cleanup_all_ex_data();
  p_Var2 = &SSL_COMP_get_compression_methods()->stack;
  sk_free(p_Var2);
                    /* WARNING: Subroutine does not return */
  exit(0);
}

