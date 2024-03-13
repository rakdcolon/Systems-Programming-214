#include "spchk.h"

void checkDirectory(HashSet* dictionary, char* dirname) {
    DIR* dir = opendir(dirname);
    if (dir == NULL) {
        perror("Could not open directory");
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
        perror("Could not open file");
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
                    checkWord(dictionary, word, line, column, filename);
                    wordIndex = 0;
                }
            } else {
                word[wordIndex++] = buffer[i];
            }
        }
    }

    if (bytesRead == -1) {
        perror("Error reading file");
        exit(EXIT_FAILURE);
    }

    close(fd);
}

HashSet* loadDictionary(char* filename) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Could not open dictionary file");
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
        perror("Error reading dictionary file");
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

        if (access(argv[i], F_OK) != 0) {
            printf("Could not open file: %s\n", argv[i]);
            status = EXIT_FAILURE;
        } else {
            checkFile(dictionary, argv[i]);
            checkDirectory(dictionary, argv[i]);
        }
    }

    return status;
}