/* rorschach.c: simple file watching utility */

#include "rorschach.h"

#include <errno.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

/**
 * Check file for MODIFY and CREATE events
 *
 * Note:
 *  The file's mtime should be updated if it has been modified.  Additionally,
 *  the file's timestamp should be updated if the file is in the files list.
 */
void
check_file(const char *path, const time_t mtime, struct hooks_t *hooks, struct files_t *files, const time_t timestamp)
{
    struct file_t *file;

    /* TODO */
}

/** Check the directory for new file events.
 *
 * Note:
 *  If any directory entries are sub-directories and the recursive flag is set,
 *  then this function recurses on the sub-directories.
 */
void
check_directory(const char *path, struct hooks_t *hooks, struct files_t *files, bool recursive, const time_t timestamp)
{
    struct dirent **entries;
    char fullpath[PATH_MAX];
    int n;

    /* TODO */
}

/**
 * Check timestamps of files in files list.
 *
 * If any files don't have the specified timestamp, then that indicates that
 * they have been removed and we should run any DELETE hooks.
 */
void
check_timestamps(struct hooks_t *hooks, struct files_t *files, const time_t timestamp)
{
    struct file_t *file;
    struct file_t *next;

    /* TODO */
}

/**
 * Display help message
 */
void
usage(const char *progname)
{
    fprintf(stderr, "usage: %s [options] rules directory\n", progname);
    fprintf(stderr, "\noptions:\n");
    fprintf(stderr, "    -h        Print this help message\n");
    fprintf(stderr, "    -r        Monitor directory recursively\n");
    fprintf(stderr, "\nparameters:\n");
    fprintf(stderr, "    rules     Rules specifying actions based on events and patterns\n");
    fprintf(stderr, "    directory Directory to monitor\n");
}

int
main(int argc, char *argv[])
{
    struct hooks_t hooks;
    struct files_t files;
    bool recursive;
    char *directory;
    char *rules;
    int c;

    /* TODO: Parse command line arguments */

    /* TODO: Initialize hooks and files lists */

    /* TODO: Load hooks */

    /* TODO: Continuously check directory and timestamps
     *
     * Note:
     *	Use small timeout (i.e. sleep) to prevent tying up the CPU.
     */

    return (EXIT_SUCCESS);
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
