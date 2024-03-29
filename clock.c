/*
 * ASCII analog clock in terminal
 * Copyright (C) 2014 Alexandre Dantas
 * Copyright (C) 2014 Yu-Jie Lin
 * Copyright (C) 1993 ZOIS Ltd.
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
#include <errno.h>
#include <getopt.h>
#include <locale.h>
#include <math.h>
#include <setjmp.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>

#include "common.h"
#include "clock.h"


char *program_name = PROGRAM_NAME;
char *help_message = HELP_MESSAGE;

char *second_styles[] = {
    [SS_OFF ] = "OFF",
    [SS_BLOB] = "BLOB",
    [SS_LINE] = "LINE"
};
unsigned int n_second_styles = sizeof(second_styles) / sizeof(char *);

jmp_buf reset;


void
roman_clock_face (char *title, char *date)
{
    static int lines = 0;
    static int cols = 0;
    int dash;
    static char *number[12] = {
        "XII",
        "I",
        "II",
        "III",
        "IIII",
        "V",
        "VI",
        "VII",
        "VIII",
        "IX",
        "X",
        "XI"
    };
    static struct icartesian at[12];
    float x;
    float y;
    float vector;

    if (lines != LINES || cols != COLS) {
        lines = LINES;
        cols = COLS;

        for (dash = 0; dash < 12; dash++) {
            switch (dash) {
            case 0:
                x =  0.0;
                y = -1.0;
                break;
            case 3:
                x = 1.0;
                y = 0.0;
                break;
            case 6:
                x = 0.0;
                y = 1.0;
                break;
            case 9:
                x = -1.0;
                y =  0.0;
                break;
            default:
                vector = (M_PI / 6.0 * (float) dash) - (M_PI / 2.0);
                y = tan(vector);
                x = tan((M_PI / 2.0) - vector);

                if (vector < 0.0)
                    x *= -1.0;
                if (vector < M_PI && vector > (M_PI / 2.0))
                    y *= -1.0;
                if (vector > M_PI) {
                    x *= -1.0;
                    y *= -1.0;
                }

                if (y < -1.0)
                    y = -1.0;
                if (x < -1.0)
                    x = -1.0;
                if (y > 1.0)
                    y = 1.0;
                if (x > 1.0)
                    x = 1.0;
            }

            at[dash].ix = (int) ((x * HALF_X * ASPECT) + HALF_X);
            at[dash].iy = (int) ((y * HALF_Y)          + HALF_Y);

            if (at[dash].iy == 0 || at[dash].iy == (LINES - 1))
                at[dash].ix -= (int) strlen(number[dash]) / (int) 2;
            if (at[dash].ix >= (int) ((HALF_X * ASPECT) + HALF_X))
                at[dash].ix -= strlen(number[dash]) - 1;
        }
    }

    mvaddstr(
        (int) (LINES - 1) * (int) 3 / (int) 4,
        ((int) (COLS - 1) / (int) 2) - ((int) strlen(date) / (int) 2),
        date
    );

    mvaddstr(
        (int) (LINES - 1) / (int) 4,
        ((int) (COLS - 1) / (int) 2) - ((int) strlen(title) / (int) 2),
        title
    );

    for (dash = 0; dash < 12; dash++)
        mvaddstr(at[dash].iy, at[dash].ix, number[dash]);
}


void
clock_face (char *title, char *date)
{
    static int lines = 0;
    static int cols  = 0;
    int dash;
    static char dashes[] = "|//-\\\\|//-\\\\";
    float vector;
    static struct icartesian at[12];

    if (lines != LINES || cols != COLS) {
        lines = LINES;
        cols  = COLS;
        for (dash = 0; dash < 12; dash++) {
            vector = (M_PI / 6.0 * (float) dash) - (M_PI / 2.0);
            at[dash].iy = (int) ((sin(vector) * HALF_Y)          + HALF_Y);
            at[dash].ix = (int) ((cos(vector) * HALF_X * ASPECT) + HALF_X);
        }
    }

    if ((int) strlen(date) <= (int) COLS)
        mvaddstr(
            (int) ((LINES - 1) * 3) / (int) 4,
            ((int) (COLS - 1) / (int) 2) - ((int) strlen(date) / (int) 2),
            date
        );

    if ((int) strlen(title) <= (int) COLS)
        mvaddstr(
            (int) (LINES - 1) / (int) 4,
            ((int) (COLS - 1) / (int) 2) - ((int) strlen(title) / (int) 2),
            title
        );

    for (dash = 0; dash < 12; dash++)
        mvaddch(at[dash].iy, at[dash].ix, dashes[dash]);
}


/*
 * vector: radians
 * length: relative 0.0 .. 1.0
 */
void
drawline (double vector, double length, char c)
{
    struct cartesian end;
    struct cartesian start;

    // orientate 0 == straight up
    vector -= M_PI / 2.0;
    end.y = (sin(vector) * length * HALF_Y)          + HALF_Y;
    end.x = (cos(vector) * length * HALF_X * ASPECT) + HALF_X;
    start.y = HALF_Y;
    start.x = HALF_X;
    plot(&start, &end, c);
}


/*
 * vector: radians
 * length: relative 0.0 .. 1.0
 */
void
drawblob (double vector, double length, char c)
{
    vector -= M_PI / 2.0;
    mvaddch(
        (int) ((sin(vector) * length * HALF_Y)          + HALF_Y),
        (int) ((cos(vector) * length * HALF_X * ASPECT) + HALF_X),
        c
    );
}


void
plot (struct cartesian *start, struct cartesian *end, char c)
{
    putline(
        (int) start->x,
        (int) start->y,
        (int) end->x,
        (int) end->y,
        c
    );
}


/*
 * See Newman & Sproull "Principles of Interactive Computer
 * Graphics", McGraw-Hull, New York, 1979 pp 33-44.
 */
void
putline (int x0, int y0, int x1, int y1, char c)
{
    int dx;
    int dy;
    int a;
    int b;
    int two_a;
    int two_b;
    int xcrit;
    int eps;

    dx = 1;
    a = x1 - x0;
    if (a < 0) {
        dx = -1;
        a = -a;
    }

    dy = 1;
    b = y1 - y0;
    if (b < 0) {
        dy = -1;
        b = -b;
    }

    two_a = 2 * a;
    two_b = 2 * b;

    xcrit = -b + two_a;
    eps = 0;
    mvaddch(y0, x0, c);
    while  (x0 != x1 || y0 != y1) {
        if (eps <= xcrit) {
            x0 += dx;
            eps += two_b;
        }
        if (eps >= a || a <= b) {
            y0 += dy;
            eps -= two_a;
        }

        mvaddch(y0, x0, c);
    }
}


double
asrads (int degrees)
{
    return (((double) degrees / (double) 180.0) * (double) M_PI);
}


char *
get_date (time_t *pt)
{
    char *s;

    s = ctime(pt);
    s[10] = '\0';
    return s;
}


void
winch_handle ()
{
    struct winsize size;

    if (ioctl(2, TIOCGWINSZ, &size) >= 0) {
        LINES = size.ws_row;
        COLS  = size.ws_col;
    }
    endwin();
    longjmp(reset, 1);
}


void
my_clock (struct clock_mode *mode, char *title)
{
    struct  tm *t;
    time_t  tr;
    int     hour_hand;
    int     min_hand;
    char    *date;
    int     ch;
    // 1/100 seconds
    struct  timespec req = {
        .tv_sec = 0,
        .tv_nsec = 10000000
    };
    time_t  next_update = 0;

    initscr();
    noecho();
    curs_set(0);
    nonl();
    cbreak();
    nodelay(stdscr, true);
    signal(SIGINT,  abort_handle);
    signal(SIGTERM, abort_handle);

    for (;;) {
        ch = getch();
        if (ch == 'q')
            break;

        switch (ch) {
        case 's':
            mode->second = mode->second == SS_END ? SS_BEGIN : mode->second + 1;
            next_update = 0;
            break;

        case 'r':
            mode->roman = !mode->roman;
            next_update = 0;
            break;

        case 'f':
            mode->day_date = !mode->day_date;
            next_update = 0;
            break;

        case 'd':
            mode->digital = !mode->digital;
            next_update = 0;
            break;
        }

        nanosleep(&req, NULL);
        if (time(NULL) >= next_update) {
            time(&tr);
            t = localtime(&tr);

            if (mode->second != SS_OFF) {
                next_update = time(NULL) + 1;
            } else {
                // sync up to min
                time(&tr);
                t = localtime(&tr);
                next_update = time(NULL) + (60 - t->tm_sec);
            }
        } else
            continue;

        erase();

        if (mode->day_date)
            date = get_date(&tr);
        else if (mode->digital)
            date = ctime(&tr);
        else
            date = "";

        if (mode->roman)
            roman_clock_face(title, date);
        else
            clock_face(title, date);

        hour_hand = (
            (t->tm_hour % 12) * (360 / 12)) +
            (t->tm_min * 360 / 60 / 12) +
            (t->tm_sec * 360 / 60 / 60 / 12
        );
        drawline(asrads(hour_hand), 0.6, '#');

        min_hand = (t->tm_min * 360 / 60) + (t->tm_sec * 360 / 60 / 60);
        drawline(asrads(min_hand), 0.9, '*');

        switch (mode->second) {
        case SS_OFF:
            break;

        case SS_LINE:
            drawline(asrads(t->tm_sec * 360 / 60), 0.7, '@');
            break;

        default:
            drawblob(asrads(t->tm_sec * 360 / 60), 0.7, '@');
            break;
        }

        move(0, 0);
        refresh();
    }

    endwin();
}


int
main (int argc, char *argv[])
{
    int  c;
    long i;
    char *title;
    char *ns;
    char *end;

    struct clock_mode mode = {
        .second   = SS_DEFAULT,
        .roman    = false,
        .day_date = false,
        .digital  = false
    };
    struct option long_options[] = {
        {"second" , optional_argument, NULL, 's'},
        {"roman"  , no_argument      , NULL, 'r'},
        {"date"   , no_argument      , NULL, 'd'},
        {"help"   , no_argument      , NULL, 'h'},
        {"version", no_argument      , NULL, 'v'},
        {NULL     , 0                , NULL,  0 }
    };

    setlocale(LC_ALL, "");
    while ((c = getopt_long(argc, argv, "s::rfdhv", long_options, NULL)) != -1)
        switch (c) {
        case 's':
            if (optarg == NULL) {
                mode.second = SS_DEFAULT_ON;
                break;
            }

            i = strtol(optarg, &end, 10);
            if (end == optarg || *end != '\0' || errno == ERANGE) {
                for (i = 0; (unsigned) i < n_second_styles; i++)
                    if (strcmp(optarg, second_styles[i]) == 0)
                        break;
                if (i <= SS_END) {
                    mode.second = i;
                    break;
                }
            } else {
                mode.second = atoi(optarg);
            }
            break;

        case 'r':
            mode.roman = true;
            break;

        case 'f':
            mode.day_date = true;
            break;

        case 'd':
            mode.digital = true;
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

    title = DEFAULT_TITLE;
    if (optind < argc)
        title = strdup(argv[optind++]);
    for (; optind < argc; optind++) {
        ns = malloc(strlen(title) + strlen(argv[optind]) + 2);
        if (ns == NULL) {
            perror(argv[0]);
            exit(EXIT_FAILURE);
        }
        strcpy(ns, title);
        strcat(ns, " ");
        strcat(ns, argv[optind]);
        free(title);
        title = ns;
    }

    setjmp(reset);
    signal(SIGWINCH, winch_handle);
    my_clock(&mode, title);
    exit(EXIT_SUCCESS);
}
