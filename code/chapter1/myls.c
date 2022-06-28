#include "../include/apue.h"
#include <dirent.h> /* opendir readdir dirent */

int main(int argc, char *argv[])
{
    DIR *dp;
    struct dirent *dirp;

    if (argc != 2) {
        err_quit("usage: ls directory name");
    }
    /* opendir 返回指向 DIR 的指针 */
    if ((dp = opendir(argv[1])) == NULL) {
        err_sys("can't open %s", argv[1]);
    }
    /* readdir 返回指向 dirent 的指针 */
    while ((dirp = readdir(dp)) != NULL) {
        printf("%s\n", dirp->d_name);
    }

    closedir(dp);
    exit(0);
}