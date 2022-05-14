version = 1.0

srcdir = .

PREFIX = /usr/local
MANPREFIX = $(PREFIX)/share/man

cflags = -O3 -std=gnu99 -Wall -Wpedantic -Wextra $(CFLAGS)

ldlibs = $(LDLIBS)

objs = new_lines.o

all: new_lines

.PHONY: all clean install uninstall
.SUFFIXES:
.SUFFIXES: .c .o
$(V).SILENT:

new_lines: $(objs)
	@echo "LINK $@"
	$(CC) $(LDFLAGS) -o $@ $(objs) $(ldlibs)

$(objs): Makefile

.c.o:
	@echo "CC $@"
	$(CC) $(cflags) -c -o $@ $<

clean:
	rm -f *.o new_lines

install: all
	@echo "INSTALL bin/new_lines"
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp new_lines $(DESTDIR)$(PREFIX)/bin/
	chmod 755 $(DESTDIR)$(PREFIX)/bin/new_lines
	@echo "INSTALL new_lines.1"
	mkdir -p $(DESTDIR)$(MANPREFIX)/man1
	sed "s!PREFIX!$(PREFIX)!g; s!VERSION!$(version)!g" new_lines.1 \
		>$(DESTDIR)$(MANPREFIX)/man1/new_lines.1
	chmod 644 $(DESTDIR)$(MANPREFIX)/man1/new_lines.1

uninstall:
	@echo "REMOVE bin/new_lines"
	rm -f $(DESTDIR)$(PREFIX)/bin/new_lines
	@echo "REMOVE new_lines.1"
	rm -f $(DESTDIR)$(MANPREFIX)/man1/new_lines.1
	@echo "REMOVE new_lines.1"
