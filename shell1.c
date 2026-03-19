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
	char	buf[MAXLINE];
	char *args[100];

	printf("%% ");	/* print prompt (printf requires %% to print %) */
	while (fgets(buf, MAXLINE, stdin) != NULL) {
		if (buf[strlen(buf) - 1] == '\n')
			buf[strlen(buf) - 1] = 0; /* replace newline with null */


		// Tokenize the input
		/* 
			./countnames names.txt names1.txt

			args: [./countnames, names.txt, names1.txt, NULL]

		*/
		int i = 0;
		char *token = strtok(buf, " ");
		while( token != NULL)
		{
			args[i++] = token;
			token = strtok(NULL, " ");
		}
		args[i] = NULL;

		// for each input file
		// fork and then exec the countnames program with the file as an argument

		for (int j = 1; j < i; j++) {

			pid_t pid = fork();

			if(pid == 0)
			{
				char *child_argv[] = {args[0], args[j], NULL};
				execvp(child_argv[0], child_argv);
				exit(1);
			}
		}


		

		/* parent wait until all children are finished*/
		while (wait(NULL) > 0) {}
		printf("%% ");
	}
	exit(0);
}


//  #include "apue.h"
// #include <sys/wait.h>

// int
// main(void)
// {
// 	char	buf[MAXLINE];	/* from apue.h */
// 	pid_t	pid;
// 	int		status;

// 	printf("%% ");	/* print prompt (printf requires %% to print %) */
// 	while (fgets(buf, MAXLINE, stdin) != NULL) {
// 		if (buf[strlen(buf) - 1] == '\n')
// 			buf[strlen(buf) - 1] = 0; /* replace newline with null */

// 		if ((pid = fork()) < 0) {
// 			err_sys("fork error");
// 		} else if (pid == 0) {		/* child */
// 			execlp(buf, buf, (char *)0);
// 			err_ret("couldn't execute: %s", buf);
// 			exit(127);
// 		}

// 		/* parent */
// 		if ((pid = waitpid(pid, &status, 0)) < 0)
// 			err_sys("waitpid error");
// 		printf("%% ");
// 	}
// 	exit(0);
// }
