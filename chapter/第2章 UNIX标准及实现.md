## 第2章 `UNIX`标准及实现

### 2.2 `UNIX`标准化

* `ISO C`
* `IEEE POSIX`
* `Single UNIX Specification`
* `FIPS`

### 2.3 `UNIX`系统实现

* `SVR4`
* `4.4BSD` 加州大学伯克利分校出品
* `FreeBSD`
* `Linux`
* `Mac OS X` 苹果公司开发的操作系统
* `Solaris`

### 2.5 限制

由于`UNIX`是可移植的，系统定义的许多常亮和幻数在不同平台上不同。因此必须有以下两种类型的限制：

1. 编译时限制（例如，短整型的最大值是多少？）
2. 运行时限制（例如，文件名有多少个字符？）

编译时限制可以在头文件中定义，运行时限制则可以通过`sysconf`，`pathconf`，`fpathconf`三个函数以确定其运行时的值。

* `ISO C`限制
  * `ISO C`定义的所有编译时限制都在头文件`<limits.h>`中。

