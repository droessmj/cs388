/* macros.h: nifty macros */

#ifndef MACROS_H
#define MACROS_H

#ifdef NDEBUG
#define debug(M, ...)
#else
#define debug(M, ...)	fprintf(stderr, "DEBUG %s:%d: " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#endif

#define chomp(s)	(s)[strlen(s) - 1] = '\0';

#endif
