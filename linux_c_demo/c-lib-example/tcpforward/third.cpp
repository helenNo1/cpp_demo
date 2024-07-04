#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_PORT 9999
#define MAX_BUFFER_SIZE 1024

void handle_client(int client_socket)
{
    char buffer[MAX_BUFFER_SIZE];
    ssize_t bytes_received;

    // 接收客户端数据
    while (bytes_received = recv(client_socket, buffer, sizeof(buffer), 0) > 0)
    {
        printf("Received data from server: %s\n",  buffer);

        // 在实际应用中，这里可以添加对数据的处理逻辑
        // 在此示例中，简单地将数据原样转发回客户端
        // send(client_socket, buffer, bytes_received, 0);
    }

    // 关闭客户端连接
    close(client_socket);
}

int main()
{
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_len = sizeof(client_address);

    // 创建套接字
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // 设置服务器地址结构
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(SERVER_PORT);

    // 绑定套接字到指定端口
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    // 监听连接
    if (listen(server_socket, 5) == -1)
    {
        perror("Error listening on socket");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", SERVER_PORT);

    // 接受连接并处理数据
    while (1)
    {
        if ((client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_len)) == -1)
        {
            perror("Error accepting connection");
            continue;
        }

        printf("Accepted connection from %s\n", inet_ntoa(client_address.sin_addr));

        // 处理客户端数据
        handle_client(client_socket);
    }

    // 关闭服务器套接字
    close(server_socket);

    return 0;
}
