#include "common.h"

int tcp_client(char *address, int server__port) {
    int socket_fd;
    /* TCP, 字节流协议 */
    socket_fd = socket(PF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(server__port);
    inet_pton(AF_INET, address, &servaddr.sin_addr);

    socklen_t servlen = sizeof(servaddr);
    /* If the connection or binding succeeds, zero is returned.  On
       error, -1 is returned, and errno is set to indicate the error. */
    if (connect(socket_fd, (struct sockaddr *) &servaddr, servlen) < 0) {
        error(1, errno, "connect failed ");
    }
    return socket_fd;
}