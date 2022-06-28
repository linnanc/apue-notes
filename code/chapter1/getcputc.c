#include "../include/apue.h"

int main(void)
{
    int c;
    while ((c = getc(stdin)) != EOF) {
        if (putc(c, stdout) == EOF) {
            err_sys("output error");
        }
    }
    /* 测试stdin的错误标识符 */
    if (ferror(stdin)) {
        err_sys("input error");
    }

    exit(0);
}