/*
 * Animated ASCII bubbles in terminal
 * Copyright (C) 2014 Alexandre Dantas
 * Copyright (C) 2014 Yu-Jie Lin
 * Copyright (C) 1984-1986 Martin Sullivan
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation (version 1).
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <curses.h>
#include <getopt.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "common.h"
#include "bubbles.h"


char *program_name = PROGRAM_NAME;
char *help_message = HELP_MESSAGE;


// in range 0.0 .. 1.0
float
frand ()
{
    return ((float) (rand() & 0377) / 256.0);
}


void
new (struct bubble *pb)
{
    int size;

    pb->y = LINES - 1;
    pb->x = (int) ((frand() * (float) (COLS - 1)) + 0.5);
    pb->speed = (int) (frand() * 3.0) + 1;
    size      = (int) (frand() * 4.0);

    switch (size) {
    case 1:
        pb->image = '.';
        break;

    case 2:
        pb->image = 'o';
        break;

    case 3:
        pb->image = 'O';
        break;

    default:
        pb->image = NOBUBBLE;
    }
}


void
pop (struct bubble *pb)
{
    switch (pb->image) {
    case '.':
        mvaddch(SURFACE, pb->x, '_');
        break;

    case 'o':
        mvaddch(SURFACE, pb->x, 'v');
        break;

    case 'O':
        mvaddch(SURFACE, pb->x, '*');
        mvaddch(SURFACE - 1, pb->x, '.');
        break;

    default:
        return;
    }

    pb->image = NOBUBBLE;
}


int
main (int argc, char *argv[])
{
    static struct bubble *bubbles;
    int c;
    int ch;
    long delay_count = DELAY_COUNT;
    long count = 0;
    struct timespec req = {
        .tv_sec = 0,
        .tv_nsec = 1000000
    };
    int quantity;
    int ix;
    struct option long_options[] = {
        {"delay"  , required_argument, NULL, 'd'},
        {"help"   , no_argument      , NULL, 'h'},
        {"version", no_argument      , NULL, 'v'},
        {NULL     , 0                , NULL,  0 }
    };

    while ((c = getopt_long(argc, argv, "d:hv", long_options, NULL)) != -1)
        switch (c) {
        case 'd':
            delay_count = atoi(optarg);
            break;

        case 'h':
            usage(EXIT_SUCCESS);
            break;

        case 'v':
            print_version();
            exit(EXIT_SUCCESS);

        default:
            usage(EXIT_FAILURE);
        }

    srand(time(NULL));
    initscr();
    noecho();
    curs_set(0);
    nonl();
    cbreak();
    nodelay(stdscr, true);
    signal(SIGTERM, abort_handle);
    signal(SIGINT,  abort_handle);

    quantity = COLS / 4;
    bubbles = (struct bubble *) calloc(quantity, sizeof(struct bubble));
    if (bubbles == NULL) {
        perror(argv[0]);
        exit(EXIT_FAILURE);
    }

    for (ix = 0; ix < quantity; ix++)
        bubbles[ix].image = NOBUBBLE;

    count = 0;
    for (;;) {
        ch = getch();
        if (ch == 'q')
            break;

        nanosleep(&req, NULL);
        if (--count > 0)
            continue;
        count = delay_count;

        erase();

        mvaddch(SURFACE, 0, '\\');
        for (ix = 1; ix < (COLS - 1); ix++)
            mvaddch(SURFACE, ix, '_');
        mvaddch(SURFACE, COLS - 1, '/');

        for (ix = 0; ix < quantity; ix++) {
            if (bubbles[ix].image == NOBUBBLE)
                new(&(bubbles[ix]));
            else {
                bubbles[ix].y -= bubbles[ix].speed;
                if (bubbles[ix].y < SURFACE)
                    pop(&(bubbles[ix]));
            }

            if (bubbles[ix].image != NOBUBBLE)
                mvaddch(bubbles[ix].y, bubbles[ix].x, bubbles[ix].image);

        }
        move(0, 0);
        refresh();
        nanosleep(&req, NULL);
    }
    endwin();
}
