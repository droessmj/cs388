/* files.c: rorschach files */

#include "rorschach.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Add file to files list with specified path, mtime, and timestamp.
 */
void
add_file(struct files_t *files, const char *path, const time_t mtime, const time_t timestamp)
{
    struct file_t *file;

    /* TODO: Allocate file and set fields */

    /* TODO: Insert file into files list */

    debug("ADD FILE: path=%s, mtime=%lu, timestamp=%lu", path, mtime, timestamp);
}

/**
 * Search files list for specified path.
 *
 * Return file if found, otherwise NULL.
 */
struct file_t *
search_files(struct files_t *files, const char *path)
{
    /* TODO */

    return (NULL);
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
