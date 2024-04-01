#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <dirent.h>
#include <fcntl.h>
#include <fnmatch.h>

#define MAX_LINE 80

void parse_input(char* input, char** args) {
    char* token = strtok(input, " \n");
    int i = 0;
    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " \n");
    }
    args[i] = NULL;
}

void expand_wildcard(char* pattern, char** args, int* num_args) {
    char* slash = strrchr(pattern, '/');
    char* directory;
    char* file_pattern;

    if (slash != NULL) {
        *slash = '\0';
        directory = pattern;
        file_pattern = slash + 1;
    } else {
        directory = ".";
        file_pattern = pattern;
    }

    DIR* dir = opendir(directory);
    if (dir == NULL) {
        perror("opendir");
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (file_pattern[0] != '.' && entry->d_name[0] == '.') {
            continue;
        }
        if (fnmatch(file_pattern, entry->d_name, 0) == 0) {
            args[(*num_args)++] = strdup(entry->d_name);
        }
    }

    closedir(dir);
}

int execute_command(char** args1, char** args2, char* input_file, char* output_file, int piping) {
    int pipefd[2];
    pid_t pid1, pid2 = -1;

    if (piping && pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    pid1 = fork();
    if (pid1 == -1) {
        perror("fork");
        return 1;
    }

    if (pid1 == 0) {
        if (input_file != NULL) {
            int fd = open(input_file, O_RDONLY);
            if (fd == -1) {
                perror("open");
                exit(1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }

        if (piping && output_file == NULL) {
            close(pipefd[0]);
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[1]);
        } else if (output_file != NULL) {
            int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0640);
            if (fd == -1) {
                perror("open");
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        execv(args1[0], args1);
        perror("execv");
        exit(1);
    }

    if (piping) {
        close(pipefd[1]);

        pid2 = fork();
        if (pid2 == -1) {
            perror("fork");
            return 1;
        }

        if (pid2 == 0) {
            if (input_file == NULL) {
                dup2(pipefd[0], STDIN_FILENO);
            }
            close(pipefd[0]);

            if (output_file != NULL) {
                int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0640);
                if (fd == -1) {
                    perror("open");
                    exit(1);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }

            execv(args2[0], args2);
            perror("execv");
            exit(1);
        }

        close(pipefd[0]);
    }

    int status;
    waitpid(pid1, &status, 0);
    if (piping) {
        waitpid(pid2, &status, 0);
    }

    return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
}

int main(int argc, char *argv[]) {
    int should_run = 1;
    int prev_exit_status = 0;

    while (should_run) {
        char input[MAX_LINE];
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        char* args[MAX_LINE];
        parse_input(input, args);

        char* args1[MAX_LINE];
        char* args2[MAX_LINE];
        char* input_file = NULL;
        char* output_file = NULL;
        int num_args1 = 0;
        int num_args2 = 0;
        int piping = 0;

        for (int i = 0; args[i] != NULL; i++) {
            if (strcmp(args[i], "<") == 0) {
                input_file = args[++i];
            } else if (strcmp(args[i], ">") == 0) {
                output_file = args[++i];
            } else if (strcmp(args[i], "|") == 0) {
                piping = 1;
            } else if (strchr(args[i], '*') != NULL) {
                expand_wildcard(args[i], args1, &num_args1);
            } else if (piping) {
                args2[num_args2++] = args[i];
            } else {
                args1[num_args1++] = args[i];
            }
        }

        args1[num_args1] = NULL;
        args2[num_args2] = NULL;

        if (strcmp(args1[0], "then") == 0 && prev_exit_status != 0) {
            continue;
        }

        if (strcmp(args1[0], "else") == 0 && prev_exit_status == 0) {
            continue;
        }

        prev_exit_status = execute_command(args1, args2, input_file, output_file, piping);
    }

    return 0;
}