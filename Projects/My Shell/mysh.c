#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <getopt.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <glob.h>

#define MYSH_LINE 1024

typedef struct Node{
	char* data;
	struct Node* next;
} Node;

typedef struct CmdSet
{
    struct Cmd **commands;
    int background;
    int numCmds;
} *cmdset;

typedef struct Cmd
{
    char **args;
    char *inputFile;
    char *outputFile;
    int n;
    int appendOut;
    int pipeInput;
    int pipeOutput;
} *cmd;

typedef struct FreeNode
{
    struct FreeNode *nextFree;
    pid_t process_id;
} *FreeNode;

char **get_tokens(const char *line)
{
    char **tokens = NULL;
    char *lineCopy;
    const char *delimiter = " \t\n";
    char *currentToken;
    int n = 0;

    tokens = (char **)malloc(sizeof(char *));
    tokens[0] = NULL;

    if (line == NULL) return tokens;

    lineCopy = strdup(line);
    currentToken = strtok(lineCopy, delimiter);

    if (currentToken == NULL) return tokens;

    do {
        n++;
        tokens = (char **)realloc(tokens, (n + 1) * sizeof(char *));
        tokens[n - 1] = strdup(currentToken);
        tokens[n] = NULL;
    } while ((currentToken = strtok(NULL, delimiter)));

    free(lineCopy);

    return tokens;
}

void free_tokens(char **tokens)
{
    if (tokens == NULL) return;

    for (int i = 0; tokens[i]; i++) { free(tokens[i]); }

    free(tokens);
}

FreeNode head;

void insert_node(FreeNode n)
{
    FreeNode temp = head;
    n->nextFree = 0;
    if (head == 0)
    {
        head = n;
        return;
    }
    else
    {
        if (n < head)
        {
            n->nextFree = head;
            head = n;
            return;
        }

        while (temp->nextFree != 0)
        {
            if (n < temp->nextFree)
            {
                n->nextFree = temp->nextFree;
                temp->nextFree = n;
                return;
            }
            temp = temp->nextFree;
        }
        temp->nextFree = n;
        return;
    }
}

void expandWildCards(char* args, Node* ptr, int* numArg){
    glob_t pglob;
    if (glob(args, GLOB_NOCHECK | GLOB_TILDE, NULL, &pglob) == 0) {
        for (int i = 0; i < (int)pglob.gl_pathc; i++) {
            ptr->data = strdup(pglob.gl_pathv[i]);
            (*numArg)++;
            if (i < (int)pglob.gl_pathc - 1) { // Check if not last element
                ptr-> next = (Node*)malloc(sizeof(Node));
                ptr = ptr->next;
            } else {
                ptr->next = NULL; // Set the next of the last node to NULL
            }
        }
    }
    globfree(&pglob);
}

void remove_node(pid_t n)
{

    if (head->process_id == n)
    {
        head = head->nextFree;
        return;
    }

    FreeNode temp = head;
    while (temp->nextFree != 0)
    {
        if (temp->nextFree->process_id == n)
        {
            temp->nextFree = temp->nextFree->nextFree;
            return;
        }
        temp = temp->nextFree;
    }
    return;
}

int main(int argc, char *argv[])
{
    int init = 0;
    while (1)
    {

        int error = 0;
        if (argc == 2)
        {
            if (!strcmp(argv[1], "-"))
            {
                fprintf(stdout, "");
            }
            else
            {
                fprintf(stdout, "%s ", argv[1]);
            }
        }
        else if (argc == 1)
        {
            if (init == 0)
            {
                fprintf(stdout, "Welcome to my shell!\n");
            }
            init = 1;
            fprintf(stdout, "mysh> ");
        }
        else
        {
            fprintf(stderr, "Error: Usage: %s [prompt]\n", argv[0]);
            exit(-1);
        }

        char *lineBuffer = malloc(sizeof(char) * (MYSH_LINE));
        if (!lineBuffer)
        {
            fprintf(stderr, "Error: Out of memory\n");
            exit(-1);
        }

        char *result;
        errno = 0;
        do
        {
            result = fgets(lineBuffer, MYSH_LINE, stdin);
        } while ((result == NULL) && (errno == EINTR));
        if ((!result) || (strcmp(lineBuffer, "exit\n") == 0))
        {
            pid_t kill_id;
            int status;
            fprintf(stdout, "mysh: exiting\n");
            do {
                kill_id = wait3(&status, WNOHANG, NULL);
                if (kill_id > 0)
                {
                    kill(kill_id, SIGTERM);
                }
            } while (kill_id != -1);
            return 0;
        }

        cmdset commandSet = calloc(1, sizeof(struct CmdSet));
        commandSet->numCmds = 1;
        commandSet->commands = calloc(1, sizeof(char *) * (commandSet->numCmds + 1));
        cmd command = calloc(1, sizeof(struct Cmd));
        command->args = calloc(1, sizeof(char **));
        commandSet->commands[0] = command;
        commandSet->commands[1] = NULL;

        char **breakup = get_tokens(lineBuffer);

        for (int i = 0; breakup[i]; i++)
        {
            if (strchr(breakup[i], '*')) {  // If the argument contains a wildcard
                glob_t glob_result;

                switch (glob(breakup[i], GLOB_TILDE, NULL, &glob_result)) {
                    case 0:  // Successful glob
                        breakup[i] = NULL;  // Remove the wildcard pattern
                        for (size_t j = 0; j < glob_result.gl_pathc; j++) {
                            // Add each matching filename to the argument list
                            breakup = realloc(breakup, (i + j + 2) * sizeof(char *));
                            breakup[i + j] = strdup(glob_result.gl_pathv[j]);
                        }
                        breakup[i + glob_result.gl_pathc] = NULL;  // Null-terminate the argument list
                        break;
                    case GLOB_NOMATCH:  // No matches
                        break;  // Leave the argument as is
                    default:  // Some other error
                        perror("glob");
                        break;
                }
                globfree(&glob_result);
            }

            if (strcmp(breakup[0], "cd") == 0) {
                if (breakup[1] == NULL) {
                    fprintf(stderr, "cd: no argument provided\n");
                    break;
                }
                if (chdir(breakup[1]) == -1) {
                    perror("cd");
                    break;
                }
                break;
            }

            if (strcmp(breakup[i], "&") == 0)
            {
                if (breakup[i + 1] != NULL)
                {
                    fprintf(stderr, "Error: \"&\" must be last token on command line\n");
                    error = 1;
                    break;
                }
                else
                {
                    commandSet->background = 1;
                }
            }
            else if (strcmp(breakup[i], "<") == 0)
            {
                i++;
                if (command->inputFile)
                {
                    fprintf(stderr, "Error: Ambiguous input redirection.\n");
                    error = 1;
                    break;
                }
                if (breakup[i])
                {
                    command->inputFile = breakup[i];
                }
                else
                {
                    fprintf(stderr, "Error: Missing filename for input redirection.\n");
                    error = 1;
                    break;
                }
            }
            else if (strcmp(breakup[i], ">") == 0)
            {
                i++;
                if (command->outputFile)
                {
                    fprintf(stderr, "Error: Ambiguous output redirection.\n");
                    error = 1;
                    break;
                }
                if (breakup[i])
                {
                    struct stat buf;
                    int exists = lstat(breakup[i], &buf);
                    if (exists >= 0)
                    {
                        fprintf(stderr, "Error: open(\"%s\"): File exists\n", breakup[i]);
                        error = 1;
                        break;
                    }
                    command->outputFile = breakup[i];
                }
                else
                {
                    fprintf(stderr, "Error: Missing filename for output redirection.\n");
                    error = 1;
                    break;
                }
            }
            else if (strcmp(breakup[i], ">>") == 0)
            {
                i++;
                if (command->outputFile)
                {
                    fprintf(stderr, "Error: Ambiguous output redirection.\n");
                    error = 1;
                    break;
                }
                if (breakup[i])
                {
                    command->outputFile = breakup[i];
                    command->appendOut = 1;
                }
                else
                {
                    fprintf(stderr, "Error: Missing filename for output redirection.\n");
                    error = 1;
                    break;
                }
            }
            else if (strcmp(breakup[i], "|") == 0)
            {
                if (!command->args)
                {
                    fprintf(stderr, "Error: Invalid null command.\n");
                    error = 1;
                    break;
                }
                commandSet->numCmds++;
                commandSet->commands = realloc(commandSet->commands, sizeof(char *) * commandSet->numCmds + 1);
                command->pipeOutput = 1;
                cmd command2 = calloc(1, sizeof(struct Cmd));
                command2->pipeInput = 1;
                command2->args = calloc(1, sizeof(char **) * 2);
                commandSet->commands[commandSet->numCmds - 1] = command2;
                command = command2;
                commandSet->commands[commandSet->numCmds] = NULL;
            }
            else
            {
                if (command->n == 0)
                {
                    command->args[0] = breakup[i];
                    command->n++;
                }
                else
                {
                    command->n++;
                    command->args = (char **)realloc(command->args, sizeof(char **) * (command->n + 1));
                    command->args[command->n - 1] = breakup[i];
                    command->args[command->n] = NULL;
                }
            }
        }

        for (int c = 0; commandSet->commands[c]; c++)
        {
            cmd command = commandSet->commands[c];
            if (command->pipeOutput)
            {
                int pipefds[2] = {-1, -1};
                if (!command->args[0])
                {
                    fprintf(stderr, "Error: Invalid null command.\n");
                    error = 1;
                    break;
                }
                else if (command->outputFile)
                {
                    fprintf(stderr, "Error: Ambiguous output redirection.\n");
                    error = 1;
                    break;
                }
                else
                {
                    if (pipe(pipefds) < 0)
                    {
                        perror("Error: Cannot create pipe()");
                        error = 1;
                        break;
                    }
                    command->pipeOutput = pipefds[1];
                }
                if (commandSet->commands[c + 1])
                {
                    if (!commandSet->commands[c + 1]->pipeInput)
                    {
                        fprintf(stderr, "Error: Destination not set to accept pipe.\n");
                        error = 1;
                        break;
                    }
                    else if (!commandSet->commands[c + 1]->args[0])
                    {
                        fprintf(stderr, "Error: Invalid null command.\n");
                        error = 1;
                        break;
                    }
                    else if (commandSet->commands[c + 1]->inputFile)
                    {
                        fprintf(stderr, "Error: Ambiguous input redirection.\n");
                        error = 1;
                        break;
                    }
                    else
                    {
                        commandSet->commands[c + 1]->pipeInput = pipefds[0];
                    }
                }
                else
                {
                    fprintf(stderr, "Error: No destination for pipe.\n");
                    error = 1;
                    break;
                }
            }
        }
        if (error)
            continue;
        int status;
        pid_t process_id = 0;
        int stdout_save;

        for (int c = 0; commandSet->commands[c]; c++)
        {
            cmd command = commandSet->commands[c];
            pid_t pid = fork();
            if (pid > 0)
            {
                if (command->pipeOutput) close(command->pipeOutput);
                if (command->pipeInput) close(command->pipeInput);

                if (!commandSet->background)
                {
                    FreeNode newNode = malloc(sizeof(struct FreeNode));
                    newNode->process_id = pid;
                    newNode->nextFree = NULL;
                    insert_node(newNode);
                }
            }
            else if (pid == 0)
            {
                if (execvp(command->args[0], command->args) == -1) {
                    perror("Error executing command");
                    exit(EXIT_FAILURE);
                }

                if (command->inputFile)
                {
                    int fin = open(command->inputFile, O_RDONLY);
                    if (fin == -1)
                    {
                        fprintf(stderr, "Error: open(\"%s\"): %s\n", command->inputFile, strerror(errno));
                        exit(-1);
                    }
                    if (dup2(fin, 0) == -1)
                    {
                        perror("Error: dup2");
                        exit(-1);
                    }
                    close(fin);
                }
                if (command->outputFile)
                {
                    int fout;
                    stdout_save = dup(STDOUT_FILENO);
                    if (command->appendOut)
                    {
                        fout = open(command->outputFile,
                                    O_WRONLY | O_CREAT | O_APPEND,
                                    S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
                    }
                    else
                    {
                        fout = open(command->outputFile,
                                    O_CREAT | O_WRONLY,
                                    S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
                    }
                    if (fout == -1)
                    {
                        fprintf(stderr, "Error: open(\"%s\"): %s\n", command->outputFile, strerror(errno));
                        exit(-1);
                    }
                    if (dup2(fout, 1) == -1)
                    {
                        perror("Error: dup2()");
                        exit(-1);
                    }
                    close(fout);
                }
                if (command->pipeOutput)
                {
                    if (dup2(command->pipeOutput, STDOUT_FILENO) == -1)
                    {
                        fprintf(stderr, "Error: dup2 stdout, pipeOutput %d\n", command->pipeOutput);
                        close(command->pipeOutput);
                        exit(1);
                    }
                }
                if (command->pipeInput)
                {
                    if (dup2(command->pipeInput, STDIN_FILENO) == -1)
                    {
                        fprintf(stderr, "Error: dup2 stdin, pipeInput %d\n", command->pipeInput);
                        close(command->pipeInput);
                        exit(1);
                    }
                }
                int execresult = execvp(command->args[0], command->args);
                if (execresult == -1) {
                    perror("Error executing command");
                    exit(EXIT_FAILURE);
                }
                // do
                // {
                //     execresult = execvp(command->args[0], command->args);
                // } while (execresult == -1);

                if (dup2(stdout_save, STDOUT_FILENO) == -1)
                {
                    perror("Error: Cannot restore standard output");
                    error = 1;
                    break;
                }

                close(stdout_save);

                exit(EXIT_FAILURE);
            } else {
                fprintf(stderr, "Error: fork()\n");
                exit(EXIT_FAILURE);
            }
        }

        while (head != NULL)
        {
            process_id = wait3(&status, WNOHANG, NULL);
            if (process_id > 0)
            {
                remove_node(process_id);
            }
            else if (process_id == -1)
            {
                head = NULL;
                break;
            }
            usleep(10000);
        }

        free(lineBuffer);
        free_tokens(breakup);

        for (int i = 0; commandSet->commands[i]; i++)
        {
            command = commandSet->commands[i];
            if (command->args)
                free(command->args);
            free(command);
        }
        free(commandSet);
    }
}