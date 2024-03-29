============================
Curses Based Clock & Bubbles
============================

This repository contains codes of ``clock.c`` and ``bubbles.c``, which was
written by Martin Sullivan in 80s and 90s, you can download the original
tarball, in K&R C, from `A Curses Based Clock`_.

.. _A Curses Based Clock: http://www.zois.co.uk/tn/tn-1991-01-01.html

.. contents:: **Contents**
   :local:


Installation
============

You need a C compiler that supports C99 and POSIX.1-2008.

By default, both ``bubbles`` and ``clock`` binaries will be installed under
``/usr/local``, you can use ``prefix=DIR`` and/or specify a target to only
install either binary.

.. code:: sh

  $ make [prefix=$HOME/.local] [bubbles|clock] install[-strip]

Use ``install-strip`` if you want stripped binaries. For uninstallation, use
``uninstall`` target.


``clock.c``: *ASCII analog clock in terminal*
=============================================

.. figure:: https://bytebucket.org/livibetter/clock/raw/tip/clock.gif

   Some options

.. figure:: https://bytebucket.org/livibetter/clock/raw/tip/clock.roman.png

   Roman clock face

Options
-------

The usage is::

  clock [OPTION] [TITLE]

+------------------------+----------------------------------------+
| option and argument    | description                            |
+========+===============+========================================+
| ``-s`` | ``--second``  | display second hand                    |
|        | = ``STYLE``   | , three different Styles_              |
+--------+---------------+----------------------------------------+
| ``-r`` | ``--roman``   | use Roman numeral on square clock face |
+--------+---------------+----------------------------------------+
| ``-f`` |               | display date and day of week           |
+--------+---------------+----------------------------------------+
| ``-d`` | ``--date``    | display full date/time                 |
+--------+---------------+----------------------------------------+
| ``-h`` | ``--help``    | display help message and exit          |
+--------+---------------+----------------------------------------+
| ``-v`` | ``--version`` | display version and exit               |
+--------+---------------+----------------------------------------+
| ``TITLE``              | use custom text instead of "(\|./)"    |
+------------------------+----------------------------------------+

Styles
------

0: ``OFF`` (default)
  Do not display second hand

1: ``BLOB`` (default on)
  Display as a blob

2: ``LINE``
  Display the normal clock second hand

Controls
--------

+-------+----------------------------------+
| key   | description                      |
+=======+==================================+
| ``q`` | quit the clock                   |
+-------+----------------------------------+
| ``s`` | toggle second hand               |
+-------+----------------------------------+
| ``r`` | toggle Roman numeral square face |
+-------+----------------------------------+
| ``f`` | toggle date and day of week      |
+-------+----------------------------------+
| ``d`` | toggle full date and time        |
+-------+----------------------------------+


``bubbles.c``: *Animated ASCII bubbles in terminal*
===================================================

.. figure:: https://bytebucket.org/livibetter/clock/raw/tip/bubbles.gif

Options
-------

The usage is::

  bubbles [OPTION]

+------------------------+-------------------------------+---------+
| option                 | description                   | default |
+========+===============+===============================+=========+
| ``-d`` | ``--delay``   | update delay in milliseconds  | 500     |
+--------+---------------+-------------------------------+---------+
| ``-h`` | ``--help``    | display help message and exit |         |
+--------+---------------+-------------------------------+---------+
| ``-v`` | ``--version`` | display version and exit      |         |
+--------+---------------+-------------------------------+---------+

Controls
--------

+-------+------------------+
| key   | description      |
+-------+------------------+
| ``q`` | quit the bubbles |
+-------+------------------+


History
=======

In March, 2014 [#ts]_, I (@livibetter, Yu-Jie Lin) accidentally found the
*clock*, but couldn't compile using GCC 4.7.3. The code, ``clock.c``, was
written in K&R C, although GCC seems to have options_ to compile K&R C
(non-ISO), ``-E -traditional``, Gnulib ain't happy with that, asking for an ISO
C compiler to use its headers.

.. _options: https://gcc.gnu.org/onlinedocs/gcc-3.1/gcc/Incompatibilities.html

Updating the code probably is the only way to resolve those errors and
warnings, but I want to keep the most parts untouched and are able to be
compiled in modern environment. With my limited knowledge of Standard and K&R
C, I still managed to achieve my goal. Now, it could be ``make`` without
spitting out any errors and warnings. Note that -- besides ``clock.c`` -- there
is also a ``bubbles.c``, which is fixed as well.

Originally, this was only meant to fix errors and warnings, but in mid-July,
decisions were made to transitioning into a C99 standard and
POSIX.1-2008-conformant project.

After I created this repository, I emailed Martin Sullivan for some questions
and soon got an reply with a bit of history about this clock:

  Bubbles is by far the oldest of them. I wrote bubbles sometime between
  1984 and 1986 when I was teaching myself curses(3). Curses was
  important back then for it gave a fast, independent, character-based
  interface to all the VT100-like terminals we used to use. Bubbles was
  inspired by rain, a curses-based toy that was distributed with a
  variety of standard UNIXs and produced an effect not unlike rain drops
  on your screen. Subsequently bubbles has been cleaned up, and had a
  couple of bugs removed. It's the version you see now, but it's full of
  bad-practice 'C' too.

  The clock program is whole lot older, but inspired by earlier work. At
  the time I was working for ICL, in the early '90s, on a then new
  machine, a Sparc/VME-bus/SCSI based mini-computer that would become
  the DRS/6000. On display, somebody had written a rather crude script
  involving clear(1), echo(1) and date(1) to repetitively print the time
  on the middle of the console screen on the demo machine. I thought I
  could do better with a lunch-time hack. The clock program went through
  a number of iterations after that, but the code was ugly and contained
  some disgusting hacks. Since it went through a SVR4 C compiler
  relatively cleanly nothing much was done with it. I later wrote it up
  as a `Technical Note`__, giving it a whimsical date from around the time I
  wrote it. It will still compile for me with a 4.4.3 compiler under
  Linux, although not without several pages of warnings about my lax
  approached to C-language standards.

__ `A Curses Based Clock`_

`Version 0.1.0`_ was modified from the original code for modern compiling
environment.

.. _Version 0.1.0: https://bitbucket.org/livibetter/clock/commits/tag/v0.1.0


Copyright
=========

Licensed under the GNU General Public License Version 1, see |COPYING|_::

  Copyright (C) 2014 Alexandre Dantas
  Copyright (C) 2014 Yu-Jie Lin
  Copyright (C) 1993 ZOIS Ltd.
  Copyright (C) 1984-1986 Martin Sullivan

.. |COPYING| replace:: ``COPYING``
.. _COPYING: https://bitbucket.org/livibetter/clock/src/tip/COPYING

----

.. [#ts] The timestamp is ``2014-03-19T07:53:28Z``
