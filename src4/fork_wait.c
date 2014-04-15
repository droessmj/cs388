/* fork_wait.c: example of waiting for child */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>

static bool child_done = false;

void
handle_child(int signum)
{
    pid_t wpid;
    int status;

    wpid = wait(&status);
    if (wpid < 0)
    	perror("wait");

    printf("[PARENT] pid=%d, status=%d\n", wpid, WEXITSTATUS(status));
    child_done = true;
}

int
main(int argc, char *argv[])
{
    pid_t pid;

    signal(SIGCHLD, handle_child);

    pid = fork();
    switch (pid) {
    case -1:    /* Error */
	perror("fork");
	exit(EXIT_FAILURE);
	break;
    case 0:	/* Child */
    	for (int i = 0; i < 5; i++) {
	    printf("[CHILD] pid=%d, i=%d\n", getpid(), i);
	    sleep(1);
	}
	_exit(EXIT_SUCCESS);
	break;
    default:    /* Parent */
    	while (!child_done) {
    	    printf("[PARENT] child_done=%d\n", child_done);
    	    sleep(1);
	}
	printf("[PARENT] pid=%d\n", pid);
	break;
    }

    return (EXIT_SUCCESS);
}
