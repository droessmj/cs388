/* move.c: simple move utility */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "macros.h"

#define safe_close(fd)			    \
    if ((fd) >= 0) { close(fd); }

#define safe_open(fd, path, flags, ...) \
    (fd) = open((path), (flags), ##__VA_ARGS__);   \
    if ((fd) < 0) { goto cleanup; }

int
copy_file(const char *src, const char *dst)
{
    int rfd    = -1;
    int wfd    = -1;
    int result =  1;
    int nread  =  0;
    char buffer[BUFSIZ];

    /* Open source file for reading */
    safe_open(rfd, src, O_RDONLY);

    /* Open destination file for writing (create if it doesn't exist) */
    safe_open(wfd, dst, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);

    /* Read for source and write to destination */
    while ((nread = read(rfd, buffer, BUFSIZ)) > 0) {
    	int ntotal = 0;
    	do {
	    int nwritten = write(wfd, buffer + ntotal, nread - ntotal);
	    if (nwritten < 0) {
	    	goto cleanup;
	    }
	    ntotal += nwritten;
	} while (ntotal != nread);
    }
    result = 0;

cleanup:
    safe_close(rfd);
    safe_close(wfd);

    return (result);
}

int
main(int argc, char *argv[])
{
    const char *src;
    const char *dst;
    int result;

    /* Parse command line arguments */
    if (argc != 3) {
    	fprintf(stderr, "usage: %s src dst\n", argv[0]);
    	exit(EXIT_FAILURE);
    }
    src = argv[1];
    dst = argv[2];

    /* First, try to link file */
    result = link(src, dst);
    if (result != 0) { 
    	debug("Link failed: %s", strerror(errno));
    	if (errno != EXDEV) {
    	    goto error;
	}

	/* Next, try to copy instead */
	result = copy_file(src, dst);
	if (result != 0) {
	    debug("Copy failed: %s", strerror(errno));
	    goto error;
	}
    }

    /* Finally, remove old link */
    result = unlink(src);
    if (result != 0) {
	debug("Unlink failed: %s", strerror(errno));
    	goto error;
    }

error:
    if (result != 0) {
	fprintf(stderr, "Error while moving %s to %s: %s\n", src, dst, strerror(errno));
    }

    return (result);
}
