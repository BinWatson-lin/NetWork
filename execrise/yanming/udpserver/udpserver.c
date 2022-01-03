#include "../lib/common.h"

#define SERV_PORT 12340
#define MAXLINE 1024

int count;

static void recvfrom_int(int signo) 
{    
    printf("\nreceived %d datagrams\n", count);    
    exit(0);
}

int main(int argc, char **argv) 
{
    int sockfd;
    struct sockaddr_in servaddr;

    sockfd = socket(PF_INET, SOCK_DGRAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    bind(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr));

    socklen_t clilen;    
    char message[MAXLINE];    
    count = 0;

    signal(SIGINT, recvfrom_int);
    struct sockaddr_in cliaddr;
    for(;;) {
        int n = recvfrom(sockfd, message, sizeof(message), 0, (struct sockaddr *) &cliaddr, &clilen); 
        message[n] = '\0';

        printf("received %d bytes: %s\n", n, message);

        char send_line[MAXLINE];
        sprintf(send_line, "Hi, %s", message);
        sendto(sockfd, send_line, sizeof(send_line), 0, (struct sockaddr *) &cliaddr, clilen);
        count++;
    }
}