/* stats: copied from Linux stat(2) man page */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <grp.h>
#include <pwd.h>

int
main(int argc, char *argv[])
{
    int i;

    for (i = 1; i < argc; i++) {
    	struct stat sb;
    	struct passwd *pw;
    	struct group  *gr;

    	if (stat(argv[i], &sb) < 0) {
    	    fprintf(stderr, "unable to stat %s: %s\n", argv[i], strerror(errno));
    	    exit(EXIT_FAILURE);
	}

	printf("File type:                ");

	switch (sb.st_mode & S_IFMT) {
	    case S_IFBLK:  printf("block device\n");            break;
	    case S_IFCHR:  printf("character device\n");        break;
	    case S_IFDIR:  printf("directory\n");               break;
	    case S_IFIFO:  printf("FIFO/pipe\n");               break;
	    case S_IFLNK:  printf("symlink\n");                 break;
	    case S_IFREG:  printf("regular file\n");            break;
	    case S_IFSOCK: printf("socket\n");                  break;
	    default:       printf("unknown?\n");                break;
	}

	printf("I-node number:            %ld\n", (long) sb.st_ino);

	printf("Mode:                     %lo (octal)\n",
		(unsigned long) sb.st_mode);

	printf("Link count:               %ld\n", (long) sb.st_nlink);

	pw = getpwuid(sb.st_uid);
	gr = getgrgid(sb.st_gid);
	printf("Ownership:                UID=%ld (%s)   GID=%ld (%s)\n",
		(long) sb.st_uid, pw->pw_name,
		(long) sb.st_gid, gr->gr_name);

	printf("Preferred I/O block size: %ld bytes\n",
		(long) sb.st_blksize);
	printf("File size:                %lld bytes\n",
		(long long) sb.st_size);
	printf("Blocks allocated:         %lld\n",
		(long long) sb.st_blocks);

	printf("Last status change:       %s", ctime(&sb.st_ctime));
	printf("Last file access:         %s", ctime(&sb.st_atime));
	printf("Last file modification:   %s", ctime(&sb.st_mtime));
    }

    return (EXIT_SUCCESS);
}
