/* rorschach.c: simple file watching utility */

#include "rorschach.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/queue.h>
#include <fnmatch.h>
#include <signal.h>
#include <unistd.h>

/**
 * Add hook to hooks list with specified event, pattern, and action.
 */
void
add_hook(struct hooks_t *hooks, const event_t event, const char *pattern, const char *action)
{
    struct hook_t *hook = malloc(sizeof(struct hook_t));
    /* TODO: Allocate hook and set fields */    
    hook->event = event;
    hook->pattern = strdup(pattern);
    hook->action = strdup(action);

    /* TODO: Insert hook into hooks list */
    TAILQ_INSERT_HEAD(hooks, hook, rules);

    debug("ADD HOOK: event=%s, pattern=%s, action=%s", event_string(event), pattern, action);
}

/** 
 * Execute specified hook for given path.
 *
 * Hook action is executed as a shell command: 
 *
 *  /bin/sh -c <HOOK->ACTION>.
 */
void
run_hook(struct hook_t *hook, const char *path)
{
    const char *event     = event_string(hook->event);
    char       *timestamp = timestamp_string();
    char       *basepath  = basepath_string(path);
    pid_t pid;
    
    /* TODO: Export environment variables */

    /* TODO: Ignore SIGCHLD */
    
    debug("RUN HOOK: event=%s, timestamp=%s, basepath=%s, fullpath=%s, action=%s",
            event, timestamp, basepath, path, hook->action);
    
    /* TODO: Fork and execute hook action */
    pid = fork();
    switch (pid) {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
        break;
    case 0: /* CHILD */
        execlp("/bin/sh -c", hook->action);
    default: /* PARENT */
        break;    
    }    
    
    free(timestamp);
    free(basepath);
}

/**
 * Add default hooks to hooks list
 */
void
load_default_hooks(struct hooks_t *hooks)
{
   add_hook(hooks, event_type("CREATE"), "*", "[${TIMESTAMP}] ${EVENT}: ${FULLPATH}"); 
   add_hook(hooks, event_type("MODIFY"), "*", "[${TIMESTAMP}] ${EVENT}: ${FULLPATH}"); 
   add_hook(hooks, event_type("DELETE"), "*", "[${TIMESTAMP}] ${EVENT}: ${FULLPATH}");  
}

#define parse_next_token(t, s)          \
    (t)     = skip_nonwhitespace(s);    \
    if (!*(t)) {                        \
        goto invalid;                   \
    }                                   \
    *(t)++  = '\0';                     \
    (t)     = skip_whitespace(t);

/**
 * Load hooks from file specifed by path into hooks list
 */
int
load_hooks(struct hooks_t *hooks, const char *path)
{
    load_default_hooks(hooks);   

    FILE *fs;
    char buffer[BUFSIZ];
    char rule[BUFSIZ];
    char *event;
    char *pattern;
    char *action;

    // printf("path:%s\n", path);
    fs = fopen(path, "r");
    if(fs == NULL){
        fprintf(stderr,"Error opening \'rules\' file");
        exit(EXIT_FAILURE);
    } 

    while((fgets(buffer, BUFSIZ, fs)) != NULL){
        printf("%s\n",buffer);
        /*
        event = parse_next_token(event, rule);
        pattern = parse_next_token(pattern, rule);
        action = parse_next_token(action, rule);
        if(event != NULL && pattern != NULL && action != NULL){
            add_hook(hooks, event_type(event), pattern, action);
        }
        */
    }
   
    fclose(fs);
    return (EXIT_SUCCESS);

invalid:
    fprintf(stderr, "Invalid hook rule: %s\n", buffer);

fail:
    if (fs) {
        fclose(fs);
    }
    return (EXIT_FAILURE);
}

/**
 * Print every hook in hooks list
 */
void
print_hooks(struct hooks_t *hooks)
{
    struct hook_t *np;
    for(np = hooks->tqh_first; np != NULL; np = np->rules.tqe_next){
        printf("%s \t %s \t %s\n", event_string(np->event), np->pattern, np->action);
    }
}

/**
 * Run hooks that match event and path
 *
 * Pseudo-Code:
 *
 *  for hook in hooks:
 *      if hook.event == event and fnmatch(hook.pattern, path):
 *          run_hook(hook, path)
 */
void
run_hooks(struct hooks_t *hooks, const event_t event, const char *path)
{
    debug("RUN HOOKS: event=%s, path=%s", event_string(event), path);
    
    /* TODO */
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
