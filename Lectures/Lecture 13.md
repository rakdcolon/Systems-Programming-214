***
# Systems Programming
# Professor David Menendez
# Lecture 13: Exam and more Systems

***
## Class Info
***
### Midterm Exam
Wednesday, March 6th, In-class
- Topic list will be posted by Monday
- You can bring one page single sided of handwritten notes
Format:
- Primarily short answer questions (a few sentences at most)
- Some code reading, very little code writing

### Project 2:
Due date changed to March 18th

***
## Systems
***
#### Shell
- Search path used for first word in command (except for paths)
	- `prog`: runs a program "prog" in the search directories
	- `./prog`: runs a program "prog" in the working directory
- `PATH` is the list of directories to search for programs

#### File Globs
- Specify one or more files
- `*`: matches any sequence of characters
- `?` matches any one character
- The Shell expands globs before executing the program

#### Stdin, Stdout, Stderr
- Normally, these files are inherited from the shell (or other parent process)
- Redirection lets us change these:
	- `some_program < foo` use the file `foo` as standard input
	- `some_program > foo` use the file `foo` as standard output
	- `someprogram 2> errs` use the file `errs` as stderr

#### Pipes
Pipes chain two or more programs together, where the output of one program becomes the input of the next
- `some_program | grep ERROR`
	- runs `some_program` and `grep`
	- stdout for `some_program` writes to stdin for `grep`

#### Useful Programs
- `wc`: counts characters/words/lines in a file
- `cat`: reads several files and writes them to stdout
- `more`: reads from a file or stdin and paginates the output
- `less`: more powerful version of more (lets you go backward)

#### Grep
Search files (or stdin) for lines matching a pattern
- `grep ERROR files...`
	- prints all lines containing ERROR
- `grep ^ERROR`
	- prints all lines beginning with ERROR
- `grep 'foo.*bar' files`
	- prints lines containing "foo" followed at some point by "bar"
- `grep "[0-9]" files`
	- prints lines containing a digit
Grep regular expressions are very powerful. Many tutorials exist online.

#### Exit Status
- `$?`: Shell variable containing the exit status of the last process.
- `foo && bar`: Executes `foo` and then if `foo` succeeded, it executes `bar`.
- Example: `make prog && ./prog`

#### Processes
A process is a program that is being executed. It includes the state of the computer(memory, files, etc.) as well as a program it is executing. Every process has a unique process ID (PID).

In UNIX, a process group is a collection of one or more processes. A process group  created by a shell is a "job." Running a single program starts a job. All processes in a pipeline are part of the same job. 

A session is a set of process groups. Typically, each time you log into a system, you get a new login session. Every job and process you start will be part of this session.

#### Useful Control Codes:
- `^C`: Terminates all processes in the foreground
- `^Z`: Pauses all processes in the foreground progress group
- `fg`: Brings a stopped job back to the foreground
- `bg`: Resumes a stopped job in the background
- `jobs`: lists all active jobs with their status
- `ps`: lists all processes in the current session
- Put `&` at the end of a shell command to start the job in the background
