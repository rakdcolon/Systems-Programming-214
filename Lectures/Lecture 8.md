***
# Systems Programming
# Professor David Menendez
# Lecture 8: Project, File Input/Output

***
## Project 1

***
### Run-time Error Conditions
Your library code must detect these error conditions and report them
- Not enough memory
- Freeing a bad pointer
- `malloc()` should return `NULL` (and print a message)
- `free()` should do nothing and terminate the program (and print a message)
	- Print to standard error:
		- `fprintf(stderr, ...)`

### Correctness Conditions
How do you know that your code is good enough to submit?
- You will need to run tests to decide whether you wrote the library correctly
- Testing strategy
	- `1)` Determine several requirements your library must satisfy
		- Library detects all required errors
		- `malloc` should allocate memory
		- `free` should deallocate memory
	- `2)` Figure out how to tell whether your requirements are satisfied
	- `3)` Write code
		- Allocate the largest possible allocation and see if the next malloc fails
		- Allocate all of memory, free it, allocate again
#### `Correctness Testing Example`
```C
int main {int argc, char **argv) {
	int test = atoi(argv[1]);

	switch (test) {
		case 1:
			int n;
			free(&n); // Freeing a variable not on the heap
			break;
		case 2:
			char *p = malloc(25);
			free(p);
			free(p); // Freeing a variable twice
			break;
		case 3:
			p = malloc(25);
			free(p + 10); // Freeing from the middle of an object
			break;
		// More cases...
	}
	return EXIT_SUCCESS;
}
```

#### Hint: Section 9.9 from 211 Textbook contains implementations of `malloc` and `free`.

### Performance Testing
Testing the efficiency of your code
- This is what `memgrind.c` is for. `memgrind.c` measures the speed of your library
- You are given 2 tests and you will have to write 2 more tests to additionally test your system.
- General Structure of testing:
	- `1)` Start Time
	- `2)` Run test 50 times
	- `3)` End Time
	- `4)` Subtract Start Time from End Time and divide by 50
	- `5)` Report average elapsed time in milliseconds

### "Black Box" and "White Box" Testing
- Black Box testing uses the public interface of a library
	- Coined Black Box because you cannot see inside
	- `memtest.c` is an example of a black box test
- White Box testing uses internal (private) aspects of the library

### Other Helpful Tools when Debugging:
- Function that checks the integrity of the heap
	- Do the sizes of the chunks add up to the size of the heap?
	- Are all the sizes reasonable?
	- Are there adjacent free blocks?
- Function that prints the structure of the heap
	- For each chunk, print its size and whether it is in use
	- Can hold other information you find helpful

### Use Make File
- `test: test-program`
	- `./test-program` Compiles and runs in one step
- `test2: test-program`
	- `./test-program > test2.out`
	- `diff test2.out test2.ref` Compiles and compares output to a pre-written output. Will highlight differences on what lines.

### Useful Compiler Options
- `-g` includes debugging information (symbols)
- `-fsanitize=address` enable address sanitizer 
- `-fsanitize=undefined` enable undefined behavior sanitizer
- `-Wall` turns on additional compiler warnings
- `Wvla` warns for variable-length arrays
- `-O` optimizes code
- `-O2` optimizes code (even more)

***
## Files
***
In Unix, a file is a stream of bytes
- Previous systems represented files as sequences of records
- Byte streams are more general, but require more work

Unix generalizes this concept to work with all byte streams
- Actual files on disk
- Console input/output
- Other IO devices
- Inter-process communication (IPC)
- Network communication

### File Descriptor
A file descriptor is a number that identifies an open file
- the run-time system maintains a table of all open files
- The file descriptor is an index into this table
	- 0 is standard input (`STDIN_FILENO`)
	- 1 is standard output (`STDOUT_FILENO`)
	- 2 is standard error (`STDERR_FILENO`)
- `fileno()` returns the file descriptor associated with a FILE
- `fdopen()` creates a FILE for a file descriptor

### Buffering
- Reading or writing a file requires help from the OS
	- User programs can't talk directly to hardware
	- Instead, we call the OS to access file data
	- OS calls introduce overhead
- idea: request large number of bytes together
- A buffer is an array of bytes used to store data that is about to be sent from a file or has been received from a file
- We only call the OS when the buffer is full/empty.
