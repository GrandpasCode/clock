/*
 * puts bubbles on a character based terminal
 * Copyright (C) 2014 Alexandre Dantas
 * Copyright (C) 2014 Yu-Jie Lin
 * Copyright (C) 1984-1986 Martin Sullivan
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation (version 1).

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <curses.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <getopt.h>

#define PROGRAM_NAME      "bubbles"
#define SURFACE        ((LINES) / 5)
#define NOBUBBLE        ' '

// macro helpers
#define STR(s)            XSTR(s)
#define XSTR(s)           #s

// default option values
#define DELAY_COUNT       500


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

/** make a new one */
void new (struct bubble *pb)
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

/** pop it */
void pop (struct bubble *pb)
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

void abortHandle (int signum)
{
    (void)(signum); /* avoiding "unused parameter" */
    move (LINES - 1, 0);
    refresh ();
    endwin ();
    exit (0);
} /* abortHandle */

void
usage(int status)
{
    if (status != EXIT_SUCCESS)
        fprintf(
            stderr,
            "Try '%s --help' for more information.\n",
            PROGRAM_NAME
        );
    else {
        printf(
        "\
Usage: %s [OPTION]...\n\
Displays an animation with bubbles on the terminal.\n\
\n\
  -d, --dalay=DELAY         update interval in milliseconds (Default: " STR(DELAY_COUNT) ")\n\
  -h, --help                display this help and exit\n\
\n\
Report bugs to <https://bitbucket.org/livibetter/clock/issues>\n\
Home page: <https://bitbucket.org/livibetter/clock>\n\
For complete documentation, run: man %s\n",
            PROGRAM_NAME,
            PROGRAM_NAME
        );
    }

    exit(status);
}

int main (int argc, char *argv[])
{
    static struct bubble *bubbles;
    int c;
    int ch;
    long delay_count = DELAY_COUNT;
    long count = 0;
    struct timespec req = { .tv_sec = 0, .tv_nsec = 1000000 };
    int quantity;
    register int ix;
    struct option long_options[] = {
        {"delay", required_argument, NULL, 'd'},
        {"help" , no_argument      , NULL, 'h'},
        {NULL   , 0                , NULL,  0 }
    };

    while ((c = getopt_long(argc, argv, "d:h", long_options, NULL)) != -1)
        switch (c) {
        case 'd':
            delay_count = atoi(optarg);
            break;

        case 'h':
            usage(EXIT_SUCCESS);
            break;

        default:
            usage(EXIT_FAILURE);
        }

    initscr ();
    noecho ();
    curs_set (0);
    nonl ();
    cbreak ();
    nodelay (stdscr, TRUE);
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

    count = 0;
    for (;;) {
        ch = getch();
        if (ch == 'q')
            break;

        nanosleep (&req, NULL);
        if (--count > 0)
            continue;
        count = delay_count;

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
        nanosleep (&req, NULL);
    } /* for */
    endwin ();
} /* main */
