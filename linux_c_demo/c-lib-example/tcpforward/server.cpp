#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_PORT 8888
#define FORWARD_SERVER_IP "127.0.0.1"
#define FORWARD_SERVER_PORT 9999
#define MAX_BUFFER_SIZE 1024

void forward_data(int client_socket)
{
    int forward_socket;
    struct sockaddr_in forward_server_address;

    // 创建套接字
    if ((forward_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Error creating forward socket");
        exit(EXIT_FAILURE);
    }

    // 设置转发服务器地址结构
    memset(&forward_server_address, 0, sizeof(forward_server_address));
    forward_server_address.sin_family = AF_INET;
    forward_server_address.sin_addr.s_addr = inet_addr(FORWARD_SERVER_IP);
    forward_server_address.sin_port = htons(FORWARD_SERVER_PORT);

    // 连接到转发服务器
    if (connect(forward_socket, (struct sockaddr *)&forward_server_address, sizeof(forward_server_address)) == -1)
    {
        perror("Error connecting to forward server");
        close(forward_socket);
        return;
    }

    char buffer[MAX_BUFFER_SIZE];
    ssize_t bytes_received;

    while ((bytes_received = recv(client_socket, buffer, sizeof(buffer), 0)) > 0)
    {
        printf("server recv from client %s\n",  buffer);
        // 在实际应用中，这里可以添加对数据的处理逻辑
        // 在此示例中，简单地将数据转发到第三方服务器
        send(forward_socket, buffer, bytes_received, 0);
        // // 接收服务器的响应
        // if (int bytes_received_fromthird = recv(forward_socket, buffer, sizeof(buffer), 0) > 0)
        // {
        //     printf("Received response from third: %.*s\n", (int)bytes_received_fromthird, buffer);
        // }
    }
    printf("exit while 43\n");

    // 关闭转发套接字
    close(forward_socket);
    printf("forward sock close\n");
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

        // 转发客户端数据到第三方服务器
        forward_data(client_socket);
        printf("forwarded\n");

        // 关闭客户端连接
        close(client_socket);
        printf("close\n");
    }

    // 关闭服务器套接字
    close(server_socket);

    return 0;
}
