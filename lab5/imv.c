#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

char *ProgramName 	= NULL;
char *TemporaryOrigPath	= NULL;
char *TemporaryEditPath = NULL;


void
parse_to_temp(int argc, char *argv[])
{
	
	if (argc == 1){
		fprintf(stderr, "Not enough arguments entered \n");
		exit(EXIT_FAILURE);
	}

	ProgramName = argv[0];

	int count;
	char template[BUFSIZ];
	char *tmpdir;
	FILE *temp_edit_stream;
	FILE *temp_stream;
	FILE *terminal_stream;
	
	if ((tmpdir = getenv("TMPDIR")) == NULL){
		strncpy(template, "/tmp", BUFSIZ);
	}else{
		strncpy(template, tmpdir, BUFSIZ);
	}
	strncat(template, "/imv.XXXXXX", BUFSIZ);
	TemporaryOrigPath = strndup(mktemp(template), BUFSIZ);
	TemporaryEditPath = strndup(mktemp(template), BUFSIZ);

	//safe stream open
	temp_stream = fopen(TemporaryOrigPath, "w+");
	if (temp_stream == NULL){
		fprintf(stderr, "unable to open file %s\n", TemporaryOrigPath);
		exit(EXIT_FAILURE);
	}
	temp_edit_stream = fopen(TemporaryEditPath, "w+");
	if (temp_edit_stream == NULL){
		fprintf(stderr, "unable to open file %s\n", TemporaryEditPath);
		exit(EXIT_FAILURE);
	}
	

	//put the files in the temp file
	for (count = 1; count < argc; count++){
		printf("agrv[%d] = %s \n", count, argv[count]);
		fputs(argv[count], temp_stream);
		fputs("\n", temp_stream);		
		fputs(argv[count], temp_edit_stream);
		fputs("\n", temp_edit_stream);		
	}
	printf("%s\n", TemporaryOrigPath);
	//close temp file
	fclose(temp_stream);





}	
/*
FILE*
safe_fopen(const char *path, const char *mode)
{
	FILE *fs;
	fs = fopen(path, mode);
	if (fs == NULL){
		fprintf(stderr, "unable to open file %s\n", path);
		exit(EXIT_FAILURE);
	}

	return (fs);
}
*/

void
cleanup(void)
{
	if(unlink(TemporaryPath) != 0){
		fprintf(stderr, "unable to unlink %s: %s \n", TemporaryPath, strerror(errno));
	}else{
		fprintf(stdout, "temp file unlinked\n");
	}
}



int 
main(int argc, char *argv[])
{
	//parse and place the arguments into a temporary file
	parse_to_temp(argc, argv);

	//copy temporary file to have an original and an editable
		
	
	//run $EDITOR to edit filenames
	

	//read file names and rename accordingly


	//remove temp file
	cleanup();

	return (EXIT_SUCCESS);
}
