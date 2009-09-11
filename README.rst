====================================================
                  screen-cpu-mem
====================================================
----------------------------------------------------
CPU and RAM memory monitors for use with GNU Screen_
----------------------------------------------------



Description
===========


Two programs are provided for system monitoring in the *hardstatus* line of **GNU
Screen**: a CPU usage monitor and a memory usage monitor.  These programs are
intended to use minimal system resources.

The CPU usage monitor outputs a percent CPU usage over all processors that is
updated every second.  It also displays a textual bar graph of the current
percent usage where every '|' character represents 10% usage.  

The memory monitor is updated every three seconds and displays the used and
available memory.

Example::

  Mem:2885MB/7987MB  Cpu:[|||||     ]  51.2%



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



Configuring GNU Screen_
=======================


The ``$HOME/.screenrc`` file must be edited to make use of the installed
programs, ``screen-cpu-usage`` and ``screen-mem-usage``.

An example configuration::

  # backticks to display information in the statusbar
  backtick 1 0 0 /usr/local/bin/screen-mem-usage
  backtick 2 0 0 /usr/local/bin/screen-cpu-usage

  hardstatus alwayslastline  
  hardstatus string '%{kg}Host:%{kG}%H%{kg} %=%{kb}Mem:%{kB}%1`  %{ky}Cpu:%{kY}%2`%%  %{kR}Load:%{kr}%l%=%{kc} %D %m/%d %{kC}%C%a%{w}' 

The example configuration will result in::

  Host:flea   Mem:2961MB/7987MB  Cpu:[||        ]  25.6%  Load:0.64 0.35 0.28 Fri 09/11 12:56am




Author
======

Matt McCormick (thewtex) <matt@mmmccormick.com>


.. _screen: http://www.gnu.org/software/screen/
.. _cmake: http://www.cmake.org
.. _`project homepage`: http://github.com/thewtex/screen-cpu-mem
