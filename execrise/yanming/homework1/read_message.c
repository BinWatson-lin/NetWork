#include "common.h"

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


size_t read_message(int fd, char *buffer, size_t length) {
    u_int32_t msg_length;
    u_int32_t msg_type;
    int rc;

    /* 先读入头部中的msg_length */
    rc = readn(fd, (char *) &msg_length, sizeof(u_int32_t));
    if (rc != sizeof(u_int32_t))
        return rc < 0 ? -1 : 0;
    msg_length = ntohl(msg_length);

    /* 再读入头部中的msg_type */
    rc = readn(fd, (char *) &msg_type, sizeof(msg_type));
    if (rc != sizeof(u_int32_t))
        return rc < 0 ? -1 : 0;

    /* 大于缓冲区上限则返回 */
    if (msg_length > length) {
        return -1;
    }

    /* 读入负载payload */
    rc = readn(fd, buffer, msg_length);
    if (rc != msg_length)
        return rc < 0 ? -1 : 0;
    return rc;
}