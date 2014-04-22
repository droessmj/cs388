/* sigchld.c: wait for child */

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>

time_t	 StartTime;
pid_t	 ChildProcess;

void
handle_sigchld(int signum)
{
    int status;

    fprintf(stderr, "[SIGCHLD] Elapsed time: %ld\n", time(NULL) - StartTime);

    if (waitpid(ChildProcess, &status, 0) < 0) {
    	fprintf(stderr, "waitpid error: %s\n", strerror(errno));
    }
    
    printf("[SIGCHLD] Process %d exited with status %d\n", ChildProcess, WEXITSTATUS(status));
    ChildProcess = 0;
}

int
main(int argc, char *argv[])
{
    if (argc < 2) {
    	fprintf(stderr, "usage: %s command...\n", argv[0]);
    	exit(EXIT_FAILURE);
    }
    
    ChildProcess = fork();
    switch (ChildProcess) {
    case -1:
    	fprintf(stderr, "Could not fork: %s\n", strerror(errno));
    	break;
    case  0:
    	execvp(argv[1], &argv[1]);
    	_exit(EXIT_FAILURE);
    	break;
    default:
    	break;
    }
    
    signal(SIGCHLD, handle_sigchld);
    StartTime = time(NULL);

    while (ChildProcess) {
    	printf("Waiting for child...\n");
    	sleep(1);
    }

    return (EXIT_SUCCESS);
}
