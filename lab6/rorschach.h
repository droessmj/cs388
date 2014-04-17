/* rorschach.h: rorschach header */

#ifndef RORSCHACH_H
#define RORSCHACH_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/queue.h>

/* Macros */

#ifdef NDEBUG
#define debug(M, ...)
#else
#define debug(M, ...)	fprintf(stderr, "DEBUG %10s:%04d: " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#endif

#define chomp(s)        (s)[strlen(s) - 1] = '\0'
#define streq(a, b)     (strcmp(a, b) == 0)

/* Events */

typedef enum {
    EVT_CREATE,
    EVT_MODIFY,
    EVT_DELETE,
    EVT_UNKNOWN,
} event_t;

const char *	event_string(event_t event);
event_t         event_type(const char *s);

/* Files */

struct file_t {
    char  *path;
    time_t mtime;
    time_t timestamp;
    
    TAILQ_ENTRY(file_t) files;
};

TAILQ_HEAD(files_t, file_t);

void            add_file(struct files_t *files, const char *path, const time_t mtime, const time_t timestamp);
struct file_t * search_files(struct files_t *files, const char *path);

/* Hooks */

struct hook_t {
    event_t event;
    char   *pattern;
    char   *action;

    TAILQ_ENTRY(hook_t) rules;
};

TAILQ_HEAD(hooks_t, hook_t);

void            add_hook(struct hooks_t *hooks, const event_t event, const char *pattern, const char *action);
void            run_hook(struct hook_t *hook, const char *fullpath);
int             load_hooks(struct hooks_t *hooks, const char *path);
void            print_hooks(struct hooks_t *hooks);
void            run_hooks(struct hooks_t *hooks, const event_t event, const char *path);

/* Utilities */

#define TIMESTAMP_MAX   32

char *          timestamp_string();
char *          basepath_string(const char *path);
char *          skip_nonwhitespace(char *s);
char *          skip_whitespace(char *s);
char            determine_file_type(const char *path);

#endif

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
