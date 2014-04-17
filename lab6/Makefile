CFLAGS   = -g -std=gnu99 -Wall -DNDEBUG

SOURCE   = $(shell ls *.c)
OBJECTS	 = ${SOURCE:.c=.o}
PROGRAM  = rorschach

all:	${PROGRAM}

rorschach: ${OBJECTS}
	$(CC) -o $@ $(CFLAGS) $^

%.o:	%.c
	$(CC) -o $@ $(CFLAGS) -c $^

clean:
	rm -f ${PROGRAM} ${OBJECTS}

.PHONY:	all clean
