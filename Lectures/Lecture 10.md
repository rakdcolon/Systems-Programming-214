***
# Systems Programming
# Professor David Menendez
# Lecture 10: Reading Files

***
## Project Notes
***
### Late Submission:
For every day late, your score is reduced by 10 points

### README.md
Should contain
- Both partners' names and netIDs
- Any info needed to compile and run your test cases.
	- e.g. how to use your test programs
- Any notes on the design of your library
- Your test plan/testing strategy
	- Why do you think this is sufficient/good?
- Description of your performance tests and why you chose them

***
## POSIX File I/O
***
#### `open()`
Opens a file for reading and/or writing. Returns a file number (file descriptor).

#### `read()`
Obtains data from a file and writes it to memory. We have to specify how much data to read (in bytes).

#### `write()`
Obtains data from memory and writes it to a file.

If we are reading a binary file:
```C
	fd = open(data_file, O_RDONLY);
	if (fd < 0) { ... }

	int n;
	int *p;
	int bytes;

	bytes = read(fd, &n, sizeof(int));
	if (bytes < sizeof(int)) { ... }

	p = (int *) malloc(n * sizeof(int));
	bytes = read(fd, p, n * sizeof(int));
	if (bytes < n * sizeof(int)) { ... }
```

