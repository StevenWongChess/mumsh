###### Big Picture

 Q: What's the difference between sh, bash, and zsh?

- A: Think about C, C++, and Modern C++

There are several different standards and implementations for shell



###### To Compile

```bash
mkdir cmake-build-debug && cd cmake-build-debug
cmake -DCMAKE_C_COMPILER=clang .
make
```



###### Reference

[POSIX Standard](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/contents.html)

- 2.1 Shell Introduction
- 2.3 Token Recognition

A minimal POSIX [shell](https://github.com/emersion/mrsh)

What is [Bash](https://www.gnu.org/software/bash/manual/bash.html#What-is-Bash_003f)?

 

##### How to tackle it by step?

###### Step 1

How to build a REPL?

- NOTE: good structure for future extensions/modifications

    ```c
    // Sample structure
    do{
    	read_line() // store into tokens
      parse() // parse tokens into command table
    	execute_command() // deal with redirection, pipe
      garbage_collection()
    } while(true)
    ```



How to read a line till `enter`?

- use `fgets`
- Command line at most 1024 chars
- use `strlen( const char *str )` to get length for iterate
- Escape char like `\033` **NOT** handled

**NO** need to handle `exit` with pipe, **BUT** how?



###### Step 2 & 3

How to parse line into tokens?

- NO `system`, `lex`, `yacc` allowed
- My idea is to avoid using `strtok` because the complex logic design



How to run command?

- with/without args should share similar idea
- Idea: `fork()` and then do `execvp()` in child
  - Must append NULL to arg list
- `exec()` family
  - *l : arguments are passed as a list of strings to the main()*
  - *v : arguments are passed as an array of strings to the main()*
  - p : path/s to search for the new running program
  - *e : the environment can be specified by the caller*
- `fork()` 
  - success, return pid for parent, 0 for child
  - fail, return -1 for parent



###### Step 4

How to open file for r/w?

- By default the file is opened with the cursor positioned at the start. Writing overwrites the bytes at the beginning of the file
  - `O_TRUNC` causes the file to be truncated if it exists
  - `O_APPEND` causes writes to append to the end of the file instead of overwrite at the start

How to replace stdin/stdout?

- use `dup` &  `dup2`
- use `open()` & `close()`
- !!! Remember to recover the stdout & stdin
- What will happen when we have both `>` and `>>` ?



###### Step 5

Too much for Bash style

- No ` ` need after `>`, `>>`, `<`  
- multi space are ignored (already covered when parse tokens)
- `>`, `>>`, `<` can happen anywhere (already covered when implement redirection)



###### Step 6

Run in parallel + cascade pipes => use for loop

A special case is redirection 

- < should only be in first pipe
- , >> and > can only be in last pipe

```bash
< 1.txt cat | grep 1
cat | grep < 1.txt 1
```



###### Step 7

unfinished command => nothing (but what about double `ctrl-d` ?)

- When using `fgets()` and `feof()`, =>  double `ctrl-d` will cause bug
- If **EOF** is entered other than at the beginning of the line, the results are unspecified.

Empty command => exit



How `fgets` work?

- return `char*` on success, nullptr on fail
- if eof, set the eof indicator 



###### Step 8

Built-in commands, **NO** need to launch new process

- `pwd` => use `getcmd`
- `cd` => use `chdir`
  - case `cd -`, (use a global variable `OLDPWD`)
  - case `cd ~`
  - case `cd` only
  - rest
  - Error when cd has more than 1 arg



###### Step 9

The key is to use `sigaction`

- sa_mask
- sa_flags
- sa_handler

differentiate incomplete command & under executing

- use `static volatile sig_atomic_t isexecuting`



###### Step 10

idea of finite state machine

- use `quote` to differ `''` and `""`
  - use `complete` to check for multi-line



###### Step 11

`>>` don’t need to be supported, but i still support it

logic is all in `complete` variable



###### Step 12

error handling



##### Feature list ( 125' / 145' )

- [x] 1, Write a working read/parse/execute loop and `exit` command (5')

- [x] 2, handle single commands without args (e.g. `ls`) (5')

- [x] 3, support commands with args (e.g. `apt-get update` or `ls -a`) (5')

- [x] 4, File IO redirection (5' + 5' + 5' + 2')

  - [x] 4.1 Output redirection by overwriting a file (e.g. `echo 123 > 1.txt`)
  - [x] 4.2 Output redirection by appending to a file (e.g. `echo 465 >> 1.txt`)
  - [x] 4.3 Input redirection (e.g. `cat < 1.txt`)
  - [x] 4.4 Combine 4.1 and 4.2 with 4.3

- [x] 5, Support for bash style redirection syntax (e.g. `cat < 1.txt 2.txt > 3.txt 4.txt`) (8')

- [x] 6, Pipes (5' + 5' + 5' + 10')

  - [x] 6.1 Basic pipe support (e.g. `echo 123 | grep 1`)

  - [x] 6.2. Run all ‘stages’ of piped process in parallel. (e.g. `yes ve482 | grep 482`)

  - [x] 6.3 Extend 6.2 to support requirements 4 and 5 (e.g. `cat < 1.txt 2.txt | grep 1 > 3.txt`)

  - [x] 6.4 Extend 6.3 to support arbitrarily deep “cascade pipes” (e.g. `echo 123 | grep 1 | grep 1 | grep 1`)

    Note: the sub-processes must be reaped in order to be awarded the marks

- [x] 7, Support `CTRL-D` (similar to bash, when there is no/an unfinished command) (5')

- [x] 8, Internal commands (5' + 5' + 5')

  - [x] 8.1 Implement pwd as a built-in command
  - [x] 8.2 Allow changing working directory using `cd`
  - [x] 8.3 Allow pwd to be piped or redirected as specified in requirement 4

- [x] 9, Support `CTRL-C` (5' + 3' + 2' + 10')

  - [x] 9.1 Properly handle `CTRL-C` in the case of requirement 4
  - [x] 9.2 Extend 9.1 to support subtasks 6.1 to 6.3
  - [x] 9.3 Extend 9.2 to support requirement 7, especially on an incomplete input
  - [x] 9.4 Extend 9.3 to support requirement 6

- [x] 10, Support quotes (5' + 2' + 3' + 5')

  - [x] 10.1 Handle single and double quotes (e.g `echo "de'f' ghi" '123"a"bc' a b c`)
  - [x] 10.2 Extend 10.1 to support requirement 4 and subtasks 6.1 to 6.3
  - [x] 10.3 Extend 10.2 in the case of incomplete quotes (e.g. Input `echo "de`, hit `enter` and input `cd"`)
  - [x] 10.4 Extend 10.3 to support requirements 4 and 6, together with subtask 9.3

- [x] 11, Wait for the command to be completed when encountering `>`, `<`, or `|` (2' + 3')

  - [x] 11.1 Support requirements 3 and 4 together with subtasks 6.1 to 6.3
  - [x] 11.2 Extend 11.1 to support requirement 10

- [ ] 12, Handle errors for all supported features (10')

  Note: a list of test cases will be published at a later stage. Marks will be awarded based on the number of cases that are correctly handled, i.e. if only if:

  - A precise error message is displayed (e.g. simply saying “error happened!” is not enough); 

  - The program continues executing normally after the error is identified and handled;

- [ ] 13, A command ending with an `&` should be run in background (10')

  - [ ] 13.1 For any background job, the shell should print out the command line, prepended with the job ID and the process ID (e.g. if the two lines `/bin/ls &` and `/bin/ls | cat &` are input the output could be the two lines `[1] (32757) /bin/ls &` and `[2] (32758) (32759) /bin/ls | cat &`)
  - [ ] 13.2 Implement the command `jobs` which prints a list of background tasks together with their running states (e.g. in the previous case output the two lines `[1] done /bin/ls &` and `[2] running /bin/ls | cat &`)




Milestone 1: 1-5

Milestone 2: 6-9

Final: 10-13
