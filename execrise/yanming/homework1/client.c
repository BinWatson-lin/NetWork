#include "common.h"
#include "message_t.h"

/* 
客户端程序要求可以指定待连接的服务器端 IP 地址和端口。
在输入一个命令之后，回车结束，之后等待服务器端将执行结果返回，客户端程序需要将结果显示在屏幕上。
样例输出如下所示。
pwd：显示服务器应用程序启动时的当前路径。
cd：改变服务器应用程序的当前路径。
ls：显示服务器应用程序当前路径下的文件列表。
quit：客户端进程退出，但是服务器端不能退出，第二个客户可以再次连接上服务器端。
 */

#define KEEP_ALIVE_TIME 10

int tcp_client(char *address, int server__port);



/* 
    #define COMM_COUNT 4
    char *command[COMM_COUNT] = {"quit", "pwd", "cd", "ls"};

    判断命令是否合法
    for (int i = 0; i < COMM_COUNT; ++i) {
        if (strncmp(command[i], message.buf, strlen(command[i])) == 0) {
            isCommand = 1;
            break;
        }
    }
 */


int main(int argc, char **argv) 
{
    if (argc != 2) {
        printf("usage: client <server address>");
        exit(0);
    }

    struct message_t message;

    int socket_fd = tcp_client(argv[1], SERV_PORT);
    

    fd_set readmask;
    fd_set allmasks;

    FD_ZERO(&allmasks);
    FD_SET(socket_fd ,&allmasks);
    FD_SET(STDIN_FILENO, &allmasks);

    for (;;) {
        readmask = allmasks;
        int rc = select(socket_fd + 1, &readmask, NULL, NULL, NULL);
        if (rc < 0) {
            error(1, errno, "select failed ");
        }
        if (FD_ISSET(socket_fd, &readmask)) {
            /* socket套接字 */
            char buff[MAXLINE + 1];
            int rc = read_message(socket_fd, buff, MAXLINE);
            if (rc < 0) {
                error(1, errno, "read error");
            } else if (rc == 0) {
                close(socket_fd);
                printf("server closed\n");
                break;
            }
            buff[rc] = '\0';
            printf("receive: %s\n", buff);
        }
        if (FD_ISSET(STDIN_FILENO, &readmask)) {
            if (fgets(message.buf, sizeof(message.buf), stdin) != NULL) {
                int n = strlen(message.buf);
                if (message.buf[n - 1] == '\n') {
                    message.buf[n - 1] = 0;
                }
                if ((strncmp(message.buf, "quit", strlen(message.buf)) == 0)) {
                    if (shutdown(socket_fd, 1)) {
                        error(1, errno, "shutdown failed");                    
                    }
                    break;
                }
                message.message_length = htonl(n);
                message.message_type = 1;
                int length = sizeof(message.message_length) + sizeof(message.message_type) + n;
                if (send(socket_fd, (char *) &message, length, 0) < 0) {
                    error(1, errno, "send failure");
                }
            }
        }
    }

    return 0;
}
