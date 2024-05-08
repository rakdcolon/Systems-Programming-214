/*
 *
 * INCLUDE STATEMENTS
 *
 */
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

/*
 *
 * DEFINITIONS
 *
 */
#define _MYMALLOC_H
#define malloc(s) mymalloc(s, __FILE__, __LINE__)
#define free(p) myfree(p, __FILE__, __LINE__)
#define NEXT_MULTIPLE_OF_EIGHT (size + 7) & ~7;
#define SET_HEAD_SIZE(s) *((int *)head) = s + 8
#define MEMORY_LENGTH (512)
#define MEMORY_LENGTH_IN_BYTES (512 * 8)
#define START_OF_MEMORY (int *)memory
#define HEAD_SIZE *((int *)head)
#define SET_HEAD_ALLOCATED *(((int *)head) + 1) = 1
#define IS_HEAD_FREE (*(((int *)head) + 1) == 0)
#define IS_HEAD_UNINITIALIZED (HEAD_SIZE == 0 && IS_HEAD_FREE)
#define HEAD_NEXT ((int *)head) + (*(int *)head) / 4
#define HEAD_NEXT_SIZE *(int *)(HEAD_NEXT)
#define HEAD_NEXT_IS_FREE *((int *)(HEAD_NEXT) + 1) = 0
#define IS_CURR_FREE (*(curr + 1) == 0)
#define IS_PREV_FREE (*(prev + 1) == 0)
#define IS_NEXT_BLOCK_FREE ((curr + *curr / 4) < END_OF_MEMORY && *((curr + *curr / 4) + 1) == 0)
#define MALLOC_POINTER_FOUND (void *)(curr + 2) == ptr
#define SET_CURR_FREE *(curr + 1) = 0;

/*
 *
 * METHOD HEADERS
 *
 */
void *mymalloc(size_t size, char *file, int line);
void myfree(void *ptr, char *file, int line);

/*
 *
 * MEMORY ARRAY
 *
 */
static double memory[MEMORY_LENGTH];

/*
 *
 * METADATA STRUCT (OLD IMPLEMENTATION)
 *
 */
typedef struct header {
    int free;
    int size;
    struct header *next;
} header;

