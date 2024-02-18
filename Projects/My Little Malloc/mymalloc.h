#include <stddef.h>
#include <stdbool.h>

#ifndef _MYMALLOC_H
#define _MYMALLOC_H
#define malloc(s) mymalloc(s, __FILE__, __LINE__)
#define free(p) myfree(p, __FILE__, __LINE__)
#define MEMORY_LENGTH 512

void *mymalloc(size_t size, char *file, int line);
void myfree(void *ptr, char *file, int line);

typedef struct header
{
    bool free;
    int size;
    struct header *next;
} header;
#endif
