/* vis: make funny characters visible (version 1) */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
    int c;

    while ((c = getchar()) != EOF) {
    	if (isascii(c) && (isprint(c) || c == '\n' || c == '\t' || c == ' ')) {
    	    putchar(c);
    	} else {
    	    printf("\\%03o", c);
	}
    }

    return (EXIT_SUCCESS);
}
