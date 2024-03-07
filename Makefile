PREFIX ?= /usr/local
BINDIR := $(PREFIX)/bin
MANDIR := $(PREFIX)/share/man
SRCDIR := src
HDRS := $(SRCDIR)/version.h $(SRCDIR)/includes/

TARGET := linklooter
SOURCE := $(SRCDIR)/$(TARGET).c
LIBS := 
LDFLAGS :=

.PHONY: all install install-man clean

all: $(TARGET)

install: install-bin install-man

$(TARGET): $(SRCDIR) $(HDRS)
	$(CC) $(CFLAGS) $(SOURCE) -o $(TARGET) $(LIBS) $(LDFLAGS)

install-bin: $(TARGET)
	install -d $(DESTDIR)$(BINDIR)
	install -m 755 $< $(DESTDIR)$(BINDIR)

install-man: creep.1
	install -d $(DESTDIR)$(MANDIR)/man1
	install -m 644 $< $(DESTDIR)$(MANDIR)/man1

uninstall: uninstall-bin uninstall-man

uninstall-bin:
	rm $(DESTDIR)$(BINDIR)/$(TARGET)

uninstall-man:
	rm $(DESTDIR)$(MANDIR)/man1/creep.1

clean:
	rm -f $(TARGET)