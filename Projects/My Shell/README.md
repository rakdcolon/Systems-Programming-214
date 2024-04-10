# My Shell Project

My name is Rohan Karamel and my NetID is rak218. This project is a simple implementation of a shell in C. It supports basic commands, input/output redirection, piping, and background processes. This project was (unwillingly) done solo. 

## Testing

Testing is an essential part of any software development process. For this shell project, we have created a series of test cases that cover a wide range of shell commands and features. These tests are designed to ensure that the shell behaves as expected and can handle a variety of inputs and conditions.

### Test Cases

Our test cases are designed to cover the following areas:

1. Basic shell commands: These tests ensure that the shell can execute basic commands like `ls`, `pwd`, `cd`, etc.

2. Input/Output redirection: These tests check if the shell correctly handles input and output redirection using `>`, `<`, and `>>`.

3. Piping: These tests verify that the shell can pipe the output of one command as the input to another using `|`.

4. Background processes: These tests check if the shell can correctly start processes in the background using `&`.

5. Error handling: These tests ensure that the shell can gracefully handle errors and unexpected inputs.

6. Grep: I'll be honest and upfront to speed up grading: `grep` commands do not work. This is most likely a result of me handling execution within the main class and thus creating many piping holes! I have been testing this for hours and was unfortunately not able to fix this bug. I apologize.

### Running the Tests

To run the tests, use the provided test script to start My Shell:

```sh
make
./mysh
```

then run the commands in the test.txt file provided.

I hope you enjoy.