#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_client(int client_socket)
{
    char buffer[BUFFER_SIZE];
    ssize_t bytes_received;
    bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytes_received <= 0)
    {
        perror("recv");
        close(client_socket);
        return;
    }
    printf("received http req\n%s\n", buffer);

    const char *resp = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHelloWorld!";
    send(client_socket, resp, strlen(resp), 0);
    close(client_socket);
}

int main(int argc, char const *argv[])
{
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_len = sizeof(client_address);
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        perror("bind");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 5) == -1)
    {
        perror("listen");
        close(server_socket);
        exit(EXIT_FAILURE);
    }
    printf("httpserver is lsten on port %d\n", PORT);
    while (1)
    {
        client_socket = accept(server_socket, (struct sockaddr *)&client_address,
                               &client_address_len);
        if (client_socket == -1)
        {
            perror("accept");
            continue;
        }
        handle_client(client_socket);
    }

    close(server_socket);

    return 0;
}
