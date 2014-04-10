/* tree.c: recursively print tree of directory */

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h> 
#include <sys/types.h> 
#include <dirent.h> 

#include "macros.h"

#define INDENT_SPACES	2

typedef void(*print_func_t)(const char *, const int);

char
determine_path_type(const char *path)
{
    struct stat st;
    char type = '?';

    if (stat(path, &st) != 0) {
	debug("Unable to stat %s: %s", path, strerror(errno));
	goto exit;
    }

    switch (st.st_mode & S_IFMT) {
	case S_IFBLK:  type = 'B'; break;
	case S_IFCHR:  type = 'C'; break;
	case S_IFDIR:  type = 'D'; break;
	case S_IFIFO:  type = 'P'; break;
	case S_IFLNK:  type = 'L'; break;
	case S_IFREG:  type = 'F'; break;
	case S_IFSOCK: type = 'S'; break;
	default:       type = '?'; break;
    }

exit:
    return (type);
}

void
print_directory_tree(const char *path, const int indent)
{
    DIR* dir;
    struct dirent *entry;
    char template[BUFSIZ];
    char fullpath[PATH_MAX];
    char type;

    dir = opendir(path);
    if (dir == NULL) {
    	debug("Unable to opendir %s: %s\n", path, strerror(errno));
    	return;
    }

    snprintf(template, BUFSIZ, "%%%ds%%c %%s\n", indent * INDENT_SPACES);

    while ((entry = readdir(dir)) != NULL) {
    	if (streq(entry->d_name, ".") || streq(entry->d_name, "..")) {
    	    continue;
	}

	snprintf(fullpath, PATH_MAX, "%s/%s", path, entry->d_name);
	type = determine_path_type(fullpath);
    	printf(template, "", type, entry->d_name);

    	if (type == 'D') {
	    print_directory_tree(fullpath, indent + 1);
	}
    }

    closedir(dir);
}

void
print_directory_tree_sorted(const char *path, const int indent)
{
    struct dirent **entries;
    char template[BUFSIZ];
    char fullpath[PATH_MAX];
    char type;
    int n;

    n = scandir(path, &entries, 0, alphasort);
    if (n < 0) {
    	debug("Unable to scandir %s: %s\n", path, strerror(errno));
    	return;
    }

    snprintf(template, BUFSIZ, "%%%ds%%c %%s\n", indent * INDENT_SPACES);

    for (int i = 0; i < n; i++) {
    	if (streq(entries[i]->d_name, ".") || streq(entries[i]->d_name, "..")) {
    	    goto cleanup;
	}

	snprintf(fullpath, PATH_MAX, "%s/%s", path, entries[i]->d_name);
	type = determine_path_type(fullpath);
    	printf(template, "", type, entries[i]->d_name);

    	if (type == 'D') {
	    print_directory_tree_sorted(fullpath, indent + 1);
	}
	
cleanup:
	free(entries[i]);
    }
}

int
main(int argc, char *argv[])
{
    int offset = 1;
    print_func_t print_func = print_directory_tree;

    if (argc <= 1) {
    	fprintf(stderr, "usage: %s directories...\n", argv[0]);
    }

    if (streq(argv[1], "-s")) {
    	print_func = print_directory_tree_sorted;
    	offset = 2;
    }

    for (int i = offset; i < argc; i++) {
    	print_func(argv[i], 0);
    }

    return (EXIT_SUCCESS);
}
