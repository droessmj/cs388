/* fork_exec.c: example of fork/exec/waitpid */

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
    pid_t pid, wpid;
    int status;

    pid = fork();
    switch (pid) {
    case -1:    /* Error */
	perror("fork");
	exit(EXIT_FAILURE);
	break;
    case 0:	/* Child */
	printf("[CHILD] pid=%d\n", getpid());
	execlp("echo", "echo", "hello", "world", NULL);
	_exit(EXIT_FAILURE);
	break;
    default:    /* Parent */
	wpid = waitpid(pid, &status, 0);
	if (wpid < 0)
	    perror("waitpid");
	printf("[PARENT] pid=%d, status=%d\n", wpid, WEXITSTATUS(status));
	break;
    }

    return EXIT_SUCCESS;
}
