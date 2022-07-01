/**
 * @file testempty.c
 * @author your name (you@domain.com)
 * @brief 文件空洞测试
 * @version 0.1
 * @date 2022-06-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "../include/apue.h"
#include <fcntl.h>

int main(void)
{
    const char *pathname = "test_file.txt";
    int fd = open(pathname, O_WRONLY);
    if (lseek(fd, 24, SEEK_CUR) == -1) {
        printf("cannot seek\n");
    } else {
        write(fd, "b", 1);
    }
    exit(0);
}