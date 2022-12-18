# 网络IPC：套接字

## 套接字描述符

- 套接字是通信端点的抽象。
- 应用程序用套接字描述符访问套接字。
- 套接字描述符在UNIX系统中被当作是一种文件描述符。
- 许多处理文件描述符的函数可以用于处理套接字描述符。

### 创建一个套接字，调用socket函数

```c
#include <sys/socket.h>

int socket(int domain, int type, int protocol);

// 返回值：若成功，返回文件（套接字）描述符；若出错，返回-1；
```

1. 参数domain（域）确定通信的特性，包括地址格式。
   - AF_UNIX: Local communication
   - AF_LOCAL: Synonym for AF_UNIX
   - AF_INET: IPv4 Internet protocols
   - AF_AX25: Amateur radio AX.25 protocol
   - AF_IPX: IPX - Novell protocols
   - AF_APPLETALK:  AppleTalk
   - AF_X25: ITU-T X.25 / ISO-8208 protocol
   - AF_INET6: IPv6 Internet protocols
   - AF_DECnet: DECet protocol sockets
   - AF_KEY: Key  management protocol, originally developed for usage with IPsec
   - AF_NETLINK: Kernel user interface device
   - AF_PACKET: Low-level packet interface
   - AF_RDS: Reliable Datagram Sockets (RDS) protocol
   - AF_PPPOX: Generic PPP transport layer, for setting up L2 tunnels (L2TP and PPPoE)
   - AF_LLC: Logical  link  control  (IEEE 802.2 LLC) protocol
   - AF_IB: InfiniBand native addressing
   - AF_MPLS: Multiprotocol Label Switching
   - AF_CAN: Controller Area Network  automotive  bus protocol
   - AF_TIPC: TIPC, "cluster domain sockets" protocol
   - AF_BLUETOOTH: Bluetooth low-level socket protocol
   - AF_ALG: Interface to kernel crypto API
   - AF_VSOCK: VSOCK   (originally  "VMWare  VSockets") protocol for hypervisor-guest communication
   - AF_KCM: KCM  (kernel connection multiplexer) interface
   - AF_XDP: XDP (express data path) interface

2. 参数type确定套接字的类型，进一步确定通信特征。
   - SOCK_STREAM: Provides sequenced, reliable, two-way, connection-based byte streams. An out-of-band data transmission mechanism may be supported.
   - SOCK_DGRAM: Supports datagrams (connectionless, unreliable messages of a fixed maximum length).
   - SOCK_SEQPACKET: Provides a sequenced, reliable, two-way connectionbased data transmission path for datagrams of fixed maximum length; a consumer is required to read an entire packet with each input system call.
   - SOCK_RAW: Provides raw network protocol access.
   - SOCK_RDM: Provides a reliable datagram layer that does not guarantee ordering.
   - SOCK_PACKET: Obsolete and should not be used in new programs;
   - SOCK_NONBLOCK: Set the O_NONBLOCK file status flag on the open file description (see  open(2)) referred to by the new file descriptor.  Using this flag saves extra calls  to  fcntl(2) to achieve the same result.
   - SOCK_CLOEXEC: Set the close-on-exec (FD_CLOEXEC) flag on the new file descriptor.  See the description of the O_CLOEXEC  flag in open(2) for reasons why this may be useful.

3. 参数protocal通常是0,表示为给定的域和套接字类型选择默认协议。

### 采用shutdown函数来禁止一个套接字的I/O

```c
#include <sys/socket.h>

int shutdown(int sockfd, int how);

// 返回值：若成功，返回0；若出错，返回-1；
```

1. 参数how
   - SHUT_RD，关闭读端，无法从套接字读取数据。
   - SHUT_WR，关闭写端，无法使用套接字发送数据。
   - SHUT_RDWR，即无法读取数据，也无法发送数据。

## 寻址

进程标识由两部分组成：一部分是计算机的网络地址，它可以帮助标识网络上我们想通信的计算机；另一部分是该计算机上用端口号表示的服务，它可以帮助标识特定的进程。

### 字节序

字节序是一个处理器架构特性，用于指示像整数这样的大数据类型内部的字节如何排序。

如果处理器架构支持大端字节序，那么最大字节地址出现在最低有效字节上。

小端字节序则相反，最低有效字节包含最小字节地址。

不管字节如何排序，最高有效字节总是在左边，最低有效字节总是在右边。

 网络协议指定了字节序。

TCP/IP协议使用大端字节序。

对于TCP/IP应用程序，有4个用来在处理器字节和网络字节序之间实施转换的函数。

```c
#include <arpa/inet.h>

uint32_t htonl(uint32_t hostlong); // 返回值：以网络字节序表示的32位整数

uint16_t htons(uint16_t hostshort); // 返回值：以网络字节序表示的16位整数

uint32_t ntohl(uint32_t netlong); // 返回值：以处理器字节序表示的32位整数

uint16_t ntohs(uint16_t netshort); // 返回值：以处理器字节序表示的16位整数

```

### 地址格式

一个地址标识一个特定通信域的套接字端点，地址格式与这个特定的通信域相关。

为使不同格式地址能传入到套接字函数，地址会被强制转换成一个通用的地址结构sockaddr。

1. 通用的地址结构 sockaddr
```c
// bits/sockaddr.h
#define	__SOCKADDR_COMMON(sa_prefix) \
  sa_family_t sa_prefix##family

// sys/socket.h
struct sockaddr
{
    __SOCKADDR_COMMON (sa_);	/* Common data: address family and length.  */
    char sa_data[14];		/* Address data.  */
};
```

因特网地址定义在`<netinet/in.h>`头文件中。

2. 在IPv4因特网域（AF_INET）中，套接字地址用结构sockaddr_in表示。

```c
// netinet/in.h
struct sockaddr_in
{
   __SOCKADDR_COMMON (sin_);
   in_port_t sin_port;			/* Port number.  */
   struct in_addr sin_addr;		/* Internet address.  */

   /* Pad to size of `struct sockaddr'.  */
   unsigned char sin_zero[sizeof (struct sockaddr)
         - __SOCKADDR_COMMON_SIZE
         - sizeof (in_port_t)
         - sizeof (struct in_addr)];
};
```

3. IPv4因特网域（AF_INET6）套接字地址用结构sockaddr_in6表示。

```c
// netinet/in.h
struct sockaddr_in6
{
   __SOCKADDR_COMMON (sin6_);
   in_port_t sin6_port;	/* Transport layer port # */
   uint32_t sin6_flowinfo;	/* IPv6 flow information */
   struct in6_addr sin6_addr;	/* IPv6 address */
   uint32_t sin6_scope_id;	/* IPv6 scope-id */
};
```

4. 二进制地址格式与点分十进制字符表示之间的相互转换。

```c
// arpa/inet.h
// 将网络字节序的二进制地址转换成文本字符串格式。
extern const char *inet_ntop (int __af, const void *__restrict __cp,
			      char *__restrict __buf, socklen_t __len)
     __THROW;
// 返回值：若成功，返回地址字符串指针；若出错，返回NULL
// len 指定了保存文本字符串的缓冲区的大小
// netinet/in.h
#define INET_ADDRSTRLEN 16
#define INET6_ADDRSTRLEN 46

// 将文本字符串格式转换成网络字节序的二进制地址。
extern int inet_pton (int __af, const char *__restrict __cp,
		      void *__restrict __buf) __THROW;
// 返回值：若成功，返回1；若格式无效，返回0；若出错，返回-1；
// __af是AF_INET时，则缓冲区__buf需要足够大的空间来存放一个32位地址
// __af是AF_INET6时，则缓冲区__buf需要足够大的空间来存放一个128位地址
```

### 地址查询

