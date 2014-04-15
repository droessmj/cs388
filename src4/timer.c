/* timer.c execute program and display execution time */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>
    
void
print_timediff(const char *fmt, const struct timeval *start, const struct timeval *stop)
{
    double timediff = (double)(stop->tv_sec  - start->tv_sec) +
		      (double)(stop->tv_usec - start->tv_usec)/1000000.0; 

    printf(fmt, timediff);
}

int
main(int argc, char *argv[])
{
    struct timeval start;
    struct timeval stop;
    pid_t pid;
    int status;

    if (argc <= 1) {
    	fprintf(stderr, "usage: %s command...\n", argv[0]);
    	exit(EXIT_FAILURE);
    }

    gettimeofday(&start, NULL);

    pid = fork();
    if (pid < 0) {	    /* Error */
    	fprintf(stderr, "fork failed: %s\n", strerror(errno));
    	exit(EXIT_FAILURE);
    } else if (pid == 0) {  /* Child */
    	if (execvp(argv[1], &argv[1]) < 0) {
	    fprintf(stderr, "exec failed: %s\n", strerror(errno));
    	    exit(EXIT_FAILURE);
    	}
    } else {		    /* Parent */
    	if (wait(&status) < 0) {
	    fprintf(stderr, "wait failed: %s\n", strerror(errno));
    	    exit(EXIT_FAILURE);
	}
    
    	gettimeofday(&stop, NULL);
    	print_timediff("Elapsed time: %lf\n", &start, &stop);
    }

    return (EXIT_SUCCESS);
}
