#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "A3.h"

/*
*   NOTE: A history of this code is available on a private GitHub repository.
*   This repository can be made available upon request.
*/

#define MAXLINE 4096
#define PROGRAMNAME "countnames"

void read_from_pipe(int fd) {
    MessageHeader header;
    // Loop on read to ensure all header bytes arrive
    if (read(fd, &header, sizeof(header)) <= 0) {
        // Handle error or end of pipe
        return;
    }
    switch (header.type) {
        case TYPE_NAMECOUNT: {
            NameCountData dataNC;
            read(fd, &dataNC, header.size);
            // Process NameCountData data
            printf("Received NameCountData: name=%s, count=%d\\n", dataNC.name, dataNC.count);
           break;
        }
        case TYPE_B: {
            // This case is for a possible extension in future to process other Struct data
           break;
        }
        default:
            // Handle unknown type error
            fprintf(stderr, "Unknown message type received: %d\\n", header.type);
    }
}


int main(int argc, char* argv[])
{
	int fd[2];     // Stores two ends of parent pipe
	char	buf[MAXLINE];
	char *args[100];

	printf("%% ");	/* print prompt (printf requires %% to print %) */
	while (fgets(buf, MAXLINE, stdin) != NULL) {
		if (buf[strlen(buf) - 1] == '\n')
			buf[strlen(buf) - 1] = 0; /* replace newline with null */

		int i = 0;
		char *token = strtok(buf, " ");
		while(token != NULL)
		{
			args[i++] = token;
			token = strtok(NULL, " ");
		}
		args[i] = NULL;
		// for each input file
		// fork and then exec the countnames program with the file as an argument

		for (int j = 1; j < i; j++) {
			pid_t pid = fork(); // Create new process for file
			if(pid == 0)	// Line of code for the child to execute.
			{
				char *child_argv[] = {args[0], args[j], NULL};
				execvp(child_argv[0], child_argv);	// Execute countnames.c
				exit(1);
			}

		}

		/* parent wait until all children are finished*/
		while (wait(NULL) > 0) {}
		printf("%% ");
	}
	exit(0);
}
