/* sigint.c: suppress sigints */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <signal.h>
#include <unistd.h>

#define SIG_INT_MAX 5

uint16_t SigInts = 0;
time_t	 TimeLimit;
time_t	 StartTime;

void
handle_sigint(int signum)
{
    fprintf(stderr, "[SIGINT] Elapsed time: %ld\n", time(NULL) - StartTime);

    if (++SigInts >= SIG_INT_MAX) {
    	fprintf(stderr, "[SIGINT] SIG_INT_MAX (%d) reached, ABORT!\n", SIG_INT_MAX);
    	exit(EXIT_FAILURE);
    }
}

int
main(int argc, char *argv[])
{
    uint32_t time_delta;

    if (argc != 2) {
    	fprintf(stderr, "usage: %s time_delta\n", argv[0]);
    	exit(EXIT_FAILURE);
    }

    time_delta = strtol(argv[1], NULL, 10);
    StartTime  = time(NULL);
    TimeLimit  = StartTime + time_delta;
    
    signal(SIGINT, handle_sigint);

    while (time_delta > 0) {
	sleep(time_delta);
	time_delta = TimeLimit - time(NULL);
    }
    printf("Total Elapsed Time: %ld\n", time(NULL) - StartTime);

    return (EXIT_SUCCESS);
}
