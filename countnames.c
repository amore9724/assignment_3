/*
This ver is pasted from assignment 2
*/
#include <sys/types.h>
#include <unistd.h>
#include "A3.h"


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

int main(int argc, char *argv[]) /* int argc = argument count
                                  * char *argv[] = string array containing the actual arguments passed.*/

{
    if (argc == 1) // If no file was provided
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
    char *names[MSIZE] = {0};  // TODO: Move to shell1.c. This stores all the names and their occurences in the file.
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
    for (int i = 0; nused[i] != 0; i++) { // Send data to parent
        NameCountData ncd;
        strcpy(ncd.name, nused[i]);
        ncd.count = count[i];
        write_struct_namecount(STDOUT_FILENO, &ncd);
    }
    fflush(stdout);
    clnup(names, nused); // This will free the allocated memory.
    return 0;
}