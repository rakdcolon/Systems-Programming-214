# Project 1: My Little Malloc Project 
## Rohan Karamel - rak218

Welcome to my "My Little Malloc" Project. This project was a solo endeavor, and I learned a lot about memory management and testing strategies.

### Design Choices and Data Structures

The project primarily revolves around the implementation of `malloc()` and `free()`. Initially, I planned to use a struct to hold the metadata/header for each malloc. However, I realized that the variables were synonymous with each other. For instance, knowing the size allowed me to infer whether the memory was free (size > 0) and where the next chunk of memory was (current location + header size). As a result, I decided to simplify the design by eliminating structs and using pointers for memory, is_free, and next. 

One of the design choices I made was to return errors without halting the program and to return null pointers. This decision was influenced by the behavior of the standard `malloc()` function, and I wanted to maintain that "realism".

### Testing Strategy

The testing strategy was designed to cover both standard and edge cases. The `memtest.c` file contains a series of tests that check the functionality of the malloc and free implementations. The tests include scenarios where the entire memory is filled and then portions of it are freed. This allows us to verify if the coalescing of chunks is working correctly.

In addition to the standard tests, I also included some non-trivial tests to push the boundaries of the implementation and ensure its robustness under different conditions.

### `memgrind.c`

The `memgrind.c` file contains all the required tests and some additional non-trivial tests. The program runs quickly, with the average time per iteration being extremely small. I had a fun time learning the time header functions while working on this part of the project.

### Conclusions

This project was both challenging and rewarding. It provided a deep dive into memory management and testing strategies. However, I've learned that solo projects of this scale can be quite stressful, and I look forward to collaborating with others on future projects. Despite the challenges, I found the project to be very interesting and satisfying.