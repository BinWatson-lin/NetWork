

# 一、网络编程

## 1. 协议族

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

## 2. 接口定义

### socket

```c
int socket(int domain, int type, int protocol)
```

**domain** 就是指 PF_INET、PF_INET6 以及 PF_LOCAL 等，表示什么样的套接字。

**type** 可用的值是：

- `SOCK_STREAM` : 表示的是字节流，对应 TCP；
- `SOCK_DGRAM` ： 表示的是数据报，对应 UDP；
- `SOCK_RAW` : 表示的是原始套接字。

### bind

```c
bind(int fd, sockaddr * addr, socklen_t len)
```

 bind 函数后面的第二个参数是通用地址格式 `sockaddr * addr`。

这里有一个地方值得注意，那就是虽然接收的是**通用地址格式**，实际上传入的参数可能是 IPv4、IPv6 或者本地套接字格式。bind 函数会根据 `len` 字段判断传入的参数 `addr` 该怎么解析，`len` 字段表示的就是传入的地址长度，它是一个可变值。

> ```c
> struct sockaddr_in name;
> bind (sock, (struct sockaddr *) &name, sizeof (name));
> name.sin_addr.s_addr = htonl(INADDR_ANY); /* IPV4通配地址 */
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

### 字节流

#### send

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

### 数据报

#### recvfrom

```c
#include <sys/socket.h>
ssize_t recvfrom(int sockfd, void *buff, size_t nbytes, int flags, 
　　　　　　　　　　struct sockaddr *from, socklen_t *addrlen); 
```

- `sockfd` 是本地创建的套接字描述符

- `buff` 指向本地的缓存

- `nbytes` 表示最大接收数据字节。
- 第四个参数 `flags` 是和 I/O 相关的参数，这里我们还用不到，设置为 0。
- 后面两个参数 `from` 和 `addrlen`，实际上是$返回$对端发送方的地址和端口等信息，这和 TCP 非常不一样，TCP 是通过 accept 函数拿到的描述字信息来决定对端的信息。另外 UDP 报文每次接收都会获取对端的信息，也就是说报文和报文之间是没有上下文的。

#### sendto

```c
#include <sys/socket.h>
ssize_t sendto(int sockfd, const void *buff, size_t nbytes, int flags,
                const struct sockaddr *to, socklen_t addrlen); 
```

- `sockfd` 是本地创建的套接字描述符
- -`buff` 指向发送的缓存
- -`nbytes` 表示发送字节数。
- 第四个参数 flags 依旧设置为 0。后面两个参数 `to` 和 `addrlen`，表示发送的对端地址和端口等信息。

### close

```c
int close(int sockfd)
```

使用 close 函数关闭连接有两个需要明确的地方。

- close 函数只是把套接字**引用计数**减 1，未必会立即关闭连接；

- close 函数如果在套接字引用计数达到 0 时，立即终止读和写两个方向的数据传送。

### shutdown

```c
int shutdown(int sockfd, int howto)
```

- `howto` 是这个函数的设置选项，它的设置有三个主要选项：

  - `SHUT_RD(0)`：关闭连接的“读”这个方向，对该套接字进行读操作直接返回 `EOF`。

    从数据角度来看，套接字上接收缓冲区已有的数据将被丢弃，如果再有新的数据流到达，会对数据进行 ACK，然后悄悄地丢弃。也就是说，对端还是会接收到 ACK，在这种情况下根本不知道数据已经被丢弃了。

  - `SHUT_WR(1)`：关闭连接的“写”这个方向，这就是常被称为”半关闭“的连接。

    此时，不管套接字**引用计数的值**是多少，都会直接关闭连接的写方向。套接字上发送缓冲区已有的数据将被立即发送出去，并发送一个 `FIN` 报文给对端。应用程序如果对该套接字进行写操作会报错。

  - `SHUT_RDWR(2)`：相当于 `SHUT_RD` 和 `SHUT_WR` 操作各一次，关闭套接字的读和写两个方向。

close与shutdown的区别：

- 第一个差别：close 会关闭连接，并释放所有连接对应的资源，而 shutdown 并不会释放掉套接字和所有的资源。
- 第二个差别：close 存在引用计数的概念，并不一定导致该套接字不可用；shutdown 则不管引用计数，直接使得该套接字不可用，如果有别的进程企图使用该套接字，将会受到影响。
- 第三个差别：close 的引用计数导致不一定会发出 FIN 结束报文，而 shutdown 则总是会发出 FIN 结束报文，这在我们打算关闭连接通知对端的时候，是非常重要的。





### 辅助

#### inet_pton

```c
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
int inet_pton(int af, const char *src, void *dst);
```

`inet_pton`函数原型如下[将“点分十进制” －> “整数”]

第一个参数 `af` 是地址族，转换后存在 `dst` 中



### IO多路复用

#### select

```c
#include <sys/select.h>
#include <sys/time.h>
int select(int maxfd, fd_set* readset, fd_set* writeset, fd_set* exceptset, 
           const struct timeval* timeout);
```

select函数共有5个参数，其中参数和返回值：

- `maxfd`：监视对象文件描述符数量。

- `readset`：将所有关注“是否存在待读取数据”的文件描述符注册到 `fd_set` 变量，并传递其地址值。

- `writeset`： 将所有关注“是否可传输无阻塞数据”的文件描述符注册到 `fd_set` 变量，并传递其地址值。

- `exceptset`：将所有关注“是否发生异常”的文件描述符注册到 `fd_set` 变量，并传递其地址值。

- `timeout`：调用select后，为防止陷入无限阻塞状态，传递超时信息。

- 返回值：

  - 错误返回 `-1`
  - 超时返回 `0`
  - 当关注的事件返回时，返回大于 `0` 的值，该值是发生事件的文件描述符数。

  select辅助函数：

- `FD_ZERO(fd_set* fdset)`： 将 `fd_set` 变量的所有位初始化为 `0`。

- `FD_SET（int fd, fd_set* fdset）`：在参数 `fd_set` 指向的变量中注册文件描述符 `fd` 的信息。

- `FD_CLR(int fd, fd_set* fdset)`：参数 `fd_set` 指向的变量中清除文件描述符 `fd` 的信息。

- `FD_ISSET(int fd, fd_set* fdset)`：若参数 `fd_set` 指向的变量中包含文件描述符 `fd` 的信息，则返回真。



```
1) SIGHUP        2) SIGINT          3) SIGQUIT         4) SIGILL         5) SIGTRAP
6) SIGABRT       7) SIGBUS          8) SIGFPE          9) SIGKILL       10) SIGUSR1
11) SIGSEGV      12) SIGUSR2        13) SIGPIPE        14) SIGALRM       15) SIGTERM
16) SIGSTKFLT    17) SIGCHLD        18) SIGCONT        19) SIGSTOP       20) SIGTSTP
21) SIGTTIN      22) SIGTTOU        23) SIGURG         24) SIGXCPU       25) SIGXFSZ
26) SIGVTALRM    27) SIGPROF        28) SIGWINCH       29) SIGIO         30) SIGPWR
31) SIGSYS       34) SIGRTMIN       35) SIGRTMIN+1     36) SIGRTMIN+2    37) SIGRTMIN+3
38) SIGRTMIN+4   39) SIGRTMIN+5     40) SIGRTMIN+6     41) SIGRTMIN+7    42) SIGRTMIN+8
43) SIGRTMIN+9   44) SIGRTMIN+10    45) SIGRTMIN+11    46) SIGRTMIN+12   47) SIGRTMIN+13
48) SIGRTMIN+14  49) SIGRTMIN+15    50) SIGRTMAX-14    51) SIGRTMAX-13   52) SIGRTMAX-12
53) SIGRTMAX-11  54) SIGRTMAX-10    55) SIGRTMAX-9     56) SIGRTMAX-8    57) SIGRTMAX-7
58) SIGRTMAX-6   59) SIGRTMAX-5     60) SIGRTMAX-4     61) SIGRTMAX-3    62) SIGRTMAX-2
63) SIGRTMAX-1   64) SIGRTMAX
```

函数原型：

### 信号处理

#### signal

```c
typedef void (*sighandler_t)(int);
sighandler_t (int signum, sighandler_t handler);
```

参数解释：

- `signum`：更改的信号值

- handler：函数指针，要更改的动作是什么

  > 实际上，该函数内部也调用了 `sigaction` 函数。

#### sigprocmask

```c
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
```

参数解释：

- how，该做什么样的操作
  - SIG_BLOCK：设置信号为阻塞
  - SIG_UNBLOCK：解除信号阻塞
  - SIG_SETMASK：替换阻塞位图
- set：用来设置阻塞位图
  - SIG_BLOCK：设置某个信号为阻塞，block（new） = block（old） | set
  - SIG_UNBLOCK：解除某个信号阻塞，block（new）= block（old） & （~set）
  - SIG_SETMASK：替换阻塞位图，block（new）= set
- oldset：原来的阻塞位图







### string.h

[C 标准库 –  | 菜鸟教程 (runoob.com)](https://www.runoob.com/cprogramming/c-standard-library-stdio-h.html)
