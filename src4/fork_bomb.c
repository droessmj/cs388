/* fork_bomb.c: slow moving fork bomb */

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
    pid_t pid;

    while (1) {
    	pid = fork();
    	if (pid < 0)
    	    perror("fork");
    	sleep(1);
    }

    return EXIT_SUCCESS;
}
