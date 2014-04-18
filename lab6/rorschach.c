/* rorschach.c: simple file watching utility */

#include "rorschach.h"
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <stdbool.h>

bool recursive      = false;
bool help           = false;
char *ProgramName   = NULL;
char *rules         = NULL;
char *directory     = NULL;

void
parse_commandline(int argc, char *argv[])
{
    ProgramName = argv[0];
    int c;
    
    while ((c = getopt(argc, argv, "hr")) != -1){
        switch(c){
            case 'h':
                help = true;
                argc--;
                break;
            case 'r':
                recursive = true;
                argc--;
                break;
            default:
                fprintf(stderr, "unknown option: %c\n", c);
                usage();
                break;
        }
    }

    if(argc < 3){
        fprintf(stderr, "insufficient arguments");
        usage();
        exit(EXIT_FAILURE);
    } else{
        //set the rules and the directory
        rules = argv[argc-1];
        argc--;
        directory = argv[argc-1];    
    }
}


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
    file = search_files(files, path);
           

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

    if(recursive == true){

    }
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
usage()
{
    fprintf(stderr, "usage: %s [options] rules directory\n", ProgramName);
    fprintf(stderr, "\noptions:\n");
    fprintf(stderr, "    -h        Print this help message\n");
    fprintf(stderr, "    -r        Monitor directory recursively\n");
    fprintf(stderr, "\nparameters:\n");
    fprintf(stderr, "    rules     Rules specifying actions based on events and patterns\n");
    fprintf(stderr, "    directory Directory to monitor\n");
}

struct hooks_t
init_hooks()
{
    struct hooks_t *head;
    //initialize the queue as per documentation
    TAILQ_INIT(&head);    

    return head;
}

struct files_t
init_files()
{
    struct files_t *head;
    TAILQ_INIT(&head);
    
    return head;
}

void
set_hooks(struct hooks_t *hooks)
{
    //read through rules and set hooks 
    
    // for each hook in the rules  

}


int
main(int argc, char *argv[])
{
    struct hooks_t hooks;
    struct files_t files;
    int c;

    /* TODO: Parse command line arguments */
    parse_commandline();
    
    /* TODO: Initialize hooks and files lists */
    hooks = init_hooks();
    files = init_files();
    
    /* TODO: Load hooks */
    set_hooks(hooks);
    
    // TODO: Continuously check directory and timestamps
    while(true){
        check_directory(directory, hooks, files, recursive, timestamp);
        sleep(1);
    }
    
    return (EXIT_SUCCESS);
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
