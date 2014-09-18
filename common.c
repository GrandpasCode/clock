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


#include <curses.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"


void
print_version ()
{
    printf("%s %s\n", program_name, VERSION);
    puts("\
Copyright (C) 2014 Authors of Clock project\n\
License GPLv1+: GNU GPL version 1 or later <https://www.gnu.org/licenses/gpl-1.0.html>\n\
This is free software: you are free to change and redistribute it.\n\
There is NO WARRANTY, to the extent permitted by law.");
}


void
usage (int status)
{
    if (status != EXIT_SUCCESS)
        fprintf(stderr, TRY, program_name);
    else
        puts(help_message);

    exit(status);
}


void
abort_handle (int signum GCC_UNUSED)
{
    move(LINES - 1, 0);
    refresh();
    endwin();
    exit(EXIT_SUCCESS);
}
