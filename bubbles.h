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


#include "common.h"


#define PROGRAM_NAME "bubbles"
#define HELP_MESSAGE "\
Usage: " PROGRAM_NAME  " [OPTION]...\n\
Animated ASCII bubbles in terminal\n\
\n\
  -d, --dalay=DELAY         update interval in milliseconds\n\
                            (Default: " STR(DELAY_COUNT) ")\n\
  -h, --help                display this help and exit\n\
  -v, --version             display version and exit\n\
\n\
Report bugs to <" URL_ISSUES ">\n\
Home page: <" URL_HOMEPAGE ">\n\
For complete documentation, run: man " PROGRAM_NAME

#define SURFACE  ((LINES) / 5)
#define NOBUBBLE ' '

// macro helpers
#define STR(s)  XSTR(s)
#define XSTR(s) #s

// default option values
#define DELAY_COUNT 500


struct bubble {
    // 0 .. LINES
    int x;
    // 0 .. COLS
    int y;
    // '.', 'o', 'O': ' ' == no bubble
    char image;
    // 1 .. 3
    int speed;
};
