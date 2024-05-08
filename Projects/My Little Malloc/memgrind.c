#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "mymalloc.h"

int isPrime(int n) {
    int primes[30] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113};
    for (int i = 0; i < 30; i++) {
        if (n == primes[i]) return 1;
    }
    return 0;
}

/*
 * 
 * MEMGRIND: Stress Testing malloc and free
 * 
 */
int main(int argc, char **argv)
{
    double time_spent[5][50] = {{0.0}};
    int count = 0;

    while (count != 50)
    {

        /*
        * 
        * TEST 1:
        *  - malloc and free 1 byte of data 120 times
        * 
        */
        clock_t begin = clock();
        for (int i = 0; i < 120; i++)
        {
            char *p = malloc(1);
            free(p);
        }

        clock_t end = clock();

        time_spent[0][count] += (double)(end - begin) / CLOCKS_PER_SEC;

        /*
        * 
        * TEST 2:
        *  - malloc 120 characters
        *  - free 120 characters
        * 
        */
        begin = clock();
        char *q[120];
        for (int i = 0; i < 120; i++)
        {
            q[i] = malloc(1);
        }

        for (int i = 0; i < 120; i++)
        {
            free(q[i]);
        }

        end = clock();

        time_spent[1][count] += (double)(end - begin) / CLOCKS_PER_SEC;

        /*
        * 
        * TEST 3:
        *  - malloc and free varying sizes of data from 1 to 120. 
        * 
        */
        begin = clock();
        for(int i = 1; i < 120; i++) {
            int *p = malloc(i);
            free(p);
        }
        end = clock();
        time_spent[2][count] += (double)(end - begin) / CLOCKS_PER_SEC;

        /*
        * 
        * TEST 4:
        *  - malloc but based on if the current index is even or odd
        *  - sizes will be dependent on parity as well
        * 
        */
        begin = clock();
        char *task4[120];

        for (int i = 0; i < 120; i++)
        {
            if (i % 2 == 0) 
                task4[i] = malloc(1);
            else
                task4[i] = malloc(2);
        }

        for (int i = 0; i < 120; i++) 
        {
            free(task4[i]);
        }
        end = clock();
        time_spent[3][count] += (double)(end - begin) / CLOCKS_PER_SEC;

        /*
        * 
        * TEST 5:
        *  - malloc 120 objects
        *  - free 60 of those
        *  - malloc another 60
        *  - free all 120 objects
        * 
        */
        begin = clock();
        char *task5[120];
        for (int i = 0; i < 120; i++)
        {
            task5[i] = malloc(1);
        }
        for (int i = 0; i < 60; i++)
        {
            free(task5[i]); 
        }
        for (int i = 0; i < 60; i++)
        {
            task5[i] = malloc(5);
        }
        for (int i = 0; i < 120; i++)
        {
            free(task5[i]);
        }

        end = clock();
        time_spent[4][count] += (double)(end - begin) / CLOCKS_PER_SEC;
        count++;
    }

    double sumTaskTime[5] = {0.0};
    double averageTaskTime[5];
    for (int i = 0; i < 5; i++)
    {
        for (int j = 1; j < 50; j++) { sumTaskTime[i] += time_spent[i][j]; }
        averageTaskTime[i] = sumTaskTime[i] / 50;
    }

    printf("\nAVERAGE TIME ON TEST 1: %-12.10f", averageTaskTime[0]);
    printf("\nAVERAGE TIME ON TEST 2: %-12.10f", averageTaskTime[1]);
    printf("\nAVERAGE TIME ON TEST 3: %-12.10f", averageTaskTime[2]);
    printf("\nAVERAGE TIME ON TEST 4: %-12.10f", averageTaskTime[3]);
    printf("\nAVERAGE TIME ON TEST 5: %-12.10f\n\n", averageTaskTime[4]);

    return EXIT_SUCCESS;
}