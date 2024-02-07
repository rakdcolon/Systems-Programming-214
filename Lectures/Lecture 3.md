---
tags:
  - CS
  - Systems
  - Lecture
date: 2024-01-24
---
***
# Systems Programming
# Professor David Menendez
# Lecture 3: Additional C Review

***
## Declarations and Definitions
### Declaration
- ==Declaration says what somethings is, but does not create it==
	- e.g. a function prototype declares that a function exists
	- `int compare(double, double);`
	- `extern int some_global;`
- Things that are declared but not defined can be referred to, the linker will be responsible for connecting the references to the actual definition (which must be provided somewhere)

  ### Definitions
  - ==Definitions create the thing being described==
	  - e.g. function definition, global variable definition
```C
int compare(double a, double b) {
	return a < b;
}
```
- A given program can declare the same thing many times, but ==it must be defined exactly once==
### Every Declaration is an Implicit Declaration
- We often do not need to provide an explicit declaration
	- When would we?
		- Reference something defined outside the current source file
			- e.g. calling a function in the standard library
		- Reference something defined later in the same source file
			- e.g. organizing code, mutual recursion

### Header Files
- Special kind of C source file that contains declarations
- Usually end in `.h`
	- e.g. `stdio.h, stdlib.h, unistd.h, string.h, pthread.h`
- Header files should only contain declarations
	- Function Prototypes
	- Extern variables
	- Type declarations
- Typically, a header file will be included into multiple C files
	- `#include "myheader.h"`
		- Looks for files in the current directory
	- `#include <standardheader.h>`
		- Looks for this file in the "standard" location(s)

### The Preprocessor
- Lines that begin with # are preprocessor directives
- ==The C preprocessor is a separate program that runs through your source code and makes changes prior to compilations==
- The `#include`directive literally includes the contents of the specified file
	- All declarations in an included header file become part of the source file after preprocessing
	- Any file can be included, not just headers
- `#define MACRO "hello"`
	- After this line in the source, the preprocessor will replace `MACRO` every time it appears in the code with `"hello"`
- `#define ARRAYLENGTH 128`
	- `int x[ARRAYLENGTH]` -> `PREPROCESSOR`  -> `int x[128]`
- We can write macros with parameters
	- `#define SQUARE(X) X * X`
	- `#define FOREACH(P) for (; P; P = P->next)`

### Memory Objects
- All data is in memory
- An object is a place in memory to store data
- Every variable is associated with an object
	- The compiler keeps track of which objects foes with each variable
- Not every object has a variable associated with it
- To make use of an object, we need to know its location (pointer) and we need to know its type
- We can create objects that are not associated with a variable
	- `malloc()` allocates an object of a specified size
		- Dynamic allocation
		- A static analysis cannot tell us how many times we will call `malloc()` or what arguments we will five it (in general)
- How do we know how many bytes to request?
	- Types like `int` do not have one specified size. It is architecture dependent
	- We will use `sizeof()` to learn the number of bytes needed for a type
- What kind of pointer does `malloc()` return?
	- `void *malloc(size_t);`
	- Returns a "void pointer." A pointer of no specified type
	- `intt *array = (int *) malloc(arraysize * sizeof(int));`
