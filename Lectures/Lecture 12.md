***
# Systems Programming
# Professor David Menendez
# Lecture 12: Directories

***
### Recap:
***
A directory is a special file that contains directory entries. Each directory entry has (at minimum):
- a file name
- an inode identifier

Every directory has two special entries
`.` always refers to the directory itself
`..` always refers to the parent of the directory

There are two ways to open a directory file:
- `open()`
	- gives access to the raw bytes
- `opendir()`
	- convenient way to iterate through directory entries

`#include <dirent.h>`

`DIR *opendir(char *path);`
- opens the specified directory and returns a handle. On failure, returns `NULL` and sets errno

`struct dirent *readdir(DIR *handle);`
- returns the next entry in the directory
- returns `NULL` if we have read all the entries
- `struct dirent` contains at least these fields:
	- `int_t d_ino;` i-node identifier
	- `char d_name[];` file name(null-terminated string)
- the pointer we get is only valid 
