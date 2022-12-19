# 文件输入、输出

## 文件描述符

文件描述符是一个非负整数。

当打开一个现有文件或创建一个新文件时，内核向进程返回一个文件描述符。

当读、写一个文件时，使用open或create返回的文件描述符标识该文件，将其作为参数传送给read或write。

UNXI系统shell把文件描述符0与进程的标准输入关联，文件描述符1与标准输出关联，文件描述符2与标准错误关联。

```c
// unistd.h
/* Standard file descriptors.  */
#define	STDIN_FILENO	0	/* Standard input.  */
#define	STDOUT_FILENO	1	/* Standard output.  */
#define	STDERR_FILENO	2	/* Standard error output.  */
```
```sh
系统最大打开文件描述符数： /proc/sys/fs/file-max
$ cat /proc/sys/fs/file-max #查看
进程最多打开文件描述符数：user limit中的nofile的soft limit
$ ulimit -n #查看
```

## 操作

1. 打开文件
2. 创建文件
3. 关闭文件
4. 设置文件偏移量
5. 读文件
6. 写文件
7. 复制文件描述符
8. 同步写
9. 操作文件描述符
10. 控制驱动

## 文件共享
## 原子操作