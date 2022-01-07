#include "../include/common.h"

int read_line(int fd, char *buf, int size) {
    int i = 0;
    char c = '\0';
    int n;

    while ((i < size - 1) && (c != '\n')) {
        n = recv(fd, &c, 1, 0);
        if (n > 0) {
        /* 
            MSG_PEEK:
            This flag causes the receive operation to return data from
            the beginning of the receive queue without removing that
            data from the queue.  Thus, a subsequent receive call will
            return the same data.
         */
            if (c == '\r') {
                /* 以\n结尾或者以\r结尾 */
                n = recv(fd, &c, 1, MSG_PEEK);
                if ((n > 0) && (c == '\n'))
                    recv(fd, &c, 1, 0);
                else
                    c = '\n';
            }
            buf[i] = c;
            i++;
        } else
            c = '\n';
    }
    buf[i] = '\0';

    return (i);
}