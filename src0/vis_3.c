/* vis: make funny characters visible (version 3) */

#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
    
bool Strip = false;

void
vis(FILE *fp)
{
    int c;

    while ((c = getc(fp)) != EOF) {
    	if (isascii(c) && (isprint(c) || c == '\n' || c == '\t' || c == ' ')) {
    	    putchar(c);
    	} else if (!Strip) {
    	    printf("\\%03o", c);
	}
    }
}

int
main(int argc, char *argv[])
{
    while (argc > 1 && argv[1][0] == '-') {
    	switch (argv[1][1]) {
	    case 's':
	    	Strip = true;
	    	break;
	    default:
	    	fprintf(stderr, "%s: unknown arg %s\n", argv[0], argv[1]);
	    	return (EXIT_FAILURE);
	}
	argc--;
	argv++;
    }

    if (argc == 1) {
    	vis(stdin);
    } else {
    	for (int i = 1; i < argc; i++) {
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
