#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "mymalloc.h"

#define MEMSIZE 4096
#define HEADERSIZE 8
#define OBJECTS 64
#define OBJSIZE (MEMSIZE / OBJECTS - HEADERSIZE)

int main() {

	/* 
	 * 
	 * TEST 1:
	 *  - MALLOC entire array
	 *  - FREE entire array
	 * 
	 */
	printf("\nTEST 1: BEGIN      [     ]");
	int *arr1 = malloc(4080);
	free(arr1);
	printf("\nTEST 1: SUCCESSFUL [-    ]");

	/* 
	 * 
	 * TEST 2:
	 *  - MALLOC 512 objects of size 1 sequentially
	 *  - FREE each object sequentially
	 * 
	 */
	printf("\nTEST 2: BEGIN      [-    ]");
	int *arr2[256];
	for (int i = 0; i < 256; i++)
	{
		arr2[i] = malloc(2);
	}	
	for (int i = 0; i < 256; i++)
	{
		free(arr2[i]);
	}
	printf("\nTEST 2: SUCCESSFUL [--   ]");

	/* 
	 * 
	 * TEST 3:
	 *  - MALLOC 256 objects of size 2 sequentially
	 *  - FREE each object sequentially
	 * 
	 */
	printf("\nTEST 3: BEGIN      [--   ]");
	for (int i = 0; i < 256; i++) {
		arr2[i] = malloc(2);
	}
	for (int i = 0; i < 256; i++) {
		free(arr2[i]);
	}
	printf("\nTEST 3: SUCCESSFUL [---  ]");


	/* 
	 * 
	 * TEST 4:
	 *  - fill memory with objects
	 *  - fill each object with distinct bytes
	 *  - check that all objects contain the correct bytes
	 *  - return how many are incorrect
	 * 
	 */
	printf("\nTEST 4: BEGIN      [---  ]");
	char *obj[OBJECTS];
	int i, j, errors = 0;
	for (i = 0; i < OBJECTS; i++) {
		obj[i] = malloc(OBJSIZE);
	}
	for (i = 0; i < OBJECTS; i++) {
		memset(obj[i], i, OBJSIZE);
	}
	for (i = 0; i < OBJECTS; i++) {
		for (j = 0; j < OBJSIZE; j++) {
			if (obj[i][j] != i) {
				errors++;
				printf("Object %d byte %d incorrect: %d\n", i, j, obj[i][j]);
			}
		}
	}
	for (i = 0; i < OBJECTS; i++) {
		free(obj[i]);
	}
	if (errors == 0) {
		printf("\nTEST 4: SUCCESSFUL [---- ]");
	}

	/* 
	 * 
	 * TEST 5:
	 *  - fill memory with one big object and one small object then free both
	 *  - allocate the entire memory then free
	 *  - allocate the entire memory again then free
	 */
	printf("\nTEST 5: BEGIN      [---- ]");
	int *arr3 = malloc(4072);
	int *test = malloc(1);
	free(arr3);
	free(test);
	int *arr4 = malloc(4080);
	free(arr4);
	int *arr5 = malloc(4088);
	free (arr5);
	printf("\nTEST 5: SUCCESSFUL [-----]\n");

	printf("\nALL TESTS PASSED !\n\n");

	return 0;
}