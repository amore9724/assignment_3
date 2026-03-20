#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "A3.h"

/*
*   NOTE: A history of this code is available on a private GitHub repository.
*   This repository can be made available upon request.
*/

#define MAXLINE 4096
#define PROGRAMNAME "countnames"

void read_from_pipe(int fd, char **arr, int *nused, int *count) {
    MessageHeader header;
	while (read(fd, &header, sizeof(header)) > 0) {
		int i;
		switch (header.type) {
			case TYPE_NAMECOUNT: {
				NameCountData dataNC;
				read(fd, &dataNC, sizeof(NameCountData));
				if ((i = check_in(dataNC.name, arr)) != -1) {
					count[i] += dataNC.count;
				} else {
					arr[*nused] = strdup(dataNC.name);  // add name to array
					count[*nused] = dataNC.count;        // set its count
					(*nused)++;                          // advance to next slot
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

		char **nused = calloc(2048, sizeof(char*));	// Array of names
		int *count = calloc(2048, sizeof(int));     // Total count array
		int nused_count = 0;
		// for each input file
		// fork and then exec the countnames program with the file as an argument
		int fds[MSIZE][2];     // Stores two ends of parent pipe
		for (int j = 1; j < i; j++) {
			if (pipe(fds[j]) == -1) {	// Create pipe
				perror("Error creating pipe");	// Exit program if pipe creation failed
				return 1;
			}
			pid_t pid = fork(); // Create new process for file
			if(pid == 0)	// Line of code for the child to execute.
			{
				close(fds[j][0]);
				dup2(fds[j][1], STDOUT_FILENO);	// Set stdout to the write end of the pipe temporarily.
				close(fds[j][1]);
				char *child_argv[] = {args[0], args[j], NULL};
				execvp(child_argv[0], child_argv);	// Execute countnames.c
				perror("execvp failed");
				exit(1);
			}
			close(fds[j][1]);
		}
		for (int j = 1; j < i; j++) {
			read_from_pipe(fds[j][0], nused, &nused_count, count);
			close(fds[j][0]);
		}
		/* parent wait until all children are finished */
		while (wait(NULL) > 0) {}
		printf("%% ");
		nprinter(nused, count);
		free(nused);
		free(count);
	}
	exit(0);
}
