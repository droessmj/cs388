/* copy.c: simple cp implementation */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


int
main(int argc, char *argv[])
{
    char *src;
    char *dst;
    int   rfd;
    int   wfd;
    int   nread;
    char  buffer[BUFSIZ];

    /* Check command line arguments */
    if (argc != 3) {
    	fprintf(stderr, "usage: %s src dst\n", argv[0]);
    	exit(EXIT_FAILURE);
    }

    /* Open source file for reading */
    src = argv[1];
    rfd = open(src, O_RDONLY);
    if (rfd < 0) {
    	fprintf(stderr, "cannot open %s: %s\n", src, strerror(errno));
    	exit(EXIT_FAILURE);
    }

    /* Open destination file for writing (create if it doesn't exist) */
    dst = argv[2];
    wfd = open(dst, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
    if (wfd < 0) {
    	fprintf(stderr, "cannot open %s: %s\n", dst, strerror(errno));
    	exit(EXIT_FAILURE);
    }

    /* Read for source and write to destination */
    while ((nread = read(rfd, buffer, BUFSIZ)) > 0) {
    	int ntotal = 0;
    	do {
	    int nwritten = write(wfd, buffer + ntotal, nread - ntotal);
	    if (nwritten < 0) {
	    	fprintf(stderr, "error during write: %s\n", strerror(errno));
	    	exit(EXIT_FAILURE);
	    }
	    ntotal += nwritten;
	} while (ntotal != nread);
    }

    close(rfd);
    close(wfd);

    return (EXIT_SUCCESS);
}
