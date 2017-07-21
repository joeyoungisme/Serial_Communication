CC=gcc
CFLAGS=-g -Wall -O3
LDLIBS=
OBJECT=
EXAMPLE_FILE=

example:
	$(CC) $(CFLAGS) $(LDLIBS) example.c -o example
	$(CC) $(CFLAGS) $(LDLIBS) example2.c -o example2

clean:
	rm example example2
