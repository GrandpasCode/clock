INSTALL = install
INSTALL_PROGRAM = $(INSTALL) -D -m 755
INSTALL_MAN     = $(INSTALL) -D -m 644

CFLAGS   = -std=c99 -Wextra -Wall
CPPFLAGS = -D _POSIX_C_SOURCE=200809L
LDFLAGS  = -lm -lcurses

prefix  = /usr/local

bindir  = $(prefix)/bin
mandir  = $(prefix)/share/man
man1dir = $(mandir)/man1
man6dir = $(mandir)/man6

ALL = clock bubbles

INSTALL_BINS  = $(filter $(ALL),$(MAKECMDGOALS))
INSTALL_BINS := $(if $(INSTALL_BINS),$(INSTALL_BINS),$(ALL))

.PHONY: all
all: $(ALL)

.PHONY: clobber
clobber: clean
	rm -f $(ALL)

.PHONY: clean
clean:
	rm -f *.o

clock: clock.c

bubbles: bubbles.c

.PHONY: install
install: $(INSTALL_BINS)
ifneq (,$(findstring bubbles,$(INSTALL_BINS)))
	$(INSTALL_PROGRAM) bubbles $(DESTDIR)$(bindir)/bubbles
	$(INSTALL_MAN) bubbles.6 $(DESTDIR)$(man6dir)/bubbles.6
endif
ifneq (,$(findstring clock,$(INSTALL_BINS)))
	$(INSTALL_PROGRAM) clock $(DESTDIR)$(bindir)/clock
	$(INSTALL_MAN) clock.1 $(DESTDIR)$(man1dir)/clock.1
endif

.PHONY: install-strip
install-strip:
	$(MAKE) INSTALL_PROGRAM='$(INSTALL_PROGRAM) -s' \
	        $(INSTALL_BINS) install

.PHONY: uninstall
uninstall:
ifneq (,$(findstring bubbles,$(INSTALL_BINS)))
	rm -f $(DESTDIR)$(bindir)/bubbles
	rm -f $(DESTDIR)$(man6dir)/bubbles.6
endif
ifneq (,$(findstring clock,$(INSTALL_BINS)))
	rm -f $(DESTDIR)$(bindir)/clock
	rm -f $(DESTDIR)$(man1dir)/clock.1
endif
