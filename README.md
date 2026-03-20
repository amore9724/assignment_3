# TODO

Have some way of communication between countnames and shell1



Move name list (and possibly other logic) to shell1

# Compilation

First do
```cc countnames.c helper_functions.c -o countnames -Wall -Werror``` for countnames

Then do ```cc shell1.c helper_functions.c -o shell1 -Wall -Werror``` for shell1.

# Running

First run ./shell1.

If you want to test countnames, copy and paste this to the terminal to test every text file in test.
```./countnames test/names.txt test/names1.txt test/names2.txt test/names_long.txt test/names_long_redundant.txt test/names_long_redundant1.txt test/names_long_redundant2.txt test/names_long_redundant3.txt test/namesB.txt```