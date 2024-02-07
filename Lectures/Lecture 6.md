***
# Systems Programming
# Professor David Menendez
# Lecture 6: ArrayList and Make

***
## ArrayList
***
An Array that we can expand or contract

### Operations
- Push
- Pop
- Indexing
- Initialize
- Destroy
- Struct that holds ...
	- an array
	- the number of elements in the list
	- the size of the array

### Central Idea
- We add elements to the array until it is full
- When it is full, we replace the array with a bigger one

### Our Project
	- test.c        <- test driver (a client of ArrayList)
	- arraylist.c   <- implementatin of functions
	- arraylist.h   <- type definition & function prototypes

### Compilation Strategy
	- compile arraylist.c -> arraylist.o
	- compile test.c -> test.o
	- link arraylist.o test.o -> test

### `arraylist.h`
```C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "arraylist.h" // Should include the .h file in the .h file

typedef struct {
	int *data;
	unsigned length;
	unsigned capacity;
} arraylist_t;

/*
 * Initializes the ArrayList
 * 
 * Parameters:
 *  - arraylist_t * -> Pointer to the ArrayList
 *  - capacity -> Initial capacity of the ArrayList
 */
void al_init(arraylist_t *, unsigned capacity) {
	L->data = malloc(size * sizeof(int));
	L->length = 0;
	L->capacity = size;
}

/*
 * Destroys the ArrayList
 * 
 * Parameters:
 *  - arraylist_t * -> Pointer to the ArrayList
 */
void al_destroy(arraylist_t *) {
	free(L->data);
}

/*
 * Return the ArrayList Length
 * 
 * Parameters:
 *  - arraylist_t *L -> Pointer to the struct
 */
unsigned al_length(arraylist_t *L) {
	return L->length;
}

/*
 * Pushes an integer onto the list
 * 
 * Parameters:
 *  - arraylist_t *L -> Pointer to the struct
 *  - int item -> Integer being pushed 
 */
void al_push(arraylist_t *L, int item) {
	if(L->length == capacity) {
		L->capacity *= 2;
		int *temp = realloc(L->data, L->capacity * sizeof(int));
		if (!temp) { /* Handle Error */ }
		L->data = temp;
	}
	L->data[L->length] = item;
}

/*
 * Pops an integer off the list
 * Returns 1 on success, 0 on failure
 * 
 * Parameters:
 *  - arraylist_t *L -> Pointer to the struct
 *  - int *dest -> Pointer to the integer being removed from the list
 */
void al_pop(arraylist_t *L, int *dest) {
	if (L->length == 0) return 0;
	L->length--;
	*dest = L->data[L->length];
	return 1;
}
```

### Realloc
- Changes the size of a heap object
- `void *realloc(void *ptr, size_t);`
	- `*ptr := a pointer to the previous object`
	- `size_t := the intended size of the new object`
- `int *q = realloc(p, new_size);`
	- If `realloc` has enough space, q and p will point to the same address
	- Otherwise, q points to the new location and p is dead
- It is safest to assume that any object passed to `realloc()` has been moved
	- This can be a problem if we have multiple pointers into an object, because we need to update the all
- `p = realloc(p, size);`
	- This can be dangerous as if `realloc()` fails, it will return NULL and not free the original object
	- Therefore, you have lost the pointer to the previous memory.

### `test.c`
```C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "arraylist.h"

int main(int argc, char **argv) {
	arraylist_t A;
	al_init(&A, 4);

	for (int i = 0; i < 20; i++) {
		al_push(&A, i);
	}

	printf("List Length: %d\n", al_length(&A)); // Should show "20"

	int n;
	while(al_pop(&A, &n)) {
		n 
	}

	al_destroy(&A);

	return EXIT_SUCCESS;
}
```
