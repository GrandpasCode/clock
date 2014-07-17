/*
 * puts bubbles on a character based terminal
 */

#include <curses.h>
#include <signal.h>
#ifdef __STDC__
#include <stdlib.h>
#endif
#include <unistd.h>
#include <getopt.h>

#define SURFACE        ((LINES) / 5)
#define NOBUBBLE        ' '


struct bubble {
    int x;        /* 0 .. LINES    */
    int y;        /* 0 .. COLS    */
    char image;   /* '.', 'o', 'O': ' ' == no bubble     */
    int speed;    /* 1 .. 3    */
};


float frand ()    /** in range 0.0 .. 1.0    */
{
    return ((float) (rand () & 0377) / 256.0);
} /* frand */

void new (pb)     /** make a new one    */
struct bubble *pb;
{
    int size;

    pb->y = LINES - 1;
    pb->x = (int) ((frand () * (float) (COLS - 1)) + 0.5);
    pb->speed = (int) (frand () * 3.0) + 1;
    size = (int) (frand () * 4.0);

    switch (size) {
    case 1: pb->image = '.';
        break;
    case 2: pb->image = 'o';
        break;
    case 3:    pb->image = 'O';
        break;
    default:
        pb->image = NOBUBBLE;
    } /* switch */
} /* new */

void pop (pb)            /** pop it    */
struct bubble *pb;
{
    switch (pb->image) {
    case '.':
        mvaddch (SURFACE, pb->x, '_');
        break;
    case 'o':
        mvaddch (SURFACE, pb->x, 'v');
        break;
    case 'O':
        mvaddch (SURFACE, pb->x, '*');
        mvaddch (SURFACE - 1, pb->x, '.');
        break;
    default:
        return;
    } /* switch */

    pb->image = NOBUBBLE;
} /* pop */

#ifdef __STDC__
void abortHandle (signum)
int signum;
#else
abortHandle ()
#endif
{
    (void)(signum); /* avoiding "unused parameter" */
    move (LINES - 1, 0);
    refresh ();
    endwin ();
    exit (0);
} /* abortHandle */

int main (argc, argv) 
int argc;
char *argv[];
{
    static struct bubble *bubbles;
    int c;
    /* update interval in ms */
    int delay = 500;
    int quantity;
    register int ix;
#ifdef __STDC__
    void abortHandle ();
#else
    int abortHandle ();
#endif

    while ((c = getopt(argc, argv, "d:")) != EOF)
        switch (c) {
        case 'd':
            delay = atoi(optarg);
            break;
        case '?':
            fprintf (stderr, "usage: %s [-d DELAY]\n",
                argv[0]);
            exit (1);
        }

    initscr ();
    curs_set (0);
    nonl ();
    cbreak ();
    signal (SIGTERM, abortHandle);
    signal (SIGINT, abortHandle);

    quantity = COLS / 4;
    bubbles = (struct bubble *) calloc (quantity, 
        sizeof (struct bubble));
    if (bubbles == NULL) {
        perror (argv[0]);
        exit (1);
    } /* if */

    for (ix = 0; ix < quantity; ix++) 
        bubbles[ix].image = NOBUBBLE;

    for (;;) {
        erase ();

        mvaddch (SURFACE, 0, '\\');
        for (ix = 1; ix < (COLS - 1); ix++)
            mvaddch (SURFACE, ix, '_');
        mvaddch (SURFACE, COLS - 1, '/');

        for (ix = 0; ix < quantity; ix++) {
            if (bubbles[ix].image == NOBUBBLE) 
                new (&(bubbles[ix]));
            else {
                bubbles[ix].y -= bubbles[ix].speed;
                if (bubbles[ix].y < SURFACE) 
                    pop (&(bubbles[ix]));
            } /* else */

            if (bubbles[ix].image != NOBUBBLE) 
                mvaddch (bubbles[ix].y, bubbles[ix].x,
                    bubbles[ix].image);

        } /* for */
        move (0, 0);
        refresh ();
        usleep (delay * 1000);
    } /* for */
    endwin ();
} /* main */
