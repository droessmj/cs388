/* sigint.c: suppress sigints */

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>

#define SIG_INT_MAX 5

uint16_t SigInts = 0;
time_t	 StartTime;
pid_t	 ChildProcess;

void
handle_sigint(int signum)
{
    fprintf(stderr, "[SIGINT] Elapsed time: %ld\n", time(NULL) - StartTime);
    kill(ChildProcess, SIGTERM);
    exit(EXIT_FAILURE);
}

void
handle_sigalrm(int signum)
{
    fprintf(stderr, "[SIGALRM] Elapsed time: %ld\n", time(NULL) - StartTime);
    kill(ChildProcess, SIGTERM);
    exit(EXIT_FAILURE);
}

int
main(int argc, char *argv[])
{
    uint32_t time_delta;
    pid_t result;
    int status;

    if (argc < 2) {
    	fprintf(stderr, "usage: %s time_delta command...\n", argv[0]);
    	exit(EXIT_FAILURE);
    }
    
    ChildProcess = fork();
    switch (ChildProcess) {
    case -1:
    	fprintf(stderr, "Could not fork: %s\n", strerror(errno));
    	break;
    case  0:
    	execvp(argv[2], &argv[2]);
    	_exit(EXIT_FAILURE);
    	break;
    default:
    	break;
    }
    
    signal(SIGINT , handle_sigint);
    signal(SIGALRM, handle_sigalrm);
    
    time_delta = strtol(argv[1], NULL, 10);
    StartTime  = time(NULL);
    alarm(time_delta);

    do {
    	result = waitpid(ChildProcess, &status, 0);
    } while (result < 0);

    printf("Child process %d run for %ld seconds, returned exit code %d\n",
    	ChildProcess, time(NULL) - StartTime, WEXITSTATUS(status));

    return (EXIT_SUCCESS);
}
