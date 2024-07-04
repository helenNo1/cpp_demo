#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 4096
#define PORT 8888

void handle_client(int client_socket)
{
    char buffer[BUFFER_SIZE];
    char oldbuffer[BUFFER_SIZE];
    ssize_t bytes_received;

    // 接收客户端的请求
    bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytes_received < 0)
    {
        perror("Error receiving data from client");
        close(client_socket);
        return;
    }
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
        bytes_sent = send(client_socket, buffer, bytes_received, 0);
        if (bytes_sent == -1)
        {
            perror("Error sending response to client");
            close(target_socket);
            close(client_socket);
            return;
        }
    }

    close(target_socket);

    // 关闭客户端连接
    close(client_socket);
}

int main()
{
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_len = sizeof(client_address);

    // 创建套接字
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // 设置服务器地址结构
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    // 绑定套接字到指定端口
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        perror("Error binding socket");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // 开始监听连接
    if (listen(server_socket, 10) == -1)
    {
        perror("Error listening for connections");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Proxy server listening on port %d...\n", PORT);

    // 接受并处理连接
    while (1)
    {
        client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_len);
        if (client_socket == -1)
        {
            perror("Error accepting connection");
            continue;
        }

        printf("Accepted connection from %s\n", inet_ntoa(client_address.sin_addr));

        // 创建子进程处理客户端请求
        pid_t pid = fork();
        if (pid == 0)
        {
            // 子进程
            close(server_socket);
            handle_client(client_socket);
            exit(EXIT_SUCCESS);
        }
        else if (pid > 0)
        {
            // 父进程
            close(client_socket);
        }
        else
        {
            perror("Error forking process");
            close(client_socket);
        }
    }

    return 0;
}
