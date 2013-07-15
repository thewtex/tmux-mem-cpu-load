====================================================
               tmux-mem-cpu-load
====================================================
----------------------------------------------------
CPU, RAM memory, and load monitor for use with tmux_
----------------------------------------------------

.. image:: https://travis-ci.org/thewtex/tmux-mem-cpu-load.png
  :target: https://travis-ci.org/thewtex/tmux-mem-cpu-load

Description
===========

A simple, lightweight program provided for system monitoring in the *status*
line of **tmux**.

The memory monitor displays the used and available memory.

The CPU usage monitor outputs a percent CPU usage over all processors. It also
displays a textual bar graph of the current percent usage.

The system load average is also displayed.

Example output::

  2885/7987MB [|||||     ]  51.2% 2.11 2.35 2.44

   ^    ^          ^         ^     ^    ^    ^
   |    |          |         |     |    |    |
   1    2          3         4     5    6    7

1. Currently used memory.
2. Available memory.
3. CPU usage bar graph.
4. CPU usage percentage.
5. Load average for the past minute.
6. Load average for the past 5 minutes.
7. Load average for the past 15 minutes.

For `terminals with 256 color support`_, graded colors can be displayed by
passing the **--colors** flag.


Installation
============

Dependencies
------------

Currently, Linux and Mac OSX are supported.

Building
~~~~~~~~

* >= CMake_ -2.6
* C++ compiler (e.g. gcc/g++)

Download
--------

There are links to the source code at the `project homepage`_.

Build
-----

::

  cd <source dir>
  cmake .
  make

Install
-------

::

  su -
  make install
  logout


Configuring tmux_
=================

Edit ``$HOME/.tmux.conf`` to display the program's output in *status-left* or
*status-right*.  For example::

  set -g status-interval 2
  set -g status-left "#S #[fg=green,bg=black]#(tmux-mem-cpu-load --colors 2)#[default]"

Note that the first argument to `tmux-mem-cpu-load` should be the same number
of seconds that *status-interval* is set at.

An optional second argument is the number of bars in the bar graph, which
defaults to 10.  This can, for instance, be set to the number of cores in a
multi-core system.


Author
======

Matt McCormick (thewtex) <matt@mmmccormick.com>

Contributions from:

* Justin Crawford <justinc@pci-online.net>
* krieiter <krieiter@gmail.com>


.. _tmux: http://tmux.sourceforge.net/
.. _CMake: http://www.cmake.org
.. _`project homepage`: http://github.com/thewtex/tmux-mem-cpu-load
.. _`terminals with 256 color support`: http://misc.flogisoft.com/bash/tip_colors_and_formatting#terminals_compatibility
