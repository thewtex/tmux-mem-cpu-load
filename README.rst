====================================================
               tmux-mem-cpu-load
====================================================
----------------------------------------------------
CPU, RAM memory, and load monitor for use with tmux_
----------------------------------------------------



Description
===========


A simple, lightweight program provided for system monitoring in the *status*
line of **tmux**.

The memory monitor displays the used and available memory.

The CPU usage monitor outputs a percent CPU usage over all processors. It also
displays a textual bar graph of the current percent usage where every '|'
character represents 10% usage.  

The system load average is also displayed.

Example output::

  2885/7987MB [|||||     ]  51.2% 2.11 2.35 2.44

   ^    ^          ^         ^     ^    ^    ^
   |    |          |         |     |    |    |
   A    B          C         D     E    F    G

A. Currently used memory.
B. Available memory.
C. CPU usage bar graph.
D. CPU usage percentage.
E. Load average for the past minute.
F. Load average for the past 5 minutes.
G. Load average for the past 15 minutes.


Installation
============


Dependencies
------------

Building
~~~~~~~~

* >= cmake_ -2.6
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
=======================


Edit ``$HOME/.tmux.conf`` to display the programs output in *status-left* or
*status-right*.  For example::

  set -g status-interval 2
  set -g status-left "#S #[fg=green,bg=black,bright]#(tmux-mem-cpu-load 2)#[default]" 

Note that the first argument to `tmux-mem-cpu-load` should be the same number
of seconds that *status-interval* is set at.


Author
======

Matt McCormick (thewtex) <matt@mmmccormick.com>


.. _tmux: http://tmux.sourceforge.net/
.. _cmake: http://www.cmake.org
.. _`project homepage`: http://github.com/thewtex/tmux-mem-cpu-load
