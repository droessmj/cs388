/* fast_popen.c: quick and dirty popen */

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

void
string_split_spaces(char *s, int *argc, char ***argv)
{
    *argc = 0;
    *argv = malloc((strlen(s) + 1) * sizeof(char *));

    while (*s) {
    	/* Skip spaces */
    	while (isspace(*s)) {
    	    s++;
	}

	/* Set string */
	(*argv)[(*argc)++] = s;
	
	/* Skip non-spaces */
	while (*s && !isspace(*s)) {
	    s++;
	}

	/* Terminate token */
	if (*s) {
	    *s++ = 0;
	}
    }
}

FILE *
fast_popen(const char *command)
{
    int pfds[2];
    int result;
    pid_t pid;
    int argc;
    char **argv;
    char buffer[BUFSIZ];
    FILE *pfs = NULL;

    /* Create pipe */
    result = pipe(pfds);
    if (result < 0) {
    	perror("pipe");
    	exit(EXIT_FAILURE);
    }

    /* Fork */
    pid = fork();
    switch (pid) {
    case -1:	/* Error */
    	perror("fork");
    	exit(EXIT_FAILURE);
    	break;
    case  0:	/* Child */
    	close(0);	    /* Close child's stdin */
    	dup2(pfds[1], 1);   /* Connect write-end of pipe to child's stdout */
    	dup2(pfds[1], 2);   /* Connect write-end of pipe to child's stdin  */
    	close(pfds[0]);	    /* Close pipe read-end */
    	close(pfds[1]);	    /* Close pipe write-end */
    	
    	/* Setup argument vector */
	strncpy(buffer, command, BUFSIZ);
	string_split_spaces(buffer, &argc, &argv);

	/* Execute */
	if (execvp(argv[0], argv) < 0) {
	    perror("exec");
	    _exit(1);
	}
    	break;
    default:	/* Parent */
    	close(pfds[1]);	    /* Close pipe write-end */
    	pfs = fdopen(pfds[0], "r");
    	break;
    }

    return (pfs);
}

int
main(int argc, char *argv[])
{
    FILE *fs;
    char buffer[BUFSIZ];

    fs = fast_popen("ls -l");
    while (fgets(buffer, BUFSIZ, fs) != NULL) {
    	printf(buffer);
    }
    fclose(fs);

    return (EXIT_SUCCESS);
}
