---
tags:
  - CS
  - Systems
  - Lecture
date: 2024-01-22
---
***
# Systems Programming
# Professor David Menendez
# Lecture 2: More C Introduction and Review

***
### The Type System in C is Minimal
- Primarily intended to guide code generation
	- e.g. size of data, which instructions to use
- Provides some safety
	- Errors are detected at compile-time

### Implicit Casting
- `int x = ...;`
- `x = x / 1.6;`
	- This implicitly casts x's value to float (or double)

### Basic Types: `integers, char, float, double`

### Arrays
- A contiguous sequence of values of some type
- Array variables have a type and dimension
	- A type is used to guide code generation for indexing
	- A dimension is used to guide allocation
- `type array_name[dimension]`
	- e.g. `int arr[4]`

### Strings
- C does not have a string type
- Instead, we use arrays of characters containing a terminator
	- Terminator character is the char with value 0
		- `0, \0`
		- This is distinct from `'0'`
	- Why have a terminator?
		- Allows us to store strings of various lengths in an array
	- `char \*s = "string literal";`
		- s refers to a specific constant string in memory
	- `char s[] = "foo";`
		- Declares and initializes a char array containing this string because we did not provide a dimension, s will have length 4 (length of string plus one for the terminator).
		- Equivalently, `char s[] = {'f', 'o', 'o', '\0'};`
	- `char t[16] = "foo";`
		- Here, t's dimension is 16 but only the first 4 chars are initialized

### Enum
- Useful for types with a discrete set of values
- `enum direction { north, south, east, west };`
	- Declares a type called "enum direction"
	- Declares four "enumerators"
- `enum direction heading = north;`
- `switch (heading) { case north: ...; case south: ...; }`
- Enums are implemented using integers
- Useful to document what you are doing

### Struct
- Used to bundle multiple values, possibly of different types
```C
// Creation of a stuct called point which holds two double values
struct point {
	double x;
	double y;
};

// Initalizing a point with a name and values for both doubles
struct point location;
location.x = 0.0;
location.y = 25;

// Shorthand for creating a point with x = 1, y = 2
struct point other_location = { 1, 2 };
```
- struct fields can be any type
```C
struct triangle {
	struct point verticees[3];
	enum color background;
	...
};
```

### Unions
- Allows us to declare variables that could have different types
```C
union intOrFloat {
	int i;
	float f;
};
```
- The declaration resembles a struct, but only one element is present at a time
- union intOrFloat x;
- `x.i = 15;`
- `x.f = 2.3; // This overwrites the 15 in x.i`
	- Note: it is undefined what happens if you use the wrong variant to read from the union
- `x.i = 15;`
- `float y = x.f; // undefined what value y gets`
- Unions have very limited uses

### Typedef
- Lets us give a new name to an existing type
- `typedef existing_type new_name;`
- `typedef struct point point`
- `typedef double vector[3];`
	- This means that vector is a name of the type of arrays with 3 doubles
	- `vector v;` $\iff$ `double v[3];`

### Pointers
- Every datum is stores somewhere in memory
	- Every location in memory has an address
- A pointer is a value that represents an address
- Implications:
	- Every variable has an address
		- We can obtain that address using `&`
	- Every struct field and array element has an address
		- `&array[4]`
		- `&p.x`
	- Every function has an address
		- Writing a function name with no arguments obtains its address
- When we work with pointers, we need to know the type of data they point to
- Pointer values are usually typed
- `int *p; // p holds an address that should point to an integer`
- `int i;`
- `p = &i; // now p points to the location of i`
