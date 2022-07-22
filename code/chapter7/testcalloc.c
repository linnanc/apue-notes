#include "../include/apue.h"

int main()
{
    long *array = calloc(32, sizeof(long));
    int i = 0;
    for (i = 0; i < 32; i++) {
        printf("array[%d] = %d\n", i, array[i]);
    }

    int **array2 = calloc(32, sizeof(int **));

    for (i = 0; i < 32; i++) {
        array2[i] = calloc(32, sizeof(int *));
        int j = 0;
        for (j = 0; j < 32; j++) {
            printf("%p\n", array2[i][j]);
        }
        
    }
    return 0;
}