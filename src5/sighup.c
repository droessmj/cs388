/* sighup.c: reload data */

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <signal.h>
#include <unistd.h>

char *DataPath  = NULL;
int   DataValue = -1;

int
read_data(const char *path)
{
    char  buffer[BUFSIZ];
    FILE *data_file;
    int   data_value;

    data_file = fopen(path, "r");
    if (data_file == NULL) {
    	fprintf(stderr, "Could not open %s: %s\n", path, strerror(errno));
    	return -1;
    }

    fgets(buffer, BUFSIZ, data_file);
    data_value = strtol(buffer, NULL, 10);

    fclose(data_file);

    return (data_value);
}

void
handle_sighup(int signum)
{
    DataValue = read_data(DataPath);
    fprintf(stderr, "[SIGHUP] DataPath = %s, DataValue = %d\n", DataPath, DataValue);
}

int
main(int argc, char *argv[])
{
    if (argc != 2) {
    	fprintf(stderr, "usage: %s data_path\n", argv[0]);
    	exit(EXIT_FAILURE);
    }

    signal(SIGHUP, handle_sighup);

    DataPath  = argv[1];
    DataValue = read_data(DataPath);

    for (;;) {
    	sleep(1);
    }

    return (EXIT_SUCCESS);
}
