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
	
	if ((tmpdir = getenv("TMPDIR")) == NULL){
		strncpy(template, "/tmp", BUFSIZ);
	}else{
		strncpy(template, tmpdir, BUFSIZ);
	}
	strncat(template, "/imv.XXXXXX", BUFSIZ);
	TemporaryOrigPath = strndup(mktemp(template), BUFSIZ);
	
	if ((tmpdir = getenv("TMPDIR")) == NULL){
		strncpy(template, "/tmp", BUFSIZ);
	}else{
		strncpy(template, tmpdir, BUFSIZ);
	}
	strncat(template, "/imv.XXXXXX", BUFSIZ);
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
//		printf("agrv[%d] = %s \n", count, argv[count]);
		fputs(argv[count], temp_stream);
		fputs("\n", temp_stream);		
		fputs(argv[count], temp_edit_stream);
		fputs("\n", temp_edit_stream);		
	}
	//printf("%s\n", TemporaryOrigPath);
	//printf("%s\n", TemporaryEditPath);
	//close temp file
	fclose(temp_stream);
	fclose(temp_edit_stream);

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
edit_temp(void)
{
	char *editor;
	char *command;
	int systemReturn;
	command = malloc(strlen(TemporaryEditPath) + 10);

	editor = getenv("EDITOR");

	if(editor == NULL){
		editor = "vim";
	}

	strcpy(command, editor);
	strcat(command, " ");
	strcat(command, TemporaryEditPath);

	//open editor and modify tempEditPath file
	systemReturn = system(command);
	if(systemReturn == -1){
		fprintf(stderr,"Editor called failed: %s \n", strerror(errno));
		exit(EXIT_FAILURE);
	}
}

void
compare_temps_write_changes(void)
{
	FILE *fp1, *fp2;
    	char buff1[BUFSIZ], buff2[BUFSIZ];;
    	char fname1[40], fname2[40];
	int ret;

    	fp1 = fopen( TemporaryEditPath,  "r" );
    	fp2 = fopen( TemporaryOrigPath,  "r" ) ;

    	if ( fp1 == NULL ){
       		printf("Cannot open %s for reading ", fname1 );
		exit(EXIT_FAILURE);	
       	}
    	else if (fp2 == NULL){
       		printf("Cannot open %s for reading ", fname2 );
		exit(EXIT_FAILURE);
       	}
    	else{
		while ((fgets(buff1, BUFSIZ, fp1)) != NULL && fgets(buff2, BUFSIZ, fp2) != NULL){
			if (strlen(buff1) != strlen(buff2)){
				//fclose(fp1);
				//fclose(fp2);
				ret = rename(buff2, buff1);
				printf("%s\n%s\n%d\n", buff2, buff1, ret);
				if (ret == -1){
					printf("%s\n", strerror(errno));
					exit(EXIT_FAILURE);
				}
			}	

			//printf("Edit: %s \nOrig: %s \n", buff1, buff2);
		}	
	
        	fclose ( fp1 );
        	fclose ( fp2 );
       }
	
}


void
cleanup(void)
{
	if(unlink(TemporaryEditPath) != 0 && unlink(TemporaryOrigPath) != 0){
		fprintf(stderr, "unable to unlink %s or %s: %s \n", TemporaryEditPath, TemporaryOrigPath, strerror(errno));
	}	
}



int 
main(int argc, char *argv[])
{
	//parse and place the arguments into a temporary file
	parse_to_temp(argc, argv);

	//run $EDITOR to edit filenames
	edit_temp();	

	//read file names and rename accordingly
	compare_temps_write_changes();

	//remove temp file
	cleanup();

	return (EXIT_SUCCESS);
}
