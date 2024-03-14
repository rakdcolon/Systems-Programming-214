#include "spchk.h"

void toLowerCase(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int isCapitalizedCorrectly(const char* word) {
    if (!word || !*word) // Check if the word is empty or NULL
        return 0;

    int allLower = 1;
    int allUpper = 1;
    int firstUpper = 0;

    // Check each character in the word
    for (const char *ptr = word; *ptr != '\0'; ptr++) {

        if (islower(*ptr)) // Check if character is lower case
            allUpper = 0;
        else if (isupper(*ptr)) { // Check if character is upper case
            allLower = 0;
            if (ptr == word)
                firstUpper = 1;
            else
                firstUpper = 0;
        }
    }

    // Check the conditions for correct capitalization
    return allLower || allUpper || firstUpper;
}

void checkWord(HashSet* dictionary, char* word, int line, int column, char* filename) {
    char* lowerCaseWord = strdup(word);
    toLowerCase(lowerCaseWord);

    if (!isInHashSet(dictionary, lowerCaseWord) && !isCapitalizedCorrectly(word)) {
        if (isCapitalizedCorrectly(word)) {
            printf("%s:%d:%d: %s is not in the dictionary\n", filename, line, column, word);
        } else {
            printf("%s:%d:%d: %s is not capitalized correctly\n", filename, line, column, word);
        }
    }

    free(lowerCaseWord);
}

unsigned int hash(char* str) {
    unsigned int hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

void addToHashSet(HashSet* set, char* word) {
    if (set->size == set->capacity) {
        printf("Hash set is full");
        exit(EXIT_FAILURE);
    }

    unsigned int index = hash(word) % set->capacity;

    while (set->table[index] != NULL) {
        index = (index + 1) % set->capacity;
    }

    set->table[index] = strdup(word);
    toLowerCase(set->table[index]);
    set->size++;
}

int isInHashSet(HashSet* set, char* word) {
    unsigned int index = hash(word) % set->capacity;

    while (set->table[index] != NULL) {
        if (strcmp(set->table[index], word) == 0) {
            return 1;
        }

        index = (index + 1) % set->capacity;
    }

    return 0;
}

HashSet* createHashSet(int capacity) {
    HashSet* set = malloc(sizeof(HashSet));
    if (set == NULL) {
        printf("Could not allocate memory for hash set");
        exit(EXIT_FAILURE);
    }

    set->table = malloc(sizeof(char*) * capacity);
    if (set->table == NULL) {
        printf("Could not allocate memory for hash set table");
        exit(EXIT_FAILURE);
    }

    set->size = 0;
    set->capacity = capacity;

    for (int i = 0; i < capacity; i++) {
        set->table[i] = NULL;
    }

    return set;
}

void checkDirectory(HashSet* dictionary, char* dirname) {
    DIR* dir = opendir(dirname);
    if (dir == NULL) {
        // If it's not a directory, check if it's a file
        if (access(dirname, F_OK) == 0) {
            checkFile(dictionary, dirname);
            return;
        }

        printf("Could not open directory or file");
        exit(EXIT_FAILURE);
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') {
            continue;
        }

        char* path = malloc(strlen(dirname) + strlen(entry->d_name) + 2);
        strcpy(path, dirname);
        strcat(path, "/");
        strcat(path, entry->d_name);

        if (entry->d_type == DT_REG && strlen(entry->d_name) >= 4 && strcmp(entry->d_name + strlen(entry->d_name) - 4, ".txt") == 0) {
            checkFile(dictionary, path);
        } else if (entry->d_type == DT_DIR) {
            checkDirectory(dictionary, path);
        }

        free(path);
    }

    closedir(dir);
}

void checkFile(HashSet* dictionary, char* filename) {

    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        printf("Could not open file");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    char word[MAX_WORD_LENGTH];
    int wordIndex = 0;
    ssize_t bytesRead;
    int line = 1;
    int column = 1;

    while ((bytesRead = read(fd, buffer, BUFFER_SIZE)) > 0) {
        for (int i = 0; i < bytesRead; i++) {
            if (isspace(buffer[i])) {
                if (buffer[i] == '\n') {
                    line++;
                    column = 1;
                } else {
                    column++;
                }

                if (wordIndex > 0) {
                    word[wordIndex] = '\0';
                    printf("Checking word: %s\n", word);
                    checkWord(dictionary, word, line, column, filename);
                    wordIndex = 0;
                }
            } else {
                word[wordIndex++] = buffer[i];
            }
        }

        // Check the last word in the buffer
        if (wordIndex > 0) {
            word[wordIndex] = '\0';
            printf("Checking word: %s\n", word);
            checkWord(dictionary, word, line, column, filename);
            wordIndex = 0;
        }
    }

    if (bytesRead == -1) {
        printf("Error reading file");
        exit(EXIT_FAILURE);
    }

    close(fd);
}

HashSet* loadDictionary(char* filename) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        printf("Could not open dictionary file");
        exit(EXIT_FAILURE);
    }

    HashSet* dictionary = createHashSet(1000);

    char buffer[BUFFER_SIZE];
    char word[MAX_WORD_LENGTH];
    int wordIndex = 0;
    ssize_t bytesRead;

    while ((bytesRead = read(fd, buffer, BUFFER_SIZE)) > 0) {
        for (int i = 0; i < bytesRead; i++) {
            if (buffer[i] == '\n') {
                word[wordIndex] = '\0';
                addToHashSet(dictionary, word);
                wordIndex = 0;
            } else {
                word[wordIndex++] = buffer[i];
            }
        }
    }

    if (bytesRead == -1) {
        printf("Error reading dictionary file");
        exit(EXIT_FAILURE);
    }

    close(fd);
    return dictionary;
}

int main(int argc, char** argv) {
    if (argc < 3) {
        printf("Usage: spchk <dictionary> <file1> [<file2> ...]\n");
        return EXIT_FAILURE;
    }

    HashSet* dictionary = loadDictionary(argv[1]);

    int status = EXIT_SUCCESS;
    for (int i = 2; i < argc; i++) {
        struct stat path_stat;
        stat(argv[i], &path_stat);

        if (access(argv[i], F_OK) != 0) {
            printf("Could not open file or directory: %s\n", argv[i]);
            status = EXIT_FAILURE;
        } else if (S_ISREG(path_stat.st_mode)) {
            checkFile(dictionary, argv[i]);
        } else if (S_ISDIR(path_stat.st_mode)) {
            checkDirectory(dictionary, argv[i]);
        }
    }

    printf("\nSpelling check complete\nStatus: %s\n\n", status ? "FAILED" : "SUCCESSFUL");

    return status;
}