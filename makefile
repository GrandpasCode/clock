ALL=clock bubbles stars balls bounce fire speedo bubble.orig
#
# Note: for speedo to work you'll need to be able to read /unix and
# /dev/kmem (sometimes the latter is not allowed for ordinary users).

all: $(ALL)

clobber: clean
	rm -f $(ALL)

clean: 
	rm -f *.o

speedo: speedo.c
	$(CC) $(CFLAGS) speedo.c -lelf -lm -lcurses -ltermcap -o $@
clock: clock.c
	$(CC) $(CFLAGS) clock.c -lm -lcurses -ltermcap -o $@
bubbles: bubbles.c
	$(CC) $(CFLAGS) bubbles.c -lcurses -ltermcap -o $@
bubble.orig: bubble.orig.c
	$(CC) $(CFLAGS) bubble.orig.c -lcurses -ltermcap -o $@
stars: stars.c
	$(CC) $(CFLAGS) stars.c -lm -lcurses -ltermcap -o $@
balls: balls.c
	$(CC) $(CFLAGS) balls.c -lm -lcurses -ltermcap -o $@
bounce: bounce.c
	$(CC) $(CFLAGS) bounce.c -lm -lcurses -ltermcap -o $@

fire: fire.c 
	$(CC) $(CFLAGS) fire.c -lcurses -ltermcap -o $@

