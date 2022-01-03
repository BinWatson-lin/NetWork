



### 通用套接字地址

```c
/* POSIX.1g 规范规定了地址族为2字节的值.  */
typedef unsigned short int sa_family_t;
/* 描述  */
struct sockaddr{
    sa_family_t sa_family;  /* 地址族.  16-bit*/
    char sa_data[14];   /* 具体的地址值 112-bit */
};
```

### IPV4套接字地址

```c
/* IPV4套接字地址，32bit值.  */
typedef uint32_t in_addr_t;
struct in_addr
{
    in_addr_t s_addr;
};
  
/* 描述IPV4的套接字地址格式  */
struct sockaddr_in
{
    sa_family_t sin_family; /* 16-bit */
    in_port_t sin_port;     /* 端口号  16-bit*/
    struct in_addr sin_addr;    /* Internet address. 32-bit */


    /* 这里仅仅用作占位符，不做实际用处  */
    unsigned char sin_zero[8];
};
```

### IPV6套接字地址

```c
struct sockaddr_in6
{
    sa_family_t sin6_family; /* 16-bit */
    in_port_t sin6_port;  /* 传输端口号 # 16-bit */
    uint32_t sin6_flowinfo; /* IPv6流控信息 32-bit*/
    struct in6_addr sin6_addr;  /* IPv6地址128-bit */
    uint32_t sin6_scope_id; /* IPv6域ID 32-bit */
};
```

### 本地套接字格式

```c
struct sockaddr_un {
    unsigned short sun_family; /* 固定为 AF_LOCAL */
    char sun_path[108];   /* 路径名 */
};
//  AF_LOCAL
```

<img src="https://static001.geekbang.org/resource/image/ed/58/ed49b0f1b658e82cb07a6e1e81f36b58.png" alt="img" style="zoom: 12%;" />

### 各种地址族的宏定义 

```c
/* 各种地址族的宏定义  */
#define AF_UNSPEC PF_UNSPEC
#define AF_LOCAL  PF_LOCAL
#define AF_UNIX   PF_UNIX
#define AF_FILE   PF_FILE
#define AF_INET   PF_INET
#define AF_AX25   PF_AX25
#define AF_IPX    PF_IPX
#define AF_APPLETALK  PF_APPLETALK
#define AF_NETROM PF_NETROM
#define AF_BRIDGE PF_BRIDGE
#define AF_ATMPVC PF_ATMPVC
#define AF_X25    PF_X25
#define AF_INET6  PF_INET6
```



### Standard well-known ports

```c
/* Standard well-known ports.  */
enum
  {
    IPPORT_ECHO = 7,    /* Echo service.  */
    IPPORT_DISCARD = 9,   /* Discard transmissions service.  */
    IPPORT_SYSTAT = 11,   /* System status service.  */
    IPPORT_DAYTIME = 13,  /* Time of day service.  */
    IPPORT_NETSTAT = 15,  /* Network status service.  */
    IPPORT_FTP = 21,    /* File Transfer Protocol.  */
    IPPORT_TELNET = 23,   /* Telnet protocol.  */
    IPPORT_SMTP = 25,   /* Simple Mail Transfer Protocol.  */
    IPPORT_TIMESERVER = 37, /* Timeserver service.  */
    IPPORT_NAMESERVER = 42, /* Domain Name Service.  */
    IPPORT_WHOIS = 43,    /* Internet Whois service.  */
    IPPORT_MTP = 57,




    IPPORT_TFTP = 69,   /* Trivial File Transfer Protocol.  */
    IPPORT_RJE = 77,
    IPPORT_FINGER = 79,   /* Finger service.  */
    IPPORT_TTYLINK = 87,
    IPPORT_SUPDUP = 95,   /* SUPDUP protocol.  */


    IPPORT_EXECSERVER = 512,  /* execd service.  */
    IPPORT_LOGINSERVER = 513, /* rlogind service.  */
    IPPORT_CMDSERVER = 514,
    IPPORT_EFSSERVER = 520,


    /* UDP ports.  */
    IPPORT_BIFFUDP = 512,
    IPPORT_WHOSERVER = 513,
    IPPORT_ROUTESERVER = 520,


    /* Ports less than this value are reserved for privileged processes.  */
    IPPORT_RESERVED = 1024,


    /* Ports greater this value are reserved for (non-privileged) servers.  */
    IPPORT_USERRESERVED = 5000
    
    ...
}
```

## 接口定义

### socket

```c
int socket(int domain, int type, int protocol)
```

**domain** 就是指 PF_INET、PF_INET6 以及 PF_LOCAL 等，表示什么样的套接字。

**type** 可用的值是：

- SOCK_STREAM: 表示的是字节流，对应 TCP；
- SOCK_DGRAM： 表示的是数据报，对应 UDP；
- SOCK_RAW: 表示的是原始套接字。

### bind

```c
bind(int fd, sockaddr * addr, socklen_t len)
```

 bind 函数后面的第二个参数是通用地址格式 `sockaddr * addr`。

这里有一个地方值得注意，那就是虽然接收的是$通用地址格式$，实际上传入的参数可能是 IPv4、IPv6 或者本地套接字格式。bind 函数会根据 `len` 字段判断传入的参数 `addr` 该怎么解析，`len` 字段表示的就是传入的地址长度，它是一个可变值。

> ```c
> struct sockaddr_in name;
> bind (sock, (struct sockaddr *) &name, sizeof (name)
> name.sin_addr.s_addr = htonl (INADDR_ANY); /* IPV4通配地址 */
> ```

### listen

```c
int listen (int socketfd, int backlog)
```

- 第一个参数 `socketdf` 为套接字描述符，

- 第二个参数 `backlog`，在 Linux 中表示已完成 (ESTABLISHED) 且未 accept 的队列大小，这个参数的大小决定了可以接收的并发数目。这个参数越大，并发数目理论上也会越大。但是参数过大也会占用过多的系统资源，一些系统，比如 Linux 并不允许对这个参数进行改变。

### accept

```c
int accept(int listensockfd, struct sockaddr *cliaddr, socklen_t *addrlen)
```

- 第一个是监听套接字描述字 `listensockfd`，它是作为输入参数存在的；

- 第二个是返回的已连接套接字描述字。

### connect

```c
int connect(int sockfd, const struct sockaddr *servaddr, socklen_t addrlen)
```

第一个参数 `sockfd` 是连接套接字，通过前面讲述的 socket 函数创建。

第二个、第三个参数 `servaddr` 和 `addrlen` 分别代表指向套接字地址结构的指针和该结构的大小。套接字地址结构必须含有服务器的 IP 地址和端口号。

> 客户在调用函数 connect 前不必非得调用 bind 函数，因为如果需要的话，内核会确定源 IP 地址，并按照一定的算法选择一个临时端口作为源端口。

### send

```C
int send(int socketfd, const void * msg, int len, unsigned int falgs);
```

将数据由指定的 `socketfd` 传给对方主机

- 参数 `socketfd` 为已建立好连接的socket

- 参数 `msg` 指向欲连线的数据内容

- 参数 `len` 则为数据长度

- 参数 `flags` 一般设0, 其他数值定义如下：

  - MSG_OOB 传送的数据以out-of-band 送出
  - MSG_DONTROUTE 取消路由表查询
  - MSG_DONTWAIT 设置为不可阻断运作
  - MSG_NOSIGNAL 此动作不愿 被 `SIGPIPE` 信号中断

- 返回值：成功则返回实际传送出去的字符数, 失败返回-1

  - 错误原因存于 `errno`，错误代码：

    - EBADF 参数 `socketfd` 非合法的socket处理代码

    - EFAULT 参数中有一指针指向无法存取的内存空间

    - ENOTSOCK 参数 `socketfd` 为一文件描述词, 非socket

    - EINTR 被信号所中断.

    - EAGAIN 此操作会令进程阻断, 但参数 `socketfd` 的socket 为不可阻断

    - ENOBUFS 系统的缓冲内存不足

    - ENOMEM 核心内存不足

    - EINVAL 传给系统调用的参数不正确

### inet_pton

```c
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
int inet_pton(int af, const char *src, void *dst);
```

`inet_pton`函数原型如下[将“点分十进制” －> “整数”]

第一个参数 `af` 是地址族，转换后存在 `dst` 中



