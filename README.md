# Names

Arya More, Michael Chen

# Compilation

First do
```cc countnames.c helper_functions.c -o countnames -Wall -Werror``` for countnames

Then do ```cc shell1.c helper_functions.c -o shell1 -Wall -Werror``` for shell1.

# Running the program

First run ./shell1.
If you want to test countnames, copy and paste this to the terminal to test every text file in test.
```./countnames test/names.txt test/names1.txt test/names2.txt test/names_long.txt test/names_long_redundant.txt test/names_long_redundant1.txt test/names_long_redundant2.txt test/names_long_redundant3.txt test/namesB.txt```

# Lessons Learned:

1.) Proper order of fds in dup2 function(was originally swapped).

2.) Closing unused ends of pipe to prevent deadlock

3.) How to deal with pipes from multiple processes sending to one parent while also having parallelism(just have a 2D array of pipes with a read and write end).

4.) Moving all unrelated declarations from countnames.c to a header file(as they were all in countnames originally which caused problems when trying to call the same methods in shell1.c), and making a separate .c file with their definitions.

5.) How to use the gdb debugger when dealing with processes.

# Acknowledgments

https://stackoverflow.com/questions/36221038/how-to-debug-a-forked-child-process-using-clion