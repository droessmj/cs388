/* vis: make funny characters visible (version 2) */

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(int argc, char *argv[])
{
    int c;
    bool strip;

    strip = false;
    if (argc > 1 && strcmp(argv[1], "-s") == 0) {
    	strip = true;
    }

    while ((c = getchar()) != EOF) {
    	if (isascii(c) && (isprint(c) || c == '\n' || c == '\t' || c == ' ')) {
    	    putchar(c);
    	} else if (!strip) {
    	    printf("\\%03o", c);
	}
    }

    return (EXIT_SUCCESS);
}
