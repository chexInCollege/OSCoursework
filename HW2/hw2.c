// Alex Groves
// Homework 2, CSCI 340-01

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   // for fork()
#include <sys/wait.h> // for wait()

int main(int argc, char *argv[])
{
    if ( argc < 2 ) {
        fprintf(stderr, "usage: hw2 <command>\n");
        exit(1);
    }

    // fork the process
    int rc = fork();

    // child process runs the command
    if( rc <  0 ) { // error!
	fprintf(stderr, "a forking error has occurred. oops!");
    } else if( rc == 0 ) { // we're the child process
	// create a new argv array excluding argv[0]
	char *newargv[argc];

	for( int i = 1; i < argc; i++ )
	    newargv[i-1] = argv[i];
	newargv[argc-1] = NULL;

	// finally, execute the new command
	execvp(newargv[0], newargv);
    } else { // we're the parent process
	// wait for child process to complete
	wait(NULL);
    }

    return 0;
}

