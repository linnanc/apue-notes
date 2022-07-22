/**
 * @file testjmp.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-07-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "../include/apue.h"
#include <setjmp.h>

static void	f1(int, int, int, int);
static void	f2(void);

static jmp_buf	jmpbuffer;
static int		globval;

int
main(void)
{
	int				autoval;
	register int	regival; /* 被修饰的变量存放在CPU的寄存器中供程序进行读写 */
	volatile int	volaval;
	static int		statval;

	globval = 1; autoval = 2; regival = 3; volaval = 4; statval = 5;

	if (setjmp(jmpbuffer) != 0) {
		printf("after longjmp:\n");
		printf("globval = %d, autoval = %d, regival = %d,"
		    " volaval = %d, statval = %d\n",
		    globval, autoval, regival, volaval, statval);
		exit(0);
	}

	/*
	 * Change variables after setjmp, but before longjmp.
	 */
	globval = 95; autoval = 96; regival = 97; volaval = 98;
	statval = 99;

	f1(autoval, regival, volaval, statval);	/* never returns */
	exit(0);
}

static void
f1(int i, int j, int k, int l)
{
	printf("in f1():\n");
	printf("globval = %d, autoval = %d, regival = %d,"
	    " volaval = %d, statval = %d\n", globval, i, j, k, l);
	f2();
}

static void
f2(void)
{
	longjmp(jmpbuffer, 1);
}