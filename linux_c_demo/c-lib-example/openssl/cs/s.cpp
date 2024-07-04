#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define SERVER_PORT 12345
#define BUFFER_SIZE 1024

void handle_error()
{
    perror("Error");
    exit(EXIT_FAILURE);
}

SSL_CTX *init_server_context()
{
    SSL_CTX *ctx;

    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    ctx = SSL_CTX_new(TLS_server_method());
    if (ctx == NULL)
    {
        ERR_print_errors_fp(stderr);
        handle_error();
    }

    if (SSL_CTX_use_certificate_file(ctx, "server.crt", SSL_FILETYPE_PEM) <= 0 ||
        SSL_CTX_use_PrivateKey_file(ctx, "server.key", SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        handle_error();
    }

    return ctx;
}

void start_server(SSL_CTX *ctx)
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    SSL *ssl;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        handle_error();
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        handle_error();
    }

    if (listen(server_fd, 10) == -1)
    {
        handle_error();
    }

    printf("Server listening on port %d...\n", SERVER_PORT);

    while (1)
    {
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
        if (client_fd == -1)
        {
            handle_error();
        }

        printf("Client connected: %s\n", inet_ntoa(client_addr.sin_addr));

        ssl = SSL_new(ctx);
        SSL_set_fd(ssl, client_fd);

        if (SSL_accept(ssl) <= 0)
        {
            ERR_print_errors_fp(stderr);
        }
        else
        {
            char buffer[BUFFER_SIZE];
            memset(buffer, 0, sizeof(buffer));

            int len = SSL_read(ssl, buffer, sizeof(buffer) - 1);
            if (len > 0)
            {
                printf("Received message: %s\n", buffer);

                const char *response = "Hello from server!";
                SSL_write(ssl, response, strlen(response));
            }
        }

        SSL_free(ssl);
        close(client_fd);
    }

    close(server_fd);
}

int main()
{
    SSL_CTX *ctx = init_server_context();
    start_server(ctx);

    SSL_CTX_free(ctx);

    return 0;
}
