#include "../include/common.h"

int tcp_client(char *address, int server_port)
{
    int socket_fd;
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    inet_pton(AF_INET, address, &server_addr.sin_addr);

    socklen_t server_len = sizeof(server_addr);
    int connect_rt = connect(socket_fd, (struct sockaddr *) &server_addr, server_len);
    printf("%d\n", connect_rt);
    if (connect_rt < 0) {
        error(1, errno, "connect failed ");
    }
    return socket_fd;
}