/*
This ver is pasted from assignment 2
*/

#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "A3.h"

#define MSIZE 10001 // Maximum number of lines in the program(empty or non empty)
#define MNAME 101   // Maximum number of names.
#define MLINE 31    // Maximum number of characters in a line.
/*
 *   NOTE: A history of this code is available on a private GitHub repository.
 *   This repository can be made available upon request.
 */

void write_struct_namecount(int fd, NameCountData *data) {
    MessageHeader header;
    header.type = TYPE_NAMECOUNT;
    header.size = sizeof(NameCountData);

    write(fd, &header, sizeof(header)); // Write the header
    write(fd, data, sizeof(NameCountData)); // Write the payload (name counts)
}

int check_in(char *a, char *b[])
{ // Checks if a string is in the file.
    int rval = -1;
    for (int i = 0; b[i] != 0; i++)
    {
        if (strcmp(b[i], a) == 0)
        {
            rval = i;
            break;
        }
    }
    return rval;
}

void ncount(char *arr[], char *nused[], int count[])
{ // Counts the number of times a string occurs in a file.
    int j = 0, k;
    for (int i = 0; arr[i] != 0; i++)
    {
        k = check_in(arr[i], nused);
        if (k == -1)
        {
            nused[j] = strdup(arr[i]); /* This allocates memory on the heap to store the string
                                        which needs to be freed later. */
            count[j++]++;
        }
        else
        {
            count[k]++;
        }
    }
}

void nprinter(char *nused[], int count[])
{ // Prints a string and the amount of times it occurs.
    for (int i = 0; nused[i] != 0; i++)
    {
        printf("%s: %d\n", nused[i], count[i]);
    }
}

void clnup(char *a1[], char *a2[])
{ // Frees allocated memory.
    int i;
    for (i = 0; a1[i] != 0; i++)
    {
        free(a1[i]);
    }
    for (i = 0; a2[i] != 0; i++)
    {
        free(a2[i]);
    }
}

int main(int argc, char *argv[]) /* int argc = argument count
                                  * char *argv[] = string array containing the actual arguments passed.*/

{

    /* Create a PID.out for this child process
    and then set stdout to this PID.out */
    char filename[64];
    sprintf(filename, "output/%d.out", getpid());

    if (freopen(filename, "w", stdout) == NULL)
    {
        perror("freopen failed");
        return 1;
    }

    /* Create a PID.err for this child process
    and then set stderr to this PID.err */
    char fileerr[64];
    sprintf(fileerr, "output/%d.err", getpid());

    if (freopen(fileerr, "w", stderr) == NULL)
    {
        perror("freopen failed");
        return 1;
    }

    if (argc == 1)
    {
        puts("No file provided, exiting."); // This informs the user that there is no file.
        return 0;
    }

    FILE *f = fopen(argv[1], "r");
    if (f == NULL)
    {
        fprintf(stderr, "Error: Child %d could not open file %s and is exiting.\n", getpid(), argv[1]);
        return 1;
    }

    int i = 0, lnum = 0;
    char namebuf[MLINE] = {0}; // This buffer temporarily stores a line in the file.
    char *names[MSIZE] = {0};  // This stores all the names and their occurences in the file.
    while (fgets(namebuf, MLINE, f))
    {
        lnum++;
        char *tok = strtok(namebuf, "\n"); // Uses "\n" character to tokenize string.
        if (tok == NULL)
        {
            fprintf(stderr, "Warning: Line %d in Child %d is empty.\n", lnum, getpid());
            continue;
        }
        names[i++] = strdup(tok); /* This allocates memory on the heap to store the string,
                                      which needs to be freed later. */
    }

    fclose(f);

    int count[MNAME] = {0};   // Contains the number of times a name occurs in the file.
    char *nused[MNAME] = {0}; // Contains the number of unique names used in the file.
    ncount(names, nused, count);
    nprinter(nused, count);
    fclose(stdout);
    fclose(stderr);
    clnup(names, nused); // This will free the allocated memory.
    return 0;
}