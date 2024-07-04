#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 12345
#define BUFFER_SIZE 1024

void handle_error() {
    perror("Error");
    exit(EXIT_FAILURE);
}

SSL_CTX* init_client_context() {
    SSL_CTX *ctx;

    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    ctx = SSL_CTX_new(TLS_client_method());
    if (ctx == NULL) {
        ERR_print_errors_fp(stderr);
        handle_error();
    }

    return ctx;
}

void start_client(SSL_CTX *ctx) {
    int client_fd;
    struct sockaddr_in server_addr;

    SSL *ssl;

    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) {
        handle_error();
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
    server_addr.sin_port = htons(SERVER_PORT);

    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        handle_error();
    }

    printf("Connected to server at %s:%d\n", SERVER_ADDRESS, SERVER_PORT);

    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, client_fd);

    if (SSL_connect(ssl) <= 0) {
        ERR_print_errors_fp(stderr);
    } else {
        const char* message = "Hello from client!";
        SSL_write(ssl, message, strlen(message));

        char buffer[BUFFER_SIZE];
        memset(buffer, 0, sizeof(buffer));

        int len = SSL_read(ssl, buffer, sizeof(buffer) - 1);
        if (len > 0) {
            printf("Received message: %s\n", buffer);
        }
    }

    SSL_free(ssl);
    close(client_fd);
}

int main() {
    SSL_CTX *ctx = init_client_context();
    start_client(ctx);

    SSL_CTX_free(ctx);

    return 0;
}
