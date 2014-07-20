CFLAGS = -std=c99 -Wextra -Wall
CPPFLAGS = -D _POSIX_C_SOURCE=200809L
LDFLAGS = -lm -lcurses

ALL = clock bubbles

all: $(ALL)

clobber: clean
	rm -f $(ALL)

clean:
	rm -f *.o

clock: clock.c

bubbles: bubbles.c

.PHONY: all clobber clean
