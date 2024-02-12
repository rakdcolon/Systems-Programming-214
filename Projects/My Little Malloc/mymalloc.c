#include <stdlib.h>
#include <stdio.h>
#include "mymalloc.h"

#define MEMORY_LENGTH 512
static double memory[MEMORY_LENGTH];

char *memoryHead = (char *) memory;

void *mymalloc(size_t size, char *file, int line)
{
    header* curr = *memoryHead;
}
