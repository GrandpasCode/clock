/*
     Clock - puts a clock on a character based terminal by Martin Sullivan
     Copyright (C) 1993 ZOIS Ltd.
     
     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation (version 1).
     
     This program is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     GNU General Public License for more details.
     
     You should have received a copy of the GNU General Public License
     along with this program; if not, write to the Free Software
     Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

     Author: Martin Sullivan.
     ZOIS Ltd., Stag House, 55 Kirkgate, Cockermouth CA13 9PH
     Email: sullivan@zois.co.uk
______________________________________________________________________________

	CLOCK -

	Puts a clock on a character based terminal. The arguments
	are the same as found on the Sun clocktool. -DHOMEBREW gets
	a naive homebrew algorithm for plotting lines else one
	cribbed from a book. This program was written as a lunch
	time hack whilest I was working on contract to ICL. You
	should leave the default title as it is in their honour.

	Martin Sullivan, Chester, 1993.

*/

#include <curses.h>
#include <math.h>
#include <time.h>
#include <signal.h>
#include <setjmp.h>
#ifdef __STDC__
#include <stdlib.h>
#endif
#ifdef BSD
#include <strings.h>
#else
#include <string.h>
#endif
#ifndef BSD
#include <locale.h>
#endif
#include <sys/ioctl.h>

#define DEFAULT_TITLE	"ICL"
#define NO	0
#define YES	1

#define HALF_Y	((double) (LINES - 1) / (double) 2.0)
#define HALF_X	((double) (COLS - 1) / (double) 2.0)
#define ASPECT	((double) (7.5 / 10.0))	/* normal presentation of screen */
				/* ratio y : x on normal screen	*/

struct cartesian {
	double x;
	double y;
};

struct clockMode {
	int dispSecs;
	int romFace;
	int dayDate;
	int digital;
};

jmp_buf	reset;

main (argc, argv)
int	argc;
char	*argv[];
{
	int	c;
	extern char	*optarg;
	extern int	optind;
	struct clockMode mode;
	int 	error;
	char 	*title;
	char	*ns;
#ifdef __STDC__
	void	*malloc ();
#else
	char	*malloc ();
#endif
	char	*strdup ();
	int	winchHandle ();

#ifndef BSD
	setlocale (LC_ALL, "");
#endif
	mode.dispSecs = NO;
	mode.romFace = NO;
	mode.dayDate = NO;
	mode.digital = NO;
	error = NO;
	while ((c = getopt(argc, argv, "srfd")) != EOF)
		switch (c) {
		case 's':
			mode.dispSecs = YES;
			break;
		case 'r':
			mode.romFace = YES;
			break;
		case 'f':
			mode.dayDate = YES;
			break;
		case 'd':
			mode.digital = YES;
			break;
		case '?':
			error = YES;
		}

	if (error) {
		fprintf (stderr, "usage: %s -rfsd title ...\n", argv[0]);
		exit (1);
	} /* if */

	title = DEFAULT_TITLE;
	if (optind < argc)
		title = strdup (argv[optind++]);
	for ( ; optind < argc; optind++) {
		ns = malloc (strlen (title) + strlen (argv[optind]) + 2);
		if (ns == NULL) {
			perror (argv[0]);
			exit (1);
		} /* if */
		strcpy (ns, title);
		strcat (ns, " ");
		strcat (ns, argv[optind]);
		free (title);
		title = ns;
	} /* for */

	setjmp (reset);
	signal (SIGWINCH, winchHandle);
	myClock (&mode, title);
	exit (0);		
} /* main */

myClock (mode, title) 
struct clockMode *mode;
char	*title;
{
	int abort ();
	double asrads ();
	struct tm *t;
#ifdef __STDC__
	time_t	tr;
#else
	unsigned tr;
#endif
	int hourHand;
	int minHand;
	char	*date;
	char	*getDate ();

	initscr ();
	nonl ();
	cbreak ();
	signal (SIGINT, abort);
	signal (SIGTERM, abort);

	for (;;) {
		erase ();
		time (&tr);
		t = localtime (&tr);

		if (mode->dayDate)
			date = getDate (&tr);
		else if (mode->digital)
			date = ctime (&tr);
		else
			date = "";

		if (mode->romFace)
			romCFace (title, date);
		else
			clockFace (title, date);

		hourHand = ((t->tm_hour % 12) * (360 / 12)) +
		    (t->tm_min * 360 / 60 / 12) +
		    (t->tm_sec * 360 / 60 / 60 / 12);
		drawline (asrads (hourHand), 0.6, '#');

		minHand = (t->tm_min * 360 / 60) +
		    (t->tm_sec * 360 / 60 / 60);
		drawline (asrads (minHand), 0.9, '*');

		if (mode->dispSecs)
			drawblob (asrads (t->tm_sec * 360 / 60), 
			    0.7, '@');

		move (0, 0);
		refresh ();

		if (mode->dispSecs)
			sleep (1);
		else {
			time (&tr);	/* sync up to min	*/
			t = localtime (&tr);
			sleep (60 - t->tm_sec);
		} /* else */

	} /* for */

	endwin ();
} /* main */

drawline (vector, length, c)
double vector;		/* radians	*/
double length;		/* relative 0.0 .. 1.0	*/
char c;
{
	struct cartesian end;
	struct cartesian start;

	vector -= M_PI / 2.0;	/* orientate 0 == straight up	*/
	end.y = (sin (vector) * length * HALF_Y) + HALF_Y;
	end.x = (cos (vector) * length * HALF_X * ASPECT) + HALF_X;
	start.y = HALF_Y;
	start.x = HALF_X;
	plot (&start, &end, c);
} /* drawline */

drawblob (vector, length, c)
double vector;		/* radians	*/
double length;		/* relative 0.0 .. 1.0	*/
char c;
{
	vector -= M_PI / 2.0;
	mvaddch ((int) ((sin (vector) * length * HALF_Y) + HALF_Y),
	    (int) ((cos (vector) * length * HALF_X * ASPECT) + HALF_X), c);
} /* drawblob */

#ifdef HOMEBREW
static double min (f1, f2) 
double f1;
double f2;
{
	return (f1 < f2 ? f1 : f2);
} /* min */

static double max (f1, f2) 
double f1;
double f2;
{
	return (f1 > f2 ? f1 : f2);
} /* min */

static plot (start, end, c)
struct cartesian *start;
struct cartesian *end;
char c;
{
	float hereX;
	float endX;
	float incX;
	float hereY;
	float endY;
	float incY;
	int cnt;

	hereX = min (start->x, end->x);
	endX = max (start->x, end->x);
	if ((endX - hereX) < 1.0) {	/* line vertical	*/
		hereY = min (start->y, end->y);
		endY = max (start->y, end->y);
		incY = (endY - hereY) / (float) LINES;
		while (hereY < endY) {
			mvaddch ((int) hereY, (int) hereX, c);
			hereY += incY;
		} /* while */
		return;
	} /* if */
	incX = (endX - hereX) / (float) COLS;

	for (cnt = 0; cnt < COLS; cnt++) {
		mvaddch ((int) (((hereX - start->x) * (end->y - start->y) 
		    / (end->x - start->x)) + start->y), (int) hereX, c);
		hereX += incX;
	} /* for */
} /* plot */

#else /* !HOMEBREW */
static plot (start, end, c)
struct cartesian *start;
struct cartesian *end;
char c;
{
	putline ((int) start->x, (int) start->y, 
	    (int) end->x, (int) end->y, c);
} /* plot */

static putline (x0, y0, x1, y1, c)
int x0;
int y0;
int x1;
int y1;
char c;
/* 

	See Newman & Sproull "Principles of Interactive Computer
	Graphics", McGraw-Hull, New York, 1979 pp 33-44.

*/
{
	register int dx;
	int a;
	register int dy;
	int b;
	int two_a;
	int two_b;
	int xcrit;
	register int eps;

	dx = 1;
	a = x1 - x0;
	if (a < 0) {
		dx = -1;
		a = -a;
	} /* if */

	dy = 1;
	b = y1 - y0;
	if (b < 0) {
		dy = -1;
		b = -b;
	} /* if */

	two_a = 2 * a;
	two_b = 2 * b;

	xcrit = -b + two_a;
	eps = 0;
	mvaddch (y0, x0, c);
	while  (x0 != x1 || y0 != y1) {
		if (eps <= xcrit) {
			x0 += dx;
			eps += two_b;
		} /* if */
		if (eps >= a || a <= b) {
			y0 += dy;
			eps -= two_a;
		} /* if */

		mvaddch (y0, x0, c);
	} /* while */
} /* putline */
#endif /* HOMEBREW */

double asrads (degrees)
int degrees;
{
	return (((double) degrees / (double) 180.0) * (double) M_PI);
} /* asrads */

clockFace (title, date)
char *title;
char *date;
{
	static int lines = 0;
	static int cols = 0;
	register int dash;
	static char dashes[] = "|//-\\\\|//-\\\\";
	float	vector;
	static struct {
		int 	ix;
		int 	iy;
	} at[12];

	if (lines != LINES ||  cols != COLS) {
		lines = LINES;
		cols = COLS;
		for (dash = 0; dash < 12; dash++) {
			vector = (M_PI / 6.0 * (float) dash)
			    - (M_PI / 2.0);
			at[dash].iy = (int) ((sin (vector) 
			    * HALF_Y) + HALF_Y);
			at[dash].ix = (int) ((cos (vector)
			    * HALF_X * ASPECT) + HALF_X);
		} /* for */
	} /* if */

	if ((int) strlen (date) <= (int) COLS)
		mvaddstr ((int) ((LINES - 1) * 3) / (int) 4, 
		    ((int) (COLS - 1) / (int) 2) - ((int) strlen (date) 
		    / (int) 2), date);

	if ((int) strlen (title) <= (int) COLS)
		mvaddstr ((int) (LINES - 1) / (int) 4, 
		    ((int) (COLS - 1) / (int) 2) - ((int) strlen (title) 
		    / (int) 2), title);

	for (dash = 0; dash < 12; dash++)
		mvaddch (at[dash].iy, at[dash].ix, dashes[dash]);
} /* clockFace */

romCFace (title, date)
char *title;
char *date;
{
	static int lines = 0;
	static int cols = 0;
	register int dash;
#ifdef BSD
	static char *number[12];
#else
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
#endif /* BSD */

	static struct {
		int 	ix;
		int	iy;
	} at[12];

	float	x;
	float	y;
	float	vector;

#ifdef BSD
	number[0] = "XII";
	number[1] = "I";
	number[2] = "II";
	number[3] = "III";
	number[4] = "IIII";
	number[5] = "V";
	number[6] = "VI";
	number[7] = "VII";
	number[8] = "VIII";
	number[9] = "IX";
	number[10] = "X";
	number[11] = "XI";
#endif
	if (lines != LINES || cols != COLS) {
		lines = LINES;
		cols = COLS;

		for (dash = 0; dash < 12; dash++) {
			switch (dash) {
			case 0:	x = 0.0;
				y = -1.0;
				break;
			case 3:	x = 1.0;
				y = 0.0;
				break;
			case 6:	x = 0.0;
				y = 1.0;
				break;
			case 9:	x = -1.0;
				y = 0.0;
				break;
			default:
				vector = (M_PI / 6.0 * (float) dash)
				    - (M_PI / 2.0);
				y = tan (vector);
				x = tan ((M_PI / 2.0) - vector);

				if (vector < 0.0)
					x *= -1.0;
				if (vector < M_PI && vector > (M_PI / 2.0))
					y *= -1.0;
				if (vector > M_PI) {
					x *= -1.0;
					y *= -1.0;
				} /* if */
					
				if (y < -1.0)
					y = -1.0;
				if (x < -1.0)
					x = -1.0;
				if (y > 1.0)
					y = 1.0;
				if (x > 1.0)
					x = 1.0;
			} /* switch */

			at[dash].ix = (int) ((x * HALF_X * ASPECT) + HALF_X);
			at[dash].iy = (int) ((y * HALF_Y) + HALF_Y);

			if (at[dash].iy == 0 || at[dash].iy == (LINES - 1))
				at[dash].ix -= (int) strlen (number[dash]) 
				    / (int) 2;
			if (at[dash].ix >= 
			    (int) ((HALF_X * ASPECT) + HALF_X))
				at[dash].ix -= strlen (number[dash]) - 1;
		} /* for */
	} /* if */

	mvaddstr ((int) (LINES - 1) * (int) 3 / (int) 4 , 
	    ((int) (COLS - 1) / (int) 2) - ((int) strlen (date) / 
	    (int) 2), date);

	mvaddstr ((int) (LINES - 1) / (int) 4, ((int) (COLS - 1) / 
	    (int) 2) - ((int) strlen (title) / (int) 2), title);

	for (dash = 0; dash < 12; dash++) 
		mvaddstr (at[dash].iy, at[dash].ix, number[dash]);
} /* romCFace */

char *getDate (pt)
#ifdef __STDC__
time_t *pt;
#else
unsigned *pt;
#endif
{
	char *s;

	s = ctime (pt);
	s[10] = '\0';
	return (s);
} /* getDate */

abort ()
{
	move (LINES - 1, 0);
	refresh ();
	endwin ();
	exit (0);
} /* abort */

winchHandle ()
{
	struct winsize size;

	if (ioctl(2, TIOCGWINSZ, &size) >= 0) {
		LINES = size.ws_row;
		COLS = size.ws_col;
	}
	endwin ();
	longjmp (reset, 1);
} /* winchHandle */
