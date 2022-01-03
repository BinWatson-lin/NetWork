#include "../lib/common.h"

/*
// IPV4套接字地址，32bit值.
typedef uint32_t in_addr_t;
struct in_addr
{
    in_addr_t s_addr; // 32bit
};
  
// 描述IPV4的套接字地址格式
struct sockaddr_in
{
    sa_family_t sin_family; // 16-bit
    in_port_t sin_port;     // 端口号  16-bit
    struct in_addr sin_addr;    // Internet address. 32-bit
    // 这里仅仅用作占位符，不做实际用处
    unsigned char sin_zero[8];
};
*/
size_t readn(int fd, void *buffer, size_t size);

void read_data(int socketfd) { 
    // int fprintf(FILE *stream, const char *format, ...)
    // int printf(const char *format, ...);
    ssize_t n;
    char buff[1024];

    int time = 0;
    for(;;) {
        fprintf(stdout, "block in read.\n");
        // 0: End of File 读完了
        // size_t readn(int fd, void *buffer, size_t size)
        if ((n = readn(socketfd, buff, 1024) == 0))
            return;
        time++;
        fprintf(stdout, "1K read for %d \n", time);
        // void usleep(int micro_seconds);
        usleep(10000);
    }
}

// 服务器的流程
// 1. 创建套接字
// 2. 绑定端口号
// 3. 改为监听
// 4. 等待用户连接
int main()
{
    int listenfd, connfd;
    socklen_t clilen;
    // IPv4地址结构
    /*
    struct sockaddr_in
    {
        sa_family_t sin_family;
        in_port_t sin_port;
        struct in_addr sin_addr;
        unsigned char sin_zero[8];
    };
    */
    struct sockaddr_in cliaddr, servaddr;

    /* 1. 创建套接字 */
    listenfd = socket(PF_INET, SOCK_STREAM, 0);
    // 将servaddr清零
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    
    // 将一个无符号整型数值转换为网络字节序，由于主机的字节顺序和网络可能不一样
    // 网络字节序是 big-endian，也就是整数的高位字节存放在内存的低地址处。
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // host to network long IPV4通配地址
    servaddr.sin_port = htons(12345); // host to network short
    /* 2. 绑定端口号 */
    // 需要将 IPv4、IPv6 或者本地套接字格式转化为通用套接字格式 sockaddr
    bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    /* 3. 改为监听 */
    // 第二个参数 backlog，在 Linux 中表示已完成 (ESTABLISHED) 且未 accept 的队列大小
    // 这个参数的大小决定了可以接收的并发数目。这个参数越大，并发数目理论上也会越大
    listen(listenfd, 1024);
    for(;;) {
        /* 4. 等待用户连接 */
        clilen = sizeof(cliaddr);
        // 这里会阻塞在accept中等待客户端的连接
        // 第一个部分 cliadd 是通过指针方式获取的客户端的地址，addrlen 告诉我们地址的大小，
        // 另一个部分是函数的返回值，这个返回值是一个全新的描述字，代表了与客户端的连接。
        connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);
        read_data(connfd);
        close(connfd);
    }
}