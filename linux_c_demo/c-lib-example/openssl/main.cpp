#include <iostream>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/rsa.h>
#include <openssl/bio.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void generate_key_and_cert(const char *key_file, const char *cert_file)
{
    RSA *rsa = RSA_generate_key(2048, RSA_F4, NULL, NULL);

    FILE *key_file_ptr = fopen(key_file, "w");
    if (!key_file_ptr)
    {
        perror("err open private key file");
        exit(EXIT_FAILURE);
    }
    PEM_write_RSAPrivateKey(key_file_ptr, rsa, NULL, NULL, 0, NULL, NULL);

    X509 *x509 = X509_new();
    X509_set_version(x509, 2);
    ASN1_INTEGER_set(X509_get_serialNumber(x509), 1);

    X509_NAME *name = X509_get_subject_name(x509);
    X509_NAME_add_entry_by_txt(name, "C", MBSTRING_ASC, (const unsigned char *)"US", -1, -1, 0);
    X509_NAME_add_entry_by_txt(name, "O", MBSTRING_ASC, (const unsigned char *)"OpenSSL Group", -1, -1, 0);
    X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, (const unsigned char *)"localhost", -1, -1, 0);

    X509_set_issuer_name(x509, name);
    X509_gmtime_adj(X509_get_notBefore(x509), 0);
    X509_gmtime_adj(X509_get_notAfter(x509), 60 * 60 * 24 * 365);

    EVP_PKEY *pkey = EVP_PKEY_new();
    EVP_PKEY_assign_RSA(pkey, rsa);
    X509_set_pubkey(x509, pkey);

    X509_EXTENSION *ext = X509V3_EXT_conf_nid(
        NULL,
        NULL,
        NID_basic_constraints,
        "critical,CA:FALSE");
    X509_add_ext(x509, ext, -1);
    X509_EXTENSION_free(ext);

    X509_sign(x509, pkey, EVP_sha256());

    FILE *cert_file_ptr = fopen(cert_file, "w");
    if (!cert_file_ptr)
    {
        perror("err open cert file");
        exit(EXIT_FAILURE);
    }
    PEM_write_X509(cert_file_ptr, x509);
    fclose(key_file_ptr);
    fclose(cert_file_ptr);
    X509_free(x509);
    RSA_free(rsa);
}

void handle_client(SSL *ssl)
{
    char buffer[1024];
    const char *response = "hello ssl world\n";

    if (SSL_accept(ssl) <= 0)
    {
        ERR_print_errors_fp(stderr);
        return;
    }
    int bytes_received = SSL_read(ssl, buffer, sizeof(buffer) - 1);
    if (bytes_received > 0)
    {
        buffer[bytes_received] = '\0';
        printf("received %s ", buffer);

        SSL_write(ssl, response, strlen(response));
    }
    else
    {
        ERR_print_errors_fp(stderr);
    }

    SSL_shutdown(ssl);
    SSL_free(ssl);
}

int main()
{

    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    generate_key_and_cert("server.key", "server.crt");
    printf("pri key and cert generate succ\n");

    SSL_CTX *ctx = SSL_CTX_new(SSLv23_method());
    if (!ctx)
    {
        ERR_print_errors_fp(stderr);
        return -1;
    }

    if (SSL_CTX_use_certificate_file(ctx, "server.crt", SSL_FILETYPE_PEM) <= 0 ||
        SSL_CTX_use_PrivateKey_file(ctx, "server.key", SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        std::cerr << "err load cert or private key" << std::endl;
        return -1;
    }

    SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3 | SSL_OP_NO_TLSv1);
    SSL_CTX_set_cipher_list(ctx, "ALL");

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        perror("err create sock");
        return -1;
    }
    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Error binding");
        return -1;
    }
    if (listen(server_socket, 10) == -1)
    {
        perror("Error listening");
        return -1;
    }
    printf("Server is listening on port 8080...\n");

    while (1)
    {
        int client_socket = accept(server_socket, NULL, NULL);
        if (client_socket == -1)
        {
            perror("err accept conn");
            continue;
        }
        SSL *ssl = SSL_new(ctx);
        SSL_set_fd(ssl, client_socket);
        handle_client(ssl);
    }

    SSL_CTX_free(ctx);
}