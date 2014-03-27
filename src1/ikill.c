/* ikill: interactively kill a process */

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

const char *PSCommand = "ps ux";

#define chomp(s)		(s)[strlen(s) - 1] = '\0'
#define skip_whitespace(s)	while (*(s) != '\0' && isspace(*(s))) { (s)++; }
#define parse_one_value(v, b)	(v) = strsep(&(b), " \t"); skip_whitespace(b);

FILE *
safe_popen(const char *command, const char *type)
{
    FILE *pfs;

    pfs = popen(command, type);
    if (pfs == NULL) {
    	fprintf(stderr, "unable to popen %s: %s\n", command, strerror(errno));
    	exit(EXIT_FAILURE);
    }

    return pfs;
}

void
parse_ps_entry(char *buffer, int *pid, char *command)
{
    char *value;

    parse_one_value(value, buffer); /* Parse USER	*/
    parse_one_value(value, buffer); /* Parse PID	*/
    *pid = strtol(value, NULL, 10);
    parse_one_value(value, buffer); /* Parse %CPU	*/
    parse_one_value(value, buffer); /* Parse %MEM	*/
    parse_one_value(value, buffer); /* Parse VSZ	*/
    parse_one_value(value, buffer); /* Parse RSS	*/
    parse_one_value(value, buffer); /* Parse TT		*/
    parse_one_value(value, buffer); /* Parse STAT	*/
    parse_one_value(value, buffer); /* Parse STARTED	*/
    parse_one_value(value, buffer); /* Parse TIME	*/
    strncpy(command, buffer, BUFSIZ); 
}

void
list_processes()
{
    FILE *pfs;
    char buffer[BUFSIZ];

    pfs = safe_popen(PSCommand, "r");

    while (fgets(buffer, BUFSIZ, pfs) != NULL) {
	int  pid;
	char command[BUFSIZ];

	/* Strip newline */
	chomp(buffer);

	/* Skip header */
	if (strncmp(buffer, "USER", 4) == 0) {
	    continue;
	}

	/* Parse each entry and print PID and command */
    	parse_ps_entry(buffer, &pid, command);
    	printf("%8d %s\n", pid, command);
    }

    pclose(pfs);
}

void
read_integer(int *integer, const char *prompt)
{
    char buffer[BUFSIZ];

    /* Display prompt */
    fprintf(stdout, prompt);
    fflush(stdout);

    /* Read input into buffer and then scan for integer */
    fgets(buffer, BUFSIZ, stdin);
    if (sscanf(buffer, "%d", integer) != 1) {
    	*integer = -1;
    }
}

struct signal_entry_t { 
    int	  value;
    char *name;
    char *description;
};

struct signal_entry_t SignalTable[] = {
    { 1, "HUP" , "Hang up" },
    { 2, "INT" , "Interrupt" },
    { 3, "QUIT", "Quit"	},
    { 6, "ABRT", "abort" },
    { 9, "KILL", "non-catchable, non-ignorable kill" },
    {14, "ALRM", "alarm clock" },
    {15, "TERM", "software termination signal" },
    { 0, NULL  , NULL },
};

void
list_signals()
{
    struct signal_entry_t *s;

    for (s = SignalTable; s->value != 0; s++) {
	printf("%8d %4s %s\n", s->value, s->name, s->description);
    }
}

int
main(int argc, char *argv[])
{
    int pid = -1;
    int sig = -1;

    while (pid < 0) {
	list_processes();
	read_integer(&pid, "Which process to signal? ");
    }

    while (sig < 0) {
    	list_signals();
	read_integer(&sig, "Which signal to send? ");
    }

    if (kill(pid, sig) < 0) {
    	fprintf(stderr, "unable to send process %d signal %d: %s\n", pid, sig, strerror(errno));
    	return (EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}
