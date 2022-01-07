#include "../include/common.h"

size_t readn(int fd, void *buffer, size_t length) {
    size_t count;
    ssize_t nread;
    char *ptr;

    /* ptr指向当前缓冲区位置 */
    ptr = buffer;
    /* count还需要读入的数据数 */
    count = length;
    while (count > 0) {
        nread = read(fd, ptr, count);

        if (nread < 0) {
            if (errno == EINTR)
                continue;
            else
                return (-1);
        } else if (nread == 0)
            break;                /* EOF */
        /* 还需要读入的数量减一 */
        /* 指针指向往后移动 */
        count -= nread;
        ptr += nread;
    }
    return (length - count);        /* return >= 0 */
}