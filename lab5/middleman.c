#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

void sigproc();

char *ProgramName 	= NULL;
char *command 		= NULL;
char *fifo_path 	= NULL;
int echo 		= 0;

void usage()
{
	fprintf(stderr, "Usage: %s \n", ProgramName);
	fprintf(stderr, "-c command \t Command to write to \n");
	fprintf(stderr, "-f fifo_path \t Path of FIFO to read \n");
	fprintf(stderr, "-E \t\t Echo FIFO to STDOUT \n");
}


void
parse_commandline(int argc, char *argv[])
{
    
    ProgramName = argv[0];
    int c;
    
    while ((c = getopt(argc, argv, "c:f:E")) != -1){
	switch (c){
	  case 'c':
		command = optarg;
		break;
	  case 'f':
		fifo_path = optarg;
		break;
	  case 'E':
		echo = 1;
		break; 
	  default:
		fprintf(stderr, "unknown option: %c\n", c);
		usage();
		break;
	}
    }

   if (argc == 1){
	fprintf(stderr, "No arguments entered \n");
	usage();
	exit(EXIT_FAILURE);
   }   
}

FILE *
safe_popen(const char *command, const char *type)
{
	FILE *pfs;
	pfs = popen(command, type);
	if(pfs == NULL){
		fprintf(stderr, "unable to popen the command\n");
		exit(EXIT_FAILURE);
	}
	return pfs;
}

FILE *
safe_fopen(const char *path, const char *type)
{
	FILE *stream;
	stream = fopen(path, type);
	if (stream == NULL){
		fprintf(stderr, "error opening fifo stream\n");
		exit(EXIT_FAILURE);
	}
	return stream;
}



char *
read_from_pipe()
{
	FILE *stream;
	char buffer[BUFSIZ];
	char *str = malloc(strlen(buffer) + 1);

	stream = safe_fopen(fifo_path, "r");

	while ((fgets(buffer, BUFSIZ, stream)) != NULL){
		if(strlen(str) < /*insert variable to compare to*/){
			strcpy(str,buffer);
		} else {
			str = realloc(strlen(str) + BUFSIZ);
			strcpy(str,buffer);
		}
	}

	fclose(stream);

	return str;
}


void
pipeInput(){

	FILE *pfs;
	char *input;
	int result = 1;

	while( result == 1){
		//continue trying to read input
		input = read_from_pipe();
		result = strncmp(input, "\0", 1);	
	}	
	
	if (echo  == 1){
		/*echo FIFO to STDOUT*/	
		printf("%s \n", input);
	}

	//open the command, then send input to command
	//send input to run_file

	//run the command that was input
	pfs = safe_popen(command, "w");

	fputs(input, pfs);
	fputc('\n', pfs);

	//run at end
	pclose(pfs);	
}

void sigproc()
{
	signal(SIGINT, sigproc);
	printf("\nmiddleman shutting off...\n");
	exit(EXIT_SUCCESS);
}

int
main(int argc, char *argv[])
{
   parse_commandline(argc, argv);
  
   signal(SIGINT, sigproc);
   signal(SIGQUIT, sigproc);
   
   //infinte loop to pipe input until program is killed
   for(;;) {
	pipeInput();
   }

   return (EXIT_SUCCESS);
}
