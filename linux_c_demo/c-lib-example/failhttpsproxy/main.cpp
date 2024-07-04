#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define BUFFER_SIZE 4096
#define PORT 8888

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


void handle_client_ssl(int client_socket, SSL *ssl)
{
    char buffer[BUFFER_SIZE];
    ssize_t bytes_received;

    // Receive client's request
    bytes_received = SSL_read(ssl, buffer, sizeof(buffer));
    if (bytes_received < 0)
    {
        perror("Error receiving data from client");
        close(client_socket);
        return;
    }

    // Process the request as needed
    // (You may need to modify this part based on your requirements)

    // Example: Print the received data
    printf("Received request from client:\n%s\n", buffer);

    char oldbuffer[BUFFER_SIZE];
    strcpy(oldbuffer, buffer);
    // 解析请求
    char *method = strtok(buffer, " ");
    char *url = strtok(NULL, " ");
    char *version = strtok(NULL, "\r\n");
    char *hostt = strtok(NULL, " ");
    char *hosttt = strtok(NULL, "\r\n");
    if (method == NULL || url == NULL || version == NULL || hosttt == NULL)
    {
        fprintf(stderr, "Invalid request format\n");
        close(client_socket);
        return;
    }
    printf("Received request from client:\n%s\n", oldbuffer);
    printf("Method: %s\n", method);
    printf("URL: %s\n", url);
    printf("Version: %s\n", version);
    printf("Host2: %s\n", hosttt);
    // 解析主机和端口
    char hoststr[256];
    char *port = strstr(hosttt, ":");
    if (port == NULL)
    {
        strcpy(port, "80");
        strcpy(hoststr, hosttt);
    }
    else
    {
        port++;
        char *host_end = strchr(hosttt, ':');
        *host_end = '\0';
        strcpy(hoststr, hosttt);
    }
    printf("port--------%d\n", atoi(port));

    // 与目标服务器通信
    struct hostent *target_host = gethostbyname(hoststr);
    if (target_host == NULL)
    {
        perror("Error resolving host");
        close(client_socket);
        return;
    }
    struct sockaddr_in target_address;
    memset(&target_address, 0, sizeof(target_address));
    target_address.sin_family = AF_INET;
    target_address.sin_port = htons(8119);
    memcpy(&target_address.sin_addr, target_host->h_addr, target_host->h_length);
    int target_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (target_socket == -1)
    {
        perror("Error creating target socket");
        close(client_socket);
        return;
    }
    if (connect(target_socket, (struct sockaddr *)&target_address, sizeof(target_address)) == -1)
    {
        perror("Error connecting to target server");
        close(target_socket);
        close(client_socket);
        return;
    }
    // 将请求发送给目标服务器
    if (send(target_socket, oldbuffer, bytes_received, 0) == -1)
    {
        perror("Error sending request to target server");
        close(target_socket);
        close(client_socket);
        return;
    }

    // 从目标服务器接收响应并发送给客户端
    ssize_t bytes_sent;
    while ((bytes_received = recv(target_socket, buffer, sizeof(buffer), 0)) > 0)
    {

        SSL_write(ssl, buffer, bytes_received);
        if (bytes_sent == -1)
        {
            perror("Error sending response to client");
            close(target_socket);
            close(client_socket);
            return;
        }
    }

    // Close the SSL connection
    SSL_shutdown(ssl);
    SSL_free(ssl);

    // Close the client socket
    close(target_socket);
    close(client_socket);
}

void ssl_info_callback(const SSL *ssl, int type, int val)
{
    printf("SSL info callback: type=%d, val=%d\n", type, val);
}

int main()
{

    generate_key_and_cert("server.key", "server.crt");
    printf("pri key and cert generate succ\n");

    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_len = sizeof(client_address);

    // Initialize OpenSSL
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();

    // Create an SSL context
    SSL_CTX *ctx = SSL_CTX_new(SSLv23_server_method());
    if (!ctx)
    {
        perror("Error creating SSL context");
        exit(EXIT_FAILURE);
    }

    SSL_CTX_set_info_callback(ctx, ssl_info_callback);
    // SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, NULL);

    // Load the server certificate and private key
    if (SSL_CTX_use_certificate_file(ctx, "server.crt", SSL_FILETYPE_PEM) <= 0 ||
        SSL_CTX_use_PrivateKey_file(ctx, "server.key", SSL_FILETYPE_PEM) <= 0)
    {
        perror("Error loading certificate or private key");
        SSL_CTX_free(ctx);
        exit(EXIT_FAILURE);
    }

    // Create a new SSL structure
    SSL *ssl;

    // Create a new socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address structure
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    // Bind the socket to the specified port
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        perror("Error binding socket");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 10) == -1)
    {
        perror("Error listening for connections");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Proxy server listening on port %d...\n", PORT);

    // Accept and handle incoming connections
    while (1)
    {
        client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_len);
        if (client_socket == -1)
        {
            perror("Error accepting connection");
            continue;
        }

        printf("Accepted connection from %s\n", inet_ntoa(client_address.sin_addr));

        // Create a new SSL structure for the connection
        ssl = SSL_new(ctx);
        if (!ssl)
        {
            perror("Error creating SSL structure");
            close(client_socket);
            continue;
        }

        // Assign the socket to the SSL structure
        SSL_set_fd(ssl, client_socket);

        // Perform the SSL handshake
        if (SSL_accept(ssl) <= 0)
        {
            perror("Error during SSL handshake");
            SSL_free(ssl);
            close(client_socket);
            continue;
        }

        // Create a child process to handle the client request
        pid_t pid = fork();
        if (pid == 0)
        {
            // Child process
            close(server_socket);

            // Handle the client request using SSL
            handle_client_ssl(client_socket, ssl);

            // Exit the child process
            exit(EXIT_SUCCESS);
        }
        else if (pid > 0)
        {
            // Parent process
            close(client_socket);
        }
        else
        {
            perror("Error forking process");
            close(client_socket);
        }
    }

    // Cleanup OpenSSL
    SSL_CTX_free(ctx);
    EVP_cleanup();

    return 0;
}
