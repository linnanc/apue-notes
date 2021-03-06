/**
 * @file ftw8.c
 * @author your name (you@domain.com)
 * @brief 递归降序遍历目录层次结构，并按文件类型计数，功能类似于 ftw，但是不跟随符号链接
 * @version 0.1
 * @date 2022-07-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "../include/apue.h"
#include <dirent.h>
#include <limits.h>

typedef int Myfunc(const char *, const struct stat *, int);

static Myfunc myfunc;
static int myftw(char *, Myfunc *);
static int dopath(Myfunc *);

static long nreg, ndir, nblk, nchr, nfifo, nslink, nsock, ntot;


int main(int argc, char *argv[])
{
    int ret;

    if (argc != 2) {
        err_quit("usage: ftw <starting-pathname>");
    }

    ret = myftw(argv[1], myfunc);

   	ntot = nreg + ndir + nblk + nchr + nfifo + nslink + nsock;
    if (ntot == 0)
        ntot = 1; /* avoid divide by 0; print 0 for all counts */
    printf("regular files  = %7ld, %5.2f %%\n", nreg,
        nreg*100.0/ntot);
    printf("directories    = %7ld, %5.2f %%\n", ndir,
        ndir*100.0/ntot);
    printf("block special  = %7ld, %5.2f %%\n", nblk,
        nblk*100.0/ntot);
    printf("char special   = %7ld, %5.2f %%\n", nchr,
        nchr*100.0/ntot);
    printf("FIFOs          = %7ld, %5.2f %%\n", nfifo,
        nfifo*100.0/ntot);
    printf("symbolic links = %7ld, %5.2f %%\n", nslink,
        nslink*100.0/ntot);
    printf("sockets        = %7ld, %5.2f %%\n", nsock,
        nsock*100.0/ntot);
    exit(ret);
}

/**
 * 从 pathname 开始，层次遍历的每个文件都会调用 func()
 */
#define FTW_F 1 /* 目录以外的文件 */
#define FTW_D 2 /* 目录 */
#define FTW_DNR 3 /* 不能读的目录 */
#define FTW_NS 4 /* 不能 stat 的目录 */

static char *fullpath; /* 包含每个文件的完整路径名 */
static size_t pathlen;

/**
 * @brief 
 * 
 * @param pathname 
 * @param func 
 * @return int 返回 func() 返回的内容
 */
static int myftw(char *pathname, Myfunc *func)
{
    fullpath = path_alloc(&pathlen); /* malloc PATH_MAX+1 bytes */

    if (pathlen <= strlen(pathname)) {
        pathlen = strlen(pathname) * 2;
        if ((fullpath = realloc(fullpath, pathlen)) == NULL) {
            err_sys("realloc failed");
        }
    }

    strcpy(fullpath, pathname);
    return(dopath(func));
}

static int dopath(Myfunc *func)
{
    struct stat statbuf;
    struct dirent *dirp;
    DIR *dp;
    int ret, n;

    if (lstat(fullpath, &statbuf) < 0) { /* stat 错误 */
        return (func(fullpath, &statbuf, FTW_NS));
    }
    if (S_ISDIR(statbuf.st_mode) == 0) { /* 非目录 */
        return (func(fullpath, &statbuf, FTW_F));
    }

    /**
     * 对于目录，先调用 func()，然后处理目录的每个文件
     */
    if ((ret = func(fullpath, &statbuf, FTW_D)) != 0) {
        return (ret);
    }

    n = strlen(fullpath);
    if (n + NAME_MAX + 2 > pathlen) {
        pathlen *= 2;
        if ((fullpath = realloc(fullpath, pathlen)) == NULL) {
            err_sys("realloc failed");
        }
    }
    fullpath[n++] = '/';
    fullpath[n] = 0;

    if ((dp = opendir(fullpath)) == NULL) { /* 读目录失败 */
        return(func(fullpath, &statbuf, FTW_DNR));
    }

    while ((dirp = readdir(dp)) != NULL) {
        if (strcmp(dirp->d_name, ".") == 0  ||
            strcmp(dirp->d_name, "..") == 0) {
                continue; /* 忽略 . 和 ..*/
            }
        strcpy(&fullpath[n], dirp->d_name);	/* append name after "/" */
        if ((ret = dopath(func)) != 0) { /* recursive */
            break; 
        }
    }
    fullpath[n-1] = 0;	/* erase everything from slash onward */

    if (closedir(dp) < 0) {
        err_ret("can't close directory %s", fullpath);
    }
	return(ret);
}

static int myfunc(const char *pathname, const struct stat *statptr, int type)
{
    switch (type) {
    case FTW_F:
        switch (statptr->st_mode & S_IFMT) {
        case S_IFREG: nreg++; break;
        case S_IFBLK: nblk++; break;
        case S_IFCHR: nchr++; break;
        case S_IFIFO: nfifo++; break;
        case S_IFLNK: nslink++; break;
        case S_IFSOCK: nsock++; break;
        case S_IFDIR:
            err_dump("for S_IFDIR for %s", pathname);
        }
        break;
    case FTW_D:
        ndir++;
        break;
    case FTW_DNR:
        err_ret("can't read directory %s\n", pathname);
        break;
    case FTW_NS:
        err_ret("stat error for %s", pathname);
        break;
    default:
        err_dump("unknown type %d for pathname %s", type, pathname);
    }

    return 0;
}