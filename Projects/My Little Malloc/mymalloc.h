#include <stddef.h>

#ifndef _MYMALLOC_H
#define _MYMALLOC_H

#define malloc(s) mymalloc(s, __FILE__, __LINE__)
#define free(p) myfree(p, __FILE__, __LINE__)

void *mymalloc(size_t size, char *file, int line);
void myfree(void *ptr, char *file, int line);

typedef struct header {
    bool isFree;
    int size;
    struct header *next;
} header;

#define MEMORY_LENGTH 512
#endif
