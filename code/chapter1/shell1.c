#include "../include/apue.h"
#include <sys/wait.h>

int main(void)
{
    char buf[MAXLINE];
    pid_t pid;
    int status;

    printf("%% ");  /* 打印'%'需要一个'%'做转义 */
    while (fgets(buf, MAXLINE, stdin) != NULL) {
        if (buf[strlen(buf) - 1] == '\n') {
            buf[strlen(buf) - 1] = 0;
        }
        if ((pid = fork()) < 0) { /* fork 失败会返回负值 */
            err_sys("fork error");
        } else if (pid == 0) { /* fork 被调用一次会返回两次，pid == 0 表示子进程 */
            execlp(buf, buf, (char *)0);
            err_ret("couldn't execute: %s", buf);        
            exit(127);
        }
        /* 父进程，调用 waitpid 等待子进程终止 */
        if ((pid = waitpid(pid, &status, 0)) < 0) {
            err_sys("waitpid error");
        }
        printf("%% ");
    }
    exit(0);
}