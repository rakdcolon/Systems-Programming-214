***
# Systems Programming
# Professor David Menendez
# Lecture 5: Generic Memory Operations

***
### Sorting an Array
```C
void sort_ints(int *array, int length);
void sort_chars(char *array, int length);
```
- What is the difference here?
	- Comparison function
	- Size of the data units
- What do we need to write a generic sorting function?
	- Some way to pass an array without restricting its type
		- Use `void *`
	- Some way to specify how to compare items
		- Use a function pointer
		- `return_type (*compare) (arg1_type, arg2_type, ...)`
			- `int (*compare) (void *, void *)`
	- Size of array elements
```C
void sort(
	void *array,
	size_t array_length,
	size_t item_size,
	int (*comparison_function) (void *, void *)
);
```
### Copying Memory
- `memcpy()`
	- Copies a specified number of bytes from one location to another
	- `void *memcpy(void *dest, void *src, size_t size);`
		- Copies size number of bytes from `src` to `dest`, returns `dest`.
	- There are no safety checks
		- We must ensure that the source and destination objects are large enough (and that they allocated)
	- Example: `a` and `b` are both arrays of `20` integers
		- `b = a; // NOPE`
		- `memcpy(b, a, sizeof(int) * 20); // YES`
- `memmove()`
	- copies a specified number of bytes from one location to another (locations may overlap)
	- `void *memmove(`
