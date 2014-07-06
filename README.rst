====================
A Curses Based Clock
====================

`A Curses Based Clock`_ was written by Martin Sullivan in 1991 based on the timestamps of the files from the tarball, the last update was made on 2004-02-02.

.. _A Curses Based Clock: http://www.zois.co.uk/tn/tn-1991-01-01.html

In March, 2014 (``2014-03-19T07:53:28Z``), I (Yu-Jie Lin) found it, but couldn't compile using GCC 4.7.3. The code, ``clock.c``, was written in K&R C, although GCC seems to have options_ to compile K&R (non-ISO), ``-E --traditional``, Gnulib ain't happy with that, asking for an ISO C compiler to use its headers.

.. _options: https://gcc.gnu.org/onlinedocs/gcc-3.1/gcc/Incompatibilities.html

Updating the code probably the only way to resolves those errors and warnings, but I want to keep the most parts untouched and are able to be compiled in modern environment. With my limited knowledge about Standard C and K&R C, I still managed to achieve my goal. Now, it could be ``make`` without spitting out any errors and warnings. Note that—besides ``clock.c``—there is also a ``bubbles.c``, which is fixed as well.


Some mistakes might be added by me, since this is an open source, please don't hesitate to create pull requests or open an issue.

.. contents:: **Contents**
   :local:


Screenshots
===========

``clock.c``
-----------

.. figure:: https://bytebucket.org/livibetter/clock/raw/tip/clock.gif

.. figure:: https://bytebucket.org/livibetter/clock/raw/tip/clock.roman.png

``bubbles.c``
-------------

.. figure:: https://bytebucket.org/livibetter/clock/raw/tip/bubbles.gif


Options
=======

The usage is::

  clock [ -frsd ] [ title ...  ]

``-s``:
  display second hand

``-r``:
  use Roman numeral on square clock face

``-f``:
  display date and day of week

``-d``:
  display full date/time

``TITLE``:
  use custom text instead of "ICL"


Copyright
=========

Licensed under the GNU General Public License Version 1, see |license|_::

  Copyright (C) 1993 ZOIS Ltd.

.. |license| replace:: ``license``
.. _license: https://bitbucket.org/livibetter/clock/src/tip/license
