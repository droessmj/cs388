/* http_get: Simple HTTP Client */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#ifdef NDEBUG
#define debug(M, ...)
#else
#define debug(M, ...)	fprintf(stderr, "DEBUG %s:%d: " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#endif

#define HTTP_PORT   80

int    http_get(char *url);
void   parse_url(char *url, char *host, int *port, char *path);
FILE * socket_dial(const char *host, const int port);

int
main(int argc, char *argv[])
{
    if (argc < 2) {
    	fprintf(stderr, "usage: http_client url...\n");
    	exit(EXIT_FAILURE);
    }

    for (int i = 1; i < argc; i++) {
    	if (http_get(argv[i]) != EXIT_SUCCESS) {
    	    fprintf(stderr, "http_get failed: %s", strerror(errno));
	}
    }

    return (EXIT_SUCCESS);
}

int
http_get(char *url) {
    char  host[BUFSIZ];
    char  path[BUFSIZ];
    char  buffer[BUFSIZ];
    int   port;
    FILE *socket_file;

    /* Extract host, port, and path from url */
    parse_url(url, host, &port, path);
    debug("Parsed url=%-35s, host=%10s, port=%5d, path=%s", url, host, port, path);

    /* Connect to host and port */
    if ((socket_file = socket_dial(host, port)) == NULL) {
    	debug("Could not dial %s:%d = %s", host, port, strerror(errno));
    	return (EXIT_FAILURE);
    }
    debug("Connected to %s:%d", host, port);

    /* Send HTTP 1.0 GET Request */
    fprintf(socket_file, "GET %s HTTP/1.0\r\n", path);
    fprintf(socket_file, "HOST: %s\r\n", host);
    fprintf(socket_file, "\r\n");

    /* Dump Server response */
    while (fgets(buffer, BUFSIZ, socket_file) != NULL) {
    	fputs(buffer, stdout);
    }

    fclose(socket_file);
    return (EXIT_SUCCESS);
}

void
parse_url(char *url, char *host, int *port, char *path)
{
    char *hp;
    char *pp;

    /* Split host and path */
    hp = url;
    if (strncmp(hp, "http://", 7) == 0) {
    	hp += 7;
    }

    pp = strchr(hp, '/');
    if (pp) {
    	size_t hlen = pp - hp;

    	strncpy(host, hp, hlen);
    	host[hlen] = '\0';

    	strcpy(path, pp);
    } else {
    	strcpy(host, hp);
    	strcpy(path, "/");
    }

    /* Split host and port */
    pp = strchr(host, ':');
    if (pp) {
    	*pp++ = '\0';
    	*port = strtol(pp, NULL, 10);
    } else {
    	*port = HTTP_PORT;
    }
}

FILE *
socket_dial(const char *host, const int port)
{
    struct sockaddr_in addr;
    struct hostent    *hentry;
    int socketfd = -1;

    /* Lookup host */
    if ((hentry = gethostbyname(host)) == NULL) {
    	goto fail;
    }

    /* Setup socket address */
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(port);
    memcpy(&addr.sin_addr.s_addr, hentry->h_addr_list[0], hentry->h_length);

    /* Allocate socket */
    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    	goto fail;
    }

    /* Connect to host */
    if (connect(socketfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    	goto fail;
    }

    /* Open and return FILE from socket */
    return fdopen(socketfd, "r+");

fail:
    if (socketfd > 0) {
    	close(socketfd);
    }
    return (NULL);
}
