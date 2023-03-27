srcdir = .

PREFIX = /usr/local
MANPREFIX = $(PREFIX)/share/man

cflags = -O3 -std=gnu99 -Wall -Wpedantic -Wextra -s $(CFLAGS)

ldlibs = $(LDLIBS)

objs = new_lines.o

all: new_lines

.PHONY: all clean install uninstall
.SUFFIXES:
.SUFFIXES: .c .o

new_lines: $(objs)
	$(CC) $(cflags) $(LDFLAGS) -o $@ $(objs) $(ldlibs)

$(objs): Makefile

.c.o:
	$(CC) $(cflags) -c -o $@ $<

clean:
	rm -f *.o new_lines

install: all
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp new_lines $(DESTDIR)$(PREFIX)/bin/
	chmod 755 $(DESTDIR)$(PREFIX)/bin/new_lines
	mkdir -p $(DESTDIR)$(MANPREFIX)/man1
	cp new_lines.1 $(DESTDIR)$(MANPREFIX)/man1/new_lines.1
	chmod 644 $(DESTDIR)$(MANPREFIX)/man1/new_lines.1

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/new_lines
	rm -f $(DESTDIR)$(MANPREFIX)/man1/new_lines.1
