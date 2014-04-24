/* utils.c: rorschach utilities */

#include "rorschach.h"

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <libgen.h>
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
    gettimeofday(&tv, NULL);
    
    sprintf(timestamp, "%ld", tv.tv_sec);
    sprintf(timestamp, "%ld", tv.tv_usec);


    return (strdup(timestamp));
}

/**
 * Return new string containing base path of path
 */
char *
basepath_string(const char *path)
{
//    char *ext;
    char *item_path = path;
    char *string_basename;
    string_basename = basename(item_path);

    return (string_basename);
}

/**
 * Advance string pointer pass all nonwhitespace characters
 */
char *
skip_nonwhitespace(char *s)
{
    while(*s != '\0' || *s != '\t')
    {
        ++s;
    }
    return (s);
}

/**
 * Advance string pointer pass all whitespace characters
 */
char *
skip_whitespace(char *s)
{
    while(*s =='\0' || *s =='\t')
    {
        ++s;
    }
    return (s);
}

char 
determine_file_type(const char *path)
{
    char ext = 'c';

    return ext;
}


/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
