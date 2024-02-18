#include <stdlib.h>
#include <stdio.h>
#include "mymalloc.h"

#define MEMORY_SIZE 512
#define HEADER_SIZE sizeof(struct header)
static double memory[MEMORY_SIZE];

void *firstChunk = (header *)memory;

void *mymalloc(size_t size, char *FILE, int line)
{
    header *head = firstChunk;

    if (head->next == NULL)
    {
        if (size >= MEMORY_SIZE - sizeof(struct header))
        {
            fprintf(stderr, "\nError in %s line %d: No space available \n", FILE, line);
            return NULL;
        }
        head->free = false;

        void *ptr = head;

        if (MEMORY_SIZE - size <= HEADER_SIZE)
        {
            head->next = head;
            head->size = MEMORY_SIZE - HEADER_SIZE;
        }
        else
        {
            struct header *temp = (header *)(ptr + HEADER_SIZE + size);
            temp->size = MEMORY_SIZE - size - 2 * HEADER_SIZE;
            temp->free = false;
            temp->next = head;

            head->next = temp;
            head->size = size;
        }
    }

    while (head != NULL && (!head->free || head->size < size))
    {
        if (head->next == firstChunk)
        {
            fprintf(stderr, "\nError in %s line %d: No space available \n", FILE, line);
            return NULL;
        }
        head = head->next;
    }

    head->free = false;

    if (head->size - size > HEADER_SIZE)
    {
        void *ptr = (void *)head;
        struct header *temp = (struct header *)(ptr + HEADER_SIZE + size);
        temp->size = head->size - size - HEADER_SIZE;
        temp->free = true;
        temp->next = head->next;

        head->next = temp;
        head->size = size;
    }

    return head + 1;
}

void myfree(void *ptr, char *FILE, int line)
{

    if (ptr == NULL || ptr >= firstChunk + MEMORY_SIZE)
    {
        fprintf(stderr, "\nError in %s line %d: Invalid pointer \n", FILE, line);
        return;
    }

    struct header *prev = (struct header *)(ptr - HEADER_SIZE);

    if (prev->free)
    {
        fprintf(stderr, "\nError in %s line %d: Invalid pointer \n", FILE, line);
        return;
    }
    prev->free = true;

    struct header *next = prev->next;

    if (next->free && next != firstChunk)
    {
        prev->size += HEADER_SIZE + next->size;
        prev->next = next->next;
        next = NULL;
    }

    header *curr = firstChunk;
    while (curr != NULL && (!curr->free || curr->next != prev && curr->next == firstChunk))
    {
        curr = curr->next;
    }

    curr->size += HEADER_SIZE + prev->size;
    curr->next = prev->next;
    prev = NULL;
}
