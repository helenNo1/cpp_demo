#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PROXY_PORT 8888
#define MAX_BUFFER_SIZE 1024

void parsehost(char *host, char *ip, int &port)
{
    char *port_separator = strchr(host, ':');
    if (port_separator != NULL)
    {
        *port_separator = '\0';
        char *port_str = port_separator + 1;
        port = atoi(port_str);
        strcpy(ip, host);
    }
    else
    {
        port = 443;
    }
    strcpy(ip, host);
}

int parse_host_header(const char *request, char *ip, int &port)
{
    char rethost[256];
    const char *host_start = strstr(request, "Host: ");

    if (host_start != NULL)
    {
        host_start += strlen("Host: ");
        const char *host_end = strchr(host_start, '\r');

        if (host_end != NULL)
        {
            size_t host_length = host_end - host_start;
            char host[MAX_BUFFER_SIZE];

            if (host_length < sizeof(host) - 1)
            {
                strncpy(host, host_start, host_length);
                host[host_length] = '\0';
                strcpy(rethost, host);
                printf("Host: %s\n", host);
            }
            else
            {
                // Handle host string too long
                printf("Host string too long\n");
                return -1;
            }
        }
        else
        {
            // Handle missing '\r' in Host header
            printf("Invalid Host header format\n");
            return -1;
        }
    }
    else
    {
        // Handle missing Host header
        printf("Host header not found\n");
        return -1;
    }
    parsehost(rethost, ip, port);
    return 0;
}

void forward_data(int source_socket, int destination_socket)
{
    char buffer[MAX_BUFFER_SIZE];
    ssize_t bytes_received;

    while ((bytes_received = recv(source_socket, buffer, sizeof(buffer), 0)) > 0)
    {
        char ip[256];
        int port;
        if(parse_host_header(buffer, ip, port) == 0) {
            printf("parse host header %s ---- %d\n", ip, port); 
        }
        // 在实际应用中，这里可以添加对数据的处理逻辑
        // 在此示例中，简单地将数据转发到目标服务器或客户端
        send(destination_socket, buffer, bytes_received, 0);
    }
}

int main()
{
    int proxy_socket, client_socket, server_socket;
    struct sockaddr_in proxy_address, client_address, server_address;
    socklen_t client_address_len = sizeof(client_address);

    // 创建代理服务器套接字
    if ((proxy_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Error creating proxy socket");
        exit(EXIT_FAILURE);
    }

    // 设置代理服务器地址结构
    memset(&proxy_address, 0, sizeof(proxy_address));
    proxy_address.sin_family = AF_INET;
    proxy_address.sin_addr.s_addr = INADDR_ANY;
    proxy_address.sin_port = htons(PROXY_PORT);

    // 绑定代理服务器套接字到指定端口
    if (bind(proxy_socket, (struct sockaddr *)&proxy_address, sizeof(proxy_address)) == -1)
    {
        perror("Error binding proxy socket");
        exit(EXIT_FAILURE);
    }

    // 监听连接
    if (listen(proxy_socket, 5) == -1)
    {
        perror("Error listening on proxy socket");
        exit(EXIT_FAILURE);
    }

    printf("Proxy server listening on port %d...\n", PROXY_PORT);

    // 接受客户端连接并处理数据
    while (1)
    {
        // 接受客户端连接
        if ((client_socket = accept(proxy_socket, (struct sockaddr *)&client_address, &client_address_len)) == -1)
        {
            perror("Error accepting connection");
            continue;
        }

        printf("Accepted connection from %s\n", inet_ntoa(client_address.sin_addr));

        // 创建套接字连接到目标服务器
        if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            perror("Error creating server socket");
            close(client_socket);
            continue;
        }

        // 设置目标服务器地址结构
        memset(&server_address, 0, sizeof(server_address));
        server_address.sin_family = AF_INET;
        server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); // 替换为实际的目标服务器 IP 地址
        server_address.sin_port = htons(80);                     // 替换为实际的目标服务器端口号

        // 连接到目标服务器
        if (connect(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
        {
            perror("Error connecting to server");
            close(client_socket);
            close(server_socket);
            continue;
        }

        printf("Connected to target server\n");

        // 启动两个线程分别转发数据
        if (fork() == 0)
        {
            // 子进程：转发客户端到目标服务器的数据
            forward_data(client_socket, server_socket);
            exit(EXIT_SUCCESS);
        }
        else
        {
            // 父进程：转发目标服务器到客户端的数据
            forward_data(server_socket, client_socket);
            close(client_socket);
            close(server_socket);
        }
    }

    // 关闭代理服务器套接字
    close(proxy_socket);

    return 0;
}