## 第3章 文件`I/O`

### 3.2 文件描述符

对内核而言所有打开的文件都是通过文件描述符引用的。文件描述符的范围是`0 ~ OPEN_MAX - 1`

### 3.3 函数`open`和`openat`

```c
#include <fcntl.h>
int open(const char *pathname, int flags);
int open(const char *pathname, int flags, mode_t mode);
int creat(const char *pathname, mode_t mode);
int openat(int dirfd, const char *pathname, int flags);
int openat(int dirfd, const char *pathname, int flags, mode_t mode);
```

`open()`用于打开或创建一个文件，若成功，会返回一个文件描述符（这个文件描述符是当前未用描述符的最小值），失败则返回-1。

参数`flags`可用来说明此函数的多个选项，可以是下列的一个或多个常量进行或运算。

* `O_RDONLY` 只读打开

* `O_WRONLY` 只写打开

* `O_RDWR` 读、写打开

* `O_EXEC` 只执行打开

* `O_SEARCH` 只搜索打开，用于目录。（不支持）

上面的常量必须指定且只能指定一个，下面的常量则是可选的。

* `O_APPEND` 每次写时追加到文件的尾端。

* `O_CLOEXEC` 把`FD_CLOEXEC`常量设置为文件描述符标志。

* `O_CREAT` 若此文件不存在则创建它。使用此选项时，`open`函数需同时说明第3个参数`mode`，用`mode`指定新文件的访问权限位。

* `O_DIRECTORY` 如果`pathname`不是目录，则出错。

* `O_EXCL` 如果同时制定了`O_CREAT`，若文件以及存在，则出错。可以用它来测试文件是否存在，如果不存在则创建，测试和创建是一个原子操作。

* `O_NOCTTY` 如果`pathname`引用的是终端设备（比如`/dev/tty`），则不将该设备分配作为进程的控制终端。

* `O_NOFOLLOW` 如果`pathname`引用的是一个符号链接，则出错。
* `O_NONBLOCK` 如果`pathname`引用的是一个`FIFO`，一个块特殊文件或一个字符特殊文件，则此选项为文件的本次打开操作和后续`I/O`操作设置非阻塞模式。
* `O_SYNC` 使每次`write`等待物理`I/O`操作完成，包括由该`write`操作引起的文件属性更新所需的`I/O`。
* `O_TRUNC` 如果此文件存在，而且为只写或读-写成功打开，则将其长度截断为0。
* `O_TTY_INIT` 如果打开一个还未打开的终端设备，设置非标准`termios`参数值，使其符合`Single UNIX Specifications`。
* `O_DSYNC` 使每次`write`要等待物理`I/O`操作完成，但是如果该读写操作并不影响读取刚写入的数据，则不需要等待文件属性被更新。
* `O_RSYNC` 使每一个以文件描述符作为参数进行的`read`操作等待，直至所有对文件同一部分挂起的写操作都完成。

#### `open`和`openat`的区别

3点区别：

1. 当`pathname`是绝对路径名时，`fd`参数被忽略，`openat`等价于`open`。

2. 当`pathname`是相对路径名时，`fd`参数指出了相对路径名在文件系统中的开始地址，`fd`通过打开相对路径名所在的目录来获取。

3. 当`pathname`是相对路径名，`fd`参数具有特殊值`AT_FDCWD`，这种情况下，路径名在当前工作目录中获取，`openat`函数在操作上与`open`类似。

看起来`openat`能实现的，`open`都能实现，为什么还要`openat`呢？

1. 引入`openat`是为了方便一个进程内的各线程拥有不同的当前目录，`chdir`会影响整个进程，而`openat`则可以为每个线程设置一个当前目录，比如`cat`就使用到了`openat`。
2. 可以避免`time-of-check-to-time-of-use (TOCTTOU)`错误。

#### 文件名和路径截断

文件名最大长度为255。

### 3.4 函数`create`

早期的`UNIX`版本，`open`不支持创建文件，所以需要先调用`create`，现在已经不需要了。

### 3.5 函数`close`

```C
#include <unistd.h>
int close(int fd);
```

关闭一个文件时还会释放加在该文件上的所有记录锁。当一个进程终止时，内核会自动关闭打开的文件，不需要显式调用`close`。

### 3.6 函数`lseek`

`lseek`可以修改文件的偏移量。

```C
#include <sys/types.h>
#include <unistd.h>

off_t lseek(int fd, off_t offset, int whence);
```

参数`offset`和参数`whence`有关：

* 若`whence`是`SEEK_SET`，则将文件的偏移量设置为距文件开始处`offset`个字节。
* 若`whence`是`SEEK_CUR`，则将文件的偏移量设置为当前值加`offset`个字节，`offset`可正可负。
* 若`whence`是`SEEK_END`，则将文件的偏移量设置为文件长度加`offset`，`offset`可正可负。

若`lseek`执行成功，则返回新的文件偏移量。如果要获取文件的当前偏移量可以通过下面的方法：

```c
lseek(fd, 0, SEEK_CUR)
```

如果`fd`指向一个管道，`FIFO`或网络套接字，则`lseek`返回-1。用下面的代码，测试`seek`

```C
#include "../include/apue.h"

int main(void)
{
    if (lseek(STDIN_FILENO, 0, SEEK_CUR) == -1) {
        printf("cannot seek\n");
    } else {
        printf("seek OK\n");
    }
    exit(0);
}
```

`seek`一个文件：

```shell
$ ./a.out < /etc/passwd
seek OK
```

`seek`一个管道：

```shell
$ cat < /etc/passwd | ./a.out
cannot seek
```

注意：某些设备允许负的偏移量，所以在比较`lseek`的返回值时应当谨慎，不要测试它是否小于0，而要测试它是否等于-1。

文件的偏移量可以大于文件的当前长度，在偏移量大于文件长度上写是允许的，会形成一个文件空洞。

例子：

```c
#include "../include/apue.h"
#include <fcntl.h>

char buf1[] = "abcdefghij";
char buf2[] = "ABCDEFGHIJ";

int main(void)
{
    int fd;
    if ((fd = creat("file.hole", FILE_MODE)) < 0) {
        err_sys("creat error");
    }

    if (write(fd, buf1, 10) != 10) {
        err_sys("buf1 write error");
    }

    if (lseek(fd, 16384, SEEK_SET) == -1) {
        err_sys("lseek error");
    }

    if (write(fd, buf2, 10) != 10) {
        err_sys("buf2 write error");
    }

    exit(0);
}
```

编译运行，生成`file.hole`文件，查看：

```SHELL
$ od -c file.hole
0000000   a   b   c   d   e   f   g   h   i   j  \0  \0  \0  \0  \0  \0
0000020  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0
*
0040000   A   B   C   D   E   F   G   H   I   J
0040012

```

### 3.7 函数`read`

调用`read`函数从打开的文件中读取数据。

```c
 #include <unistd.h>

ssize_t read(int fd, void *buf, size_t count);
```

`read`成功则返回读到的字节数，当到达文件的尾端时，返回0。

下面的情况会导致实际读到的字节数少于要求读的字节数。

* 读普通文件时，在读到要求字节数之前已经到达了文件尾端。
* 当从终端设备读时，通常一次最多读一行。
* 当从网络读时，网络中的缓冲机制可能造成返回值小于所要求读的字节数。
* 当从管道或`FIFO`读时，如若管道包含的字节少于所需的数量，那么`read`将只返回实际可用的字节数。
* 当一信号造成中断，而已经读了部分数据量时。

### 3.8 函数`write`

调用`write`函数向打开的文件写数据。

```c
#include <unistd.h>

ssize_t write(int fd, const void *buf, size_t count);
```

`write`返回实际写入的字节数，其返回值通常与`count`相同。可以在`open`文件时指定`O_APPEND`追加写入。

### 3.9 `I/O`的效率

* 预读：（read ahead）大多数文件系统为改善系统性能会采用预读技术，当检测到正进行顺序读写时，系统就试图读入比应用所要求的更多数据。

### 3.10 文件共享

内核使用3种数据结构表示打开的文件：

1. 每个进程在进程表中有一个记录项，记录项中包含一张打开文件描述符表，可将其视为一个矢量，每个描述符占用一项。与每个文件描述符相关联的是：
   1. 文件描述符标志（`close_on_exec`）
   2. 指向一个文件表项的指针
2. 内核为所有打开文件维持一张文件表。每个文件表项包含：
   1. 文件状态标志（读、写、添写、同步和非阻塞等）
   2. 当前文件偏移量
   3. 指向该文件`v`节点表项的指针
3. 每个打开文件都有一个`v-node`结构。`v-node`包含了文件类型和对此文件进行各种操作函数的指针。`v-node`是只有一份的。

### 3.11 原子操作

`pread`和`pwrite`相当于调用`lseek`后再调用`read`和`write`。但是它们有如下区别：

* 调用`pread`时，无法中断其定位和读操作
* 不更新当前文件偏移量

### 3.12 函数`dup`和函数`dup2`

调用`dup`和`dup2`可以复制一个现有的文件描述符。

```c
#include <unistd.h>

int dup(int oldfd);
int dup2(int oldfd, int newfd);
```

由`dup`返回的新文件描述符是当前可用文件描述符中的最小值。`dup2`可以指定一个新的文件描述符。

### 3.13 函数`sync`、`fsync`和`fdatasync`

当我们向文件写数据时，内核通常将数据复制到缓冲区，然后排入队列，晚些时候再写入磁盘。这就是延迟写。调用`sync`，`fsync`，和`fdatasync`会将缓冲区的数据排入写队列，然后返回，它并不等实际写磁盘操作结束。`fsync`会等待磁盘操作结束再返回。

思考：`Linux`的`sync`命令会等待磁盘操作结束再返回吗？

`Linux`中的`sync`函数和`fsync`一样，都会等待磁盘操作结束后再返回的。见https://man7.org/linux/man-pages/man2/sync.2.html

### 3.14 函数`fcntl`

`fcntl`可以改变已经打开文件的属性。

```C
#include <unistd.h>
#include <fcntl.h>

int fcntl(int fd, int cmd, ... /* arg */ );
```

`fcntl`有下面5种功能：

1. 复制一个已有的描述符（`cmd = F_DUPFD`或`F_DUPFD_CLOEXEC`）
2. 获取/设置文件描述符标志（`cmd = F_GETFD` 或 `F_SETFD`)
3. 获取/设置文件状态标志（`cmd = F_GETFL`或`F_SETFL`）
4. 获取/设置异步`I/O`所有权（`cmd = F_GETOWN` 或 `F_SETOWN`）
5. 获取/设置记录锁（`cmd = FGETLK`、`F_SETLK`或`F_SETLKW`）

要注意的是修改文件描述符标志或文件状态标志时，要先获得当前的标志值，然后按期望修改它。不要只执行`F_SETFD`或`F_SETFL`，这样会关闭以前设置的标志位。

### 3.15 函数`ioctl`

`ioctl`是`I/O`操作的杂物箱。驱动程序经常会看到这个函数。

### 3.16 `/dev/fd`

打开`/dev/fd/n`等效于复制描述符`n`。

