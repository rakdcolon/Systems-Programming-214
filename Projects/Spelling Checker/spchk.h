#ifndef header
#define header

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/stat.h>

#define MAX_WORD_LENGTH 100
#define BUFFER_SIZE 4096

// Define the HashSet data structure
typedef struct {
    char** table;
    int size;
    int capacity;
} HashSet;

// Function prototypes
HashSet* loadDictionary(char* filename);
void checkFile(HashSet* dictionary, char* filename);
void checkDirectory(HashSet* dictionary, char* dirname);
void checkWord(HashSet* dictionary, char* word, int line, int column, char* filename);
int isInHashSet(HashSet* set, char* word);

#endif 