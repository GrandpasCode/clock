ALL=clock bubbles

all: $(ALL)

clobber: clean
	rm -f $(ALL)

clean: 
	rm -f *.o

clock: clock.c
	$(CC) $(CFLAGS) clock.c -lm -lcurses -o $@
bubbles: bubbles.c
	$(CC) $(CFLAGS) bubbles.c -lcurses -ltermcap -o $@
