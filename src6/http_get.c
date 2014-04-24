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

#define HTTP_PORT   "80"

int    http_get(char *url);
void   parse_url(char *url, char *host, char *port, char *path);
FILE * socket_dial(const char *host, const char *port);

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
    char  port[BUFSIZ];
    char  buffer[BUFSIZ];
    FILE *socket_file;

    /* Extract host, port, and path from url */
    parse_url(url, host, port, path);
    debug("Parsed url=%-35s, host=%10s, port=%5s, path=%s", url, host, port, path);

    /* Connect to host and port */
    if ((socket_file = socket_dial(host, port)) == NULL) {
    	debug("Could not dial %s:%s = %s", host, port, strerror(errno));
    	return (EXIT_FAILURE);
    }
    debug("Connected to %s:%s", host, port);

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
parse_url(char *url, char *host, char *port, char *path)
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
    	strcpy(port, pp);
    } else {
    	strcpy(port, HTTP_PORT);
    }
}

FILE *
socket_dial(const char *host, const char *port)
{
    struct addrinfo   *serv_info;
    struct addrinfo    hints;
    int    socket_fd = -1;

    /* Lookup server address information */
    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(host, port, &hints, &serv_info) < 0) {
	return (NULL);
    }

    /* For each server entry, allocate socket and try to connect */
    for (struct addrinfo *p = serv_info; p != NULL; p = p->ai_next) {
	/* Allocate socket */
	if ((socket_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) {
	    debug("Unable to make socket: %s", strerror(errno));
	    continue;
	}

	/* Connect to host */
	if (connect(socket_fd, p->ai_addr, p->ai_addrlen) < 0) {
	    debug("Unable to connect: %s", strerror(errno));
	    close(socket_fd);
	    continue;
	}

	break;
    }
    freeaddrinfo(serv_info);

    /* Open and return FILE from socket */
    return fdopen(socket_fd, "r+");
}
