/* xcrypt.c: simple xor encryption and decryption */

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char Key = SCHAR_MAX;
    
void
xcrypt(FILE *fp)
{
    char buffer[BUFSIZ];

    while ((fgets(buffer, BUFSIZ, fp)) != NULL) {
	for (char *bp = buffer; *bp; bp++) {
	    putchar(*bp ^ Key);
	}
    }
}

int
main(int argc, char *argv[])
{
    int c;

    while ((c = getopt(argc, argv, "k:")) != -1) {
    	switch (c) {
	    case 'k':
	    	Key = atoi(optarg);
	    	break;
	    default:
	    	fprintf(stderr, "%s: unknown arg %c\n", argv[0], c);
	    	return (EXIT_FAILURE);
	}
    }

    if ((argc - optind) == 0) {
    	xcrypt(stdin);
    } else {
    	for (int i = optind; i < argc; i++) {
    	    FILE *fp;

    	    if ((fp = fopen(argv[i], "r")) == NULL) {
	    	fprintf(stderr, "%s: can't open %s: %s\n", argv[0], argv[i], strerror(errno));
    	    	return (EXIT_FAILURE);
	    }

	    xcrypt(fp);
	    fclose(fp);
	}
    }

    return (EXIT_SUCCESS);
}
