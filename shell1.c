#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "A3.h"

/*
*   NOTE: A history of this code is available on a GitHub repository.
*   This repository can be made available upon request.
*/

void read_from_pipe(int fd, char **arr, int *pos, int *count) { // This function reads the data from the pipe.
    MessageHeader header;
	while (read(fd, &header, sizeof(header)) > 0) {	// Reads from the message header sent by child process.

		switch (header.type) {	// This switches based on what the header type is.
			case TYPE_NAMECOUNT: {
				NameCountData dataNC;				// Initializes struct to be used when reading data from pipe.
				read(fd, &dataNC, sizeof(NameCountData)); // Reads the data.
				int i;
				if ((i = check_in(dataNC.name, arr)) != -1) {
					count[i] += dataNC.count;		// This adds the count to the name if the name is already there.
				} else {
					arr[*pos] = strdup(dataNC.name); // This adds a name to the array.
					count[*pos] = dataNC.count; // This sets the count to the name in the array.
					(*pos)++;					  // This moves the pointer to the next slot.
				}
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
}


int main(int argc, char* argv[])
{
	//raise(SIGSTOP); // Comment if unneeded, this is for debugging purposes.
	char	buf[MAXLINE];
	char *args[MAXARGS];

	printf("%% ");	/* print prompt (printf requires %% to print %) */

	while (fgets(buf, MAXLINE, stdin) != NULL) {	// Read argument from stdin.
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


		/* Set up the arrays to be used later by the program */
		char **nused = calloc(MAXLINE, sizeof(char*));	// Array of names
		int *count = calloc(MAXLINE, sizeof(int));     // Total count array
		int nused_count = 0;

		// For each input file, fork and exec the countnames program with the file as the argument.

		int fds[MSIZE][2];     // Stores two ends of parent pipe


		for (int j = 1; j < i; j++) {	// Loop where all processes are forked.
			if (pipe(fds[j]) == -1) {	// Create pipe
				perror("Error creating pipe");	// Exit program if pipe creation failed
				return 1;
			}
			pid_t pid = fork(); // Create new process for each file.

			if(pid == 0)	// Line of code for the child to execute.
			{
				close(fds[j][0]);	// Closes unused end of pipe.
				dup2(fds[j][1], STDOUT_FILENO);	// Set stdout to the write end of the pipe.
				close(fds[j][1]);	// Closes write end of pipe.
				char *child_argv[] = {args[0], args[j], NULL};	// Creates arguments to pass to execvp for child process to execute.
				execvp(child_argv[0], child_argv);	// Execute countnames.c

				/* The child process should not get here, if it did, then something is wrong. */

				perror("execvp failed");
				exit(1);
			}
			close(fds[j][1]);	// Closes final write end of pipe.
		}

		// This is for the parent program to read the data sent to it through the pipe.
		for (int j = 1; j < i; j++) {
			read_from_pipe(fds[j][0], nused, &nused_count, count);	// Reads from the read end of the pipe at j.
			close(fds[j][0]);											// Closes the read end of the pipe at j.
		}

		/* Parent waits until all children are finished */

		while (wait(NULL) > 0) {}
		printf("%% ");
		nprinter(nused, count);	// Prints the names to output.
		free(nused);			// Removes nused array.
		free(count);			// Removes count array.
	}
	exit(0);
}
