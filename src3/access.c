/* access: summarize access permissions */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
    int i;

    for (i = 1; i < argc; i++) {
    	printf("%16s is%sreadable\n", argv[i], access(argv[i], R_OK) == 0 ? " " : " not ");
    	printf("%16s is%swritable\n", argv[i], access(argv[i], W_OK) == 0 ? " " : " not ");
    	printf("%16s is%sexecutable\n", argv[i], access(argv[i], X_OK) == 0 ? " " : " not ");
    }

    return (EXIT_SUCCESS);
}
