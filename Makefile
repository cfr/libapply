SO ?= so

ALL_CFLAGS ?= -g -O2 -Wall -std=c99

# For Linux
LD_LIBRARY_PATH=$(CURDIR)
export LD_LIBRARY_PATH

all:: libapply.$(SO) unittest

clean:
	rm -f libapply.$(SO) unittest

test: all
	./unittest

rambo: clean all test

libapply.$(SO): apply.h apply.c
	$(CC) $(ALL_CFLAGS) -I. -shared apply.c -o libapply.$(SO)

unittest: unittest.c libapply.$(SO)
	$(CC) $(ALL_CFLAGS) -I. -L. unittest.c -lapply -o unittest
