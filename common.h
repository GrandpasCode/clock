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

#define VERSION                     "0.1.0"
#define PRINT_VERSION(NAME)         \
        do  {                       \
            puts(NAME " " VERSION); \
            puts("\
Copyright (C) 2014 Authors of Clock project\n\
License GPLv1+: GNU GPL version 1 or later <https://www.gnu.org/licenses/gpl-1.0.html>\n\
This is free software: you are free to change and redistribute it.\n\
There is NO WARRANTY, to the extent permitted by law.");\
        } while (false);

#define URL_HOMEPAGE  "https://bitbucket.org/livibetter/clock"
#define URL_ISSUES    "https://bitbucket.org/livibetter/clock/issues"


#ifdef __GNUC__
// curses.h or other sources may already define
#undef  GCC_UNUSED
#define GCC_UNUSED __attribute__((unused))
#else
#define GCC_UNUSED /* nothing */
#endif
