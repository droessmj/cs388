
/* vis: make funny characters visible (version 3) */

#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
    
bool Strip = false;

void
vis(FILE *fp)
{
    char buffer[BUFSIZ];

    while ((fgets(buffer, BUFSIZ, fp)) != NULL) {
	for (char *bp = buffer; *bp; bp++) {
	    if (isascii(*bp) && (isprint(*bp) || *bp == '\n' || *bp == '\t' || *bp == ' ')) {
		putchar(*bp);
	    } else if (!Strip) {
		printf("\\%03o", *bp);
	    }
	}
    }
}

int
main(int argc, char *argv[])
{
    int c;

    while ((c = getopt(argc, argv, "s")) != -1) {
    	switch (c) {
	    case 's':
	    	Strip = true;
	    	break;
	    default:
	    	fprintf(stderr, "%s: unknown arg %c\n", argv[0], c);
	    	return (EXIT_FAILURE);
	}
    }

    if ((argc - optind) == 0) {
    	vis(stdin);
    } else {
    	for (int i = optind; i < argc; i++) {
    	    FILE *fp;

    	    if ((fp = fopen(argv[i], "r")) == NULL) {
	    	fprintf(stderr, "%s: can't open %s: %s\n", argv[0], argv[i], strerror(errno));
    	    	return (EXIT_FAILURE);
	    }

	    vis(fp);
	    fclose(fp);
	}
    }

    return (EXIT_SUCCESS);
}
