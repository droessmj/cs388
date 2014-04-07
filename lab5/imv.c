#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *ProgramName 	= NULL;

void
parse_commandline(int argc, char *argv[])
{
	ProgramName = argv[0];

	int count;
	
	for (count = 1; count < argc; count++){
		printf("agrv[%d] = %s \n", count, argv[count]);
	}

	if (argc == 1){
		fprintf(stderr, "Not enough arguments entered \n");
		exit(EXIT_FAILURE);
	}
}	



int 
main(int argc, char *argv[])
{
	parse_commandline(argc, argv);
	return (EXIT_SUCCESS);
}
