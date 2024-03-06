***
# Systems Programming
# Professor David Menendez
# Lecture 9: Posix File I/O

***
### Open

```C
#include <fcntl.h>
int open(char* pathname, int flags);
int open(char* pathname, int flags, mode_t mode);
```
- Pathname: indicates where a file is located in the file system
	- A name by itself (no slashes) refers to a file in the working directory
	- A path with slashes indicates directory containment
		- `foo/bar/baz/quux` indicates `quux` inside `baz` inside `bar` inside `foo` inside working directory
	- A path that begins with a slash is absolute: it starts from the filesystem root
	- Special directory entries:
		- `..` is the parent of the directory
		- `.` is the directory itself
	- `../foo` means `foo` is in the parent directory of the working directory
- Flags: bitmap indicating how we intend to use the file
	- `O_RDONLY` --> open file for reading
	- `O_WRONLY` --> open file for writing
	- `O_RDRW` --> open file for reading and writing
- Additional flags (combine using `|`)
	- `O_CREAT` --> create the file if it does not exist (must provide mode)
	- `O_TRUNC` --> truncate file (set its length to 0) if it exists
	- `O_EXCL` --> fail if the file already exists
	- `O_APPEND` --> start writing from the end of the file
	- `O_NONBLOCK` --> open file in "non-blocking" mode
- Open returns a file descriptor when successful
- If open cannot open the file, it returns -1 and sets `errno`

### `Errno`
- `Errno` is a global variable that stores a number describing the last error that happened (from a function that sets `errno`)
	- Not every function sets `errno`; check the documentation
- The library defines constants for important error reasons
	- `EACCESS` --> don't have permission to access file
	- `EEXIST` --> file already exists
	- and many, many others
- Instead of looking at `errno` directly, we will usually use `perror()` or `strerror()`
- `void perror(char* msg);`
	- Prints an error message incorporating msg and describing the current value of `errno`
```C
int fd = open(filename, flags);
if(fd == -1){
	perror(filename);
	abort();
}

char* strerror(int error_code); // returns a string describing the given error code
```

### File Permissions
- A file mode is the traditional Unix way to indicate who has what sort of access to a file
- When opening a file with `O_CREAT`, 
- Three categories of users:
	- `u` --> the owner of a file (every file is owned by some user)
	- `g` --> the group of a file (every file has access for some group)
	- `o` --> everyone else
- Three forms of access
	- `r` --> read access
	- `w` --> write access
	- `x` --> execute access (needed to run programs or read directories)
- `uuugggooo`
- `rwxrwxrwx`
- `chmod` is a shell command that changes the mode of a file
	- `chmod <permissions code> <files...>`
	- `chmod -w some_file` --> disable write permission
	- `chmod +r some_file` --> enable read permission
	- `chmod o-r some_file` --> disable write permission for others
- We can also use octal numbers to describe permissions
	- 7 in octal is 111 in binary
	- 6 in octal is 110 in binary
	- 4 in octal is 100 in binary
- We can use three octal digits to describe a permissions setting
	- 644 --> `rw-r--r--`
	- 770 --> `rwxrwx---`
- We can specify the mode parameter for `open()` using octal
	- `fd = open(file, O_WRONLY|O_CREAT|O_TRUNC, 0644);`
		- Creates file if it does not exist with permissions
		- `u+rw, g+r, o+r`
	- Equivalent to:
		- `open(file, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);`
- reminder:
	- integer literals (in C) beginning with 0 are in octal!
		- 010 = 8
### Close
```
#include <unistd.h>
int close(int fd);
```
- closes the specified file
- returns 0 on success
- returns -1 and sets `errno` on failure
- note: all open files will be closed when the program exits either by the run-time system on normal exit, or by the OS
- there is a limit to how many open files your process can have, so it is a good idea to closer files when you are done with them
- Once a file is closed, its file descriptor may be reused by subsequent calls to `open()`

### Read
- `ssize_t read(int fd, void* buf, size_t n);`
	- Reads from an open readable file up to `n` bytes, bytes will be written to memory at `buf`
	- Returns the number of bytes obtained or 0, if you have reached the end of the file
- Open files will have the concept of a position or "file pointer"
	- We start reading from the beginning of a file
	- Every time we read, the position advances
- Typical usage:
```C
char buf[BUFSIZE];

int r = read(fd, buf, BUFSIZE);
while (r>0){
	// do stuff with contents of buf
	r = read(fd, buf, BUFSIZE);
}

// OR if you don't like repeating yourself

char buf[BUFSIZE];
int r;

while ((r = read(fd, buf, BUFSIZE)) > 0){
	// do stuff with contents of buf
	for(i = 0; i < r; i++){
		if(isspace(buf[i])) space_count++;
	}
}
```

### Write
- `ssize_t write(int fd, void* buf, size_t n);`
	- sends `n` bytes to a writeable file
	- `fd` indicates the file being written to
	- `buf` holds the bytes we will write
	- returns the number of bytes that were actually written
	- returns -1 on error (and sets `errno`)
