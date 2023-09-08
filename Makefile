srcdir = .

PREFIX = /usr/local
MANPREFIX = $(PREFIX)/share/man

CC = clang

ldlibs = $(LDLIBS)

all: release

release: CFLAGS += -O2 -Weverything -Wno-unsafe-buffer-usage
release: new_lines

debug: CFLAGS += -g -Weverything -Wno-unsafe-buffer-usage
debug: clean
debug: new_lines

objs = new_lines.o

all: new_lines

.PHONY: all clean install uninstall
.SUFFIXES:
.SUFFIXES: .c .o

new_lines: $(objs)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(objs) $(ldlibs)

$(objs): Makefile

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

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
