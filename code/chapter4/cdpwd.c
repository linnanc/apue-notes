/**
 * @file cdpwd.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-07-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "../include/apue.h"

int main(void)
{
    size_t size;
    char *ptr = path_alloc(&size);
    if (getcwd(ptr, size) == NULL) {
        err_sys("getcwd failed");
    }

    printf("cwd = %s\n", ptr);
    exit(0);
}