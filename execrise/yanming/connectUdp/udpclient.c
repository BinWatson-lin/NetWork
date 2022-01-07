
#include "../include/common.h"
# define    MAXLINE     4096

int main(int argc, char **argv) {
    if (argc != 2) {
        error(1, 0, "usage: udpclient3 <IPaddress>");
    }

    int socket_fd;
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &server_addr.sin_addr);

    socklen_t server_len = sizeof(server_addr);
    /***
     * 一般来说，客户端通过 connect 绑定服务端的地址和端口，
     * 对 UDP 而言，可以有一定程度的性能提升。这是为什么呢？
     * 因为如果不使用 connect 方式，每次发送报文都会需要这样的过程：
     * 连接套接字→发送报文→断开套接字→连接套接字→发送报文→断开套接字 →………
     * 而如果使用 connect 方式，就会变成下面这样：
     * 连接套接字→发送报文→发送报文→……→最后断开套接字
      */
    if (connect(socket_fd, (struct sockaddr *) &server_addr, server_len)) {
        error(1, errno, "connect failed");
    }

    char send_line[MAXLINE], recv_line[MAXLINE + 1];
    int n;

    while (fgets(send_line, MAXLINE, stdin) != NULL) {
        int i = strlen(send_line);
        /* 格式化数据 */
        /* 如果不是\n说明是一行数据，先发送MAXLINE，
        后面的留到后面的数据包发送 */
        if (send_line[i - 1] == '\n') {
            send_line[i - 1] = 0;
        }

        printf("now sending %s\n", send_line);
        size_t rt = send(socket_fd, send_line, strlen(send_line), 0);
        if (rt < 0) {
            error(1, errno, "send failed ");
        }
        printf("send bytes: %zu \n", rt);

        recv_line[0] = 0;
        n = recv(socket_fd, recv_line, MAXLINE, 0);
        if (n < 0)
            error(1, errno, "recv failed");
        recv_line[n] = 0;
        fputs(recv_line, stdout);
        fputs("\n", stdout);
    }

    exit(0);
}