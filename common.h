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


#ifndef COMMON_H
#define COMMON_H

#define VERSION "0.3.0"

#define URL_HOMEPAGE  "https://bitbucket.org/livibetter/clock"
#define URL_ISSUES    "https://bitbucket.org/livibetter/clock/issues"

#define TRY "Try '%s --help' for more information.\n"

#ifdef __GNUC__
// curses.h or other sources may already define
#undef  GCC_UNUSED
#define GCC_UNUSED __attribute__((unused))
#else
#define GCC_UNUSED /* nothing */
#endif


extern char *program_name;
extern char *help_message;


void print_version ();
void usage (int status);

void abort_handle (int signum GCC_UNUSED);

#endif
