***
# Systems Programming : Professor David Menendez : Lecture 25: Final Exam Review

## C Syntax
- ### Data Types
    - integer types (short, long, long long, signed unsigned)
    - floating point
    - `enum`
    - `struct`
    - `union`
    - `array`
    - `pointer`
- ### C Pointers
    - Address of data (run-time)
    - Type (compile-time)
    - Pointer types are used to generate appropriate code, but are forgotten after compilation
        - Given a pointer, there is no way to query what sort of data it refers to
    - Void Pointers have no type information
        - They must be case to a specific pointer type to be used
        - We can only use generic operations such as `memcpy()`
            - Only if we know the size of the data
        - We need to know (as programmers) what sort of data we are using, even if it is not expressed in the types
- ### Memory Allocation/Deallocation
    - `malloc()` / `calloc()` / `realloc()`
        - Reserves space on the heap
        - The run-time system maintains some sort of data structure that tracks which portions of the heap are allocated or free.
    - `free()`
        - Deallocates a previously allocated heap object
- ### Objects vs. Data
    - An object is allocation in memory that can hold data
    - The values of that object are stored within that object at some point in time.
    - An address identifies an object
    - A pointer variable has an object that stores the address of another object
        - `&p` : Address of the pointer variable itself
        - `p`  : Value of the pointer, address of the object it points to
        - `*p` : Value of the object it points to

```C
int I[2];
I[0] = 10;
I[1] = 20;
int *p;
p = &I[0];

(*p)++ // Changes I[0] to 11
*(p++) // Changes p to &I[1]
```

- ### Macros / Preprocessor
    - The preprocessor changes C source code into C source code
    - "Fancy search & replace"
```C
#define SECRET 20
#define MEAN(X,Y) ((X)/2 + (Y)/2)
```

## Posix File IO
- C (and POSIX) models files as streams of bytes
    - POSIX used the file interface to deal with many of these
        - files on disk `open()`
            - can include many "non-file" file-like things (FIFOs, devices, terminals)
        - pipes `pipe()`
        - terminals
        - sockets `socket()`, `accept()`
    - Different streams offer different operations
        - `read()`, `write()`, `lseek()`
- Streams are identified by "File Descriptors"
    - Numbers that index into a table of open streams
- POSIX File operations are "unbuffered"
    - We provide our own buffer, and get data as soon as we ask for it
- C File operations are "buffered"
    - The FILE struct includes a buffer of data from the stream.
    - Calls to `fscanf()` read from the buffer, and call `read()` internally to refresh it
    - Calls to `fprintf()` write to the bugger, and call `write()` internally to flush it

## File System
- Files are identified by i-node number (and device number)
    - Every file has exactly 1 i-node and zero or more data nodes
    - i-node stores data about the file
        - size
        - who owns it
        - what permissions it has
        - where its data are stored