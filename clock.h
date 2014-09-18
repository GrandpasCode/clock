/*
 * Copyright (C) 2014 Yu-Jie Lin
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


#include <stdbool.h>

#include "common.h"


#define PROGRAM_NAME "clock"
#define HELP_MESSAGE "\
Usage: " PROGRAM_NAME " [OPTION]...\n\
ASCII analog clock in terminal\n\
\n\
  -s, --second              display second hand\n\
  -r, --roman               display square face with Roman numerals\n\
  -d, --date                display digital date and time\n\
  -f                        display date and day of week\n\
  -h, --help                display this help and exit\n\
  -v, --version             display version and exit\n\
\n\
Also, press any of the keys above to change the behavior\n\
while the clock is running.\n\
For example, press 's' to toggle the second hand.\n\
\n\
Report bugs to <" URL_ISSUES ">\n\
Home page: <" URL_HOMEPAGE ">\n\
For complete documentation, run: man " PROGRAM_NAME

#define DEFAULT_TITLE "(|./)"

#define HALF_Y ((double) (LINES - 1) / (double) 2.0)
#define HALF_X ((double) (COLS  - 1) / (double) 2.0)
// normal presentation of screen, ratio y : x on normal screen
#define ASPECT ((double) (7.5 / 10.0))

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


struct cartesian {
    double x;
    double y;
};

struct icartesian {
    int ix;
    int iy;
};

struct clock_mode {
    bool second;
    bool roman;
    bool day_date;
    bool digital;
};


void plot (struct cartesian *start, struct cartesian *end, char c);
void putline (int x0, int y0, int x1, int y1, char c);
