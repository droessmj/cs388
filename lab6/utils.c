/* utils.c: rorschach utilities */

#include "rorschach.h"

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/time.h>

/* Utilities */

/**
 * Return new string containing current timestamp from gettimeofday
 */
char *
timestamp_string()
{
    char timestamp[TIMESTAMP_MAX];
    struct timeval tv;

    /* TODO */

    return (strdup(timestamp));
}

/**
 * Return new string containing base path of path
 */
char *
basepath_string(const char *path)
{
    char buffer[BUFSIZ];
    char *ext;

    /* TODO */

    return (strdup(buffer));
}

/**
 * Advance string pointer pass all nonwhitespace characters
 */
char *
skip_nonwhitespace(char *s)
{
    /* TODO */
    return (s);
}

/**
 * Advance string pointer pass all whitespace characters
 */
char *
skip_whitespace(char *s)
{
    /* TODO */
    return (s);
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
