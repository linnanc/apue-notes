/**
 * @file echoarg.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-07-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "../include/apue.h"

int main(int argc, char **argv)
{
    int i;
    for (i = 0; i < argc; i++) {
        printf("argv[%d]: %s\n", i, argv[i]);
    }
    exit(0);
}