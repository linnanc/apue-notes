/**
 * @file doatexit.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-07-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "../include/apue.h"

static void my_exit1(void);
static void my_exit2(void);

int main(void)
{
    if (atexit(my_exit2) != 0) {
        err_sys("can't register my_exit2");
    }

    if (atexit(my_exit1) != 0) {
        err_sys("can't register my_exit1");
    }

    if (atexit(my_exit1) != 0) {
        err_sys("can't register my_exit1");
    }

    printf("main is done\n");

    return 0;
}

static void my_exit1(void)
{
    printf("first exit handler\n");
}

static void my_exit2(void)
{
    printf("second exit handler\n");
}