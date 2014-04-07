CFLAGS   = -std=gnu99 -Wall

SOURCE   = $(shell ls *.c)
PROGRAMS = ${SOURCE:.c=}

all:	${PROGRAMS}

%:	%.c
	$(CC) -o $@ $(CFLAGS) $^

clean:
	rm -f ${PROGRAMS}

.PHONY:	all clean
