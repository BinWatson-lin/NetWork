#include "../include/common.h"
int main(int argc, char **argv) {
    if (argc != 2) {
        error(1, 0, "usage: reliable_client02 <IPaddress>");
    }

    int socket_fd = tcp_client(argv[1], SERV_PORT);

    signal(SIGPIPE, SIG_IGN);

    char *msg = "dsfsdfhsjkdfhsjkdfhkjdshfiu2  fhwi fdshkgfh sdgfhs fhshkfskhg fkhsadh fjhsd jkafhsjkdafh jsdhf jksd ";
    // msg[255] = '\0';
    ssize_t n_written;

    int count = 10000000;
    while (count > 0) {
        n_written = send(socket_fd, msg, strlen(msg), 0);
        fprintf(stdout, "send into buffer %ld \n", n_written);
        if (n_written <= 0) {
            error(1, errno, "send error");
            return -1;
        }
        count--;
    }
    return 0;
}