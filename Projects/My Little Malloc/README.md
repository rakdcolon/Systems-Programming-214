# Project 1: My Little Malloc Project 
## Rohan Karamel - rak218

Welcome to my "My Little Malloc" Project.
I attempted this on my own so forgive me for terribly written code. (I had no incentive to write better). Let's just say it was understandable to me at the time.

### `malloc()` and `free()`
So starting with the implementation of `malloc()` and `free()`. My original plans were to have a struct to hold the metadata/header for each malloc. However, I realized the variables were synonomous with each other. For example, if I knew the size, I could tell if the memory was free by checking if size was greater than 0. I could also be able to infer where the next chunk of memory is by just looking at the current location of memory and adding that header size to it. So because of that, I got rid of structs and dealt with just pointers for memory, is_free, and next. I made a few design choices, like returning errors but not halting the program and returning null pointers. This was mainly because malloc does this so I decided to just go with this for the "realism", I guess. 

### `memtest.c`
Not much to say here, I chose some really standard tests, if you check out the file, there are some more details on the tests and what they are doing. Notably, the edge cases I was mainly testing for was filling the entire memory and freeing portions of it. Seeing if it coalesces the chunks well.

### `memgrind.c`
I'm very satisfied with the results of memgrind(at least I think I am). The program was so fast the average time per iteration was so small I don't even know the scientific name for those types of time(decaseconds? no clue). This went well for the most part, had a fun time learning the time header functions. Very cool!

### Conclusions
This was a very fun project. I will never do a project again solo though. This was too much work and I am too stressed! Thanks anyway for a very interesting and satisfying project.