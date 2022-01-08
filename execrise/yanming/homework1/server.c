#include "common.h"
#include "signal_hander.h"
#include "message_t.h"

int main(int argc, char **argv)
{
    int listenfd, connfd;

    listenfd = socket(PF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servaddr, cliaddr;
    socklen_t servlen = sizeof(servaddr);
    

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    int on = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    
    if (bind(listenfd, (struct sockaddr*)&servaddr, servlen) < 0) {
        error(1, errno, "bind failure");
    }

    /* 转为监听套接字 */
    if (listen(listenfd, LISTENQ) < 0) {
        error(1, errno, "listen failure");
    }

    signal(SIGPIPE, signal_pipe);
    signal(SIGINT, signal_int);



    struct message_t message;
    char buff[MAXLINE + 1];
    socklen_t clilen = sizeof(cliaddr);
    for (;;) {
        if ((connfd = accept(listenfd, (struct sockaddr*) &cliaddr, &clilen)) < 0) {
            printf("accept error\n");
            continue;
        }
        for (;;) {
            int n = read_message(connfd, buff, MAXLINE);
            if (n < 0) {
                error(1, errno, "error read message");
            } else if (n == 0) {                
                printf("client closed \n");                
                close(connfd);                
                break;            
            }
            buff[n] = '\0';
            printf("receive: %s", buff);

            strcpy(message.buf, "hi, I fsfsdfsdf...");
            message.message_length = htonl(strlen(message.buf));
            message.message_type = 1;
            printf("send: %s\n", message.buf);
            int length = sizeof(message.message_length) + sizeof(message.message_type) + strlen(message.buf);
            if (send(connfd, (char*) &message, length, 0) < 0) {

            }
        }
        close(connfd);
    }
}