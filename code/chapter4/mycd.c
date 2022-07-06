/**
 * @file mycd.c
 * @author your name (you@domain.com)
 * @brief chdir 函数示例，这并不会修改 shell 的当前目录
 * @version 0.1
 * @date 2022-07-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "../include/apue.h"

int main(void)
{
    if (chdir("/tmp") < 0) {
        err_sys("chdir failed");
    }
    printf("chdir to /tmp succeeded\n");
    exit(0);
}