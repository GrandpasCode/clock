CFLAGS  = -Wextra -Wall
LDFLAGS = -lm -lcurses

ALL = clock bubbles

all: $(ALL)

clobber: clean
	rm -f $(ALL)

clean:
	rm -f *.o

clock: clock.c
	$(CC) $(CFLAGS) clock.c $(LDFLAGS) -o $@
bubbles: bubbles.c
	$(CC) $(CFLAGS) bubbles.c $(LDFLAGS) -o $@
