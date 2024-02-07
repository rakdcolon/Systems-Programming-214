***
# Systems Programming
# Professor David Menendez
# Lecture 4: Objects and Pointers
***

### Objects
- An object is an 
- Any object can be referred to indirectly, using a pointer 
    - For indirect references, the value of the pointer says its address and the type of the pointer indicates the type of the object
- Only static and stack objects can be referred to directly (without a pointer)
- To use an object we need to know its address and its type
    - The compiler tracks the types and locations of variables

### Static Objects
- global variables
- string literals
- code
- They exist the entire time the program runs. They are neither created nor destroyed

### Stack Objects
- local variables
- other information about functions used by the runtime
- They are created when the call begins and destroyed when the call ends

### Heap Objects
- created by calls to malloc()
- destroyed by calls to free()
- not associated with a variable
- The size can be decided at run-time

### Pointer
- An address with a type 
    - type is only known during compilation
- Pointer Arithmetic
    - If p points to something then p+1 points to the “next thing” in memory
    - The next address we gets depends on the type of p
    - If p points to something then p-1 points to the “previous thing” in memory
    - Pointer arithmetic is only valid with pointers into an array, where the indexing leaves us within the array

### Special Pointers
- NULL is a special pointer
    - NULL represents “no address”
    - attempting to reference NULL will cause a memory error
- void pointers (void *)
    - pointers with no type
    - All we can do with these are to
        - compare for equality
        - cast to some other pointer type
        - use with various functions
### Arrays vs. Array Variables
- When we declare an array variable, we must specify the dimension
- `int y[20]; // y directly refers to an array object`
    - y by itself is effectively a pointer
    - `*a = 5;` is the same as `a[0] = 5;`
- We cannot reassign y to refer to a different array
- We can make pointers point to arrays
```C
int *p = y; // p is a pointer variable. It initially holds the address of y
// We can still use array indexing with p
p[1] = 20; // same as a[1] = 20;
```
- This sort of indexing is only defined when
    - the pointer points to an array
    - the index is within the bounds of the array

### Pointer vs. Array Variables
- Array variables always point to a specific array
- Pointer variables can be changed at any time
- Pointer variables store their address at run-time
- Array variables have a fixed address
- `&a` is the same as `a`
- `&p` is a pointer to a pointer variable

### Heap
- Use `malloc()` to create objects on the heap
- `void *malloc(size_t);`
- We tell malloc how many bytes we need
- Malloc allocates an object and returns a pointer
    - or returns NULL if it can’t get enough space
- `int *p = malloc(n * sizeof(int));`
    - We know that p points to an array of `n` ints (or NULL)
    - The compiler does not know this, but it trusts us
- If malloc succeeded, we can treat p like any other array
- We aren’t required to explicitly cast the pointer from `malloc()`
    - `int *p = malloc(size); // ok`
    - `int *p = (int *) malloc(size); // ok`
- Nothing protects you from calling `malloc()` with an incoherent size
```C
struct node {
    data_t payload;
    struct node *next;
};

struct node *head = NULL;

void push(data_t item) {
    struct node *new = malloc(sizeof(struct node));
    (*new).payload = item;
    new->payload = item; // Equivalent to line above
    new->next = head;
    head = new;
}

int pop(data_t *dest) {
    if (head == NULL) return 0;
    struct node *oldhead = head;
    *dest = head->payload;
    head = head->next;
    free(oldhead);
}
```
- `free()` deallocates an object on the heap

