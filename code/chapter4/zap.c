/**
 * @file zap.c
 * @author your name (you@domain.com)
 * @brief futimens 函数实例
 * @version 0.1
 * @date 2022-07-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#define _GNU_SOURCE
#include "../include/apue.h"
#include <fcntl.h>

int main(int argc, char *argv[])
{
    int i, fd;
    struct stat statbuf;
    struct timespec times[2];
    for (i = 1; i < argc; i++) {
        if (stat(argv[i], &statbuf) < 0) {
            err_ret("%s: stat error", argv[1]);
            continue;
        }
        if ((fd = open(argv[i], O_RDWR | O_TRUNC)) < 0) {
            err_ret("%s: open error", argv[i]);
            continue;
        }
        times[0] = statbuf.st_atim; /* 最后一次访问的时间 access 需要定义 _GNU_SOURCE */
        times[1] = statbuf.st_mtim; /* 最后一次修改的时间 */
        if (futimens(fd, times) < 0) {
            err_ret("%s: futimens error", argv[i]);            
        }
        close(fd);
    }
    exit(0);
}