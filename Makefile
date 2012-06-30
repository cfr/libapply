SO ?= so
ALL_CFLAGS ?= -g -O2 -Wall -std=c99

prefix ?= $(CURDIR)
libdirbase ?= lib
libdir ?= $(prefix)/$(libdirbase)

# For Linux
LD_LIBRARY_PATH=$(CURDIR)
export LD_LIBRARY_PATH

LIBAPPLY ?= libapply.$(SO)

all:: $(LIBAPPLY) unittest

install:
	mkdir -p $(DESTDIR)$(libdir)
	cp $(LIBAPPLY) $(DESTDIR)$(libdir)

uninstall:
	rm -f $(DESTDIR)$(libdir)/$(LIBAPPLY)
	rmdir -p $(DESTDIR)$(libdir) 2>/dev/null || true

clean:
	rm -f libapply.$(SO) unittest

test: all
	@./unittest

rambo: clean all test

$(LIBAPPLY): apply.h apply.c
	$(CC) $(ALL_CFLAGS) -I. -shared apply.c -o $(LIBAPPLY)

unittest: unittest.c $(LIBAPPLY)
	$(CC) $(ALL_CFLAGS) -I. -L. unittest.c -lapply -o unittest
