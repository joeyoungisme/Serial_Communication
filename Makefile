CC=gcc
CFLAGS=-g -Wall -O3
LDLIBS=
OBJECT=
EXAMPLE_FILE=joeser example example2

$(EXAMPLE_FILE):
	$(CC) $(CGALGS) $(LDLIBS) $@.c -o $@


clean:
	rm $(EXAMPLE_FILE)
