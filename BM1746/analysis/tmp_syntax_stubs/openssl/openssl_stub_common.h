/* MinGW 语法冒烟专用桩: monitor (stratum TLS) 所用 OpenSSL API 子集, 仅满足 -fsyntax-only */
#ifndef STUB_OPENSSL_COMMON_H
#define STUB_OPENSSL_COMMON_H
typedef struct ssl_method_st   SSL_METHOD;
typedef struct ssl_ctx_st      SSL_CTX;
typedef struct ssl_st          SSL;
typedef struct ssl_cipher_st   SSL_CIPHER;
typedef struct x509_st         X509;
typedef struct x509_name_st    X509_NAME;
typedef struct stack_st        _STACK;
/* crypto.h */
extern void OPENSSL_add_all_algorithms_noconf(void);
extern void EVP_cleanup(void);
extern void ENGINE_cleanup(void);
extern void sk_free(void *);
/* err.h */
extern void ERR_free_strings(void);
extern void ERR_remove_state(unsigned long);
extern void ERR_print_errors_fp(void *);
/* ssl.h */
extern int SSL_library_init(void);
extern void SSL_load_error_strings(void);
extern SSL_CTX *SSL_CTX_new(SSL_METHOD *);
extern void SSL_CTX_free(SSL_CTX *);
extern SSL *SSL_new(SSL_CTX *);
extern void SSL_free(SSL *);
extern int SSL_connect(SSL *);
extern int SSL_read(SSL *, void *, int);
extern int SSL_write(SSL *, const void *, int);
extern int SSL_shutdown(SSL *);
extern int SSL_set_fd(SSL *, int);
extern const SSL_CIPHER *SSL_get_current_cipher(const SSL *);
extern const char *SSL_CIPHER_get_name(const SSL_CIPHER *);
extern X509 *SSL_get_peer_certificate(const SSL *);
extern SSL_METHOD *TLSv1_client_method(void);
extern SSL_METHOD *SSLv23_client_method(void);
/* comp.h */
extern _STACK *SSL_COMP_get_compression_methods(void);
/* x509.h */
extern char *X509_NAME_oneline(X509_NAME *, char *, int);
extern void X509_free(X509 *);
extern X509_NAME *X509_get_issuer_name(X509 *);
extern X509_NAME *X509_get_subject_name(X509 *);
/* conf.h */
extern void CONF_modules_free(void);
extern void CONF_modules_unload(int);
#endif
