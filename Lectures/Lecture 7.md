***
# Systems Programming
# Professor David Menendez
# Lecture 7: MyMalloc Project & Heaps

***
## Project 1
***
### Errors to Detect 
- attempt to `malloc()` too much memory
- attempt to `malloc()` more memory than is currently available
- attempt to free an address outside the heap
- attempt to free an address of a non-allocated object
- attempt to free an address in the heap but not of an object

```C
#define MEMLENGTH 512
static double memory[MEMLENGTH];
```
- We use `static` because it makes the array private to this file
	- It does not participate in "linking"
- We use `double` solely because it is 8 bytes and has 8-byte alignment

### Alignment
- Primitive data types are at addresses that are multiples of their length
	- Simplifies reading memory
	- Prevents partial overlap
- How does `malloc` ensure alignment?
	- It has no way to distinguish an array of ints from an array of doubles
	- It always gives the maximum alignment for the hardware

### Tips for MyMalloc Project
- The argument to free must always be the address of the start of a block
	- Your free needs to detect incorrect pointers and report them
	- How could you check whether an address is valid?
- Your own implementations of `malloc` and `free` must be *memory-safe* and never go past the end of your array
	- Do you need to explicitly indicate that some block is the last block?
- The real malloc and free have to store all their metadata in memory, along with the data reserved by their callers
	- Any information you store must be in the array (local variables are okay, but anything global myst be in the array)
- Arrays are a lie! You must say you are allocating 4096 chars, but it's really 4096 bytes that you can use to store anything. In the end, everything is bytes!
	- `static char memory[4096];`
	- `struct header \*p = (struct header \*) memory; // fine!`
	- `p = p->next; // makes sense!`
