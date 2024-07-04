#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PROXY_IP "127.0.0.1"
#define PROXY_PORT 8888
#define MAX_BUFFER_SIZE 1024

int main()
{
    int client_socket;
    struct sockaddr_in proxy_address;

    // 创建套接字
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // 设置代理服务器地址结构
    memset(&proxy_address, 0, sizeof(proxy_address));
    proxy_address.sin_family = AF_INET;
    proxy_address.sin_addr.s_addr = inet_addr(PROXY_IP);
    proxy_address.sin_port = htons(PROXY_PORT);

    // 连接到代理服务器
    if (connect(client_socket, (struct sockaddr *)&proxy_address, sizeof(proxy_address)) == -1)
    {
        perror("Error connecting to proxy");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // 构造 HTTP 请求
    const char *http_request = "GET / HTTP/1.1\r\n"
                               "Host: 127.0.0.1:80\r\n"
                               "Connection: close\r\n"
                               "\r\n";

    char buffer[MAX_BUFFER_SIZE];
    ssize_t bytes_received;
    send(client_socket, http_request, strlen(http_request), 0);
    printf("Sent HTTP request to proxy\n");

    // 接收代理服务器的响应
    while ((bytes_received = recv(client_socket, buffer, sizeof(buffer), 0)) > 0)
    {
        // 在实际应用中，这里可以添加对数据的处理逻辑
        // 在此示例中，简单地打印响应数据
        printf("Received response from proxy: %.*s\n", (int)bytes_received, buffer);
    }
    printf("recv while exit\n");

    // 关闭套接字
    close(client_socket);

    return 0;
}
