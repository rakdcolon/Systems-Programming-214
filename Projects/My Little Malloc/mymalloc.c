
/*
 *
 * INCLUDE HEADER FILES
 *
 */
#include "mymalloc.h"


/*
 *
 * MALLOC
 *
 */
void *mymalloc(size_t size, char *FILE, int line) {

    // If any of the parameters are wrong, then halt the program
    if (size <= 0 || !FILE || line <= 0) { 
        fprintf(stderr, "\nError in %p on line %d: Invalid Size\n", FILE, line);
        return NULL; // Here I am returning a NULL pointer, instead of an error similar to how real malloc works.
    }

    size = NEXT_MULTIPLE_OF_EIGHT; // Need to offset size to the next multiple of 8 as each chunk is 8 bytes, so the size is rounded up always.

    int *head = START_OF_MEMORY; // Pointer to the beginning of the memory

    if (IS_HEAD_UNINITIALIZED) { // Initializing the memory if this is the first time malloc is being run

        *head = MEMORY_LENGTH_IN_BYTES; // Letting this "chunk" be the entire array
        *((int *)head + 1) = 0; // Setting this chunk to be free

    }

    int numberOfBytes = 0;
    void *allocatedMemory; // The pointer being returned to the USER

    while (numberOfBytes < MEMORY_LENGTH_IN_BYTES) { // Constantly checks if we are within the confines of the memory

        if (IS_HEAD_FREE && size + 8 <= HEAD_SIZE) { // If we have found a location to put the requested memory

            int startingSize = HEAD_SIZE;
            SET_HEAD_SIZE(size);
            SET_HEAD_ALLOCATED;
            allocatedMemory = (void *)(head + 2); // +2 such that the pointer points to actual data and not to the metadata/header
 
            if (startingSize > HEAD_SIZE) { // If the rest of memory has less size than the size of the memory being allocated
            
                HEAD_NEXT_SIZE = startingSize - HEAD_SIZE;
                HEAD_NEXT_IS_FREE;

            }

            return allocatedMemory; // Returns the void pointer to the USER

        } else {

            numberOfBytes += HEAD_SIZE;
            head = HEAD_NEXT;

        }
    }

    // If could not find a suitable location, then show error and return a NULL pointer to the USER
    fprintf(stderr, "\nError in %p on line %d: Not Enough Space\n", FILE, line);
    return NULL;

}

/*
 *
 * FREE
 *
 */
void myfree(void *ptr, char *FILE, int line) {

    int *END_OF_MEMORY = START_OF_MEMORY + (MEMORY_LENGTH_IN_BYTES / sizeof(int));

    if (ptr == NULL || ptr < (void *)START_OF_MEMORY || ptr >= (void *)END_OF_MEMORY) {
        fprintf(stderr, "\nError in %s on line %d: Invalid Pointer\n", FILE, line);
        return;
    }

    int numberOfBytes = 0;

    // Hold two pointers such that we can coalesce chunks as soon as they are freed
    int *prev = START_OF_MEMORY;
    int *curr = START_OF_MEMORY;

    while (numberOfBytes <= MEMORY_LENGTH_IN_BYTES - 8) { // Loop through every chunk of memory

        if (MALLOC_POINTER_FOUND) { // If we have found the location of the memory that the USER wants to free

            if (IS_CURR_FREE) {  // If it is already freed, then return an error for attempting to free an already free object

                fprintf(stderr, "\nError in %p on line %d: Invalid Free\n", FILE, line);
                return;

            } else {

                SET_CURR_FREE; // Make the object free


                if (curr >= START_OF_MEMORY && curr < END_OF_MEMORY) {
                    int blockSize = *curr / 4;
                    if (blockSize < 0 || blockSize >= MEMORY_LENGTH_IN_BYTES) {
                        fprintf(stderr, "\nError in %p on line %d: Out of bounds memory\n", FILE, line);
                        return;
                    }
                    if ((numberOfBytes + (*curr)) < MEMORY_LENGTH_IN_BYTES) {
                        int *nextBlock = curr + blockSize;
                        if (nextBlock >= START_OF_MEMORY && nextBlock < END_OF_MEMORY && *nextBlock == 0) {
                            *curr += *(curr + blockSize);
                        }
                    }
                }

                // if ((numberOfBytes + (*curr)) < MEMORY_LENGTH_IN_BYTES && *(curr + (*curr / 4) + 1) == 0) { *curr += *(curr + (*curr / 4)); }

                if (prev != curr && IS_PREV_FREE) { 
                    *prev += *curr; 
                    curr = prev; // Update curr to point to the coalesced block
                }
                if (IS_NEXT_BLOCK_FREE) { 
                    *curr += *(curr + (*curr / 4)); 
                }

                return;

            }

        } else {

            // If the memory was not found this iteration, shift both pointers to the right and the byte indicator
            prev = curr;
            if (curr + *curr / 4 < END_OF_MEMORY) {
                numberOfBytes += *curr;
                curr += *curr / 4;
            } else {
                fprintf(stderr, "\nError in %p on line %d: Out of bounds memory\n", FILE, line);
                return;
            }

        }
    }

    // If no memory was found in the entire list, then return an error to the USER for invalid pointer
    fprintf(stderr, "\nError in %p on line %d: Invalid Pointer\n", FILE, line);
    return;

}