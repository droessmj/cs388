/* ispell.c: interactive spell checker */

#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef NDEBUG
#define debug(M, ...)
#else
#define debug(M, ...)	fprintf(stderr, "DEBUG %s:%d: " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#endif

#define chomp(s)	(s)[strlen(s) - 1] = '\0';

const size_t DEFAULT_DICTIONARY_CAPACITY = 1024;

struct dictionary_t {
    char **words;
    size_t size;
    size_t capacity;
};

char *ProgramName    = NULL;
char *DictionaryPath = "/usr/share/dict/words";
char *InputPath      = NULL;
char *TemporaryPath  = NULL;
char *TerminalPath   = "/dev/tty";
char *OutputPath     = NULL;
struct dictionary_t Dictionary = { NULL, 0, 0 };

void
usage()
{
    fprintf(stderr, "usage: %s [-d dictionary -o output] input\n\n", ProgramName);
    fprintf(stderr, "options:\n\n");
    fprintf(stderr, "    -d dictionary   Path to dictionary file\n");
    fprintf(stderr, "    -o output       Path to output file\n");
}

void
parse_command_line_options(int argc, char *argv[])
{
    int c;

    ProgramName = argv[0];

    while ((c = getopt(argc, argv, "d:o:")) != -1) {
    	switch (c) {
	    case 'd':
	    	DictionaryPath = optarg;
	    	break;
	    case 'o':
	    	OutputPath     = optarg;
	    	break;
	    default:
	    	fprintf(stderr, "unknown option: %c\n", c);
	    	usage();
	    	break;
	}
    }

    if ((argc - optind) == 1) {
    	InputPath = argv[optind];
    }
}

void
setup_temporary_path()
{
    char template[BUFSIZ];
    char *tmpdir;

    if ((tmpdir = getenv("TMPDIR")) == NULL) {
    	strncpy(template, "/tmp", BUFSIZ);
    } else {
    	strncpy(template, tmpdir, BUFSIZ);
    }

    strncat(template, "/ispell.XXXXXX", BUFSIZ);

    TemporaryPath = strndup(mktemp(template), BUFSIZ);
}

void
cleanup(void)
{
    if (unlink(TemporaryPath) != 0) {
    	fprintf(stderr, "unable to unlink %s: %s\n", TemporaryPath, strerror(errno));
    }
}

FILE*
safe_fopen(const char *path, const char *mode)
{
    FILE *fs;

    fs = fopen(path, mode);
    if (fs == NULL) {
    	fprintf(stderr, "unable to open file %s: %s\n", path, strerror(errno));
    	exit(EXIT_FAILURE);
    }

    return (fs);
}

int
compare_string(const void *p1, const void *p2)
{
    const char *s1 = *(const char **)p1;
    const char *s2 = *(const char **)p2;
    return strcmp(s1, s2);
}

void
dictionary_initialize(struct dictionary_t *d)
{
    d->capacity = DEFAULT_DICTIONARY_CAPACITY;
    d->size     = 0;
    d->words	= malloc(d->capacity * sizeof(char *));
}

void
dictionary_append(struct dictionary_t *d, const char *value)
{
    if (d->size >= d->capacity) {
    	d->capacity = 4 * d->capacity / 3;
    	d->words    = realloc(d->words, d->capacity * sizeof(char *));
    }

    d->words[d->size] = strndup(value, BUFSIZ);
    d->size++;
}

void
dictionary_load(struct dictionary_t *d, const char *path)
{
    FILE *fs;
    char buffer[BUFSIZ];

    fs = safe_fopen(path, "r");

    dictionary_initialize(d);
    while (fgets(buffer, BUFSIZ, fs) != NULL) {
    	chomp(buffer);
    	dictionary_append(d, buffer);
    }

    qsort(d->words, d->size, sizeof(char *), compare_string);

    debug("Dictionary Capacity: %lu", d->capacity);
    debug("Dictionary Size:     %lu", d->size);

    fclose(fs);
}

bool
dictionary_contains(struct dictionary_t *d, const char *value)
{
    return bsearch(&value, d->words, d->size, sizeof(char *), compare_string) != NULL;
}

void
spell_check_stream(FILE *input_stream, FILE *terminal_stream, FILE *output_stream)
{
    char input[BUFSIZ];

#define skip_whitespace(w, r, d) \
    while (isspace(*(r))) { \
	*(w)++ = *(r)++; \
    } \
    (d) = (r);

    while (fgets(input, BUFSIZ, input_stream) != NULL) {
	char output[BUFSIZ];
	char replacement[BUFSIZ];
	char current;
    	char *word   = NULL;
    	char *stop   = NULL;
    	char *reader = input;
    	char *writer = output;

    	skip_whitespace(writer, reader, word);

    	while ((current = *reader) != '\0') {
    	    if (!isspace(*reader)) {
		reader++;
		continue;
	    }

	    stop    = reader;
	    *reader = '\0';
	    if (!dictionary_contains(&Dictionary, word)) {
	    	fprintf(terminal_stream, "Enter replacement for possibly misspelled word (leave blank to ignore): %s\n", word);
	    	if (fgets(replacement, BUFSIZ, terminal_stream) != NULL && strlen(replacement) > 1) {
	    	    word = replacement;
	    	    stop = replacement + strlen(replacement);
		}
	    }

	    while (word != stop) {
	    	*writer++ = *word++;
	    }
	    *reader = current;

    	    skip_whitespace(writer, reader, word);
	}

	*writer = '\0';
	fputs(output, output_stream);
    }
}

void
copy_stream(FILE *input_stream, FILE *output_stream)
{
    char buffer[BUFSIZ];

    while (fgets(buffer, BUFSIZ, input_stream) != NULL) {
    	fputs(buffer, output_stream);
    }
}

int
main(int argc, char *argv[])
{
    FILE *input_stream;
    FILE *output_stream;
    FILE *temporary_stream;
    FILE *terminal_stream;

    parse_command_line_options(argc, argv);
    setup_temporary_path();
    atexit(cleanup);

    debug("InputPath:      %s", InputPath);
    debug("OutputPath:     %s", OutputPath);
    debug("DictionaryPath: %s", DictionaryPath);
    debug("TemporaryPath:  %s", TemporaryPath);
    debug("TerminalPath:   %s", TerminalPath);

    input_stream     = (InputPath == NULL) ? stdin  : safe_fopen(InputPath, "r");
    terminal_stream  = safe_fopen(TerminalPath, "a+");
    temporary_stream = safe_fopen(TemporaryPath, "w+");

    dictionary_load(&Dictionary, DictionaryPath);
    spell_check_stream(input_stream, terminal_stream, temporary_stream);
    fclose(input_stream);
    fclose(terminal_stream);

    output_stream    = (OutputPath == NULL) ? stdout : safe_fopen(OutputPath, "w+");

    fseek(temporary_stream, 0, SEEK_SET);
    copy_stream(temporary_stream, output_stream);
    fclose(output_stream);
    fclose(temporary_stream);

    return (EXIT_SUCCESS);
}
