#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8888
#define MAX_BUFFER_SIZE 1024

int main()
{
    int client_socket;
    struct sockaddr_in server_address;

    // 创建套接字
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // 设置服务器地址结构
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_address.sin_port = htons(SERVER_PORT);

    // 连接到服务器
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        perror("Error connecting to server");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_BUFFER_SIZE];
    ssize_t bytes_received;
    ssize_t inputlen;

    while (1)
    {
        // if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        // {
        //     perror("error read from stdin");
        //     break;
        // }
        // inputlen = strlen(buffer);
        // if (buffer[inputlen - 1] == '\n')
        // {
        //     buffer[--inputlen] = '\0';
        // }

        scanf("%s",buffer);
        inputlen = strlen(buffer);
        if (strcmp(buffer, "exit") == 0)
        {
            break;
        }
        printf("client send %s\n", buffer);
        send(client_socket, buffer, inputlen, 0);
        // 接收服务器的响应
        // if (bytes_received = recv(client_socket, buffer, sizeof(buffer), 0) > 0)
        // {
        //     printf("Received response from server: %.*s\n", (int)bytes_received, buffer);
        // }
    }

    // 关闭套接字
    close(client_socket);

    return 0;
}
