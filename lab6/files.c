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
    struct file_t *file = malloc(sizeof(struct file_t));
    /* TODO: Allocate file and set fields */
    file->path = strdup(path);
    file->mtime = mtime;
    file->timestamp = timestamp;

    /* TODO: Insert file into files list */
    TAILQ_INSERT_HEAD(files, file, files);

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
    struct file_t *np;
    np = files->tqh_first;
    while ( strcmp(np->path,path) != 0 && np != NULL){
        np = np->files.tqe_next;
    }

    return np;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
